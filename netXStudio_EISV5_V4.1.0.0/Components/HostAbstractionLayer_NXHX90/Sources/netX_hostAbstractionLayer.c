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

 ***************************************************************************************
 $Id: netX_hostAbstractionLayer.c 2894 2017-03-13 12:53:24Z Jin $:

 Description:
 Implementation of the host abstraction layer functions for NXHX 90-JTAG

 Changes:
 Date        Description
 -----------------------------------------------------------------------------------
 2017-06-13  initial version

 **************************************************************************************/

/*****************************************************************************/
/*! \file netx_hostAbstractionLayer.c
 *   Implementation of the host abstraction layer functions for NXHX 90-JTAG          */
/*****************************************************************************/
#include <stdlib.h>
#include "app_application.h"
#include "hostAbstractionLayer.h"
#include "netx_drv.h"
#include "OS_Dependent.h"
#include "hal.h"
#include "sys.h"
#include "led.h"
#include "temp_sensor.h"
#include "Hil_DualPortMemory.h"
#include <stdio.h>

static DRV_TIM_HANDLE_T  s_tArmTimer1; /* Arm Timer 1 is used for handling cyclic process data  */
extern DRV_UART_HANDLE_T g_tUART;      /* global Uart handle defined in libc_file_support.c */

/*!
 * NXHX 90 - JTAG
 * --------------
 * The development board has four dip switches and four leds and the chip
 * has an ADC on board. This ADC is capable of measuring the junction
 * temperature of the chip.
 *
 * ## The IO data area ##
 * The output data consists of 10 bytess, which will be defined as follows:
 * Byte| Brief           | Detailed                    | Usage
 * ----|-----------------|-----------------------------|-----------------------
 *   0 | Counter         | This counter is iterated by | Demo Application implements this
 *   1 | ^               | ^                           | ^
 *   2 | Sensor1 Value   | Data of the Sensor 1        | ADC Temp raw value
 *   3 | ^               | ^                           | ^
 *   4 | Sensor1 State   | Status of the Sensor 1      | In warning state if temp > 100 deg C
 *   5 | Sensor2 Value   | Data of the Sensor 2        | ADC Temp value in Degree Celsius
 *   6 | ^               | ^                           | ^
 *   7 | Sensor2 State   | Status of the Sensor 2      | In warning state if temp > 50 deg C
 *   8 | Actuator1 State | Status of the Actuator 1    | DIP Switch 3 MMIO 2
 *   9 | Actuator2 State | Status of the Actuator 2    | DIP Switch 4 MMIO 3
 *
 * The input data consists of 6 bytes, which will be defined as follows:
 * Byte| Brief           | Detailed                    | Usage
 * ----|-----------------|-----------------------------|-----------------------
 *   0 | Actuator1 Value | Data of the Actuator 1      | LED MMIO 4
 *   1 | ^               | ^                           | ^
 *   2 | Actuator2 Value | Data of the Actuator 2      | LED MMIO 5
 *   3 | ^               | ^                           | ^
 *   4 | Counter Dir     | Direction of the counter    | Demo Application uses it for asc or desc the counter value
 *   5 | Counter Speed   | Speed of the counter        | Demo Application uses it as the increment value, 0 means stopped
 *
 * ## Acyclic Messaging ##
 * If the state of the actuator or the sensors change, it is detected by the
 * DemoApplications EventHandler and the Protocol_EventHandler is called.
 *
 *
 *
 * ## The LEDs and DIP Switches on the Dev Board##
 * Signal      | Description
 * ------------|---------------------------------
 * DIP1 MMIO 0 | UART App Rx
 * DIP2 MMIO 1 | UART App Tx
 * DIP3 MMIO 2 | Actuator 1 State Bit 1
 * DIP4 MMIO 3 | Actuator 2 State Bit 1
 * LED  MMIO 4 | Actuator 1 Data Bit 1
 * LED  MMIO 5 | Actuator 2 Data Bit 2
 * LED  MMIO 6 | ND
 * LED  MMIO 7 | HOSTAL_DEVICE_STATUS_ERROR
 *
 *
 *
 */

#define MAX_ROTARY_SWITCH_ID 2 /* 0:RS1, 1:RS2, 2:RS2 */
#define SELFTEST_DELAY_MS 250

/*****************************************************************************/
/*! Static Variables                                                         */
/*****************************************************************************/
static uint16_t s_usDeviceStatus   = 0;
static uint8_t  s_bActuatorMode[2] = { 0 };

/*****************************************************************************/
/*! Private Variables                                                        */
/*****************************************************************************/
static void *s_pTimerPrm = NULL;

/*****************************************************************************/
/*! External Global Variables                                                */
/*****************************************************************************/
extern uint32_t g_fDHCPLink;
extern uint32_t s_fLink;

/*****************************************************************************/
/*! Callback function definitions                                            */
/*****************************************************************************/
static void (*s_pfTimerCallback)(void* pPrm) = NULL;

/*****************************************************************************/
/*! Timer interrupt handler                                                  */
/*****************************************************************************/
void DRV_TIM_TIMER1_Callback(void)
{
  /* Arm Timer 1 is used for handling cyclic process data  */
  if(s_pfTimerCallback != NULL)
  {
    s_pfTimerCallback(s_pTimerPrm);
  }
}

/*****************************************************************************/
/*! Initializes user interface  
 *   \return HOSTAL_OK on success                                                 */
/*****************************************************************************/
HOSTAL_RESULT_E HOSTAL_Init(void)
{
  DRV_STATUS_E eRet;
  static size_t ulErrorCounter;
  /* Temperature example*/
  /* Initialize and enable ADC driver */
  DRV_ADC_HANDLE_T tDriver = DRV_ADC_INITIALIZER;
  tDriver.tConfiguration.eVrefBufferEnable = DRV_ADC_VREF_BUFFER_ENABLED;
  ulErrorCounter = 0;
  while(DRV_OK != (eRet = DRV_ADC_Driver_Init(&tDriver)))
  {
    if(eRet != DRV_LOCKED && ulErrorCounter > 1000)
    {
      exit(DRV_ERROR);
    }
  }
  DRV_ADC_SEQ_HANDLE_T tSequencer0 = DRV_ADC_SEQ_INITIALIZER;
  /* Initialize and enable ADC sequencer for Temp Sensor*/
  tSequencer0.tConfiguration.eDeviceID = DRV_ADC_SEQ_DEVICE_ID_ADC0;
  tSequencer0.tConfiguration.eOperationMode = DRV_OPERATION_MODE_DMA;
  tSequencer0.tConfiguration.eContinuousMode = DRV_ADC_SEQ_CONTINUOUS_MODE_ENABLED;
  tSequencer0.tConfiguration.eBaseAdr = (DRV_ADC_SEQ_BASE_ADDRESS_E) &s_ulADCTempValue;
  tSequencer0.tConfiguration.eDmaModeDisable = DRV_ADC_SEQ_DMA_MODE_ENABLED;
  tSequencer0.tConfiguration.eChannelTrackingTime[2] = (DRV_ADC_SEQ_CHANNEL_TRACKING_TIME_E) 0xff;
  tSequencer0.tConfiguration.eVrefVdd3 = DRV_ADC_SEQ_VREF_VDD3_DISABLED;
  tSequencer0.tConfiguration.tMeasurement[0].eEnable = DRV_ADC_SEQ_MEAS_ENABLE;
  tSequencer0.tConfiguration.tMeasurement[0].eInputChannel = DRV_ADC_SEQ_MEAS_INPUT_CHANNEL_2;
  tSequencer0.tConfiguration.tMeasurement[0].eOversample = DRV_ADC_SEQ_MEAS_OVERSAMPLING_SUM_8;
  tSequencer0.tConfiguration.tMeasurement[0].eAdrOffset = 0;
  tSequencer0.tConfiguration.tMeasurement[0].eTrigger = DRV_ADC_SEQ_MEAS_TRIGGER_IMMEDIATELY;
  while(DRV_OK != (eRet = DRV_ADC_Seq_Init(&tSequencer0, &tDriver)))
  {
    if(eRet != DRV_LOCKED && ulErrorCounter > 1000)
    {
      exit(DRV_ERROR);
    }
  }
  while(DRV_OK != (eRet = DRV_ADC_Seq_Start(&tSequencer0)))
  {
    if(eRet != DRV_LOCKED && ulErrorCounter > 1000)
    {
      exit(DRV_ERROR);
    }
  }

  /** configure Output pins */
  DRV_DIO_LineOutEnableSet(DRV_DIO_LINE_MMIO, 0xF0ul);
  DRV_DIO_LineOutEnableReset(DRV_DIO_LINE_MMIO, 0x0Ful);
  DRV_DIO_MSK_T ullInput;
  DRV_DIO_LineInGet(DRV_DIO_LINE_MMIO, &ullInput);
  DRV_DIO_LineOutSet(DRV_DIO_LINE_MMIO, 0xF0ull);
  DRV_DIO_LineOutReset(DRV_DIO_LINE_MMIO, 0xF0ull);

  /** ArtTimer1 1ms interrupt */
  s_tArmTimer1.tConfiguration.eDeviceID = DRV_TIM_DEVICE_ID_TIMER1;
  s_tArmTimer1.tConfiguration.eOperationMode = DRV_OPERATION_MODE_IRQ;
  s_tArmTimer1.tConfiguration.eCountingMode = DRV_TIM_COUNTING_MODE_CONTINUOUS;
  s_tArmTimer1.tConfiguration.tPreloadValue = DRV_TIM_PRELOAD_VALUE_1ms;
  DRV_TIM_Init(&s_tArmTimer1);

  HOST_HAL_Init();

  return HOSTAL_OK;
}

/**************************************************************************************/
/*! This function receives data coming from the PLC, and sends data going to the PLC.
 *
 *  \param  ptSpiTxData            Data that is received from the PLC.
 *  \param  ptSpiRxData            Data that is sent to the PLC.
 *  \param  tTxDataValid           Device input data valid flag
 *
 *  \return HOSTAL_OK on success                                                      */
/**************************************************************************************/
HOSTAL_RESULT_E HOSTAL_DataExchange(void *ptSpiTxData, void* ptSpiRxData, bool tTxDataValid)
{
  /* setup new SPI transfer */
  /* Do SPI OUTPUT transfer */
  if (true == g_DoAppControl && true == tTxDataValid)
  {
    if (HOSTAL_OK != SPI_StartOutputDataTransmit(ptSpiTxData))
      return HOSTAL_ERROR;
  }

  /* Do SPI INPUT transfer */
  if(HOSTAL_OK != SPI_StartInputDataTransmit(ptSpiRxData))
    return HOSTAL_ERROR;
  
  return HOSTAL_OK;
}

/*####################################################################################
 *      _    ____ _____ _   _   _  _____ ___  ____
 *     / \  / ___|_   _| | | | / \|_   _/ _ \|  _ \
 *    / _ \| |     | | | | | |/ _ \ | || | | | |_) |
 *   / ___ \ |___  | | | |_| / ___ \| || |_| |  _ <
 *  /_/   \_\____| |_|  \___/_/   \_\_| \___/|_| \_\
 *
 #####################################################################################*/

/*****************************************************************************/
/*! puts output data to the console
 *   \param bActuatorNumber number of the actuator
 *   \param ulData cyclic output data to be displayed (HOSTAL_OUTPUT_BITS wide)
 *   \return HOSTAL_OK on success                                                 */
/*****************************************************************************/
HOSTAL_RESULT_E HOSTAL_Actuator_SetData(uint8_t bActuatorNumber, uint16_t ulData)
{
  s_bActuatorMode[bActuatorNumber] = ulData;
  if(ulData)
  {
    DRV_DIO_ChannelOutSet(DRV_DIO_ID_MMIO_4 + bActuatorNumber);
  }
  else
  {
    DRV_DIO_ChannelOutReset(DRV_DIO_ID_MMIO_4 + bActuatorNumber);
  }
  return HOSTAL_OK;
}

/*****************************************************************************/

HOSTAL_RESULT_E HOSTAL_Actuator_SetMode(uint8_t bActuatorNumber, uint8_t bMode)
{
  // not called anywhere
  return HOSTAL_ERROR;
}

/*****************************************************************************/

HOSTAL_PERIPHERAL_STATE_E HOSTAL_Actuator_GetState(uint8_t bActuatorNumber, uint16_t* pusStatusCode)
{
  DRV_DIO_ChannelInGet(DRV_DIO_ID_MMIO_2 + bActuatorNumber, (DRV_DIO_STATE_E*) pusStatusCode);

  if(0 == *pusStatusCode)
    return HOSTAL_PERIPHERAL_STATE_OK;
  else
    return HOSTAL_PERIPHERAL_STATE_WARNING;
}

/*####################################################################################
 *  ____  _______     _____ ____ _____   ____ _____  _  _____ _   _ ____
 * |  _ \| ____\ \   / /_ _/ ___| ____| / ___|_   _|/ \|_   _| | | / ___|
 * | | | |  _|  \ \ / / | | |   |  _|   \___ \ | | / _ \ | | | | | \___ \
 * | |_| | |___  \ V /  | | |___| |___   ___) || |/ ___ \| | | |_| |___) |
 * |____/|_____|  \_/  |___\____|_____| |____/ |_/_/   \_\_|  \___/|____/
 *
 ####################################################################################*/

/*****************************************************************************/
/*! updates status LED to the console
 *   \return HOSTAL_OK on success                                                 */
/*****************************************************************************/
static HOSTAL_RESULT_E HOSTAL_UpdateStatLed(void)
{
  // Blink speed of an led by severity
  // 0 low
  // HOSTAL_DEVICE_STATUS_DATA glow
  // HOSTAL_DEVICE_STATUS_ALARM blink slowly
  // HOSTAL_DEVICE_STATUS_ERROR blink fast
  if(s_usDeviceStatus & (HOSTAL_DEVICE_STATUS_DATA | HOSTAL_DEVICE_STATUS_ALARM | HOSTAL_DEVICE_STATUS_ERROR))
  {
    DRV_DIO_ChannelOutSet(DRV_DIO_ID_MMIO_7);
  }
  else
  {
    DRV_DIO_ChannelOutReset(DRV_DIO_ID_MMIO_7);
  }
  return HOSTAL_OK;
}

/*****************************************************************************/

HOSTAL_RESULT_E HOSTAL_Device_SetStatus(Led_Status_Indicators eLed, uint16_t usStatus)
{
  if (kLED_ID_NET == eLed)
    s_usNetworkStatus = usStatus;
  else if (kLED_ID_HLT == eLed)
    s_usHealthStatus = usStatus;

  return HOSTAL_OK;
}

/*****************************************************************************/
HOSTAL_RESULT_E HOSTAL_Device_ClearStatus(uint16_t usStatus)
{
  s_usDeviceStatus &= ~usStatus;

  (void) HOSTAL_UpdateStatLed();
  return HOSTAL_OK;
}

/*****************************************************************************/

HOSTAL_RESULT_E HOSTAL_Device_GetStatus(uint16_t* pusStatus)
{
  *pusStatus = s_usDeviceStatus;

  return HOSTAL_OK;
}

/*****************************************************************************/
HOSTAL_RESULT_E HOSTAL_Device_UpdateStatus (CIFXHANDLE hChannel)
{
  int32_t lRet = HOSTAL_OK;
  HIL_DPM_COMMON_STATUS_BLOCK_T tCommonStsBlk;

  lRet = xChannelCommonStatusBlock(hChannel, CIFX_CMD_READ_DATA, 0, sizeof (tCommonStsBlk), &tCommonStsBlk);

  if (CIFX_NO_ERROR == lRet)
  {
  //  update Network status 
    switch (tCommonStsBlk.ulCommunicationState)
    {
      case HIL_COMM_STATE_UNKNOWN: 
      case HIL_COMM_STATE_NOT_CONFIGURED:
      HOSTAL_Device_SetStatus(kLED_ID_NET, HIL_MB_NETWORK_STATUS_NO_CONNECTION);
      break;

      case HIL_COMM_STATE_STOP:
      case HIL_COMM_STATE_IDLE:
      if ((g_FI_unit_stats.mode & FI_DHCP_MODE) && (g_fDHCPLink == 0) && (s_fLink == 1))
        HOSTAL_Device_SetStatus(kLED_ID_NET, HIL_MB_NETWORK_STATUS_WAIT_FOR_LEASE);
      else
        HOSTAL_Device_SetStatus(kLED_ID_NET, HIL_MB_NETWORK_STATUS_STANDBY);
      break;
      
      case HIL_COMM_STATE_OPERATE:
      HOSTAL_Device_SetStatus(kLED_ID_NET, HIL_MB_NETWORK_STATUS_CONNECTED);
      break;
      
      default:
      break;
    }
  }

  Diag_Latch(DIAG_IO_READ_HSK_ERR, tCommonStsBlk.bErrorPDInCnt);
  Diag_Latch(DIAG_IO_WRITE_HSK_ERR, tCommonStsBlk.bErrorPDOutCnt);

  return HOSTAL_OK;
}

/*####################################################################################
 *  ____  _____ _   _ ____   ___  ____
 * / ___|| ____| \ | / ___| / _ \|  _ \
 * \___ \|  _| |  \| \___ \| | | | |_) |
 *  ___) | |___| |\  |___) | |_| |  _ <
 * |____/|_____|_| \_|____/ \___/|_| \_\
 *
 #####################################################################################*/


/*****************************************************************************/
/*! returns switch input process data from the console  (HOSTAL_INPUT_BITS wide)
 *   \return input data                                                       */
/*****************************************************************************/
HOSTAL_RESULT_E HOSTAL_Sensor_GetData(uint8_t bSensorNumber, uint16_t* pulData)
{
  //int temp;
  switch (bSensorNumber)
  {
  case 0:
    *pulData = (short) ((3300 * (s_ulADCTempValue / 8)) / 0xFFF);
    break;
  case 1:
    *pulData = (short) Sensor_GetTemp ();    
    break;
  default:
    return HOSTAL_ERROR;
  }
  return HOSTAL_OK;
}

/*****************************************************************************/

HOSTAL_RESULT_E HOSTAL_Sensor_SetMode(uint8_t bSensorNumber, uint8_t bMode)
{
  // Not Called anywhere
  return HOSTAL_ERROR;
}

/*****************************************************************************/

HOSTAL_PERIPHERAL_STATE_E HOSTAL_Sensor_GetState(uint8_t bSensorNumber, uint16_t* pusStatusCode)
{
  HOSTAL_PERIPHERAL_STATE_E ret = HOSTAL_PERIPHERAL_STATE_ERROR;
  int value;
  
  switch (bSensorNumber)
  {
    // Temp ADC in Celsius
  case TEMP_SENSOR:
    value = Sensor_GetTemp();
    *pusStatusCode = (short) value;
    /* the temperature sensor and the threshold are expressed in hundredths (1/100s) of a degree Celsius */
    if (value < g_FI_unit_stats.user_max_temperature_in_counts)
    {
      ret = HOSTAL_PERIPHERAL_STATE_OK;
      /* check for WD fault that has priority over temperature OK state */
      if (HIL_MB_HEALTH_STATUS_WD_FAULT == s_usHealthStatus)
        break;         
      
      HOSTAL_Device_SetStatus(kLED_ID_HLT, (g_FI_unit_stats.mode & FI_WD_ENABLED) ? HIL_MB_HEALTH_STATUS_CONNECTED : HIL_MB_HEALTH_STATUS_DIAG_MODE);
    }
    else
    {
      ret = HOSTAL_PERIPHERAL_STATE_WARNING;
      HOSTAL_Device_SetStatus(kLED_ID_HLT, HIL_MB_HEALTH_STATUS_HOT);
    }
    break;
#ifdef APP_EXAMPLE
  case 0:
    SystemTemperatureByADCValue(s_ulADCTempValue / 8, &temp);
    *pusStatusCode = (short) temp;
    if(temp < 100)
    {
      ret = HOSTAL_PERIPHERAL_STATE_OK;
    }
    else
    {
      ret = HOSTAL_PERIPHERAL_STATE_WARNING;
    }
    break;
  case 1:
    *pusStatusCode = s_ulADCTempValue;
    SystemTemperatureByADCValue(s_ulADCTempValue / 8, &temp);
    if(temp < 50)
    {
      ret = HOSTAL_PERIPHERAL_STATE_OK;
    }
    else
    {
      ret = HOSTAL_PERIPHERAL_STATE_WARNING;
    }
    break;
#endif
  default:
    break;
  }
  return ret;
}

/*####################################################################################
 *  ____   ___ _____  _    ______   __  ______        _____ _____ ____ _   _
 * |  _ \ / _ \_   _|/ \  |  _ \ \ / / / ___\ \      / /_ _|_   _/ ___| | | |
 * | |_) | | | || | / _ \ | |_) \ V /  \___ \\ \ /\ / / | |  | || |   | |_| |
 * |  _ <| |_| || |/ ___ \|  _ < | |    ___) |\ V  V /  | |  | || |___|  _  |
 * |_| \_\\___/ |_/_/   \_\_| \_\|_|   |____/  \_/\_/  |___| |_| \____|_| |_|
 *
 #####################################################################################*/

/*****************************************************************************/
/*! returns rotary switch input data from the console               
 *   \return rotary switch input                                              */
/*****************************************************************************/
HOSTAL_RESULT_E HOSTAL_RotarySwitch_GetValue(uint8_t bRotarySwitchNumber, uint8_t* pbValue)
{
  return HOSTAL_ERROR;
}

/**************************************************************************************/
/*! Performs a target specific self test.
 *  \return HOSTAL_OK on success                                                      */
/**************************************************************************************/
HOSTAL_RESULT_E HOSTAL_SelfTest( void )
{
  uint16_t usSensorData       = 0;

  HOSTAL_Sensor_GetData(0, &usSensorData);
  HOSTAL_Sleep(SELFTEST_DELAY_MS);

  HOSTAL_Sensor_GetData(1, &usSensorData);
  HOSTAL_Sleep(SELFTEST_DELAY_MS);

  return HOSTAL_OK;
}

/*####################################################################################
 *   ____    _    _     _     ____    _    ____ _  ______
 *  / ___|  / \  | |   | |   | __ )  / \  / ___| |/ / ___|
 * | |     / _ \ | |   | |   |  _ \ / _ \| |   | ' /\___ \
 * | |___ / ___ \| |___| |___| |_) / ___ \ |___| . \ ___) |
 *  \____/_/   \_\_____|_____|____/_/   \_\____|_|\_\____/
 *
 #####################################################################################*/

/*****************************************************************************/
/*! registers callback function   
 *   \param eClbType callback type
 *   \param pfnEventClb callback function pointer
 *   \param pPrm parameters
 *   \return HOSTAL_OK on success                                                 */
/*****************************************************************************/
HOSTAL_RESULT_E HOSTAL_Callback_Register(HOSTAL_CALLBACK_TYPE_E eClbType, void (*pfnEventClb)(void* pPrm), void* pPrm)
{
  switch (eClbType)
  {
  case HOSTAL_CB_TIMER:
    s_pfTimerCallback = pfnEventClb;
    s_pTimerPrm = pPrm;
    break;
  default:
    return HOSTAL_UNKNOWN_CALLBACK_TYPE;
  }
  return HOSTAL_OK;
}

/*****************************************************************************/
/*! Enable callback functions 
 *   \return HOSTAL_OK on success                                                 */
/*****************************************************************************/
HOSTAL_RESULT_E HOSTAL_Callbacks_Enable( void )
{
  DRV_STATUS_E eRet;
  uint32_t     ulCounter = 0;

  while (DRV_OK != (eRet = DRV_TIM_Start( &s_tArmTimer1 )))
  {
    if (eRet == DRV_LOCKED)
    {
      continue;
    }
    else
    {
      if (ulCounter++ > 1000)
      {
        DRV_NVIC_SetPendingIRQ( HardFault_IRQn );
      }
    }
  }
  return HOSTAL_OK;
}

/*****************************************************************************/
/*! Disable callback functions 
 *   \return HOSTAL_OK on success                                                 */
/*****************************************************************************/
HOSTAL_RESULT_E HOSTAL_Callbacks_Disable( void )
{
  DRV_STATUS_E eRet;
  uint32_t     ulCounter = 0;

  while (DRV_OK != (eRet = DRV_TIM_Stop( &s_tArmTimer1 )))
  {
    if (eRet == DRV_LOCKED)
    {
      continue;
    }
    else
    {
      if (ulCounter++ > 1000)
      {
        DRV_NVIC_SetPendingIRQ( HardFault_IRQn );
      }
    }
  }
  return HOSTAL_OK;
}

/*####################################################################################
 *  _   _ _____ _     ____  _____ ____  ____
 * | | | | ____| |   |  _ \| ____|  _ \/ ___|
 * | |_| |  _| | |   | |_) |  _| | |_) \___ \
 * |  _  | |___| |___|  __/| |___|  _ < ___) |
 * |_| |_|_____|_____|_|   |_____|_| \_\____/
 *
 #####################################################################################*/

/*****************************************************************************/
/*! Tests if any data in Terminal Uart FIFO  
 *   \return   0 -- FIFO is empty
 1 -- data in FIFO                                              */
/*****************************************************************************/
int HOSTAL_kbhit(void)
{
  DRV_UART_STATE_E eState;

  while(DRV_OK != DRV_UART_GetState(&g_tUART, &eState))
    ;

  if((eState & DRV_UART_STATE_RXFE) != 0) /* check if there is data in the FIFO  */
    return 0; /* no, FIFO is empty  */
  else
    return 1; /* yes, data in FIFO  */
}

/**************************************************************************************/
/*! Grab a character from the keyboard
 *  This will block if no character is in the terminal/keyboard input buffer until a key is pressed.
 *
 *  \return The key read from the keyboard buffer                                     */
/**************************************************************************************/
int HOSTAL_getchar(void)
{
  return getchar();
}

/*****************************************************************************/
/*! Sleeps for the given time  
 *   \param ulSleepTimeMs Time in ms to sleep (0 will sleep for 50us)         */
/*****************************************************************************/
void HOSTAL_Sleep(uint32_t ulSleepTimeMs)
{
  OS_Sleep(ulSleepTimeMs);
}


/*****************************************************************************/
/*! Load Remanent Data
 *   \param szName
 *   \param ulMaxDataSize
 *   \param pbData
 *   \param pulDataLen                                                       */
/*****************************************************************************/

HOSTAL_RESULT_E HOSTAL_RemanentData_Load(char* szName, uint32_t ulMaxDataSize, uint8_t* pbData, uint32_t* pulDataLen)
{
  // This function need to be filled with sourcecode by customer to load data from remanent customer storage which the customer has connected to the netX90
  return HOSTAL_OK;
}

/*****************************************************************************/
/*! Store Remanent Data
 *   \param szName
 *   \param pbData
 *   \param pulDataLen                                                       */
/*****************************************************************************/

HOSTAL_RESULT_E HOSTAL_RemanentData_Store(char* szName, uint8_t* pbData, uint32_t ulDataLen)
{
  // This function need to be filled with sourcecode by customer to store data remanent in the customer storage which the customer has connected to the netX90.
  return HOSTAL_OK;
}


