/**************************************************************************************
Exclusion of Liability for this software:
  The following software is intended for and must only be used for reference and in an
  evaluation laboratory environment. It is provided without charge and is subject to
  alterations. There is no warranty for the software, to the extent permitted by
  applicable law. Except when otherwise stated in writing the copyright holders and/or
  other parties provide the software "as is" without warranty of any kind, either
  expressed or implied.
  Please refer to the Agreement in README_DISCLAIMER.txt, provided together with this file!
  By installing or otherwise using the software, you accept the terms of this Agreement.
  If you do not agree to the terms of this Agreement, then do not install or use the
  Software!
**************************************************************************************/

/********************************************************************************
 ********************************************************************************
 *      Copyright (c) Hilscher North America.            All Rights Reserved.
 ********************************************************************************
 *      File Name:           App_TerminalHandler.h
 *      Product:             ETHERNET/IP MIDDLEWARE
 *      Author(s):           Jarek Bojanowicz & Andres Torres
 *      Created on:          XX/XX/XXXX
 *      Updated on:          xx/xx/xxxx
 *      Protocol Stack:      ETHERNET/IP ADAPTER V5.4.0.4
 ********************************************************************************
 * Module Source Description:
 *
 * The header contains the general packet header definitions and declarations.
 *
 ********************************************************************************
 ********************************************************************************/

#ifndef COMPONENTS_CIFXAPPLICATIONDEMO_INCLUDES_APP_TERMINALHANDLER_H_
#define COMPONENTS_CIFXAPPLICATIONDEMO_INCLUDES_APP_TERMINALHANDLER_H_

/*****************************************************************************/
/*! General Inclusion Area                                                   */
/*****************************************************************************/

#include "App_Application.h"
#include "Hil_DualPortMemory.h"
#include "hostAbstractionLayer.h"

/*****************************************************************************/
/*! DEFINITIONS                                                              */
/*****************************************************************************/

#define APP_TERMINAL_MAX_CMDLINE_LEN   64 /* Terminal buffer size */
#define APP_TERMINAL_MAX_CMD_LEN       15 /* Terminal command buffer size */

/*****************************************************************************/
/*! ENUMERATIONS of terminal commands                                        */
/*****************************************************************************/
typedef enum APP_TERMINAL_HANDLER_COMMANDS_Etag
{
  APP_TERMINAL_HANDLER_COMMAND_RESET = 0,
  APP_TERMINAL_HANDLER_COMMAND_READ_INPUT,
  APP_TERMINAL_HANDLER_COMMAND_WRITE_OUTPUT,  
  
  APP_TERMINAL_HANDLER_NUMBER_OF_COMMANDS,
  APP_TERMINAL_HANDLER_COMMAND_UNKNOWN,
} APP_TERMINAL_HANDLER_COMMANDS_E;

/*****************************************************************************/
/*! typedef terminal command structure                                       */
/*****************************************************************************/
typedef struct APP_TERMINAL_CMD_Ttag
{
  uint8_t bCommandCode;
  const char    *szString;
  const char    *szHelp;
} APP_TERMINAL_CMD_T;

/*****************************************************************************/
/*! typedef terminal command handler structure                               */
/*****************************************************************************/
typedef struct APP_TERMINAL_CMD_HANDLER_Ttag
{
  const char *cmd;                                                //command string
  uint32_t (*pf_command)( char *strCmdBuffer);                    //command handler (no arguments)
  uint32_t (*adv_pf_command)(uint32_t argc, const char *argv[]);  //command handler
  const char *cmd_desc;                                           //command description for 'ls' table
} APP_TERMINAL_CMD_HANDLER_T;

/*****************************************************************************/
/*! FUNCTION PROTOTYPES                                                      */
/*****************************************************************************/
void App_TerminalHandler(APP_DATA_T *ptAppData);
uint32_t Term_ListShellCommands( char *strCmdBuffer);
uint32_t Term_ShellCommandHandler(struct APP_DATA_Ttag* ptAppData, char *strCmdBuffer);

#endif /* COMPONENTS_CIFXAPPLICATIONDEMO_INCLUDES_APP_TERMINALHANDLER_H_ */

