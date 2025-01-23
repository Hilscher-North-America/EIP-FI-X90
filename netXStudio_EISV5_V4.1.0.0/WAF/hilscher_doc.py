#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################
# Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
########################################################################################
# $Id:  $:
#
# Description:
# waf support for building documentations
########################################################################################

from waflib.Configure import conf
from waflib import Utils, Task
import re
import os
import shlex

def configure(conf):
    # change to parent environment object
    conf.setenv('')

    # find tool used for doxygen
    conf.find_program('doxygen', var='DOXYGEN')
    conf.find_program('hhc',     var='HTMLHELPC')

    plantuml_buildtools_node = conf.root.find_dir(os.environ['PATH_BUILDTOOLS'] + os.sep + 'plantuml')

    ret = None
    if plantuml_buildtools_node is not None:
        # find all available versions
        nodes = plantuml_buildtools_node.ant_glob('**/plantuml.jar')

        # make mapping version -> file
        nodes_by_version = dict( (tuple(map(int,x.parent.name.split('.'))), x) for x in nodes)

        # get most recent version
        k = sorted(nodes_by_version.keys())[-1]

        ret = nodes_by_version[k].abspath()

    conf.msg('Checking for plantuml', ret or False)

    if ret:
        conf.env['PLANTUML'] = ret



re_match_doxysetting = re.compile(r'^\s*(?P<parameter>\S+)\s*(?P<operator>[+]?=)\s+(?P<value>\S.*)$').match

# as long as this is not properly implemented
@Task.always_run
class doxygen(Task.Task):
    color = 'CYAN'

    run_str_doxygen = '${DOXYGEN} ${SRC}'

    (run_doxygen, doxygen_vars) = Task.compile_fun(run_str_doxygen)

    run_str_hhc = '${HTMLHELPC} ${SRC}'

    (run_hhc, hhc_vars) = Task.compile_fun(run_str_hhc)

    vars = doxygen_vars + hhc_vars

    log_str = '[DOXYGEN] ${SOURCES}'

    @property
    def target(self):
        return ''

    path_parameter_names = set('''
        CITE_BIB_FILES
        DIAFILE_DIRS
        DOTFILE_DIRS
        EXCLUDE
        EXAMPLE_PATH
        IMAGE_PATH
        INCLUDE_PATH
        INPUT
        HTML_EXTRA_FILES
        HTML_EXTRA_STYLESHEET
        HTML_FOOTER
        HTML_HEADER
        HTML_STYLESHEET
        LATEX_EXTRA_FILES
        LAYOUT_FILE
        MSCFILE_DIRS
        RTF_EXTENSIONS_FILE
        RTF_STYLESHEET_FILE
        TAGFILES
        WARN_LOGFILE
    '''.split())

    def translate_paths(self, parameter, value):
        bld           = self.bld
        doxyfile_node = self.inputs[0]

        for p in shlex.split(value, posix=False):
            node = doxyfile_node.parent.find_node(p)

            if node is None:
                bld.fatal('Path %r given in parameter %s not found' % (p, parameter))

            yield node.abspath()

    def run(self):
        bld     = self.bld
        env     = self.env
        inputs  = self.inputs
        outputs = self.outputs

        doxyfile_node                           = inputs[0]
        mangled_doxyfile_node, output_dir_node  = outputs[0:2]

        doxyfile_content = doxyfile_node.read()

        first = set()

        # parse doxyfile and fix paths
        for line in doxyfile_content.splitlines():
            lhs, _, _ = line.partition('#')

            m = re_match_doxysetting(lhs)

            if m:
                parameter, operator, value = m.group('parameter', 'operator', 'value')

                if value.strip():
                    if parameter == 'OUTPUT_DIRECTORY':
                        doxyfile_content += '\nOUTPUT_DIRECTORY = "%s"\n' % output_dir_node.abspath()
                    elif parameter in self.path_parameter_names:

                        # enforce the first parameter occurence to be an '=' to clear all old settings
                        if parameter not in first:
                            operator = '='
                            first.add(parameter)

                        paths = list(self.translate_paths(parameter, value))
                        doxyfile_content += '\n%s %s %s\n' % (parameter, operator, ' '.join('"%s"' % x for x in paths))

        # enforce hhc enabled
        doxyfile_content += '\nHHC_LOCATION = "%s"\n' % env['HTMLHELPC']

        # set plantuml if found
        if env['PLANTUML']:
            doxyfile_content += '\nPLANTUML_JAR_PATH = "%s"\n' % env['PLANTUML']

        mangled_doxyfile_node.parent.mkdir()
        mangled_doxyfile_node.write(doxyfile_content)

        env.stash()
        try:
            self.inputs = [mangled_doxyfile_node]
            self.run_doxygen()
        finally:
            self.inputs = inputs
            env.revert()


pattern_doxygen_input = """
   *.c *.cc *.cxx *.cpp *.c++ *.h *.hh *.hxx *.hpp *.h++
   *.java *.ii *.ixx *.ipp *.i++ *.inl
   *.idl *.ddl *.odl
   *.cs *.d *.php *.php4 *.php5 *.phtml *.inc
   *.m *.markdown *.md *.mm *.dox *.py *.pyw
   *.f90 *.f95 *.f03 *.f08
   *.f *.for *.tcl *.vhd *.vhdl
   *.ucf *.qsf""".split()


@conf
def generate_doxygen_documentation(bld, doxyfile, **kw):
    doxyfile_node = bld.path.find_resource(doxyfile)

    if not doxyfile_node:
        bld.fatal('Doxyfile %r not found' % doxyfile)

    if not bld.env['DOXYGEN']:
        return

    doxyfile_content = doxyfile_node.read()
    doxygen_input_nodes  = []
    doxygen_output_nodes = []

    # parse doxyfile and fix paths
    doxygen_params = {}
    for line in doxyfile_content.splitlines():
        lhs, _, _ = line.partition('#')

        m = re_match_doxysetting(lhs)

        if m:
            parameter, operator, value = m.group('parameter', 'operator', 'value')

            if parameter == 'INPUT':
                nodes = []
                for x in shlex.split(value, posix=False):
                    n = doxyfile_node.parent.find_node(x)

                    if n is None:
                        bld.fatal(u'Doxygen include path %r not found' % x)

                    doxygen_input_nodes.append(n)
            else:
                doxygen_params[parameter] = value

    output_dir_node = doxyfile_node.parent.find_or_declare(doxygen_params['OUTPUT_DIRECTORY']).get_src()

    def get_doxygen_boolean(key):
        val = doxygen_params.get(key,'NO').lower()
        return val in ('true', 'yes')

    if get_doxygen_boolean('GENERATE_HTMLHELP'):
        # add chm node to outputs
        chm_node = output_dir_node.find_or_declare(doxygen_params['HTML_OUTPUT']).find_or_declare(doxygen_params['CHM_FILE']).get_src()
        doxygen_output_nodes.append(chm_node)

    # doxyfile will be mangled for processing to contain proper paths when called by waf
    mangled_doxyfile_node = output_dir_node.find_or_declare('Doxyfile').get_src()

    # make a list of files which might regarded as input by doxygen
    patterns = list('**/%s' % x for x in pattern_doxygen_input)

    def get_file_list(x):
        if os.path.isdir(x.abspath()):
            return x.ant_glob(patterns)
        else:
            return [x]

    potential_source_nodes = sum((get_file_list(n) for n in doxygen_input_nodes),[])

    tsk = doxygen(env=bld.env)
    tsk.set_inputs([doxyfile_node] + potential_source_nodes)
    tsk.set_outputs([mangled_doxyfile_node, output_dir_node] + doxygen_output_nodes)
    bld.add_to_group(tsk)

    if bld.is_install and ('install_path' in kw):
        bld.install_files(kw['install_path'], doxygen_output_nodes)

