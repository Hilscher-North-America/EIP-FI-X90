#ifndef SHELL_H_
#define SHELL_H_

#include <stdint.h>
#include "diag.h"

#define SHELL_EXIT_SUCCESS   0
#define SHELL_EXIT_ERROR    -1

//shell printing
#ifdef SHELL_QUIET
#define TERM_PRINTF(...)
#else
#define TERM_PRINTF printf
#endif

#define SHELL_CMDS \
  {"app_ctrl" , NULL , Shell_AppControl, "<0> Shell <1> APP (re)assign ownership of io expander i2c events"}, \
  {"bipsave"  , NULL , shell_baseipsave,       "<ip> Changes first 3 octets of IP address" }, \
  {"ipsave"   , NULL , ipsave,           "<ip> <mask> <gateway> Set IP address, mask and gateway"}, \
  {"link"     , NULL , link,             "Show Ethernet parameters"}, \
  {"f0"       , NULL , Shell_FileSystemInit, "Full format the default partition of the target file system"}, \
  {"l"        , NULL , Shell_FileSystemList, "List Directories and Files from File System"}, \
  {"nvdata"   ,	NULL , Shell_FileSystemShow, "Show NV data" }, \
  {"nvsave"   ,	NULL , Shell_FileSystemSave, "Save NV data" }, \
  {"nvdel"    ,	NULL , Shell_FileSystemDel, "Delete NV data" }, \
  {"temp"     ,	NULL , Shell_Temp, "Display netX Temperature" }, \
  {"temp_i"   ,	NULL , Shell_TempIntercept, "[<intercept>] Show or Save Temperature line intercept parameter" }, \
  {"temp_s"   ,	NULL , Shell_TempSlope, "[<slope>] Show or Save Temperature line slope parameter" }, \
  
#ifdef  DIAG_MODE
#define DIAG_CMDS \
  {"d" , NULL , Show_Diag, "Show FW diagnostics"}, \
  {"c" , NULL , Clear_Diag, "Clear FW diagnostics"}, \

#else
#define DIAG_CMDS
#endif
extern bool g_App_Control;

uint32_t Shell_AppControl( uint32_t argc, const char *argv[] );
uint32_t ipsave (uint32_t argc, const char * argv[]);
uint32_t shell_baseipsave (uint32_t argc, const char * argv[]);
uint32_t link (uint32_t argc, const char * argv[]);
uint32_t Shell_FileSystemInit( uint32_t argc, const char *argv[] );
uint32_t Shell_FileSystemList( uint32_t argc, const char *argv[] );
uint32_t Shell_FileSystemShow (uint32_t argc, const char * argv[]);
uint32_t Shell_FileSystemSave (uint32_t argc, const char * argv[]);
uint32_t Shell_FileSystemDel (uint32_t argc, const char * argv[]);
uint32_t Shell_Temp (uint32_t argc, const char * argv[]);
uint32_t Shell_TempIntercept (uint32_t argc, const char * argv[]);
uint32_t Shell_TempSlope (uint32_t argc, const char * argv[]);
uint32_t Show_Diag (uint32_t argc, const char * argv[]);
uint32_t Clear_Diag (uint32_t argc, const char * argv[]);
uint32_t Term_VersionInfo(uint32_t argc, const char *argv[]);

int Shell_check_hex_scan_str_to_u32(const char * chp_cmdline, uint32_t* u32ptr_outbuf);
bool Shell_check_help_request(uint32_t argc, const char *argv[], bool  *short_ptr );

#endif /* SHELL_H_ */
