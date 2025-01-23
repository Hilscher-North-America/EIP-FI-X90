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

#include "App_Application.h"
#include "App_SockIfFunctions.h"
#include "App_PacketCommunication.h"

#include "SOCK_API.h"
#include "sys.h"
#include "mks_syscounter.h"

uint32_t s_fLink = 0;

extern uint32_t g_fDHCPLink;

#define SOCK_CHANNEL_INDEX  1

/*
 * Read the interface configuration
 * */
uint32_t Sock_GetIfAddrs( APP_DATA_T* ptAppData )
{
  uint32_t ulRet = CIFX_FUNCTION_FAILED;
  SOCK_GETIFADDRS_REQ_T* ptReq = (SOCK_GETIFADDRS_REQ_T*) &ptAppData->aptChannels[SOCK_CHANNEL_INDEX]->tPacket;

  ptReq->tHead.ulCmd = SOCK_CMD_GETIFADDRS_REQ;
  ptReq->tHead.ulLen = 0;

  if (CIFX_NO_ERROR == Pkt_SendReceivePacket(ptAppData, SOCK_CHANNEL_INDEX, &ptAppData->aptChannels[SOCK_CHANNEL_INDEX]->tPacket, TXRX_TIMEOUT))
  {
    SOCK_GETIFADDRS_CNF_DATA_T* ptRes = (SOCK_GETIFADDRS_CNF_DATA_T*) &ptAppData->aptChannels[SOCK_CHANNEL_INDEX]->tPacket.abData;

    /* show Ethernet data (2 entries: IP, MAC Address) */
    if (ptReq->tHead.ulLen >= sizeof (SOCK_GETIFADDRS_ENTRY_T) * 2)
    {
      if (ptRes->atEntries[0].tIfAddress.tIp.ulIpAddr)
      {
        if (ptRes->atEntries[0].tIfAddress.tIp.bFamily == SOCK_AF_INET)
        {
          // IP Address
          printf("IP Address:        %d.%d.%d.%d\r\n", IPBYTES(ptRes->atEntries[0].tIfAddress.tIp.ulIpAddr));
          // Network Mask
          printf("Mask:              %d.%d.%d.%d\r\n", IPBYTES(ptRes->atEntries[0].tNetmask.tIp.ulIpAddr));
          // Gateway
          printf("Gateway:           %d.%d.%d.%d\r\n", IPBYTES(ptRes->atEntries[0].tBcastAddr.tIp.ulIpAddr));
          ulRet = CIFX_NO_ERROR;
          g_fDHCPLink = 1;
        }
        if (ptRes->atEntries[1].tIfAddress.tIp.bFamily == SOCK_AF_PACKET && ptRes->atEntries[1].tIfAddress.tLL.bLlAddrLen == 6)
        {
          // MAC Address
          printf("MAC Address:       %02X:%02X:%02X:%02X:%02X:%02X\r\n", ptRes->atEntries[1].tIfAddress.tLL.abLlAddr[0],ptRes->atEntries[1].tIfAddress.tLL.abLlAddr[1],
          ptRes->atEntries[1].tIfAddress.tLL.abLlAddr[2],ptRes->atEntries[1].tIfAddress.tLL.abLlAddr[3],ptRes->atEntries[1].tIfAddress.tLL.abLlAddr[4],ptRes->atEntries[1].tIfAddress.tLL.abLlAddr[5]);
        }
      }
      else
      {
        printf("No valid link information\r\n");
      }
    }
  }

  return ulRet;
}

/*
 * Check and wait as long as no IP address is available
 * */
void AppSock_InterfaceConfiguration( APP_DATA_T* ptAppData )
{
  static bool bShowed = false;
  static uint32_t ulStartTime = 0;

  /* for lost link, clear show flag to redisplay when connected again */
  if (s_fLink == 0)
  {
    bShowed = false;
    return;
  }    

  if (bShowed == true) return;

  memset(&ptAppData->aptChannels[SOCK_CHANNEL_INDEX]->tPacket, 0, sizeof(ptAppData->aptChannels[SOCK_CHANNEL_INDEX]->tPacket));
  
  /* check for timeout */
  if (MKS_TRUE == SysCounterTimeoutCounts(ulStartTime, MILLI_SECONDS_TO_SYSTEM_TICK_COUNTS(1000)))
  {
    if (CIFX_NO_ERROR == Sock_GetIfAddrs (ptAppData))
    {
      bShowed = true;
    }
    ulStartTime = GetSysCounter();    
  }
} 
