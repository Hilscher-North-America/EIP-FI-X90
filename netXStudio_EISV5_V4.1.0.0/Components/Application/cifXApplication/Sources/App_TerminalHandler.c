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

/**************************************************************************************

Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.

***************************************************************************************/

/*****************************************************************************/
/*! General Inclusion Area                                                   */
/*****************************************************************************/

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "OS_Dependent.h"
#include "App_SystemPackets.h"
#include "App_TerminalHandler.h"
#include "RTC.h"
#include "shell.h"
#include "mks_mcu.h"
#include "cifxtoolkit.h"
#include "HAL_shell_commands.h"

/*****************************************************************************/
/*! DEFINITIONS                                                              */
/*****************************************************************************/
#define MAX_ARG_COUNT 8
#define ARGC_MAX    ( 256 ) /*somewhat arbitrary*/

#define STR(tok) #tok
#define XSTR(tok) STR(tok)

/*****************************************************************************/
/*! GLOBAL, STATIC OR EXTERN VARIABLES                                       */
/*****************************************************************************/
extern APP_DATA_T g_tAppData;
static uint32_t Term_ResetCommand(char* szCommandName);
static uint32_t Term_ReadInputCommand(char* szCommandName);
static uint32_t Term_WriteOutputCommand(char* szCommandName);

static APP_TERMINAL_CMD_HANDLER_T  g_ShellCommands[] = {
  {"----- netX commands \x01--------------------------", NULL, NULL, "" }, \
  {"ls"       , Term_ListShellCommands , NULL            , "list all MKS commands" },
  {"reset"    , Term_ResetCommand      , NULL            , "Reset the netX chip" },
  {"cord"     , Term_ReadInputCommand  , NULL            , "Read input values" },
  {"cowr"     , Term_WriteOutputCommand, NULL            , "Write output values" },
  {"version"  , NULL                   , Term_VersionInfo, "display FW version and build information" },  
  HAL_SHELL_CMDS
};

static char g_temp_cmd_buff[ APP_TERMINAL_MAX_CMDLINE_LEN ];

/*****************************************************************************/
/*! PRIVATE FUNCTIONS                                                        */
/*****************************************************************************/

/*****************************************************************************/
/*! Reset command
 *
 *   \param szCommandName [in] Command name
 *
 *   \return command result
 */
/*****************************************************************************/
static uint32_t Term_ResetCommand(char* szCommandName)
{
  TERM_PRINTF("Reset now!" NEWLINE);
  mks_netX90_system_restart();

  return CIFX_NO_ERROR;
}

/*****************************************************************************/
/*! Read input values
 *
 *   \param szCommandName [in] Command name
 *
 *   \return command result
 */
/*****************************************************************************/
static uint32_t Term_ReadInputCommand(char* szCommandName)
{
  int iIdx;
  uint8_t tmpdata[sizeof(g_tAppData.tInputData)];
  PCHANNELINSTANCE ptChannel   = (PCHANNELINSTANCE)g_tAppData.aptChannels[0]->hChannel;     
      
  OS_EnterLock(ptChannel->pvLock);      
  memcpy(tmpdata, &g_tAppData.tInputData, sizeof(g_tAppData.tInputData));
  OS_LeaveLock(ptChannel->pvLock);      

  TERM_PRINTF("Input Data Image:" NEWLINE);
  for (iIdx = 0; iIdx < sizeof(g_tAppData.tInputData); iIdx++)
  {
    TERM_PRINTF("%02d 0x%02x" NEWLINE, iIdx, (int)tmpdata[iIdx]);
  }

  return CIFX_NO_ERROR;
}

/*****************************************************************************/
/*! Write output values
 *
 *   \param szCommandName [in] Command name
 *
 *   \return command result
 */
/*****************************************************************************/
static uint32_t Term_WriteOutputCommand(char* szCommandName)
{
  int iIdx;
  uint8_t tmpdata[sizeof(g_tAppData.tOutputData)];
  PCHANNELINSTANCE ptChannel   = (PCHANNELINSTANCE)g_tAppData.aptChannels[0]->hChannel;

  OS_EnterLock(ptChannel->pvLock);
  memcpy(tmpdata, &g_tAppData.tOutputData, sizeof(g_tAppData.tOutputData));
  OS_LeaveLock(ptChannel->pvLock);

  TERM_PRINTF("Output Data Image:" NEWLINE);      
  for (iIdx = 0; iIdx < sizeof(g_tAppData.tOutputData); iIdx++)
  {
    TERM_PRINTF("%02d 0x%02x" NEWLINE, iIdx, (int)tmpdata[iIdx]);
  }  

  return CIFX_NO_ERROR;
}

/*****************************************************************************/
/*! PUBLIC FUNCTIONS                                                         */
/*****************************************************************************/

/***************************************************************************************
*! List shell commands for testing
*   \param strCmdBuffer    terminal command
****************************************************************************************/
uint32_t Term_ListShellCommands( char *strCmdBuffer)
{
  uint16_t idx;
  uint16_t num_commands = sizeof ( g_ShellCommands ) / sizeof(APP_TERMINAL_CMD_HANDLER_T);

#define LIST_FORMATTING    "%-15s %s\r\n"

  TERM_PRINTF("\r\n List of MKS Shell commands:\r\n\r\n");
  TERM_PRINTF(LIST_FORMATTING, "Command" , "Description");
  TERM_PRINTF("--------------------------------------------------------------------------------\r\n");
  for( idx = 0 ; idx < num_commands ; ++idx )
  {
    TERM_PRINTF(LIST_FORMATTING, g_ShellCommands[ idx ].cmd , g_ShellCommands[ idx ].cmd_desc);
  }

  return 0;
}

/***************************************************************************************
*! Shell command handler.
*   \param ptAppData   pointer to APP_DATA_T structure
*   \param szBuffer    terminal command
****************************************************************************************/
uint32_t Term_ShellCommandHandler(struct APP_DATA_Ttag* ptAppData, char *strCmdBuffer)
{
  uint16_t idx;
  uint16_t num_commands = sizeof ( g_ShellCommands ) / sizeof(APP_TERMINAL_CMD_HANDLER_T);
  char *cmd;

  if ( num_commands )
  {
    /* search for command handler */
    for( idx = 0 ; idx < num_commands ; ++idx )
    {
      memcpy( g_temp_cmd_buff ,  strCmdBuffer , APP_TERMINAL_MAX_CMDLINE_LEN );
      cmd = strtok( g_temp_cmd_buff, " "); //grab shell cmd ( ignore args , if present )

      if ( 0 == strcmp( cmd, g_ShellCommands[ idx ].cmd) )
      {
        if ( g_ShellCommands[ idx ].pf_command )
        {
          g_ShellCommands[ idx ].pf_command(strCmdBuffer);
        }
        else if ( g_ShellCommands[ idx ].adv_pf_command )
        {
          uint32_t i , argc;
          static char *argv[ARGC_MAX];
          const char *pstr;

          pstr = strCmdBuffer;

          // cmd
          argv[0] = strtok((char *)pstr, ", \t\r\n");
          if (argv[0] == '\0') 
          {
            return 1;
          }

          // arguments
          argc = 1;
          for (i = 1; i < ARGC_MAX; i++) 
          {
            argv[i] = strtok(NULL, " \t\r\n");
            if (argv[i] == NULL) 
            {
                break;
            } 
            else 
            {
                argc++;
                //TERM_PRINTF("\r\n arg %lu = \"%s\"\r\n", i, argv[i]);
            }
          }

          g_ShellCommands[ idx ].adv_pf_command(argc, (const char**) &argv[0]);
        }

        return 0;
      }
    }
  }

  return 1; // command not found
}

/*****************************************************************************/
/*! Entry point for terminal handle. Needs to be called cyclically in order to
 *  process incoming terminal commands.
 *
 *   \param ptAppData [in]  Pointer to application data
 */
/*****************************************************************************/
void App_TerminalHandler(APP_DATA_T* ptAppData)
{
  static char   s_szInBuffer[APP_TERMINAL_MAX_CMDLINE_LEN];
  static int    s_iKeyCnt = 0;
  int           iKey      = 0;
  int           iNumberOfArguments     = 0;
  char          szCmd[APP_TERMINAL_MAX_CMD_LEN + 1];   /** plus one for the terminating NULL character */
  static char   szLastCmd[APP_TERMINAL_MAX_CMD_LEN + 1];

  if(HOSTAL_kbhit())
  {
    iKey = HOSTAL_getchar();

    if(iKey == '\r' || iKey == '\n')
    {     
      s_szInBuffer[s_iKeyCnt] = '\0';
      s_iKeyCnt               = 0;

      iNumberOfArguments = sscanf( &s_szInBuffer[0],
                                   "%" XSTR(APP_TERMINAL_MAX_CMD_LEN) "s",
                                   &szCmd[0]);
      
      /* copy last command */
      if (iNumberOfArguments == -1)
      {
        strcpy(szCmd, szLastCmd);
        strcpy(s_szInBuffer, szLastCmd);
      } 
      /* new command */
      else strcpy(szLastCmd, szCmd);

      /* handle command */
      App_AllChannels_TerminalHandler(ptAppData, s_szInBuffer);      
    }
    else
    {
      s_szInBuffer[s_iKeyCnt] = (char)iKey;
      s_iKeyCnt++;

      if(s_iKeyCnt >= APP_TERMINAL_MAX_CMDLINE_LEN)
      {
        s_iKeyCnt = 0;
      }
    }
  }
}
