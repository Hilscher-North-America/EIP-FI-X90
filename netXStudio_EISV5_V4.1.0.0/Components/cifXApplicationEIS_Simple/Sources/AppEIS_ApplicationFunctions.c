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
/* Includes */

#include <stdio.h>

#include "AppEIS_Application.h"
#include "App_Application.h"
#include "App_PacketCommunication.h"
#include "App_SystemPackets.h"
#include "hostAbstractionLayer.h"
#include "Hil_ApplicationCmd.h"
#include "Hil_Results.h"
#include "Hil_Packet.h"
#include "Hil_DualPortMemory.h"
#include "Hil_SystemCmd.h"
#include "App_EventHandler.h"

#include "cip_id.h"
#include "eip_tcp.h"
#include "eip_en.h"
#include "eip_qos.h"
#include "cip_as.h"
#include "cip_pdc.h"
#include "cip_common.h"
#include "EipAps_Public.h"

/*****************************************************************************************************************************************/
/*****************************************************************************************************************************************/
/* Configuration related definitions */

#define IP_ADDRESS(a,b,c,d) (uint32_t)(((a) << 24) | ((b) << 16) | ((c) << 8) | (d))

/*****************************************************************************************************************************************/
/* CIP Identity Object - Class 0x01 - Configuration                                                                                      */
/*****************************************************************************************************************************************/

#define      VENDOR_ID                            CIP_VENDORID_HILSCHER
#define      DEVICE_TYPE_COMMUNICATION_ADAPTER    0x0C   /* CIP Profile - "Communication Adapter" */
#define      PRODUCT_CODE                         PRODUCT_CODE_EIS_SIMPLE_DEMO_2
#define      MAJOR_REVISION                       1
#define      MINOR_REVISION                       1

static char  s_abProductName[] = "EIS_V5_SIMPLE_CONFIG_DEMO";
static char  s_abDomainName[]  = "";
static char  s_abHostName[]    = "";

/*************************************************************************************/
/* Assembly instances that shall be supported                                        */
/*************************************************************************************/

#define EIP_DEMO_CONSUMING_ASSEMBLY_INSTANCE            0x64
#define EIP_DEMO_CONSUMING_ASSEMBLY_INSTANCE_SIZE       sizeof(APP_PROCESS_DATA_INPUT_T)

#define EIP_DEMO_PRODUCING_ASSEMBLY_INSTANCE            0x65
#define EIP_DEMO_PRODUCING_ASSEMBLY_INSTANCE_SIZE       sizeof(APP_PROCESS_DATA_OUTPUT_T)

/**************************************************************************************/
/**************************************************************************************/

//#define STATIC_IP IP_ADDRESS(192,168,210,10)

uint32_t AppEIS_SendSetConfigurationParameters( APP_DATA_T* ptAppData )
{
  uint32_t                                           ulRet = CIFX_NO_ERROR;
  EIP_APS_PACKET_SET_CONFIGURATION_PARAMETERS_REQ_T* ptReq = (EIP_APS_PACKET_SET_CONFIGURATION_PARAMETERS_REQ_T *)(&ptAppData->aptChannels[EIP_DEMO_CHANNEL_INDEX]->tPacket);

  memset(&ptReq->tHead, 0, sizeof(ptReq->tHead));

  ptReq->tHead.ulDest             = HIL_PACKET_DEST_DEFAULT_CHANNEL;
  ptReq->tHead.ulLen              = sizeof(ptReq->tData);
  ptReq->tHead.ulCmd              = EIP_APS_SET_CONFIGURATION_PARAMETERS_REQ;
  ptReq->tData.ulParameterVersion = EIP_APS_CONFIGURATION_PARAMETER_SET_V3;

  ptReq->tData.unConfig.tV3.ulSystemFlags = 0;
  ptReq->tData.unConfig.tV3.ulWdgTime     = 100;

#ifndef STATIC_IP
  ptReq->tData.unConfig.tV3.ulTcpFlag = IP_CFG_FLAG_AUTO_NEGOTIATE | IP_CFG_FLAG_DHCP;
  /* Set to 0 we use DHCP */
  ptReq->tData.unConfig.tV3.ulIpAddr  = 0;
  ptReq->tData.unConfig.tV3.ulGateway = 0;
  ptReq->tData.unConfig.tV3.ulNetMask = 0;
#else
  ptReq->tData.unConfig.tV3.ulTcpFlag = (IP_CFG_FLAG_AUTO_NEGOTIATE | IP_CFG_FLAG_IP_ADDR | IP_CFG_FLAG_NET_MASK | IP_CFG_FLAG_GATEWAY);
  /* Set to 0 we use DHCP */
  ptReq->tData.unConfig.tV3.ulIpAddr  = STATIC_IP;
  ptReq->tData.unConfig.tV3.ulNetMask = IP_ADDRESS(255,255,255,0);
  ptReq->tData.unConfig.tV3.ulGateway = (STATIC_IP & ptReq->tData.unConfig.tV3.ulNetMask) | 0x64;
#endif

  ptReq->tData.unConfig.tV3.usVendId        = VENDOR_ID;
  ptReq->tData.unConfig.tV3.usProductType   = DEVICE_TYPE_COMMUNICATION_ADAPTER;
  ptReq->tData.unConfig.tV3.usProductCode   = PRODUCT_CODE;

  /* The serial number attribute is not settable anymore via CIP means.
     A user serial number may be set into the DDP (Device Data Provider) in a taglist-modified firmware (DDP_PASSIVE).
     Per default, the EtherNet/IP firmware will use the serial number located in the FDL (Flash Device Label) */
  ptReq->tData.unConfig.tV3.ulSerialNumber  = 0;

  ptReq->tData.unConfig.tV3.bMinorRev       = MINOR_REVISION;
  ptReq->tData.unConfig.tV3.bMajorRev       = MAJOR_REVISION;

  /* type CIP_SHORTSTRING: set length indicator in first byte */
  /* terminating NUL-byte is not respected here */
  ptReq->tData.unConfig.tV3.abDeviceName[0] = sizeof(s_abProductName) - 1;

  /* and the actual bytes following the length indicator */
  memcpy( &ptReq->tData.unConfig.tV3.abDeviceName[1],
          &s_abProductName[0],
          ptReq->tData.unConfig.tV3.abDeviceName[0] );

  /* type CIP_STRING: set length indicator in the first two bytes */
  /* terminating NUL-byte is not respected here */
  ptReq->tData.unConfig.tV3.abDomainName[0] = sizeof(s_abDomainName) - 1;
  ptReq->tData.unConfig.tV3.abDomainName[1] = 0;

  /* and the actual bytes following the length indicator
     (the stack will take care of the padding for you in case of basic configuration)
  */
  memcpy( &ptReq->tData.unConfig.tV3.abDomainName[2],
          &s_abDomainName[0],
          ptReq->tData.unConfig.tV3.abDomainName[0] );

  /* type CIP_STRING: set length indicator in the first two bytes */
  /* terminating NUL-byte is not respected here */
  ptReq->tData.unConfig.tV3.abHostName[0] = sizeof(s_abHostName) - 1;
  ptReq->tData.unConfig.tV3.abHostName[1] = 0;

  /* and the actual bytes following the length indicator
     (the stack will take care of the padding for you in case of basic configuration)
  */
  memcpy( &ptReq->tData.unConfig.tV3.abHostName[2],
          &s_abHostName[0],
          ptReq->tData.unConfig.tV3.abHostName[0] );


  ptReq->tData.unConfig.tV3.bQuickConnectFlags = 0; /* Quick Connect not supported yet */

  ptReq->tData.unConfig.tV3.bSelectAcd         = 1; /* Address Conflict Detection activated by default */

  memset( &ptReq->tData.unConfig.tV3.tLastConflictDetected,
          0,
          sizeof(ptReq->tData.unConfig.tV3.tLastConflictDetected) );

  memset( &ptReq->tData.unConfig.tV3.tQoS_Config,
          0,
          sizeof(ptReq->tData.unConfig.tV3.tQoS_Config) );

  ptReq->tData.unConfig.tV3.tQoS_Config.bTag802Enable     = EIP_QOS_TAG_DISABLED;
  ptReq->tData.unConfig.tV3.tQoS_Config.bDSCP_PTP_Event   = EIP_QOS_DSCP_PTP_EVENT_DEFAULT;
  ptReq->tData.unConfig.tV3.tQoS_Config.bDSCP_PTP_General = EIP_QOS_DSCP_PTP_GENERAL_DEFAULT;
  ptReq->tData.unConfig.tV3.tQoS_Config.bDSCP_Urgent      = EIP_QOS_DSCP_URGENT_DEFAULT;
  ptReq->tData.unConfig.tV3.tQoS_Config.bDSCP_Scheduled   = EIP_QOS_DSCP_SCHEDULED_DEFAULT;
  ptReq->tData.unConfig.tV3.tQoS_Config.bDSCP_High        = EIP_QOS_DSCP_HIGH_DEFAULT;
  ptReq->tData.unConfig.tV3.tQoS_Config.bDSCP_Low         = EIP_QOS_DSCP_LOW_DEFAULT;
  ptReq->tData.unConfig.tV3.tQoS_Config.bDSCP_Explicit    = EIP_QOS_DSCP_EXPLICIT_DEFAULT;

  /* Assembly instance 100 (O2T): Run/idle header format  */
  ptReq->tData.unConfig.tV3.ulInputAssInstance            = EIP_DEMO_CONSUMING_ASSEMBLY_INSTANCE;
  ptReq->tData.unConfig.tV3.ulInputLen                    = EIP_DEMO_CONSUMING_ASSEMBLY_INSTANCE_SIZE; /* Maximum value is 504 */
  ptReq->tData.unConfig.tV3.ulInputAssFlags               = EIP_AS_TYPE_INPUT | EIP_AS_OPTION_FIXEDSIZE;

  /* Assembly instance 101 (T2O): modeless format  */
  ptReq->tData.unConfig.tV3.ulOutputAssInstance           = EIP_DEMO_PRODUCING_ASSEMBLY_INSTANCE;
  ptReq->tData.unConfig.tV3.ulOutputLen                   = EIP_DEMO_PRODUCING_ASSEMBLY_INSTANCE_SIZE; /* Maximum value is 504 */
  ptReq->tData.unConfig.tV3.ulOutputAssFlags              = EIP_AS_OPTION_NO_RUNIDLE | EIP_AS_OPTION_FIXEDSIZE;

  ptReq->tData.unConfig.tV3.ulNameServer                  = 0;
  ptReq->tData.unConfig.tV3.ulNameServer_2                = 0;

  ptReq->tData.unConfig.tV3.bTTLValue                     = 1;

  ptReq->tData.unConfig.tV3.tMCastConfig.bAllocControl    = EIP_TCP_ALLOCCONTROL_DEFAULT;
  ptReq->tData.unConfig.tV3.tMCastConfig.ulMcastStartAddr = 0;
  ptReq->tData.unConfig.tV3.tMCastConfig.usNumMCast       = 0;

  ptReq->tData.unConfig.tV3.abAdminState[0]               = 1; /* Ethernet Port 1 enabled */
  ptReq->tData.unConfig.tV3.abAdminState[1]               = 1; /* Ethernet Port 2 enabled */

  ptReq->tData.unConfig.tV3.usEncapInactivityTimer        = EIP_TCP_ENCAP_TIMEOUT_DEFAULT;

  ulRet = Pkt_SendReceivePacket( ptAppData, EIP_DEMO_CHANNEL_INDEX, &ptAppData->aptChannels[EIP_DEMO_CHANNEL_INDEX]->tPacket, TXRX_TIMEOUT );

  return (ulRet != CIFX_NO_ERROR) ? ulRet : ptAppData->aptChannels[EIP_DEMO_CHANNEL_INDEX]->tPacket.tHeader.ulState;
}

/**************************************************************************************/
/**************************************************************************************/

uint32_t AppEIS_ChannelInit(APP_DATA_T* ptAppData)
{
  uint32_t ulRet;
  App_SysPkt_AssembleChannelInitReq( &ptAppData->aptChannels[EIP_DEMO_CHANNEL_INDEX]->tPacket );
  ulRet = Pkt_SendReceivePacket( ptAppData, EIP_DEMO_CHANNEL_INDEX, &ptAppData->aptChannels[EIP_DEMO_CHANNEL_INDEX]->tPacket, TXRX_TIMEOUT);
  return (ulRet != CIFX_NO_ERROR) ? ulRet : ptAppData->aptChannels[EIP_DEMO_CHANNEL_INDEX]->tPacket.tHeader.ulState;
}

/**************************************************************************************/
/**************************************************************************************/

/* A delete config request will delete the configuration and remanent data.
*/
uint32_t AppEIS_DeleteConfig(APP_DATA_T* ptAppData)
{
  uint32_t ulRet;

  memset( &ptAppData->aptChannels[EIP_DEMO_CHANNEL_INDEX]->tPacket.tHeader,
          0,
          sizeof(ptAppData->aptChannels[EIP_DEMO_CHANNEL_INDEX]->tPacket.tHeader) );

  ptAppData->aptChannels[EIP_DEMO_CHANNEL_INDEX]->tPacket.tHeader.ulDest = HIL_PACKET_DEST_DEFAULT_CHANNEL;
  ptAppData->aptChannels[EIP_DEMO_CHANNEL_INDEX]->tPacket.tHeader.ulCmd  = HIL_DELETE_CONFIG_REQ;
  ptAppData->aptChannels[EIP_DEMO_CHANNEL_INDEX]->tPacket.tHeader.ulLen  = 0;

  ulRet = Pkt_SendReceivePacket( ptAppData, EIP_DEMO_CHANNEL_INDEX, &ptAppData->aptChannels[EIP_DEMO_CHANNEL_INDEX]->tPacket, TXRX_TIMEOUT);
  return (ulRet != CIFX_NO_ERROR) ? ulRet : ptAppData->aptChannels[EIP_DEMO_CHANNEL_INDEX]->tPacket.tHeader.ulState;
}
/**************************************************************************************/
/**************************************************************************************/

/* Download the configuration */
uint32_t AppEIS_ConfigureStack( APP_DATA_T* ptAppData )
{
  uint32_t ulRet = CIFX_NO_ERROR;

  /* Send EIP_APS_SET_CONFIGURATION_PARAMETERS_REQ */
  ulRet = AppEIS_SendSetConfigurationParameters( ptAppData );
  if( ulRet != 0 )
    return ulRet;

  /* Send HIL_CHANNEL_INIT_REQ */
  ulRet = AppEIS_ChannelInit(ptAppData);
  if (ulRet != 0)
    return ulRet;

  /* Set BUS_ON */
  App_SysPkt_AssembleStartStopCommReq( &ptAppData->aptChannels[EIP_DEMO_CHANNEL_INDEX]->tPacket, true );
  ulRet = Pkt_SendReceivePacket( ptAppData, EIP_DEMO_CHANNEL_INDEX, &ptAppData->aptChannels[EIP_DEMO_CHANNEL_INDEX]->tPacket, TXRX_TIMEOUT );
  ulRet = (ulRet != CIFX_NO_ERROR) ? ulRet : ptAppData->aptChannels[EIP_DEMO_CHANNEL_INDEX]->tPacket.tHeader.ulState;

  return ulRet;
}

/**************************************************************************************/
/**************************************************************************************/

static void AppEIS_HandleResetInd( APP_DATA_T* ptAppData )
{
  uint32_t                        ulRet = CIFX_NO_ERROR;
  EIP_OBJECT_PACKET_RESET_IND_T*  ptInd = (EIP_OBJECT_PACKET_RESET_IND_T*) (&ptAppData->aptChannels[EIP_DEMO_CHANNEL_INDEX]->tPacket);

  /* keep a copy of the indicationon on the stack,
  so we can reuse the packet in the meantime */
  EIP_OBJECT_PACKET_RESET_IND_T tResetInd = *ptInd;

  PRINTF("Reset Type %i Indication" NEWLINE, (int)ptInd->tData.ulResetTyp);

  if( ptInd->tData.ulResetTyp == 1 )
  {
    /* Type 1 reset -> We are supposed to restore the factory defaults in NV storage, so
       we send a DELETE_CONFIG_REQ to delete the remanent data (i.e. render it invalid)
    */
    ulRet = AppEIS_DeleteConfig(ptAppData);
    if (ulRet != CIFX_NO_ERROR)
    {
      PRINTF("HIL_CONFIG_DELETE_REQ failed with error code 0x%08x" NEWLINE, (unsigned int)ulRet);
    }
  }

  /* Respond to the reset indication (this will trigger the reply on the network) */
  ptInd = (EIP_OBJECT_PACKET_RESET_IND_T*)&ptAppData->aptChannels[EIP_DEMO_CHANNEL_INDEX]->tPacket;
  *ptInd = tResetInd;
  ptInd->tHead.ulLen = EIP_OBJECT_RESET_RES_SIZE;
  ptInd->tHead.ulSta = SUCCESS_HIL_OK;
  ptInd->tHead.ulCmd |= 0x01; /* Make it a response */

  /* Send reset response */
  ulRet = Pkt_SendPacket( ptAppData, EIP_DEMO_CHANNEL_INDEX, (CIFX_PACKET *) ptInd, TX_TIMEOUT);

  if( ulRet != CIFX_NO_ERROR )
  {
    PRINTF("Sending EIP_OBJECT_RESET_RES failed with error code 0x%08x" NEWLINE, (unsigned int)ulRet);
  }

  /* Wait a while before starting re-configuration. Actually not necessary, but
     starting re-configuration quickly sometimes bothers the Conformance Test Tool, since the TCP connection
     is closed before the tool actually processed the reset service response. */
  HOSTAL_Sleep( 1000 ); /* 1 second */

  /* Reconfigure the stack */
  AppEIS_ConfigureStack( ptAppData );
}

/**************************************************************************************/
/**************************************************************************************/

static void AppEIS_HandleConnectionInd(APP_DATA_T* ptAppData)
{
  EIP_OBJECT_PACKET_CONNECTION_IND_T* ptInd = (EIP_OBJECT_PACKET_CONNECTION_IND_T*) (&ptAppData->aptChannels[EIP_DEMO_CHANNEL_INDEX]->tPacket);

  PRINTF("Connection changed: Type %d, State %d" NEWLINE, (int)ptInd->tData.bConnType, (int)ptInd->tData.ulConnectionState );

  PRINTF("Config path: class %d, instance %d, connpoint %d, attrib %d, member %d" NEWLINE,
    (int)ptInd->tData.tConfigPath.ulClass,
    (int)ptInd->tData.tConfigPath.ulInstance,
    (int)ptInd->tData.tConfigPath.ulConnPoint,
    (int)ptInd->tData.tConfigPath.ulAttribute,
    (int)ptInd->tData.tConfigPath.ulMember);

  PRINTF("Consumption path: class %d, instance %d, connpoint %d, attrib %d, member %d" NEWLINE,
    (int)ptInd->tData.tConsumptionPath.ulClass,
    (int)ptInd->tData.tConsumptionPath.ulInstance,
    (int)ptInd->tData.tConsumptionPath.ulConnPoint,
    (int)ptInd->tData.tConsumptionPath.ulAttribute,
    (int)ptInd->tData.tConsumptionPath.ulMember);

  PRINTF("Production path: class %d, instance %d, connpoint %d, attrib %d, member %d" NEWLINE,
    (int)ptInd->tData.tProductionPath.ulClass,
    (int)ptInd->tData.tProductionPath.ulInstance,
    (int)ptInd->tData.tProductionPath.ulConnPoint,
    (int)ptInd->tData.tProductionPath.ulAttribute,
    (int)ptInd->tData.tProductionPath.ulMember);

  ptInd->tHead.ulLen  = 0;
  ptInd->tHead.ulSta  = SUCCESS_HIL_OK;
  ptInd->tHead.ulCmd |= 0x01; /* Make it a response */

  (void)Pkt_SendPacket( ptAppData, EIP_DEMO_CHANNEL_INDEX, (CIFX_PACKET *) ptInd, TX_TIMEOUT );
}

/**************************************************************************************/
/**************************************************************************************/

static void AppEIS_HandleObjChangeInd( APP_DATA_T* ptAppData )
{
  EIP_OBJECT_PACKET_CIP_OBJECT_CHANGE_IND_T* ptInd = (EIP_OBJECT_PACKET_CIP_OBJECT_CHANGE_IND_T*) (&ptAppData->aptChannels[EIP_DEMO_CHANNEL_INDEX]->tPacket);

  PRINTF( "Object Changed: Class %x, Instance %d, Attribute %d, Member %d " NEWLINE,
          (int) ptInd->tData.ulClass,
          (int) ptInd->tData.ulInstance,
          (int) ptInd->tData.ulAttribute,
          (int) ptInd->tData.ulMember );

  /* Send object change response packet. */
  ptInd->tHead.ulSta  = SUCCESS_HIL_OK;
  ptInd->tHead.ulCmd |= 0x01; /* Make it a response */

  (void)Pkt_SendPacket( ptAppData, EIP_DEMO_CHANNEL_INDEX, (CIFX_PACKET *) ptInd, TX_TIMEOUT );
}

/**************************************************************************************/
/**************************************************************************************/

uint32_t AppEIS_SetMacAddress( APP_DATA_T* ptAppData )
{
  uint32_t                   ulRet = CIFX_NO_ERROR;
  HIL_DDP_SERVICE_SET_REQ_T* ptReq = (HIL_DDP_SERVICE_SET_REQ_T*)&ptAppData->aptChannels[EIP_DEMO_CHANNEL_INDEX]->tPacket;

  memset(&ptReq->tHead, 0, sizeof(ptReq->tHead));

  uint8_t abMyComMacAddresses[8][6] =
  {
    { 0xa, 0xb, 0xc, 0xd, 0xe, 0x0 },   /* This is the first chassis MAC address which is used by EtherNet/IP */
    { 0xa, 0xb, 0xc, 0xd, 0xe, 0x1 },   /* Port 0 MAC Address used for LLDP                                   */
    { 0xa, 0xb, 0xc, 0xd, 0xe, 0x2 },   /* Port 1 MAC Address used for LLDP                                   */
    { 0xa, 0xb, 0xc, 0xd, 0xe, 0x3 },   /* This is the second chassis MAC (RawEthernet/NDIS)                  */
    { 0xa, 0xb, 0xc, 0xd, 0xe, 0x4 },
    { 0xa, 0xb, 0xc, 0xd, 0xe, 0x5 },
    { 0xa, 0xb, 0xc, 0xd, 0xe, 0x6 },
    { 0xa, 0xb, 0xc, 0xd, 0xe, 0x7 },
  };

  ptReq->tHead.ulDest = HIL_PACKET_DEST_DEFAULT_CHANNEL;
  ptReq->tHead.ulCmd  = HIL_DDP_SERVICE_SET_REQ;
  ptReq->tHead.ulLen  = sizeof(ptReq->tData.ulDataType) + sizeof(abMyComMacAddresses);

  /* Set MAC adrress for the protocol stack (override pre-defined MAC address from FDL) */
  ptReq->tData.ulDataType = HIL_DDP_SERVICE_DATATYPE_MAC_ADDRESSES_COM;

  memcpy(ptReq->tData.uDataType.atMacAddress, abMyComMacAddresses, sizeof(abMyComMacAddresses));

  ulRet = Pkt_SendReceivePacket( ptAppData, EIP_DEMO_CHANNEL_INDEX, &ptAppData->aptChannels[EIP_DEMO_CHANNEL_INDEX]->tPacket, TXRX_TIMEOUT );
  return (ulRet != CIFX_NO_ERROR) ? ulRet : ptAppData->aptChannels[EIP_DEMO_CHANNEL_INDEX]->tPacket.tHeader.ulState;
}

/**************************************************************************************/
/**************************************************************************************/

uint32_t AppEIS_SetOemDeviceData( APP_DATA_T* ptAppData )
{
  uint32_t                   ulRet          = CIFX_NO_ERROR;
  HIL_DDP_SERVICE_SET_REQ_T* ptReq          = (HIL_DDP_SERVICE_SET_REQ_T*)&ptAppData->aptChannels[EIP_DEMO_CHANNEL_INDEX]->tPacket;
  char*                      szSerialNumber = "76543";

  memset(&ptReq->tHead, 0, sizeof(ptReq->tHead));

  ptReq->tHead.ulDest     = HIL_PACKET_DEST_DEFAULT_CHANNEL;
  ptReq->tHead.ulCmd      = HIL_DDP_SERVICE_SET_REQ;
  ptReq->tHead.ulLen      = sizeof(ptReq->tData.ulDataType) + strlen(szSerialNumber) + 1;
  ptReq->tData.ulDataType = HIL_DDP_SERVICE_DATATYPE_OEM_SERIALNUMBER;

  memcpy(ptReq->tData.uDataType.szString, szSerialNumber, strlen(szSerialNumber) + 1);

  ulRet = Pkt_SendReceivePacket( ptAppData, EIP_DEMO_CHANNEL_INDEX, &ptAppData->aptChannels[EIP_DEMO_CHANNEL_INDEX]->tPacket, TXRX_TIMEOUT );

  /* Check status of cifX function and confirmation packet */
  ulRet = (ulRet != CIFX_NO_ERROR) ? ulRet : ptAppData->aptChannels[EIP_DEMO_CHANNEL_INDEX]->tPacket.tHeader.ulState;
  if( CIFX_NO_ERROR != ulRet )
  {
    return ulRet;
  }

  /* order number */
  char* szOrderNum = "34567";
  ptReq->tHead.ulCmd = HIL_DDP_SERVICE_SET_REQ;
  ptReq->tHead.ulLen = sizeof(ptReq->tData.ulDataType) + strlen(szOrderNum) + 1;
  ptReq->tData.ulDataType = HIL_DDP_SERVICE_DATATYPE_OEM_ORDERNUMBER;
  memcpy(ptReq->tData.uDataType.szString, szOrderNum, strlen(szOrderNum) + 1);
  ulRet = Pkt_SendReceivePacket(ptAppData, EIP_DEMO_CHANNEL_INDEX, &ptAppData->aptChannels[EIP_DEMO_CHANNEL_INDEX]->tPacket, TXRX_TIMEOUT);
  if (CIFX_NO_ERROR != ulRet)
  {
    return ulRet;
  }

  /* hardware revision */
  char* szHwRev = "123";
  ptReq->tHead.ulCmd = HIL_DDP_SERVICE_SET_REQ;
  ptReq->tHead.ulLen = sizeof(ptReq->tData.ulDataType) + strlen(szHwRev) + 1;
  ptReq->tData.ulDataType = HIL_DDP_SERVICE_DATATYPE_OEM_HARDWAREREVISION;
  memcpy(ptReq->tData.uDataType.szString, szHwRev, strlen(szHwRev) + 1);
  ulRet = Pkt_SendReceivePacket(ptAppData, EIP_DEMO_CHANNEL_INDEX, &ptAppData->aptChannels[EIP_DEMO_CHANNEL_INDEX]->tPacket, TXRX_TIMEOUT);
  if (CIFX_NO_ERROR != ulRet)
  {
    return ulRet;
  }

  /* also render the OEM parameters "valid" in the corresponding bitfield */
  ptReq->tHead.ulDest            = HIL_PACKET_DEST_DEFAULT_CHANNEL;
  ptReq->tHead.ulCmd             = HIL_DDP_SERVICE_SET_REQ;
  ptReq->tHead.ulLen             = sizeof(ptReq->tData.ulDataType) + sizeof(ptReq->tData.uDataType.ulValue);
  ptReq->tData.ulDataType        = HIL_DDP_SERVICE_DATATYPE_OEM_OPTIONS;
  ptReq->tData.uDataType.ulValue = 0xF;    /* set bits to render the three parameterized OEM dates valid (they have to b set at once) */

  ulRet = Pkt_SendReceivePacket( ptAppData, EIP_DEMO_CHANNEL_INDEX, &ptAppData->aptChannels[EIP_DEMO_CHANNEL_INDEX]->tPacket, TXRX_TIMEOUT );

  /* Check status of cifX function and confirmation packet */
  return (ulRet != CIFX_NO_ERROR) ? ulRet : ptAppData->aptChannels[EIP_DEMO_CHANNEL_INDEX]->tPacket.tHeader.ulState;
}

/**************************************************************************************/
/**************************************************************************************/

uint32_t AppEIS_ActivateDdp( APP_DATA_T* ptAppData )
{
  uint32_t                   ulRet = CIFX_NO_ERROR;
  HIL_DDP_SERVICE_SET_REQ_T* ptReq = (HIL_DDP_SERVICE_SET_REQ_T*)&ptAppData->aptChannels[EIP_DEMO_CHANNEL_INDEX]->tPacket;

  memset(&ptReq->tHead, 0, sizeof(ptReq->tHead));

  /* required when inital DPP state is passive: Set DDP active now */
  ptReq->tHead.ulDest            = HIL_PACKET_DEST_DEFAULT_CHANNEL;
  ptReq->tHead.ulCmd             = HIL_DDP_SERVICE_SET_REQ;
  ptReq->tHead.ulLen             = sizeof(ptReq->tData.ulDataType) + sizeof(ptReq->tData.uDataType.ulValue);
  ptReq->tData.ulDataType        = HIL_DDP_SERVICE_DATATYPE_STATE;
  ptReq->tData.uDataType.ulValue = HIL_DDP_SERVICE_STATE_ACTIVE;

  ulRet = Pkt_SendReceivePacket( ptAppData, EIP_DEMO_CHANNEL_INDEX, &ptAppData->aptChannels[EIP_DEMO_CHANNEL_INDEX]->tPacket, TXRX_TIMEOUT );

  /* Check status of cifX function and confirmation packet */
  return (ulRet != CIFX_NO_ERROR) ? ulRet : ptAppData->aptChannels[EIP_DEMO_CHANNEL_INDEX]->tPacket.tHeader.ulState;
}

/**************************************************************************************/
/**************************************************************************************/

bool AppEIS_PacketHandler( CIFX_PACKET* ptPacket,
                           void*        pvUserData )
{
  bool        fPacketCouldBeHandled = false;
  APP_DATA_T* ptAppData             = (APP_DATA_T*) pvUserData;

  if( ptPacket != &ptAppData->aptChannels[EIP_DEMO_CHANNEL_INDEX]->tPacket )
  {
    PRINTF("Unexpected packet resource received!!!" NEWLINE);
    return false;
  }

  switch( ptPacket->tHeader.ulCmd )
  {
    case EIP_OBJECT_CONNECTION_IND:
      AppEIS_HandleConnectionInd( ptAppData );
      fPacketCouldBeHandled = true;
      break;

    case EIP_OBJECT_RESET_IND:
      AppEIS_HandleResetInd(ptAppData);
      fPacketCouldBeHandled = true;
      break;

    case EIP_OBJECT_CIP_OBJECT_CHANGE_IND:
      AppEIS_HandleObjChangeInd( ptAppData );
      fPacketCouldBeHandled = true;
      break;

    case HIL_LINK_STATUS_CHANGE_IND:
      (void)App_SysPkt_HandleLinkStatusChangeInd( ptAppData, EIP_DEMO_CHANNEL_INDEX, &ptAppData->aptChannels[EIP_DEMO_CHANNEL_INDEX]->tPacket );
      fPacketCouldBeHandled = true;
      break;

    default:
      if( (ptAppData->aptChannels[EIP_DEMO_CHANNEL_INDEX]->tPacket.tHeader.ulCmd & 0x1) == 0 ) /* received an indication*/
      {
        PRINTF("Warning: Disregarded indication packet received!" NEWLINE);
        ptAppData->aptChannels[EIP_DEMO_CHANNEL_INDEX]->tPacket.tHeader.ulLen    = 0;
        ptAppData->aptChannels[EIP_DEMO_CHANNEL_INDEX]->tPacket.tHeader.ulState  = ERR_HIL_UNKNOWN_COMMAND;
        ptAppData->aptChannels[EIP_DEMO_CHANNEL_INDEX]->tPacket.tHeader.ulCmd   |= 0x01; /* Make it a response */

        (void)Pkt_SendPacket( ptAppData, EIP_DEMO_CHANNEL_INDEX, &ptAppData->aptChannels[EIP_DEMO_CHANNEL_INDEX]->tPacket, TX_TIMEOUT );
        fPacketCouldBeHandled  = true;
      }
      else
      { /* received a confirmation */
        PRINTF("Warning: Disregarded confirmation packet received!" NEWLINE);
      }
      break;
  }

  return fPacketCouldBeHandled;
}

