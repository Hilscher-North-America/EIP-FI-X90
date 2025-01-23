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

/**************************************************************************************/
/*! \file  hostAbstractionLayer.h
*   Host Abstraction Layer - header file                                              */
/**************************************************************************************/

#ifndef COMPONENTS_HOST_ABSTRACTION_LAYER_H_
#define COMPONENTS_HOST_ABSTRACTION_LAYER_H_

#include <stdint.h>
#include "cifXUser.h"
#include "Hil_Compiler.h"
#include "led.h"

/************************************************************************/
/*! Host abstraction layer result codes */
typedef enum HOSTAL_RESULT_Etag
{
  HOSTAL_OK,
  HOSTAL_ERROR,
  HOSTAL_UNKNOWN_DEVICE_STATUS,
  HOSTAL_UNKNOWN_CALLBACK_TYPE,
  HOSTAL_INVALID_TYPE,
  HOSTAL_INVALID_VALUE,
} HOSTAL_RESULT_E;

/************************************************************************/
/* TODO: KM - What callback types are really necessary? */
/*! Callback types
 */
typedef enum HOSTAL_CALLBACK_TYPE_Etag
{
  HOSTAL_CB_INPUT,  /*!< TODO: KM - ????????????????? */
  HOSTAL_CB_DIAG,   /*!< TODO: KM - ????????????????? */
  HOSTAL_CB_ROTARY, /*!< Callback will be invoked on changes on the rotary switche(s). */
  HOSTAL_CB_TIMER,  /*!< Callback will be invoked on each timer cycle.                 */

  HOSTAL_CB_NUMBER_OF_ENTRIES, /** used internally to indicate the number of callback types */
} HOSTAL_CALLBACK_TYPE_E;

/************************************************************************/
/*! Peripheral states */
typedef enum HOSTAL_PERIPHERAL_STATE_Etag
{
  HOSTAL_PERIPHERAL_STATE_OK = 0,
  HOSTAL_PERIPHERAL_STATE_WARNING,
  HOSTAL_PERIPHERAL_STATE_ERROR,
} HOSTAL_PERIPHERAL_STATE_E;

/************************************************************************/
/*! Device status flags.
 *  There can be more that one status flag active at a time.
 *  The device status can be set/get with the following functions:
 *    HOSTAL_Device_SetStatus()
 *    HOSTAL_Device_ClearStatus()
 *    HOSTAL_Device_GetStatus()
 *  */
#define   HOSTAL_DEVICE_STATUS_ALARM  0x0001
#define   HOSTAL_DEVICE_STATUS_ERROR  0x0002
#define   HOSTAL_DEVICE_STATUS_DATA   0x0004

/************************************************************************/
/*! Device product identification.
 *  */
#define HOST_PRODUCT_CODE_EIS PRODUCT_CODE_EIS_EXTENDED_DEMO_2

/************************************************************************/
/*! Device resource sizes, cyclic, acyclic data sizes.
 *  */
#define MAX_IO_DATA_SIZE              8

/*****************************************************************************/
/*! PROCESS DATA                                                             */
/*****************************************************************************/
#ifdef __HIL_PRAGMA_PACK_ENABLE
#pragma __HIL_PRAGMA_PACK_1(APP_DATA)
#endif

typedef __HIL_PACKED_PRE struct APP_PROCESS_DATA_INPUT_Ttag /* Consumed data (PLC Output) */
{
  uint8_t  tData[MAX_IO_DATA_SIZE];   /** Double buffer for SPI RX and IoRead. Consumed process data. Data that is received from the PLC. */

} __HIL_PACKED_POST APP_PROCESS_DATA_INPUT_T;

typedef __HIL_PACKED_PRE struct APP_PROCESS_DATA_OUTPUT_Ttag /* Produced data (PLC Input) */
{  
  uint8_t tData[MAX_IO_DATA_SIZE];       /** Double buffer for SPI TX and IoWriteProduced process data. Data that is sent to the PLC.       */

} __HIL_PACKED_POST APP_PROCESS_DATA_OUTPUT_T;

#ifdef __HIL_PRAGMA_PACK_ENABLE
#pragma __HIL_PRAGMA_UNPACK_1(APP_DATA)
#endif

/*****************************************************************************/
/*! ACYCLIC DATA                                                             */
/*****************************************************************************/
typedef __HIL_PACKED_PRE struct APP_ACYCLIC_DATA_Ttag
{
  uint16_t usSensor2_StatusCode;      /* Is updated App_IODataHandler()                       */

} __HIL_PACKED_POST APP_ACYCLIC_DATA_T;


/*####################################################################################
 *  ___ _   _ ___ _____ ___    _    _     ___ _____   _  _____ ___ ___  _   _
 * |_ _| \ | |_ _|_   _|_ _|  / \  | |   |_ _|__  /  / \|_   _|_ _/ _ \| \ | |
 *  | ||  \| || |  | |  | |  / _ \ | |    | |  / /  / _ \ | |  | | | | |  \| |
 *  | || |\  || |  | |  | | / ___ \| |___ | | / /_ / ___ \| |  | | |_| | |\  |
 * |___|_| \_|___| |_| |___/_/   \_\_____|___/____/_/   \_\_| |___\___/|_| \_|
 *
 ####################################################################################*/

/**************************************************************************************/
/*! Initializes the host abstraction layer
 *   \return HOSTAL_OK on success                                                     */
/**************************************************************************************/
HOSTAL_RESULT_E HOSTAL_Init(void);


/*####################################################################################
 *  ____  _______     _____ ____ _____   ____ _____  _  _____ _   _ ____
 * |  _ \| ____\ \   / /_ _/ ___| ____| / ___|_   _|/ \|_   _| | | / ___|
 * | | | |  _|  \ \ / / | | |   |  _|   \___ \ | | / _ \ | | | | | \___ \
 * | |_| | |___  \ V /  | | |___| |___   ___) || |/ ___ \| | | |_| |___) |
 * |____/|_____|  \_/  |___\____|_____| |____/ |_/_/   \_\_|  \___/|____/
 *
 ####################################################################################*/

/**************************************************************************************/
/*! Set one or more device status flags
 *  E.g.: These flags can be used to control some device LEDs.
 *
 *  \param  usStatus [in]  Status flags to be set (use HOSTAL_DEVICE_STATUS_...).
 *                         Note: This function only SETs status flags.
 *                               The status flags coming with usStatus will be
 *                               ORed with the current status flags:
 *
 *                               usNewStatus = usCurrentStatus | usStatus;
 *
 *                               It does not clear already existing status flags.
 *                               To clear status flags use the function
 *                               HOSTAL_Device_ClearStatus()
 *
 *   \return HOSTAL_OK on success                                                     */
/**************************************************************************************/
HOSTAL_RESULT_E HOSTAL_Device_SetStatus(Led_Status_Indicators eLed, uint16_t usStatus);


/**************************************************************************************/
/*! Clear one or more device status flags
 *  E.g.: These flags can be used to control some device LEDs.
 *
 *  \param  usStatus [in]  Status flags to be cleared (use HOSTAL_DEVICE_STATUS_...).
 *                         Note: This function only CLEARs status flags.
 *                               The status flags coming with usStatus mark the
 *                               flags in the current status that shall be cleared.
 *
 *                               usNewStatus = usCurrentStatus & ~(usStatus);
 *
 *   \return HOSTAL_OK on success                                                     */
/**************************************************************************************/
HOSTAL_RESULT_E HOSTAL_Device_ClearStatus(uint16_t usStatus);


/**************************************************************************************/
/*! Get the current device status flags.
 *
 *  \param  pusStatus  [out]  Pointer that is used to store the
 *                            current device status (HOSTAL_DEVICE_STATUS_...).
 *
 *  \return HOSTAL_OK on success                                                      */
/**************************************************************************************/
HOSTAL_RESULT_E HOSTAL_Device_GetStatus(uint16_t* pusStatus);


/*####################################################################################
 *      _    ____ _____ _   _   _  _____ ___  ____
 *     / \  / ___|_   _| | | | / \|_   _/ _ \|  _ \
 *    / _ \| |     | | | | | |/ _ \ | || | | | |_) |
 *   / ___ \ |___  | | | |_| / ___ \| || |_| |  _ <
 *  /_/   \_\____| |_|  \___/_/   \_\_| \___/|_| \_\
 *
 #####################################################################################*/

/**************************************************************************************/
/*! Write actuator data.
 *
 *  \param  bActuatorNumber  [in]  Actuator number (0, 1)
 *  \param  usData           [in]  Data to be written.
 *
 *  \return HOSTAL_OK on success                                                      */
/**************************************************************************************/
HOSTAL_RESULT_E HOSTAL_Actuator_SetData(uint8_t bActuatorNumber, uint16_t usData);


/**************************************************************************************/
/*! Write actuator mode.
 *
 *  \param  bActuatorNumber  [in]  Actuator number (0, 1)
 *  \param  bMode            [in]  Actuator mode (0-255)
 *
 *  \return HOSTAL_OK on success                                                      */
/**************************************************************************************/
HOSTAL_RESULT_E HOSTAL_Actuator_SetMode(uint8_t bActuatorNumber, uint8_t bMode);


/**************************************************************************************/
/*! Read actuator state.
 *
 *  \param  bActuatorNumber  [in]   Actuator number (0, 1)
 *  \param  pusStatusCode    [out]  Pointer to store the actuator status code.
 *                                  The status code is != 0 in case the actuator
 *                                  state is != HOSTAL_PERIPHERAL_STATE_OK.
 *
 *  \return HOSTAL_PERIPHERAL_STATE_E                                                 */
/**************************************************************************************/
HOSTAL_PERIPHERAL_STATE_E HOSTAL_Actuator_GetState(uint8_t bActuatorNumber, uint16_t* pusStatusCode);

HOSTAL_RESULT_E HOSTAL_Device_UpdateStatus (CIFXHANDLE hChannel);


/*####################################################################################
 *  ____  _____ _   _ ____   ___  ____
 * / ___|| ____| \ | / ___| / _ \|  _ \
 * \___ \|  _| |  \| \___ \| | | | |_) |
 *  ___) | |___| |\  |___) | |_| |  _ <
 * |____/|_____|_| \_|____/ \___/|_| \_\
 *
 #####################################################################################*/
 #define TEMP_SENSOR 1

/**************************************************************************************/
/*! Read sensor data.
 *
 *  \param  bsensorNumber    [in]  sensor number (0, 1)
 *  \param  pusData          [out] Pointer used to store sensor data.
 *
 *  \return HOSTAL_OK on success                                                      */
/**************************************************************************************/
HOSTAL_RESULT_E HOSTAL_Sensor_GetData(uint8_t bSensorNumber, uint16_t* pusData);


/**************************************************************************************/
/*! Write sensor mode.
 *
 *  \param  bSensorNumber    [in]  Sensor number (0, 1)
 *  \param  bMode            [in]  Sensor mode (0-255)
 *
 *  \return HOSTAL_OK on success                                                      */
/**************************************************************************************/
HOSTAL_RESULT_E HOSTAL_Sensor_SetMode(uint8_t bSensorNumber, uint8_t bMode);


/**************************************************************************************/
/*! Read sensor state.
 *
 *  \param  bSensorNumber    [in]   Sensor number (0, 1)
 *  \param  pusStatusCode    [out]  Pointer to store the sensor status code.
 *                                  The status code is != 0 in case the sensor
 *                                  state is != HOSTAL_PERIPHERAL_STATE_OK.
 *
 *  \return HOSTAL_PERIPHERAL_STATE_E                                                 */
/**************************************************************************************/
HOSTAL_PERIPHERAL_STATE_E HOSTAL_Sensor_GetState  (uint8_t bSensorNumber, uint16_t* pusStatusCode);

/*##########################################
 *  ____  ____ ___ 
 * / ___||  _ \_ _|
 * \___ \| |_) | | 
 *  ___) |  __/| | 
 * |____/|_|  |___|                
 *
############################################*/


/**************************************************************************************/
/*! This function receives data coming from the PLC, and sends data going to the PLC.
 *  It also does the Header bytes & CRC16 checking for both Input Data & Output Data.
 *
 *  \param  ptInputData            Data that is received from the PLC.
 *  \param  ptOutputData           Data that is sent to the PLC.
 *  \param  tInputDataValid        Input valid flag
 *
 *  \return HOSTAL_OK on success                                                      */
/**************************************************************************************/

HOSTAL_RESULT_E HOSTAL_DataExchange(void *ptInputData, void* ptOutputData, bool tInputDataValid);


/*####################################################################################
 *  ____   ___ _____  _    ______   __  ______        _____ _____ ____ _   _
 * |  _ \ / _ \_   _|/ \  |  _ \ \ / / / ___\ \      / /_ _|_   _/ ___| | | |
 * | |_) | | | || | / _ \ | |_) \ V /  \___ \\ \ /\ / / | |  | || |   | |_| |
 * |  _ <| |_| || |/ ___ \|  _ < | |    ___) |\ V  V /  | |  | || |___|  _  |
 * |_| \_\\___/ |_/_/   \_\_| \_\|_|   |____/  \_/\_/  |___| |_| \____|_| |_|
 *
 #####################################################################################*/

/**************************************************************************************/
/*! Read rotary switch value.
 *
 *  \param  bRotarySwitchNumber    [in]   Rotary switch number
 *  \param  pbValue                [out]  Pointer to store the rotary switch value.
 *
 *  \return HOSTAL_OK on success                                                      */
/**************************************************************************************/
HOSTAL_RESULT_E HOSTAL_RotarySwitch_GetValue(uint8_t bRotarySwitchNumber, uint8_t* pbValue);


/*####################################################################################
 *  _____ _____ _     ______ _____ _____ _____ _____
 * /  ___|  ___| |    |  ___|_   _|  ___/  ___|_   _|
 * \ `--.| |__ | |    | |_    | | | |__ \ `--.  | |
 *  `--. \  __|| |    |  _|   | | |  __| `--. \ | |
 * /\__/ / |___| |____| |     | | | |___/\__/ / | |
 * \____/\____/\_____/\_|     \_/ \____/\____/  \_/
 *
 #####################################################################################*/

/**************************************************************************************/
/*! Performs a target specific self test.
 *  \return HOSTAL_OK on success                                                      */
/**************************************************************************************/
HOSTAL_RESULT_E HOSTAL_SelfTest( void );

/*####################################################################################
 *  ____  _____ __  __    _    _   _ _____ _   _ _____   ____    _  _____  _
 * |  _ \| ____|  \/  |  / \  | \ | | ____| \ | |_   _| |  _ \  / \|_   _|/ \
 * | |_) |  _| | |\/| | / _ \ |  \| |  _| |  \| | | |   | | | |/ _ \ | | / _ \
 * |  _ <| |___| |  | |/ ___ \| |\  | |___| |\  | | |   | |_| / ___ \| |/ ___ \
 * |_| \_\_____|_|  |_/_/   \_\_| \_|_____|_| \_| |_|   |____/_/   \_\_/_/   \_\
 *
 #####################################################################################*/

/**************************************************************************************/
/*! Store remanent (non volatile) data to flash.
 *
 *  \param  szName        [in]  Name that is stored together with the data.
 *  \param  pbData        [in]  Pointer to data that shall be stored.
 *  \param  ulDataLen     [in]  Number of bytes that shall be stored.
 *
 *  \return HOSTAL_OK on success                                                      */
/**************************************************************************************/
HOSTAL_RESULT_E HOSTAL_RemanentData_Store(char* szName, uint8_t* pbData, uint32_t ulDataLen);


/**************************************************************************************/
/*! Read remanent (non volatile) data from flash.
 *
 *  \param  szName         [in]   Name that was used to store remanent data.
 *                                (see HOSTAL_RemanentData_Store())
 *  \param  ulMaxDataSize  [in]   Buffer size of pbData (number of bytes).
 *  \param  pbData         [out]  Pointer to stored remanent data.
 *  \param  ulDataLen      [out]  Size of actually read remanent data (number of bytes).
 *
 *  \return HOSTAL_OK on success                                                      */
/**************************************************************************************/
HOSTAL_RESULT_E HOSTAL_RemanentData_Load(char* szName, uint32_t ulMaxDataSize, uint8_t* pbData, uint32_t* pulDataLen);


/*####################################################################################
 *   ____    _    _     _     ____    _    ____ _  ______
 *  / ___|  / \  | |   | |   | __ )  / \  / ___| |/ / ___|
 * | |     / _ \ | |   | |   |  _ \ / _ \| |   | ' /\___ \
 * | |___ / ___ \| |___| |___| |_) / ___ \ |___| . \ ___) |
 *  \____/_/   \_\_____|_____|____/_/   \_\____|_|\_\____/
 *
 #####################################################################################*/

/**************************************************************************************/
/*! Register a callback function at the host abstraction layer.
 *  Note: the registered callback function will be called by the host abstraction layer not
 *        before HOSTAL_Callbacks_Enable() is called.
 *
 *  \param  eClbType        [in]  Type of the callback function that shall be registered.
 *  \param  pfnEventClb     [in]  Callback function to register.
 *  \param  pPrm            [in]  Parameter pointer that is provided when callback is called.
 *
 *  \return HOSTAL_OK on success                                                      */
/**************************************************************************************/
HOSTAL_RESULT_E HOSTAL_Callback_Register(HOSTAL_CALLBACK_TYPE_E eClbType, void(*pfnEventClb)(void* pPrm), void* pPrm);

/**************************************************************************************/
/*! Enable all registered callback.
 *  From now on the previously registered callback functions
 *  can be called by the host abstraction layer.
 *
 *  \return HOSTAL_OK on success                                                      */
/**************************************************************************************/
HOSTAL_RESULT_E HOSTAL_Callbacks_Enable();


/**************************************************************************************/
/*! Disable all registered callback.
 *  From now on the registered callback functions must not be called by the host abstraction layer.
 *
 *  \return HOSTAL_OK on success                                                      */
/**************************************************************************************/
HOSTAL_RESULT_E HOSTAL_Callbacks_Disable();


/*####################################################################################
 *  _   _ _____ _     ____  _____ ____  ____
 * | | | | ____| |   |  _ \| ____|  _ \/ ___|
 * | |_| |  _| | |   | |_) |  _| | |_) \___ \
 * |  _  | |___| |___|  __/| |___|  _ < ___) |
 * |_| |_|_____|_____|_|   |_____|_| \_\____/
 *
 #####################################################################################*/

/**************************************************************************************/
/*! Test whether there is at least one character in the terminal/keyboard input buffer.
 *
 *  \return 0: No data available.
 *          1: There is data available                                                */
/**************************************************************************************/
int  HOSTAL_kbhit(void);

/**************************************************************************************/
/*! Grab a character from the keyboard
 *  This will block if no character is in the terminal/keyboard input buffer until a key is pressed.
 *
 *  \return The key read from the keyboard buffer                                     */
/**************************************************************************************/
int HOSTAL_getchar(void);

/**************************************************************************************/
/*! Makes the calling thread to sleep for a given time
 *
 *  \param  ulSleepTimeMs  [in]  Time to sleep in milliseconds.
 *                                                                                    */
/**************************************************************************************/
void HOSTAL_Sleep(uint32_t ulSleepTimeMs);


#endif
