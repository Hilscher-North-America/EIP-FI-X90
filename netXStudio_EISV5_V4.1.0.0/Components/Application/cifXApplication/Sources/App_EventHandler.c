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
/*! General Inclusion Area                                                            */
/**************************************************************************************/

#include "App_EventHandler.h"
#include "hostAbstractionLayer.h"
#include "event_handler.h"

/**************************************************************************************/
/*! DIAGNOSIS STRUCTURES                                                              */
/**************************************************************************************/

typedef struct APP_DIAG_Ttag
{
  bool fAlarm1;
  bool fAlarm2;
  bool fAlarm3;
  bool fAlarm4;

  bool fError1;
  bool fError2;
  bool fError3;
  bool fError4;
} APP_DIAG_T;


typedef struct APP_PERIPHERAL_DIAG_Ttag
{
  HOSTAL_PERIPHERAL_STATE_E eSensor1;
  HOSTAL_PERIPHERAL_STATE_E eSensor2;

  HOSTAL_PERIPHERAL_STATE_E eActuator1;
  HOSTAL_PERIPHERAL_STATE_E eActuator2;

} APP_PERIPHERAL_DIAG_T;

/**************************************************************************************/
/*! FUNCTIONS                                                                         */
/**************************************************************************************/

/**************************************************************************************/
/*! Checks for for certain device specific events like state changes of
 *  actuators or sensors.
 *   It detects state changes like switch of a DIP switch or changed variables
 *   (I.e. states of certain elements are stored in this function)
 *   If the state of an element changes, a respective EVENT is signaled to the
 *   protocol specific event handler function (Protocol_EventHandler).
 *   Furthermore, the specified generic hardware action might be implemented in
 *   this function, like e.g. switching on/off status LEDs
 *
 *   \param ptAppData  [in]  Pointer to application data                              */
 /*************************************************************************************/
void App_EventHandler( APP_DATA_T* ptAppData )
{
  HOST_EvnetHandler(ptAppData);
  
#ifdef HOST_DIAG
  static APP_DIAG_T       s_tDiag_Last             = {0};
  APP_DIAG_T              tDiag_Current            = {0};
  APP_PERIPHERAL_DIAG_T   tPerDiag_Current         = {0};
  APP_EVENT_E             eEvent                   = APP_EVENT_NONE;
  bool                    fUpdateDeviceAlarmStatus = false;
  bool                    fUpdateDeviceErrorStatus = false;

  /*===================== Get current sensor and actuator states =====================*/

  tPerDiag_Current.eSensor1   = HOSTAL_Sensor_GetState  ( 0, &ptAppData->tAcyclicData.usSensor1_StatusCode   );
  tPerDiag_Current.eSensor2   = HOSTAL_Sensor_GetState  ( 1, &ptAppData->tAcyclicData.usSensor2_StatusCode   );
  tPerDiag_Current.eActuator1 = HOSTAL_Actuator_GetState( 0, &ptAppData->tAcyclicData.usActuator1_StatusCode );
  tPerDiag_Current.eActuator2 = HOSTAL_Actuator_GetState( 1, &ptAppData->tAcyclicData.usActuator2_StatusCode );

  /*===================== Determine whether there are device specific alarms or errors =====================*/

  tDiag_Current.fAlarm1 = (tPerDiag_Current.eSensor1   == HOSTAL_PERIPHERAL_STATE_WARNING);
  tDiag_Current.fAlarm2 = (tPerDiag_Current.eSensor2   == HOSTAL_PERIPHERAL_STATE_WARNING);
  tDiag_Current.fAlarm3 = (tPerDiag_Current.eActuator1 == HOSTAL_PERIPHERAL_STATE_WARNING);
  tDiag_Current.fAlarm4 = (tPerDiag_Current.eActuator2 == HOSTAL_PERIPHERAL_STATE_WARNING);

  tDiag_Current.fError1 = (tPerDiag_Current.eSensor1   == HOSTAL_PERIPHERAL_STATE_ERROR);
  tDiag_Current.fError2 = (tPerDiag_Current.eSensor2   == HOSTAL_PERIPHERAL_STATE_ERROR);
  tDiag_Current.fError3 = (tPerDiag_Current.eActuator1 == HOSTAL_PERIPHERAL_STATE_ERROR);
  tDiag_Current.fError4 = (tPerDiag_Current.eActuator2 == HOSTAL_PERIPHERAL_STATE_ERROR);

  /*===================== check for changes in "Alarm 1" =====================*/

  if( tDiag_Current.fAlarm1 != s_tDiag_Last.fAlarm1 )
  {
    if( false == tDiag_Current.fAlarm1 )
      eEvent = APP_EVENT_CLEAR_ALARM1;
    else
      eEvent = APP_EVENT_SET_ALARM1;

    App_AllChannels_EventHandler( ptAppData, eEvent, ptAppData->tAcyclicData.usSensor1_StatusCode );
    fUpdateDeviceAlarmStatus = true;
  }

  /*===================== check for changes in "Alarm 2" =====================*/

  if( tDiag_Current.fAlarm2 != s_tDiag_Last.fAlarm2 )
  {
    if( false == tDiag_Current.fAlarm2 )
      eEvent = APP_EVENT_CLEAR_ALARM2;
    else
      eEvent = APP_EVENT_SET_ALARM2;

    App_AllChannels_EventHandler( ptAppData, eEvent, ptAppData->tAcyclicData.usSensor2_StatusCode );
    fUpdateDeviceAlarmStatus = true;
  }

  /*===================== check for changes in "Alarm 3" =====================*/

  if( tDiag_Current.fAlarm3 != s_tDiag_Last.fAlarm3 )
  {
    if( false == tDiag_Current.fAlarm3 )
      eEvent = APP_EVENT_CLEAR_ALARM3;
    else
      eEvent = APP_EVENT_SET_ALARM3;

    App_AllChannels_EventHandler( ptAppData, eEvent, ptAppData->tAcyclicData.usActuator1_StatusCode );
    fUpdateDeviceAlarmStatus = true;
  }

  /*===================== check for changes in "Alarm 4" =====================*/

  if( tDiag_Current.fAlarm4 != s_tDiag_Last.fAlarm4 )
  {
    if( false == tDiag_Current.fAlarm4 )
      eEvent = APP_EVENT_CLEAR_ALARM4;
    else
      eEvent = APP_EVENT_SET_ALARM4;

    App_AllChannels_EventHandler( ptAppData, eEvent, ptAppData->tAcyclicData.usActuator2_StatusCode );
    fUpdateDeviceAlarmStatus = true;
  }

  /*===================== check for changes in "Error 1" =====================*/

  if( tDiag_Current.fError1 != s_tDiag_Last.fError1 )
  {
    if( false == tDiag_Current.fError1 )
      eEvent = APP_EVENT_CLEAR_ERROR1;
    else
      eEvent = APP_EVENT_SET_ERROR1;

    App_AllChannels_EventHandler( ptAppData, eEvent, ptAppData->tAcyclicData.usSensor1_StatusCode );
    fUpdateDeviceErrorStatus = true;
  }

  /*===================== check for changes in "Error 2" =====================*/

  if( tDiag_Current.fError2 != s_tDiag_Last.fError2 )
  {
    if( false == tDiag_Current.fError2 )
      eEvent = APP_EVENT_CLEAR_ERROR2;
    else
      eEvent = APP_EVENT_SET_ERROR2;

    App_AllChannels_EventHandler( ptAppData, eEvent, ptAppData->tAcyclicData.usSensor2_StatusCode );
    fUpdateDeviceErrorStatus = true;
  }

  /*===================== check for changes in "Error 3" =====================*/

  if( tDiag_Current.fError3 != s_tDiag_Last.fError3 )
  {
    if( false == tDiag_Current.fError3 )
      eEvent = APP_EVENT_CLEAR_ERROR3;
    else
      eEvent = APP_EVENT_SET_ERROR3;

    App_AllChannels_EventHandler( ptAppData, eEvent, ptAppData->tAcyclicData.usActuator1_StatusCode );
    fUpdateDeviceErrorStatus = true;
  }

  /*===================== check for changes in "Error 4" =====================*/

  if( tDiag_Current.fError4 != s_tDiag_Last.fError4 )
  {
    if( false == tDiag_Current.fError4 )
      eEvent = APP_EVENT_CLEAR_ERROR4;
    else
      eEvent = APP_EVENT_SET_ERROR4;

    App_AllChannels_EventHandler( ptAppData, eEvent, ptAppData->tAcyclicData.usActuator2_StatusCode );
    fUpdateDeviceErrorStatus = true;
  }

  /*===================== Check for rotary switch values =====================*/
  {
    uint8_t bSwitchValue;
    HOSTAL_RotarySwitch_GetValue( 0, &bSwitchValue );
    /* TODO: Implement application specific handling of rotary switch. */
  }

  /*===================== Update device status =====================*/

  if( fUpdateDeviceAlarmStatus )
  {
    if( tDiag_Current.fAlarm1 || tDiag_Current.fAlarm2 || tDiag_Current.fAlarm3 || tDiag_Current.fAlarm4 )
    {
      HOSTAL_Device_SetStatus( HOSTAL_DEVICE_STATUS_ALARM );
    }
    else
    {
      HOSTAL_Device_ClearStatus( HOSTAL_DEVICE_STATUS_ALARM );
    }
  }

  if( fUpdateDeviceErrorStatus )
  {
    if( tDiag_Current.fError1|| tDiag_Current.fError2 || tDiag_Current.fError3 || tDiag_Current.fError4 )
    {
      HOSTAL_Device_SetStatus( HOSTAL_DEVICE_STATUS_ERROR );
    }
    else
    {
      HOSTAL_Device_ClearStatus( HOSTAL_DEVICE_STATUS_ERROR );
    }
  }

  s_tDiag_Last = tDiag_Current;
#endif
}
