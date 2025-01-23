#include "App_Application.h"
#include "App_SockIfFunctions.h"
#include "RTC.h"
#include "Watchdog_driver.h"
#include "Watchdog.h"
#include "Temp_Sensor.h"
#include "FI.h"
#include "sys.h"
#include "switch.h"

void HOST_EvnetHandler(APP_DATA_T* ptAppData)
{
  /*===================== Get temperature sensor state =====================*/
  HOSTAL_Sensor_GetState ( TEMP_SENSOR, &ptAppData->tAcyclicData.usSensor2_StatusCode );
  
  /*===================== Check for rotary switch values =====================*/

  // uint16_t usSwitchValue = SWITCH_get_value ();
    
  /* TODO: Implement application specific handling of rotary switch. */
  // TODO: Init only or during RUNTIME also?
  
  /*===================== Update netX time =====================*/
  HOSTAL_Update_netXTime ();
  
  /*===================== Kick Hardware Watchdog ==============*/
  WatchdogKick();

  /* Watchdog update */
  Watchdog_Update ();

  /*===================== Read temperature sensor value ==============*/
  Sensor_UpdateLocalTemp();
  
  /* Update Device Status */
  HOSTAL_Device_UpdateStatus(ptAppData->aptChannels[0]->hChannel);  

  /* Health LED Handle */
  HOSTAL_Health_Led_Handle();
  /* Network LED Handle */
  HOSTAL_Network_Led_Handle();

  /*============ run led statemachine (e.g. blinking) ==========*/
  LED_StateMachine(kLED_EXEC_PERIODIC);

  /* execute Manufacturer commands */
  UtilDoMfg();

  if (g_FI_unit_stats.mode & FI_DHCP_MODE)
    AppSock_InterfaceConfiguration(ptAppData);
}
