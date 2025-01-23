/**
 * @copyright (c) 2022  MKS Instruments, Inc. All rights reserved.
 * The information herein is confidential, and includes
 * trade secrets and other proprietary information.
 *
 * @file HAL.c
 * @brief HAL
 */

/** @defgroup HAL HAL
  * @ingroup HAL
  *
  * @brief HAL
  *
  * @details
  *
  *    Consolidate HAL init functions
  *
  * @par Modification History
  *
  *    Date     | Initials | Notes
  *    -------- | -------- | -----------------------------------------------------------------------
  *    10/19/22 |   JR     | Initial
  *
  * @{
 */

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "mks_types.h"
#include "HAL.h"
#include "SPI.h"

#include "system_netx.h"
#include "temp_sensor.h"

void HOST_HAL_Init(void){

  MksReturnCodes rc;  

  rc = Rotary_I2C_Init();
  if( kMksOk != rc )
    printf("\r\n Rotary_I2C_Init() failed! err = %u \r\n",rc);

  rc = RTC_I2C_Init();
  if( kMksOk != rc )
    printf("\r\n RTC_I2C_Init() failed! err = %u \r\n",rc);

  rc = LED_Init();
  if( kMksOk != rc )
      printf("\r\n LED_Init() failed! err = %u \r\n",rc);

  rc = SPI_DoInit();
  if( kMksOk != rc )
    printf("\r\n SPI_DoInit() failed! err = %u \r\n",rc);

  rc = SPI_DiInit();
  if( kMksOk != rc )
    printf("\r\n SPI_DiInit() failed! err = %u \r\n",rc);

  rc = SPI_ClockInit ();
  if( kMksOk != rc )
    printf("\r\n SPI_ClockInit() failed! err = %u \r\n",rc);

  Watchdog_IO_Rst();

//  rc = initialize_tmp112();//temperature
//  if( kMksOk != rc )
//      printf("\r\n initialize_tmp112() failed! err = %u \r\n",rc);

//  rc = NVRamInit();
//  if ( kMksOk != rc )
//   printf("\r\n NVRamInit() failed! err = %u \r\n",rc);

  /* set health led to init state */
  HOSTAL_Device_SetStatus(kLED_ID_HLT, HIL_MB_HEALTH_STATUS_FW_INITIALIZING);
  HOSTAL_Device_SetStatus(kLED_ID_NET, HIL_MB_NETWORK_STATUS_INITIALIZING);
  /* turn it on */
  HOSTAL_Health_Led_Handle();
  HOSTAL_Network_Led_Handle();  

  // fill up temp ADC sample queue
  for (uint8_t i = 0; i < ADC_AVG_COUNT; i++) Sensor_UpdateLocalTemp();
}
/** @}*/
