/**************************************************************************************

Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.

***************************************************************************************

  $Id: netX90_netX4x00.c 12966 2019-04-25 10:12:26Z AlexanderMinor $:

  Description:
    cifX Toolkit implementation of the netX90 and netX4000 detection functions

  Changes:
    Date        Description
    -----------------------------------------------------------------------------------
    2018-11-30  created

**************************************************************************************/

#include "cifXToolkit.h"
#include "cifXEndianess.h"
#include "netx90_4x00_romloader_dpm.h"


/*****************************************************************************/
/*! Detect netX4000 based chip via DPM ROMloader or Config registers
*   In case of successful detection, device instance member eChipType is set.
*
*   \param ptDevInstance        Instance to reset
*   \param fGlobalRegisterBlock Set if check should be done on DPM register block
*   \return !=0 if netX4x00 has been detected                                */
/*****************************************************************************/
int IsNetX4x00(PDEVICEINSTANCE ptDevInstance, int fGlobalRegisterBlock)
{
  int iRet = 0;
  uint32_t ulDpmNetxVersion = 0;

  if(fGlobalRegisterBlock)
  {
    /* ulDpm_netx_version in register block (end of DPM memory) */
    ulDpmNetxVersion = LE32_TO_HOST(HWIF_READ32(ptDevInstance, ptDevInstance->ptGlobalRegisters->reserved6));
  }
  else
  {
    HBOOT_V2_DPM_CFG_AREA_T* ptDpmCfg = (HBOOT_V2_DPM_CFG_AREA_T*)ptDevInstance->pbDPM;
    ulDpmNetxVersion = LE32_TO_HOST(HWIF_READ32(ptDevInstance, ptDpmCfg->ulDpm_netx_version));
  }

  if( HBOOT_DPM_NETX4000_COOKIE == ulDpmNetxVersion )
  {
    /* This is a netX4000 */
    ptDevInstance->eChipType = eCHIP_TYPE_NETX4000;
    iRet = 1;
  } else if( HBOOT_DPM_NETX4100_COOKIE == ulDpmNetxVersion )
  {
    /* This is a netX4100 */
    ptDevInstance->eChipType = eCHIP_TYPE_NETX4100;
    iRet = 1;
  }

  return iRet;
}

/*****************************************************************************/
/*! Detect netX90 based chip via DPM ROMloader or Config registers
*   In case of successful detection, device instance member eChipType is set.
*
*   \param ptDevInstance        Instance to reset
*   \param fGlobalRegisterBlock Set if check should be done on DPM register block
*   \return !=0 if netX90 has been detected                                  */
/*****************************************************************************/
int IsNetX90(PDEVICEINSTANCE ptDevInstance, int fGlobalRegisterBlock)
{
  int iRet = 0;
  uint32_t ulDpmNetxVersion = 0;
  uint32_t ulMsk = ~((uint32_t)MSK_HBOOT_DPM_NETX90_TYPE | MSK_HBOOT_DPM_NETX90_ROMSTEP); /* Mask out netX90 specific differentiation */

  if(fGlobalRegisterBlock                                     ||
     (eCIFX_DEVICE_FLASH_BASED == ptDevInstance->eDeviceType) ||
     (eCIFX_DEVICE_DONT_TOUCH  == ptDevInstance->eDeviceType))
  {
    /* ulDpm_netx_version in register block (end of DPM memory) */
    ulDpmNetxVersion = LE32_TO_HOST(HWIF_READ32(ptDevInstance, ptDevInstance->ptGlobalRegisters->reserved6));

    if( HBOOT_DPM_NETX90_COOKIE == (ulMsk & ulDpmNetxVersion))
    {
      ptDevInstance->eChipType = eCHIP_TYPE_NETX90;
      iRet = 1;
    }
  }
  else
  {
    /* When checking at DPM start, also look for ROMcode cookie 'NXBL' at Offset 0x100 */
    uint32_t ulCookie = 0;
    HBOOT_V2_DPM_CFG_AREA_T* ptDpmCfg = (HBOOT_V2_DPM_CFG_AREA_T*)ptDevInstance->pbDPM;

    ulDpmNetxVersion = LE32_TO_HOST(HWIF_READ32(ptDevInstance, ptDpmCfg->ulDpm_netx_version));

    HWIF_READN(ptDevInstance, &ulCookie, (ptDevInstance->pbDPM + HBOOT_V2_DPM_ID_ADR), sizeof(ulCookie));

    if((HBOOT_DPM_NETX90_COOKIE == (ulMsk & ulDpmNetxVersion)) &&
       (HOST_TO_LE32(HBOOT_V2_DPM_ID) == ulCookie) )
    {
      ptDevInstance->eChipType = eCHIP_TYPE_NETX90;
      iRet = 1;
    }
  }

  return iRet;
}
