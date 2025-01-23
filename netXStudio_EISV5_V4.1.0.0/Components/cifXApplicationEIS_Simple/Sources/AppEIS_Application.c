/**************************************************************************************
Exclusion of Liability for this demo software:
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

#include "AppEIS_Application.h"
#include "App_Application.h"
#include "App_PacketCommunication.h"
#include "App_SystemPackets.h"
#include "hostAbstractionLayer.h"
#include "Hil_ApplicationCmd.h"
#include "Hil_Results.h"
#include "Hil_Packet.h"
#include "Hil_DualPortMemory.h"
#include "App_EventHandler.h"
#include "EipAps_Public.h"

/*******************************************************************************/

PROTOCOL_IDENTIFICATION_T g_tProtocolIdentification =
{
  .szFirmwareName = "EtherNet/IP Adapter",
  .usFirmwareMajor = 5,
  .usFirmwareMinor = 4,
};

/***************************************************************************************/

#define STR(tok) #tok

static struct
{
  char* szCommandName;
  uint32_t ulCommandCode;
} s_atCommandNameLookupTable[] =
{
  /* Protocol stack independent packets */
  { STR(HIL_CHANNEL_INIT_REQ), HIL_CHANNEL_INIT_REQ },
  { STR(HIL_CHANNEL_INIT_CNF), HIL_CHANNEL_INIT_CNF },

  { STR(HIL_REGISTER_APP_REQ), HIL_REGISTER_APP_REQ },
  { STR(HIL_REGISTER_APP_CNF), HIL_REGISTER_APP_CNF },

  { STR(HIL_UNREGISTER_APP_REQ), HIL_UNREGISTER_APP_REQ },
  { STR(HIL_UNREGISTER_APP_CNF), HIL_UNREGISTER_APP_CNF },

  { STR(HIL_START_STOP_COMM_REQ), HIL_START_STOP_COMM_REQ },
  { STR(HIL_START_STOP_COMM_CNF), HIL_START_STOP_COMM_CNF },

  { STR(HIL_LINK_STATUS_CHANGE_IND), HIL_LINK_STATUS_CHANGE_IND },
  { STR(HIL_LINK_STATUS_CHANGE_RES), HIL_LINK_STATUS_CHANGE_RES },

  { STR(  HIL_SET_REMANENT_DATA_REQ), HIL_SET_REMANENT_DATA_REQ },
  { STR(  HIL_SET_REMANENT_DATA_CNF), HIL_SET_REMANENT_DATA_CNF },

  /* Protocol stack specific packets */
  { STR(EIP_APS_SET_CONFIGURATION_PARAMETERS_REQ), EIP_APS_SET_CONFIGURATION_PARAMETERS_REQ },
  { STR(EIP_APS_SET_CONFIGURATION_PARAMETERS_CNF), EIP_APS_SET_CONFIGURATION_PARAMETERS_CNF },

  { STR(EIP_OBJECT_MR_REGISTER_REQ), EIP_OBJECT_MR_REGISTER_REQ },
  { STR(EIP_OBJECT_MR_REGISTER_CNF), EIP_OBJECT_MR_REGISTER_CNF },

  { STR(HIL_SET_HANDSHAKE_CONFIG_REQ), HIL_SET_HANDSHAKE_CONFIG_REQ },
  { STR(HIL_SET_HANDSHAKE_CONFIG_CNF), HIL_SET_HANDSHAKE_CONFIG_CNF },

  { STR(EIP_OBJECT_CIP_SERVICE_REQ), EIP_OBJECT_CIP_SERVICE_REQ },
  { STR(EIP_OBJECT_CIP_SERVICE_CNF), EIP_OBJECT_CIP_SERVICE_CNF },

  { STR(EIP_OBJECT_CONNECTION_IND), EIP_OBJECT_CONNECTION_IND },
  { STR(EIP_OBJECT_CONNECTION_RES), EIP_OBJECT_CONNECTION_RES },

  { STR(EIP_OBJECT_RESET_IND), EIP_OBJECT_RESET_IND },
  { STR(EIP_OBJECT_RESET_RES), EIP_OBJECT_RESET_RES },

  { STR(EIP_OBJECT_CL3_SERVICE_IND), EIP_OBJECT_CL3_SERVICE_IND },
  { STR(EIP_OBJECT_CL3_SERVICE_RES), EIP_OBJECT_CL3_SERVICE_RES },

  { STR(EIP_OBJECT_CIP_OBJECT_CHANGE_IND), EIP_OBJECT_CIP_OBJECT_CHANGE_IND },
  { STR(EIP_OBJECT_CIP_OBJECT_CHANGE_RES), EIP_OBJECT_CIP_OBJECT_CHANGE_RES },

  { STR(HIL_DELETE_CONFIG_REQ), HIL_DELETE_CONFIG_REQ },
  { STR(HIL_DELETE_CONFIG_CNF), HIL_DELETE_CONFIG_CNF },

  { STR(EIP_OBJECT_AS_REGISTER_REQ), EIP_OBJECT_AS_REGISTER_REQ },
  { STR(EIP_OBJECT_AS_REGISTER_CNF), EIP_OBJECT_AS_REGISTER_CNF },

  { STR(EIP_APS_CONFIG_DONE_REQ), EIP_APS_CONFIG_DONE_REQ },
  { STR(EIP_APS_CONFIG_DONE_CNF), EIP_APS_CONFIG_DONE_CNF },

  /* This always must be the last entry in this table */
  { "Unknown command", 0xffffffff },
};

/*******************************************************************************/

extern char* LookupCommandCode(uint32_t ulCmd)
{
  int i;
  for (i = 0; i < sizeof(s_atCommandNameLookupTable) / sizeof(s_atCommandNameLookupTable[0]); i++)
  {
    if (s_atCommandNameLookupTable[i].ulCommandCode == ulCmd)
    {
      return s_atCommandNameLookupTable[i].szCommandName;
    }
  }

  /* Command not in lookup table: Return last entry of table */
  return s_atCommandNameLookupTable[i - 1].szCommandName;
}

/*******************************************************************************/

static uint32_t Protocol_StartConfiguration( APP_DATA_T* ptAppData )
{
  uint32_t ulRet = CIFX_NO_ERROR;

  if (!Pkt_Init(EIP_DEMO_CHANNEL_INDEX, 1))
  {
    return CIFX_DRV_INIT_ERROR;
  }

  /* Register handler for indication packets. */
  if( Pkt_RegisterIndicationHandler( EIP_DEMO_CHANNEL_INDEX, AppEIS_PacketHandler, (void*)ptAppData ) )
  {

#ifdef HOST_APPLICATION_SETS_MAC_ADDRESS
      /* optional when initial DDP state is passive: set DDP base device parameters: MAC address */
      ulRet = AppEIS_SetMacAddress( ptAppData );
      if( ulRet != CIFX_NO_ERROR )
      {
        return ulRet;
      }
#endif

#ifdef HOST_APPLICATION_SETS_SERIAL_NUMBER
      /* optional when initial DDP state is passive: set additional (OEM) DDP base device parameters: serial number */
      ulRet = AppEIS_SetOemDeviceData( ptAppData );
      if( ulRet != CIFX_NO_ERROR )
      {
        return ulRet;
      }
#endif

#if defined(HOST_APPLICATION_SETS_MAC_ADDRESS) || defined(HOST_APPLICATION_SETS_SERIAL_NUMBER)
      ulRet = AppEIS_ActivateDdp( ptAppData );
      if( ulRet != CIFX_NO_ERROR )
      {
        return ulRet;
      }
#endif


    /* Register application */
    App_SysPkt_AssembleRegisterAppReq( &ptAppData->aptChannels[EIP_DEMO_CHANNEL_INDEX]->tPacket );
    ulRet = Pkt_SendReceivePacket( ptAppData, EIP_DEMO_CHANNEL_INDEX, &ptAppData->aptChannels[EIP_DEMO_CHANNEL_INDEX]->tPacket, TXRX_TIMEOUT );
    if( ulRet != CIFX_NO_ERROR )
      return ulRet;

    /* Download the configuration */
    ulRet = AppEIS_ConfigureStack( ptAppData );
  }
  else
  {
    ulRet = CIFX_DRV_INIT_ERROR;
  }

  return ulRet;
}

/*******************************************************************/

static uint32_t Protocol_PacketHandler( APP_DATA_T* ptAppData )
{
  uint32_t ulRet = CIFX_NO_ERROR;

  ulRet = Pkt_CheckReceiveMailbox( ptAppData, EIP_DEMO_CHANNEL_INDEX, &ptAppData->aptChannels[EIP_DEMO_CHANNEL_INDEX]->tPacket );

  if( CIFX_DEV_GET_NO_PACKET == ulRet || CIFX_DEV_NOT_READY == ulRet )
  {
    /* Handle "no packet" and "stack not ready" as success. */
    ulRet = CIFX_NO_ERROR;
  }

  return ulRet;
}

/*****************************************************************************/
/*! Protocol special event handler
*   \param ptAppData   pointer to APP_DATA_T structure
*   \param eEvent      event type                                            */
/*****************************************************************************/
static void Protocol_EventHandler( APP_DATA_T* ptAppData,
                                   APP_EVENT_E eEvent,
                                   uint32_t    ulEventData )
{
  switch(eEvent)
  {
    case APP_EVENT_SET_ALARM1:
      break;
    case APP_EVENT_CLEAR_ALARM1:
      break;

    case APP_EVENT_SET_ALARM2:
      break;
    case APP_EVENT_CLEAR_ALARM2:
      break;

    case APP_EVENT_SET_ALARM3:
      break;
    case APP_EVENT_CLEAR_ALARM3:
      break;

    case APP_EVENT_SET_ALARM4:
      break;
    case APP_EVENT_CLEAR_ALARM4:
      break;

    case APP_EVENT_SET_ERROR1:
      break;
    case APP_EVENT_CLEAR_ERROR1:
      break;

    case APP_EVENT_SET_ERROR2:
      break;
    case APP_EVENT_CLEAR_ERROR2:
      break;

    case APP_EVENT_SET_ERROR3:
      break;
    case APP_EVENT_CLEAR_ERROR3:
      break;

    case APP_EVENT_SET_ERROR4:
      break;
    case APP_EVENT_CLEAR_ERROR4:
      break;

    default:
      break;
  }
}

static uint32_t Protocol_TerminalHandler(struct APP_DATA_Ttag* ptAppData, char *szBuffer)
{
  if (0 == strcmp(szBuffer, "help"))
  {
    PRINTF("Hello from the EIS protocol part: no terminal commands implemented yet" NEWLINE);
  }

  return 0;
}

PROTOCOL_DESCRIPTOR_T g_tRealtimeProtocolHandlers =
{
  .pfStartChannelConfiguration = Protocol_StartConfiguration,
  .pfPacketHandler             = Protocol_PacketHandler,
  .pfEventHandler              = Protocol_EventHandler,
  .pfTerminalHandler           = Protocol_TerminalHandler,
};

uint32_t Socket_StartConfiguration( APP_DATA_T* ptAppData )
{
  uint32_t ulRet = CIFX_NO_ERROR;

  return ulRet;
}

PROTOCOL_DESCRIPTOR_T g_tNetworkServicesHandlers =
{
  .pfStartChannelConfiguration = Socket_StartConfiguration,
  .pfPacketHandler             = NULL,
  .pfEventHandler              = NULL,
  .pfTerminalHandler           = NULL,
};
