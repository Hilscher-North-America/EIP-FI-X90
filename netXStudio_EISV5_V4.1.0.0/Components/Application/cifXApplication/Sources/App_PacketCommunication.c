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

#include <string.h>
#include <stdio.h>
#include <malloc.h>

#include "App_Application.h"
#include "App_PacketCommunication.h"
#include "Hil_Packet.h"
#include "Hil_Results.h"

//#define DBG_PACKET

#ifdef DBG_PACKET
#define PKTPRINT printf
#else
#define PKTPRINT(...)
#endif

/*****************************************************************************/
/*! STRUCTURES                                                               */
/*****************************************************************************/

typedef struct PACKET_COMMUNICATION_COUNTER_Ttag
{
  uint32_t ulPacketSendCountSuccess;
  uint32_t ulPacketSendCountError;
  uint32_t ulPacketReceiveCount;

} PACKET_COMMUNICATION_COUNTER_T;

/******************************************************************************/

/** Structure to store application's indication handler. */
typedef struct SYNC_PACKET_INTERFACE_INDICATON_HANDLER_Ttag
{
  bool (*fnHandler)(CIFX_PACKET* ptPacket, void* pvUserData);
  void* pvUserData;
} SYNC_PACKET_INTERFACE_INDICATON_HANDLER_T;

/******************************************************************************/

typedef struct SYNC_PACKET_INTERFACE_QUEUED_PACKET_Ttag
{
  bool fUsed;
  CIFX_PACKET tPacket;
} SYNC_PACKET_INTERFACE_QUEUED_PACKET_T;


/*****************************************************************************/
/*! GLOBAL, STATIC OR EXTERN VARIABLES                                       */
/*****************************************************************************/

/** Static packet ID counter.
 * This counter is used to assign received confirmation
 * packets uniquely to a previously sent request packet. */
static uint32_t s_ulNextPacketId = 1;

/** The LookupCommandCode function is used to print readable text instead of just the command ID. */
extern char* LookupCommandCode(uint32_t ulCmd);

/** Statistic counter for sent/received packets */
static PACKET_COMMUNICATION_COUNTER_T s_tPacketCounter;

/** Storage for Application's indication handler */
static SYNC_PACKET_INTERFACE_INDICATON_HANDLER_T s_atIndicationHandler[MAX_COMMUNICATION_CHANNEL_COUNT];

/** Receive queue for confirmation packets that are not expected when sending a request via Pkt_SendReceivePacket() */
static SYNC_PACKET_INTERFACE_QUEUED_PACKET_T** s_aaptReceiveQueue[MAX_COMMUNICATION_CHANNEL_COUNT];

static int s_aiReceiveQueueDepths[MAX_COMMUNICATION_CHANNEL_COUNT];

/*****************************************************************************/
/*! FUNCTIONS                                                                */
/*****************************************************************************/

bool Pkt_Init(int iChannelIdx, int iReceiveQueueDepth)
{
  int i;

  if (iChannelIdx >= MAX_COMMUNICATION_CHANNEL_COUNT)
  {
    return false;
  }

  if (s_aaptReceiveQueue[iChannelIdx] != NULL)
  {
    return false;
  }

  s_aaptReceiveQueue[iChannelIdx] = (SYNC_PACKET_INTERFACE_QUEUED_PACKET_T**)calloc( iReceiveQueueDepth, sizeof(SYNC_PACKET_INTERFACE_QUEUED_PACKET_T*));

  if (NULL == s_aaptReceiveQueue[iChannelIdx])
  {
    return false;
  }

  for (i = 0; i < iReceiveQueueDepth; i++)
  {
    s_aaptReceiveQueue[iChannelIdx][i] = (SYNC_PACKET_INTERFACE_QUEUED_PACKET_T*)calloc( 1, sizeof(SYNC_PACKET_INTERFACE_QUEUED_PACKET_T) );

    if (NULL == s_aaptReceiveQueue[iChannelIdx][i])
    {
      return false;
    }
  }

  s_aiReceiveQueueDepths[iChannelIdx] = iReceiveQueueDepth;

  return true;
}

/*****************************************************************************/
void Pkt_Deinit(void)
{
  int iChannelIdx;

  for( iChannelIdx = 0; iChannelIdx < MAX_COMMUNICATION_CHANNEL_COUNT; iChannelIdx++ )
  {
    if( s_aaptReceiveQueue[iChannelIdx] != NULL)
    {
      int iCnfQueueIdx;

      for (iCnfQueueIdx = 0; iCnfQueueIdx < s_aiReceiveQueueDepths[iChannelIdx]; iCnfQueueIdx++)
      {
        if (s_aaptReceiveQueue[iChannelIdx][iCnfQueueIdx] != NULL)
        {
          free( s_aaptReceiveQueue[iChannelIdx][iCnfQueueIdx] );
        }
      }

      free( s_aaptReceiveQueue[iChannelIdx] );
    }
  }

  memset( s_aiReceiveQueueDepths, 0, sizeof(s_aiReceiveQueueDepths) );
  memset( s_aaptReceiveQueue,     0, sizeof(s_aaptReceiveQueue) );
}

/*****************************************************************************/
/*! Displays a hex dump on the debug console (16 bytes per line)
 *
 *   \param pbData    [in]  Data buffer that shall be dumped
 *   \param ulDataLen [in]  Number of bytes in pbData
 */
/*****************************************************************************/
static void
Pkt_PrintFormattedHexData( unsigned char* pbData,
                           unsigned long  ulDataLen )
{
  unsigned long ulIdx;

  if(CIFX_MAX_DATA_SIZE < ulDataLen)
  {
    ulDataLen = CIFX_MAX_DATA_SIZE;
  }

  for(ulIdx = 0; ulIdx < ulDataLen; ++ulIdx)
  {
    if(0 == (ulIdx % 16))
    {
      PKTPRINT(NEWLINE);
    }

    PKTPRINT("%02X ", pbData[ulIdx]);
  }

  PKTPRINT(NEWLINE);
}

/*****************************************************************************/
/*! Get error description
*   \param  lError      cifX Error number                                    */
/*****************************************************************************/
void Pkt_GetErrorDesc(int32_t lError, char* szError)
{
  if (lError != CIFX_NO_ERROR)
  {
    /* Read driver error description */    
    xDriverGetErrorDescription(lError, szError, sizeof(szError));
    if (0 == strcmp(szError, ""))
    {
      switch (lError)
      {
        /* cifX Errors */
        case CIFX_INVALID_COMMAND: strcpy(szError, "Invalid command!"); break;                              /* 0x800A0006 */
        case CIFX_DEV_NOT_RUNNING: strcpy(szError, "Device not running (running flag failed)"); break;      /* 0x800C0012 */
        case CIFX_DEV_GET_NO_PACKET: strcpy(szError, "No Packet available!"); break;                        /* 0x800C0019 */
          //CIFX_NO_COM_FLAG,

        case ERR_HIL_UNKNOWN_COMMAND:     strcpy(szError, "Unknown command!"); break;
        case ERR_HIL_INVALID_PACKET_LEN:  strcpy(szError, "Invalid packet length!"); break;
        case ERR_HIL_INVALID_PARAMETER:   strcpy(szError, "Invalid parameter!"); break;
        case ERR_HIL_NOT_CONFIGURED:      strcpy(szError, "Configuration not available"); break;

        /* Protocol specific errors */  
        //case ERR_OMB_IF_SEND_IP_SET_CONFIG_FAILED: strcpy(szError, "Failed to forward the SET_CONFIG information to TCP_UDP task (because of a resource problem)."); break;
        default: break;
      }
    }    
  }
}

/*****************************************************************************/
/*! Show error
*   \param  lError      cifX Error number                                    */
/*****************************************************************************/
static void Pkt_ShowError(int32_t lError)
{
  char szError[1024] = { 0 };

  Pkt_GetErrorDesc(lError, szError);
  PRINTF(" Error: <%s>" NEWLINE, szError);
}

/*****************************************************************************/
/*! Dumps a packet to the debug console
 *
 *   \param ptPacket  [in] Packet to be dumped
 */
/*****************************************************************************/
static void
Pkt_DumpPacket( CIFX_PACKET* ptPacket )
{
  PKTPRINT("Dest   : 0x%08X      ID   : 0x%08X" NEWLINE, (unsigned int) ptPacket->tHeader.ulDest, (unsigned int) ptPacket->tHeader.ulId);
  PKTPRINT("Src    : 0x%08X      Sta  : 0x%08X" NEWLINE, (unsigned int) ptPacket->tHeader.ulSrc, (unsigned int) ptPacket->tHeader.ulState);
  if (CIFX_NO_ERROR != ptPacket->tHeader.ulState)
  {
    PRINTF("Cmd  : 0x%08X %s", (unsigned int) ptPacket->tHeader.ulCmd, LookupCommandCode(ptPacket->tHeader.ulCmd));
    Pkt_ShowError((int32_t)ptPacket->tHeader.ulState);
  }
  else
  {
    //PRINTF(NEWLINE);
  }

  PKTPRINT("DestID : 0x%08X      Cmd  : 0x%08X %s" NEWLINE, (unsigned int) ptPacket->tHeader.ulDestId, (unsigned int) ptPacket->tHeader.ulCmd, LookupCommandCode(ptPacket->tHeader.ulCmd));
  PKTPRINT("SrcID  : 0x%08X      Ext  : 0x%08X" NEWLINE, (unsigned int) ptPacket->tHeader.ulSrcId, (unsigned int) ptPacket->tHeader.ulExt);
  PKTPRINT("Len    : 0x%08X      Rout : 0x%08X" NEWLINE, (unsigned int) ptPacket->tHeader.ulLen, (unsigned int) ptPacket->tHeader.ulRout);

  if(ptPacket->tHeader.ulLen)
  {
    PKTPRINT("Data:");

    /** Displays a hex dump on the debug console (16 bytes per line) */
    Pkt_PrintFormattedHexData(ptPacket->abData, ptPacket->tHeader.ulLen);
  }

  PKTPRINT(NEWLINE);
}

/*****************************************************************************/
/*****************************************************************************/
uint32_t
Pkt_SendPacket( APP_DATA_T*  ptAppData,
                int          iChannelIdx,
                CIFX_PACKET* ptPacket,
                uint32_t     ulTimeout )
{
  uint32_t lRet = CIFX_NO_ERROR;

  lRet = xChannelPutPacket(ptAppData->aptChannels[iChannelIdx]->hChannel, ptPacket, ulTimeout);

  if(CIFX_NO_ERROR == lRet)
  {
    s_tPacketCounter.ulPacketSendCountSuccess++;
    PKTPRINT("========================================================" NEWLINE);
    PKTPRINT("Sent packet:" NEWLINE);
    Pkt_DumpPacket(ptPacket);
    PKTPRINT("========================================================" NEWLINE);
  }
  else
  {
    s_tPacketCounter.ulPacketSendCountError++;

    PKTPRINT("========================================================" NEWLINE);
    PKTPRINT("Sending packet failed with error: 0x%08x" NEWLINE, (unsigned int) lRet);
    Pkt_DumpPacket(ptPacket);
    PKTPRINT("========================================================" NEWLINE);
  }

  return lRet;
}

/**************************************************************************************/
/**************************************************************************************/
uint32_t
Pkt_ReceivePacket( APP_DATA_T*  ptAppData,
                   int          iChannelIdx,
                   CIFX_PACKET* ptPacket,
                   uint32_t     ulTimeout )
{
  uint32_t lRet = CIFX_NO_ERROR;

  lRet = xChannelGetPacket(ptAppData->aptChannels[iChannelIdx]->hChannel, sizeof(*ptPacket), ptPacket, ulTimeout);

  if(CIFX_NO_ERROR == lRet)
  {
    PKTPRINT("========================================================" NEWLINE);
    PKTPRINT("received packet:" NEWLINE);
    Pkt_DumpPacket(ptPacket);
    PKTPRINT("========================================================" NEWLINE);
  }

  return lRet;
}


/**************************************************************************************/
/**************************************************************************************/

bool
Pkt_RegisterIndicationHandler(int iChannelIdx,
                              bool (*fnHandler)( CIFX_PACKET* ptPacket, void* pvUserData ),
                              void* pvUserData )
{
  if( NULL == s_atIndicationHandler[iChannelIdx].fnHandler)
  {
    s_atIndicationHandler[iChannelIdx].fnHandler = fnHandler;
    s_atIndicationHandler[iChannelIdx].pvUserData = pvUserData;
    return true;
  }
  else
  {
    return false;
  }
}

/**************************************************************************************/
/*! Put a packet into the confirmation queue
 *
 *   \param ptPacket   [in]  Packet to be put into the queue
 *
 *   \return true:  Packet could be queued
 *           false: Packet could not be queued
 */
/**************************************************************************************/
static bool
Pkt_QueueConfirmation( int iChannelIdx, CIFX_PACKET* ptPacket )
{
  int i;
  for(i = 0; i < s_aiReceiveQueueDepths[iChannelIdx]; i++)
  {
    if(s_aaptReceiveQueue[iChannelIdx][i]->fUsed == false)
    {
      s_aaptReceiveQueue[iChannelIdx][i]->fUsed = true;
      s_aaptReceiveQueue[iChannelIdx][i]->tPacket = *ptPacket;
      return true;
    }
  }

  return false;
}

/**************************************************************************************/
/*! Get a packet from the confirmation queue
 *
 *   \param ptPacket [out]  Packet dequeued from the confirmation queue.
 *
 *   \return true:  Packet could be queued
 *           false: Packet could not be queued
 */
/**************************************************************************************/
static bool Pkt_GetPacketFromQueue(int iChannelIdx, CIFX_PACKET* ptPacket)
{
  int i;
  for(i = 0; i < s_aiReceiveQueueDepths[iChannelIdx]; i++)
  {
    if(s_aaptReceiveQueue[iChannelIdx][i]->fUsed == true)
    {
      *ptPacket = s_aaptReceiveQueue[iChannelIdx][i]->tPacket;
      s_aaptReceiveQueue[iChannelIdx][i]->fUsed = false;
      return true;
    }
  }
  return false;
}

/**************************************************************************************/
/*! Get a specific packet (with matching ulCmd and ulId) from the confirmation queue.
 *
 *   \param ulCmd    [in]   ulCmd value of the wanted packet
 *   \param ulId     [in]   ulId value  of the wanted packet
 *   \param ptPacket [out]  Packet dequeued from the confirmation queue.
 *
 *   \return true:  Packet was found
 *           false: Packet could not be found
 */
/**************************************************************************************/
static bool Pkt_TryDequeConfirmation( int iChannelIdx,
                                      uint32_t     ulCmd,
                                      uint32_t     ulId,
                                      CIFX_PACKET* ptPacket )
{
  int i;
  for(i = 0; i < s_aiReceiveQueueDepths[iChannelIdx]; i++)
  {
    if( (s_aaptReceiveQueue[iChannelIdx][i]->fUsed == true)                  &&
        (s_aaptReceiveQueue[iChannelIdx][i]->tPacket.tHeader.ulCmd == ulCmd) &&
        (s_aaptReceiveQueue[iChannelIdx][i]->tPacket.tHeader.ulId == ulId)
       )
    {
      *ptPacket = s_aaptReceiveQueue[iChannelIdx][i]->tPacket;
      s_aaptReceiveQueue[iChannelIdx][i]->fUsed = false;
      return true;
    }
  }
  return false;
}

/**************************************************************************************/
/*! Dispatch a packet to the registered indication handler
 *
 *   \param ptPacket [out]  Packet that shall be dispatched
 *
 *   \return true:  Packet could be dispatched
 *           false: Packet could not be dispatched (no registered indication handler available)
 */
/**************************************************************************************/

static bool
Pkt_DispatchIndication( int iChannelIdx, CIFX_PACKET* ptPacket )
{
  /* Dispatch this indication */
  if(s_atIndicationHandler[iChannelIdx].fnHandler)
  {
    return s_atIndicationHandler[iChannelIdx].fnHandler(ptPacket, s_atIndicationHandler[iChannelIdx].pvUserData);
  }
  else
  {
    return false;
  }
}

/**************************************************************************************/
/**************************************************************************************/

uint32_t
Pkt_CheckReceiveMailbox( APP_DATA_T *ptAppData,
                         int iChannelIdx,
                         CIFX_PACKET* ptPacket )
{
  uint32_t ulRet = CIFX_NO_ERROR;

  /* First, let's see if there are some unprocessed confirmations in our queue. */
  if( false == Pkt_GetPacketFromQueue(iChannelIdx, ptPacket))
  {
    /* No packets in the confirmation queue --> check the receive mailbox. */
    ulRet = Pkt_ReceivePacket(ptAppData, iChannelIdx, ptPacket, RX_TIMEOUT);
  }

  if( CIFX_NO_ERROR == ulRet)
  {
#if defined( CONSOLE_MODE ) && !defined( DONT_USE_COMMAND_LOOKUP )
    PKTPRINT( "[ok] IND:           Packet ulCmd 0x%08x (%s)" NEWLINE NEWLINE,
            (unsigned int)ptPacket->tHeader.ulCmd,
      LookupCommandCode(ptPacket->tHeader.ulCmd));
#endif

    if(!Pkt_DispatchIndication(iChannelIdx, ptPacket))
    {
      /* Indication was not handled, so we send the response on our own. */
      ptPacket->tHeader.ulCmd |= 0x01; /* Make it a response */
      ptPacket->tHeader.ulLen = 0;
      ptPacket->tHeader.ulState = CIFX_INVALID_COMMAND;

      Pkt_SendPacket(ptAppData, iChannelIdx, ptPacket, TX_TIMEOUT);
    }
  }

  return ulRet;
}

/**************************************************************************************/
/**************************************************************************************/

uint32_t
Pkt_SendReceivePacket( APP_DATA_T  *ptAppData,
                       int          iChannelIdx,
                       CIFX_PACKET* ptPacket,
                       uint32_t     ulTimeout )
{
  uint32_t ulRet = CIFX_NO_ERROR;
  uint32_t ulCmd = ptPacket->tHeader.ulCmd;
  uint32_t ulPacketId;

  if(!(ptPacket->tHeader.ulExt & (HIL_PACKET_SEQ_MIDDLE | HIL_PACKET_SEQ_LAST)))
  {
    s_ulNextPacketId++;
  }

  ulPacketId             = s_ulNextPacketId;
  ptPacket->tHeader.ulId = ulPacketId;

  /* issue the request */
  ulRet = Pkt_SendPacket(ptAppData, iChannelIdx, ptPacket, ulTimeout);

  if( CIFX_NO_ERROR != ulRet)
  {
#if defined( CONSOLE_MODE ) && !defined( DONT_USE_COMMAND_LOOKUP )
    PKTPRINT( "[!!] REQ:      Packet ulCmd 0x%08x (%s) failed with error 0x%08x" NEWLINE NEWLINE,
            (unsigned int)ptPacket->tHeader.ulCmd,
            LookupCommandCode(ptPacket->tHeader.ulCmd),
            (unsigned int)ulRet );
#endif
  }
  else
  {
    /* Sending the packet succeeded */

#if defined( CONSOLE_MODE ) && !defined( DONT_USE_COMMAND_LOOKUP )
    PKTPRINT( "[ok] REQ:           Packet ulCmd 0x%08x (%s)" NEWLINE NEWLINE,
            (unsigned int)ulCmd,
            LookupCommandCode(ptPacket->tHeader.ulCmd) );
#endif

    /* Now, let's wait for the confirmation. All indications coming up in the meantime will be dispatched. */
    while(1)
    {
      ulRet = Pkt_ReceivePacket(ptAppData, iChannelIdx , ptPacket, ulTimeout);

      if(CIFX_NO_ERROR == ulRet)
      {
        if((ptPacket->tHeader.ulCmd == (ulCmd | 0x1)) && (ptPacket->tHeader.ulId == ulPacketId))
        {
          /* This is the confirmation we were waiting for. */
          break;
        }
        else if(ptPacket->tHeader.ulCmd & 0x1)
        {
          /* This is a confirmation for another command we have issued, so put it into the receive queue for now. */
          PRINTF("Warning: Unexpected confirmation packet queued! Are you recursively issuing commands in your indication handler!?!" NEWLINE);

          if (!Pkt_QueueConfirmation(iChannelIdx, ptPacket))
          {
            PRINTF("FATAL: Confirmation queue overrun. Stopping. Don't issue commands in indication handlers." NEWLINE);
            return CIFX_FUNCTION_FAILED;
          }
        }
        else
        {
          if(!Pkt_DispatchIndication(iChannelIdx, ptPacket))
          {
            /* Indication was not handled, so we send the response on our own. */
            ptPacket->tHeader.ulCmd |= 0x01; /* Make it a response */
            ptPacket->tHeader.ulLen = 0;
            ptPacket->tHeader.ulState = CIFX_INVALID_COMMAND;

            Pkt_SendPacket(ptAppData, iChannelIdx, ptPacket, TX_TIMEOUT);
          }

          if(Pkt_TryDequeConfirmation(iChannelIdx, (ulCmd | 0x1), ulPacketId, ptPacket))
          {
            /* The confirmation we are waiting for was queued up during indication handling, we're fine. */
            PRINTF("A queued-up confirmation packet was processed!" NEWLINE NEWLINE);

            break;
          }
        }
      }
      else if( CIFX_DEV_GET_NO_PACKET == ulRet)
      {
        /* No packet within the given timeout received. */
        break;
      }
    }

    if( CIFX_NO_ERROR == ulRet)
    {
#if defined( CONSOLE_MODE ) && !defined( DONT_USE_COMMAND_LOOKUP )
      if(ptPacket->tHeader.ulState == 0)
      {
        PKTPRINT( "[ok] REQ->CNF:      Packet ulCmd 0x%08x (%s)" NEWLINE NEWLINE,
                (unsigned int)ulCmd,
                LookupCommandCode(ptPacket->tHeader.ulCmd) );
      }
      else
      {
        PKTPRINT( "[!!] REQ->CNF:      Packet ulCmd 0x%08x (%s) Sts 0x%08x " NEWLINE NEWLINE,
                (unsigned int)ulCmd,
                LookupCommandCode(ptPacket->tHeader.ulCmd),
                (unsigned int)ptPacket->tHeader.ulState );
      }
#endif
    }
    else
    {
      PKTPRINT("[!!] SEND/RECV of packet failed with Status: 0x%08x" NEWLINE NEWLINE, (unsigned int) ulRet);
    }
  }

  return ulRet;
}

/**************************************************************************************/
/**************************************************************************************/
