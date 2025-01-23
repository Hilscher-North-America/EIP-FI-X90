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
#include "App_PacketCommunication.h"
#include "Hil_DualPortMemory.h"

/*****************************************************************************/
/*! FUNCTIONS                                                                */
/*****************************************************************************/

/**************************************************************************************/
/*! Updates the application specific cyclic counter.
*  This function must be called on each IO data update cycle.
*  Note: this counter is implemented for demo purposes only.
*
* \param ptAppData  [in]  Pointer to application data
*/
/**************************************************************************************/
#ifdef HOST_ACYCLIC_DATA
void App_IO_UpdateCycleCounter( APP_DATA_T* ptAppData )
{
  static uint32_t s_ulCycle = 0;

  s_ulCycle++;

  if (ptAppData->tInputData.bCyclicCounter_Speed > 0)
  {
    if (s_ulCycle >= ptAppData->tInputData.bCyclicCounter_Speed)
    {
      if (APP_CYCLE_COUNTER_COUNT_UP == ptAppData->tInputData.bCyclicCounter_Direction)
      {
        ptAppData->tOutputData.usCyclicCounter++;
      }
      else
      {
        ptAppData->tOutputData.usCyclicCounter--;
      }

      s_ulCycle = 0;
    }
  }
  else
  {
    ptAppData->tOutputData.usCyclicCounter = 0xFFFF;
  }
}
#endif

/**************************************************************************************/
/*! This function performs an application specific self test.
*/
/**************************************************************************************/
void App_Selftest( void )
{
  HOSTAL_SelfTest();
}

/**************************************************************************************/
/*! Read board information
*
* \param hDriver     [in]   Handle if cifX driver
* \param ptBoardInfo [out]  Board information
*/
/**************************************************************************************/
void App_ReadBoardInfo( const CIFXHANDLE         hDriver,
                        char*                    szBoardName,
                        BOARD_INFORMATION* const ptBoardInfo )
{
  CIFXHANDLE                       hSys         = NULL;
  SYSTEM_CHANNEL_SYSTEM_INFO_BLOCK tSysInfo     = ptBoardInfo->tSystemInfo;
  long                             lRet         = 0;      /** Return value for common error codes  */
  DRIVER_INFORMATION               tDrvInfo     = {{0}};
  unsigned long                    ulBoardIndex = 0;

  if (CIFX_NO_ERROR != (lRet = xDriverGetInformation(hDriver, sizeof(tDrvInfo), &tDrvInfo)))
  {
    PRINTF("ERROR: xDriverGetInformation failed: 0x%08x" NEWLINE, (unsigned int)lRet);
    xDriverClose(hDriver);
    return;
  }

  /* iterate through all boards.
   * We need to find the board with the given name provided with szBoardName. */
  for (ulBoardIndex = 0; ulBoardIndex < tDrvInfo.ulBoardCnt; ulBoardIndex++)
  {
    if (CIFX_NO_ERROR != (lRet = xDriverEnumBoards(hDriver, 0, sizeof(BOARD_INFORMATION), ptBoardInfo)))
    {
      PRINTF("ERROR: xDriverEnumBoards failed: 0x%08x" NEWLINE, (unsigned int)lRet);
      xDriverClose(hDriver);
      return;
    }

    if (0 == strcmp((char*)ptBoardInfo->abBoardName, szBoardName))
    {
      /* Found the board */
      break;
    }
  }

  if ( ulBoardIndex >= tDrvInfo.ulBoardCnt )
  {
    /* Could not find the board name */
    return;
  }

  /* Get Sys-Channel information */
  if(CIFX_NO_ERROR != (lRet = xSysdeviceOpen(hDriver, szBoardName, &hSys)))
  {
    PRINTF("Error opening SystemDevice: 0x%08X !" NEWLINE, (unsigned int)lRet);
  }
  /** System channel successfully opened, try to read the System Info Block */
  else if(CIFX_NO_ERROR != (lRet = xSysdeviceInfo(hSys, CIFX_INFO_CMD_SYSTEM_INFO_BLOCK, sizeof(tSysInfo), &tSysInfo)))
  {
    PRINTF("Error querying system information block: 0x%08X !" NEWLINE, (unsigned int)lRet);
  }
  else
  {
    PRINTF("System Channel Info Block:" NEWLINE);
    PRINTF("DPM Size         : %u" NEWLINE, (unsigned int)tSysInfo.ulDpmTotalSize);
    PRINTF("Device Number    : %u" NEWLINE, (unsigned int)tSysInfo.ulDeviceNumber);
    PRINTF("Serial Number    : %u" NEWLINE, (unsigned int)tSysInfo.ulSerialNumber);
    PRINTF("Manufacturer     : %u" NEWLINE, (unsigned int)tSysInfo.usManufacturer);
    PRINTF("Production Date  : %u" NEWLINE, (unsigned int)tSysInfo.usProductionDate);
    PRINTF("Device Class     : %u" NEWLINE, (unsigned int)tSysInfo.usDeviceClass);
    PRINTF("HW Revision      : %u" NEWLINE, (unsigned int)tSysInfo.bHwRevision);
    PRINTF("HW Compatibility : %u" NEWLINE, (unsigned int)tSysInfo.bHwCompatibility);

    xSysdeviceClose(hSys);
  }
}

/**************************************************************************************/
/*! Read channel information
*
*   \param hChannel      [in]  Handle of cifX channel
*   \param ptChannelInfo [out] Channel information
*/
/**************************************************************************************/
void App_ReadChannelInfo( const CIFXHANDLE           hChannel,
                          CHANNEL_INFORMATION* const ptChannelInfo )
{
  long lRet = 0; /** Return value for common error codes      */

  /** Retrieve the global communication channel information */
  if(CIFX_NO_ERROR != (lRet = xChannelInfo(hChannel, sizeof(CHANNEL_INFORMATION), ptChannelInfo)))
  {
    PRINTF("Error querying communication channel information block: 0x%08X !" NEWLINE, (unsigned int) lRet);
  }
  else
  {
    PRINTF(NEWLINE "Communication Channel Info:" NEWLINE);
    PRINTF("-------------------------------------------------------" NEWLINE);    
    PRINTF("Device Number    : %u" NEWLINE, (unsigned int) ptChannelInfo->ulDeviceNumber);
    PRINTF("Serial Number    : %u" NEWLINE, (unsigned int) ptChannelInfo->ulSerialNumber);
    PRINTF("Protocol         : %s" NEWLINE, ptChannelInfo->abFWName);
    PRINTF("COM FW Version   : %u.%u.%u.%u" NEWLINE,
    (unsigned int) ptChannelInfo->usFWMajor,
    (unsigned int) ptChannelInfo->usFWMinor,
    (unsigned int) ptChannelInfo->usFWBuild,
    (unsigned int) ptChannelInfo->usFWRevision);

    PRINTF("COM FW Date      : %02u/%02u/%04u" NEWLINE,
    (unsigned int) ptChannelInfo->bFWMonth,
    (unsigned int) ptChannelInfo->bFWDay,
    (unsigned int) ptChannelInfo->usFWYear);
    PRINTF("Mailbox Size     : %u" NEWLINE,
    (unsigned int) ptChannelInfo->ulMailboxSize);    
  }
}

/**************************************************************************************/
/*! Call the start configuration function for all channels
*
*   \param ptAppData      [in]  Pointer to application data
*
*   \return 0 on success.
*/
/**************************************************************************************/
uint32_t App_AllChannels_Configure( APP_DATA_T* ptAppData )
{
  int     i;
  int32_t ulRet = 0;

  for (i = 0; i < MAX_COMMUNICATION_CHANNEL_COUNT; i++)
  {
    if ((ptAppData->aptChannels[i] != NULL) &&
        (ptAppData->aptChannels[i]->hChannel != NULL) &&
        (ptAppData->aptChannels[i]->tProtocol.pfStartChannelConfiguration != NULL))
    {
      if (CIFX_NO_ERROR != (ulRet = ptAppData->aptChannels[i]->tProtocol.pfStartChannelConfiguration(ptAppData)))
      {
        PRINTF("Error: Protocol_StartConfiguration failed: 0x%08X" NEWLINE, (unsigned int)ulRet);
        return ulRet;
      }
    }
  }
  return 0;
}

/**************************************************************************************/
/*! Call the packet handler function for all channels
*
*   \param ptAppData      [in]  Pointer to application data
*
*   \return 0 on success.
*/
/**************************************************************************************/
uint32_t App_AllChannels_PacketHandler( APP_DATA_T* ptAppData )
{
  int     i;
  int32_t ulRet = 0;

  for (i = 0; i < MAX_COMMUNICATION_CHANNEL_COUNT; i++)
  {
    if ((ptAppData->aptChannels[i] != NULL) &&
        (ptAppData->aptChannels[i]->hChannel != NULL) &&
        (ptAppData->aptChannels[i]->tProtocol.pfPacketHandler != NULL))
    {
      if (0 != (ulRet = ptAppData->aptChannels[i]->tProtocol.pfPacketHandler(ptAppData)))
      {
        PRINTF("Error: Protocol_PacketHandler failed: 0x%08X" NEWLINE, (unsigned int)ulRet);
        return ulRet;
      }
    }
  }
  return 0;
}

/**************************************************************************************/
/*! Call the event handler function for all channels
*
*   \param ptAppData   [in]  Pointer to application data
*   \param eEvent      [in]  Event that occurred
*   \param ulEventData [in]  Event data
*
*   \return 0 on success.
*/
/**************************************************************************************/
void App_AllChannels_EventHandler( APP_DATA_T* ptAppData,
                                   APP_EVENT_E eEvent,
                                   uint32_t    ulEventData )
{
  int i;

  for (i = 0; i < MAX_COMMUNICATION_CHANNEL_COUNT; i++)
  {
    if ((ptAppData->aptChannels[i] != NULL) &&
        (ptAppData->aptChannels[i]->hChannel != NULL) &&
        (ptAppData->aptChannels[i]->tProtocol.pfEventHandler != NULL))
    {
      ptAppData->aptChannels[i]->tProtocol.pfEventHandler(ptAppData, eEvent, ulEventData);
    }
  }
}

/**************************************************************************************/
/*! Call the terminal handler function for all channels
*
*   \param ptAppData        [in]  Pointer to application data
*   \param szTerminalBuffer [in]  Command line input
*
*   \return true on success.
*/
/**************************************************************************************/
bool App_AllChannels_TerminalHandler( APP_DATA_T* ptAppData,
                                      char*       szTerminalBuffer )
{
  bool bRet = false;
  int i;

  for (i = 0; i < MAX_COMMUNICATION_CHANNEL_COUNT; i++)
  {
    if ((ptAppData->aptChannels[i] != NULL) &&
        (ptAppData->aptChannels[i]->hChannel != NULL) &&
        (ptAppData->aptChannels[i]->tProtocol.pfTerminalHandler != NULL))
    {
      bRet = ptAppData->aptChannels[i]->tProtocol.pfTerminalHandler( ptAppData, szTerminalBuffer );
    }

    if (false == bRet) break;
  }

  return bRet;
}

/**************************************************************************************/
/*! Call the event handler function for all channels
*
*   \param ptAppData    [in]  Pointer to application data
*   \param hDriver      [in]  Drive handle retrieved with xDriverOpen()
*   \param szBoardName  [in]  Name of the board that shall be accessed
*
*   \return 0 on success.
*/
/**************************************************************************************/
int32_t App_AllChannels_Open( APP_DATA_T* ptAppData,
                              CIFXHANDLE  hDriver,
                              char*       szBoardName )
{
  int i;
  int32_t lRet;

  for (i = 0; i < MAX_COMMUNICATION_CHANNEL_COUNT; i++)
  {
    if ((ptAppData->aptChannels[i] != NULL) &&
        (ptAppData->aptChannels[i]->tProtocol.pfStartChannelConfiguration != NULL))
    {
      if (CIFX_NO_ERROR != (lRet = xChannelOpen(hDriver, szBoardName, i, &ptAppData->aptChannels[i]->hChannel)))
      {
        PRINTF("ERROR: xChannelOpen for %s, channel %d failed: 0x%08X" NEWLINE, szBoardName, i, (unsigned int)lRet);
        return lRet;
      }
    }
  }
  return CIFX_NO_ERROR;
}

/**************************************************************************************/
/*! Get channel information of all channels
*
*   \param ptAppData    [in]  Pointer to application data
*
*   \return 0 on success.
*/
/**************************************************************************************/
void App_AllChannels_GetChannelInfo_WaitReady( APP_DATA_T* ptAppData )
{
  int i;
  long lRet = 0; /** Return value for common error codes      */
  CHANNEL_INFORMATION* ptChannelInfo;


  for (i = 0; i < MAX_COMMUNICATION_CHANNEL_COUNT; i++)
  {
    if ((ptAppData->aptChannels[i] != NULL) &&
        (ptAppData->aptChannels[i]->hChannel != NULL))
    {
      do
      {
        ptChannelInfo = &ptAppData->aptChannels[i]->tChannelInfo;
        if(CIFX_NO_ERROR != (lRet = xChannelInfo(ptAppData->aptChannels[i]->hChannel, sizeof(CHANNEL_INFORMATION), ptChannelInfo)))
        {
          PRINTF("Error querying communication channel information block: 0x%08X !" NEWLINE, (unsigned int) lRet);
        }
        else
        {
          PRINTF("Communication Channel#%u Info:" NEWLINE, ((unsigned int)i));
          PRINTF("Protocol Firmware  : %s" NEWLINE, ptChannelInfo->abFWName);
          PRINTF("Protocol Version   : %u.%u.%u.%u (build %u)" NEWLINE,
                                             (unsigned int) ptChannelInfo->usFWMajor,
                                             (unsigned int) ptChannelInfo->usFWMinor,
                                             (unsigned int) ptChannelInfo->usFWBuild,
                                             (unsigned int) ptChannelInfo->usFWRevision,
                                             (unsigned int) ptChannelInfo->usFWBuild);
          PRINTF("Protocol FW Date   : %02u/%02u/%04u" NEWLINE,
                                             (unsigned int) ptChannelInfo->bFWMonth,
                                             (unsigned int) ptChannelInfo->bFWDay,
                                             (unsigned int) ptChannelInfo->usFWYear);
          PRINTF("Mailbox Size       : %u" NEWLINE NEWLINE, (unsigned int) ptChannelInfo->ulMailboxSize);
        }

      }
      while (!(ptChannelInfo->ulDeviceCOSFlags & HIL_COMM_COS_READY) ||
              (ptChannelInfo->ulDeviceCOSFlags == CIFX_DPM_NO_MEMORY_ASSIGNED));
    }
  }
}

/**************************************************************************************/
/*! Close all opened channels
*
*   \param ptAppData    [in]  Pointer to application data
*
*   \return 0 on success.
*/
/**************************************************************************************/
void App_AllChannels_Close( APP_DATA_T* ptAppData )
{
  int i;

  for (i = 0; i < MAX_COMMUNICATION_CHANNEL_COUNT; i++)
  {
    if ((ptAppData->aptChannels[i] != NULL) &&
        (ptAppData->aptChannels[i]->hChannel != NULL))
    {
      xChannelClose( ptAppData->aptChannels[i]->hChannel );
      ptAppData->aptChannels[i]->hChannel = NULL;
    }
  }
}
