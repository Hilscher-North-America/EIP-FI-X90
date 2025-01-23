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
 **************************************************************************************
 *      Copyright (c) Hilscher North America.            All Rights Reserved.
 **************************************************************************************
 *      File Name:           App_Application.h
 *      Product:             ETHERNET/IP MIDDLEWARE
 *      Author(s):           Jarek Bojanowicz & Andres Torres
 *      Created on:          XX/XX/XXXX
 *      Updated on:          xx/xx/xxxx
 *      Protocol Stack:      ETHERNET/IP ADAPTER V5.4.0.4
 **************************************************************************************
 * Module Source Description:
 *
 * This file contains the APPLICATION RESOURCES & ETHERNET/IP SLAVE FUNCTIONS
 *
 **************************************************************************************
 *************************************************************************************/


#ifndef COMPONENTS_CIFXAPPLICATION_INCLUDES_APP_APPLICATION_H_
#define COMPONENTS_CIFXAPPLICATION_INCLUDES_APP_APPLICATION_H_

/*****************************************************************************/
/*! General Inclusion Area                                                   */
/*****************************************************************************/

#include <stdio.h>                  /** Include C standard library input/output header */
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "Hil_Compiler.h"
#include "cifXErrors.h"             /** Include cifX driver API error definition */
#include "cifXUser.h"               /** Include cifX driver API definition       */
#include "hostAbstractionLayer.h"
#include "App_EventHandler.h"


/*****************************************************************************/
/*! General Definition Area                                                   */
/*****************************************************************************/

#define MAX_COMMUNICATION_CHANNEL_COUNT   3

/***************************/
#ifdef CONSOLE_MODE
#define PRINTF printf
#else
#define PRINTF(...)
#endif
/***************************/

/*****************************************************************************/
/*! FORWARD DECLARATIONS                                                     */
/*****************************************************************************/

enum   APP_EVENT_Etag;
struct APP_DATA_Ttag;

/*****************************************************************************/
/*! APPLICATION RESOURCES                                               */
/*****************************************************************************/

/** Defines entry points for the protocol specific functions */
typedef struct PROTOCOL_DESCRIPTOR_Ttag
{
  uint32_t (*pfStartChannelConfiguration)(struct APP_DATA_Ttag*);                             /* configure the channel     */
  uint32_t (*pfPacketHandler)(struct APP_DATA_Ttag*);                                         /* handle packets            */
  void     (*pfEventHandler)(struct APP_DATA_Ttag*, enum APP_EVENT_Etag, uint32_t);           /* handle events             */
  uint32_t (*pfTerminalHandler)(struct APP_DATA_Ttag*, char*);                                /* handle terminal input     */

} PROTOCOL_DESCRIPTOR_T;

/** A communication channel contains a channel handle, a packet,
    and the protocol descriptor to be associated by the application
*/
typedef struct APP_COMM_CHANNEL_Ttag
{
  CIFX_PACKET             tPacket;
  CIFXHANDLE              hChannel;         /** Handle of netX DPM communication channel                */
  CHANNEL_INFORMATION     tChannelInfo;     /** DPM channel information. Read during application start. */
  PROTOCOL_DESCRIPTOR_T   tProtocol;
} APP_COMM_CHANNEL_T;

typedef struct APP_DATA_Ttag
{
  APP_COMM_CHANNEL_T       *aptChannels[MAX_COMMUNICATION_CHANNEL_COUNT];
  BOARD_INFORMATION         tBoardInfo;       /** netX Board information. Read during application start.  */

  bool                      fRunning;
  bool                      fInputDataValid;

  APP_PROCESS_DATA_INPUT_T  tInputData;       /** Consumed process data. Data that is received from the PLC. */
  APP_PROCESS_DATA_OUTPUT_T tOutputData;      /** Produced process data. Data that is sent to the PLC.       */
  APP_ACYCLIC_DATA_T        tAcyclicData;     /** Application data available via acyclic accesses.           */

} APP_DATA_T;

#define APP_SHELL_CMDS \
  {"reconfig" , Term_ReconfigCommand, NULL, "Reconfigures the protocol"}, \

/*****************************************************************************/
/*! FUNCTION PROTOTYPES                                                      */
/*****************************************************************************/
uint32_t Term_ReconfigCommand(char* szCommandName);

void App_IODataHandler(void* ptAppResources);
void App_IO_UpdateCycleCounter(APP_DATA_T* ptAppData);

int App_CifXApplication(char* szDeviceName);
void App_Selftest(void);
void App_ReadBoardInfo(const CIFXHANDLE hDriver, char* szBoardName, BOARD_INFORMATION* const ptBoardInfo);
void App_ReadChannelInfo(const CIFXHANDLE hChannel, CHANNEL_INFORMATION* const ptChannelInfo);

int32_t  App_AllChannels_Open(APP_DATA_T* ptAppData, CIFXHANDLE hDriver, char* szBoardName);
uint32_t App_AllChannels_Configure(APP_DATA_T* ptAppData);
uint32_t App_AllChannels_PacketHandler(APP_DATA_T* ptAppData);

void App_AllChannels_EventHandler(APP_DATA_T* ptAppData, enum APP_EVENT_Etag eEvent, uint32_t ulEventData);
bool App_AllChannels_TerminalHandler(APP_DATA_T* ptAppData, char* szTerminalBuffer);
void App_AllChannels_WaitReady(APP_DATA_T* ptAppData);
void App_AllChannels_GetChannelInfo_WaitReady(APP_DATA_T* ptAppData);
void App_AllChannels_Close(APP_DATA_T* ptAppData);

typedef struct PROTOCOL_IDENTIFICATION_Ttag
{
  const char* szFirmwareName;
  const uint16_t usFirmwareMajor;
  const uint16_t usFirmwareMinor;
} PROTOCOL_IDENTIFICATION_T;

extern PROTOCOL_IDENTIFICATION_T g_tProtocolIdentification;

#endif /** COMPONENTS_CIFXAPPLICATION_INCLUDES_APP_APPLICATION_H_ */
