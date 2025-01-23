/*
 * led.h
 *
 *  Created on: Sep 28, 2022
 *      Author: Vedprakash.Pande
 */

#ifndef COMPONENTS_HAL_INCLUDE_LED_H_
#define COMPONENTS_HAL_INCLUDE_LED_H_

#include "mks_types.h"

/* network status */
#define HIL_MB_NETWORK_STATUS_NO_POWER        0
#define HIL_MB_NETWORK_STATUS_NO_CONNECTION   1
#define HIL_MB_NETWORK_STATUS_INITIALIZING    2
#define HIL_MB_NETWORK_STATUS_STANDBY         3
#define HIL_MB_NETWORK_STATUS_CONNECTED       4
#define HIL_MB_NETWORK_STATUS_CONFIG_ERROR    5
#define HIL_MB_NETWORK_STATUS_WAIT_FOR_LEASE  6

/* health status */
#define HIL_MB_HEALTH_STATUS_NO_POWER         0
#define HIL_MB_HEALTH_STATUS_FW_FAULT         1
#define HIL_MB_HEALTH_STATUS_FW_INITIALIZING  2
#define HIL_MB_HEALTH_STATUS_DIAG_MODE        3
#define HIL_MB_HEALTH_STATUS_CONNECTED        4
#define HIL_MB_HEALTH_STATUS_WD_FAULT         5
#define HIL_MB_HEALTH_STATUS_HOT              6

/*
 * Assign DIO pins for LED control, as per hardware design.
 * Must also use corresponding '.hwc' file.
 */
//#define LED_DRV_NRPEB_H90_BOARD
#define LED_DRV_CDN496_TARGET_HW

/*
 * Include serial console interface, for testing purposes.
 */
#define LED_DRV_INCLUDE_SERIAL_CONSOLE

#ifdef LED_DRV_INCLUDE_SERIAL_CONSOLE
#define LED_SHELL_CMDS \
  {"led_selftest" , console_led_selftest , NULL, "Test LED driver with predefined test sequence"},              \
  {"led_testmode" , console_led_testmode , NULL, "<true,false> Couple / decouple LED driver with serial console testing"},   \
  {"led_set" , console_led_set , NULL, "<net,hlt> <red,green,amber,off> Test by assigning a state to individual LED"},

#else
#define LED_SHELL_CMDS
#endif

/*
 * Valid values of LED indicators that can be set with API
 * Number of entries is equal to number of LEDs.
 */
typedef enum {
  kLED_ID_NET = 0,
  kLED_ID_HLT,
  kNO_OF_LED_ID
}Led_Status_Indicators;

/*
 * Invoke LED_StateMachine() with one of following values.
 *  kLED_EXEC_IMMEDIATE - Execute immediately.
 *  kLED_EXEC_PERIODIC - Execute after period is expired / repeatedly in a loop.
 */
typedef enum {
  kLED_EXEC_IMMEDIATE = 0,
  kLED_EXEC_PERIODIC,
  kNO_OF_EXEC_METHODS
}Led_StateMachine_Execution;

/*
 * Valid states of LED indicators that can be set with API
 */
typedef enum {
  kLED_ST_MIN = 0,
  kLED_ST_OFF = 0x00,
  kLED_ST_SOLID_RED = 0x01,
  kLED_ST_SOLID_GREEN = 0x02,
  kLED_ST_SOLID_AMBER = 0x03,                   /* Both Red & Green LEDs are on. */
  kLED_ST_FLASHING_RED = 0x04,
  kLED_ST_FLASHING_GREEN = 0x08,
  kLED_ST_FLASHING_AMBER = 0x0C,                /* Both Red & Green LEDs are flashing. */
  kNO_OF_LED_STATES
}Led_States;

extern uint16_t s_usHealthStatus;
extern uint16_t s_usNetworkStatus;

MksReturnCodes LED_Init();
MksReturnCodes LED_StateMachine(Led_StateMachine_Execution led_exec);
MksReturnCodes LED_Set(Led_Status_Indicators led_id, Led_States led_state);
MksReturnCodes LED_SetBlinkTime(Led_Status_Indicators led_id, uint32_t tmp_blink_time_ms);

#ifdef LED_DRV_INCLUDE_SERIAL_CONSOLE
uint32_t console_led_selftest( char *strCmdBuffer);
uint32_t console_led_testmode( char *strCmdBuffer );
uint32_t console_led_set( char *strCmdBuffer );
#endif

void HOSTAL_Health_Led_Handle();
void HOSTAL_Network_Led_Handle();

#endif /* COMPONENTS_HAL_INCLUDE_LED_H_ */
