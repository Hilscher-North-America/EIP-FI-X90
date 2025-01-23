/**************************************************************************//**
 * @file    main.c
 * @brief   main file for embedded netx90 targets: e.g. NXHX90-JTAG
 * $Revision: 8445 $
 * $Date: 2020-12-22 15:44:03 +0100 (Tue, 22 Dec 2020) $
 ******************************************************************************/

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


 **************************************************************************************/

#include "main.h"
#include "App_Application.h"
#include "libc_support.h"
#include "cifXToolkit.h"

/* timeout for monitoring a READY flag change expected from the DPM provider */
#define COM_BOARD_READY_FLAG_TIMEOUT        1000

/* timeout for monitoring the re-appearance of the cookie at the beginning of the DPM */
#define COM_BOARD_COOKIE_TIMEOUT            5000

typedef struct
{
  HIL_DPM_SYSTEM_CHANNEL_T         tSystemChannel;
  HIL_DPM_HANDSHAKE_CHANNEL_T      tHandShakeChannel;
  HIL_DPM_DEFAULT_COMM_CHANNEL_T   tCommChannel0;
} MINIMAL_DPM_T;


int32_t InitializeToolkit(void);

int main()
{
  int32_t lRet = CIFX_NO_ERROR;

  libcInit();

  //print_app_header_nai();

  if(CIFX_NO_ERROR == lRet)
  {
    lRet = InitializeToolkit();
  }

  if(CIFX_NO_ERROR == lRet)
  {
    lRet = App_CifXApplication("cifX0");
  }

  while(1);  /** do not return from main() */
}


int32_t InitializeToolkit(void)
{
  int32_t lRet = CIFX_NO_ERROR;
  static PDEVICEINSTANCE ptDevInstance;

  lRet = cifXTKitInit();

  if(CIFX_NO_ERROR == lRet)
  {
    ptDevInstance = (PDEVICEINSTANCE)OS_Memalloc(sizeof(*ptDevInstance));
    OS_Memset(ptDevInstance, 0, sizeof(*ptDevInstance));

    /** Set trace level of toolkit */
    g_ulTraceLevel =  TRACE_LEVEL_ERROR   |
                      TRACE_LEVEL_WARNING |
                      TRACE_LEVEL_INFO    |
                      TRACE_LEVEL_DEBUG;

    /** Insert the basic device information into the DeviceInstance structure
          for the toolkit.
          NOTE: The irq number are for information use only, so we skip them here.
          Interrupt is currently not supported and ignored, so we don't need to set it */
    ptDevInstance->ulPhysicalAddress = 0xB0000000U;   //Addr_NX90_idpm_slave_mirror_ext_peri;
    ptDevInstance->ulIrqNumber        = 3;
    ptDevInstance->fIrqEnabled       = 0;
    ptDevInstance->fPCICard          = 0;
    ptDevInstance->fModuleLoad       = 0;

    ptDevInstance->eDeviceType       = eCIFX_DEVICE_DONT_TOUCH;
    ptDevInstance->pfnNotify         = NULL;
    ptDevInstance->pvOSDependent     = ptDevInstance;
    ptDevInstance->ulDPMSize         = 0x8000;        //32K
    ptDevInstance->pbDPM             = (uint8_t*) ptDevInstance->ulPhysicalAddress;
    OS_Strncpy(ptDevInstance->szName, "cifX0", sizeof(ptDevInstance->szName));

    /** give netX time to get access to SPM functionality */
    OS_Sleep(1000);

    /** Add the device to the toolkits handled device list */
    lRet = cifXTKitAddDevice(ptDevInstance);

    /** If it succeeded do device tests */
    if(CIFX_NO_ERROR != lRet)
    {
      /** Uninitialize Toolkit, this will remove all handled boards from the toolkit and
            deallocate the device instance */
      cifXTKitDeinit();
    }
  }

  return lRet;
}
