/**************************************************************************************
Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
***************************************************************************************
$Id: eip_eif_packetdefinitions_connections.h 73908 2018-01-30 09:15:51Z MarcBommert $:

Description:
This header provides definitions for connection states and connection types.
This information is presented to the host application with the indication
EIP_OBJECT_CONNECTION_IND whenever the state of an EtherNet/IP connection towards the
device changes.

These definitions contribute to the LFW API of the EtherNet/IP stack and are
applicable to the DPM packet interface.

**************************************************************************************/

#ifndef eip_eif_PACKETDEFINITIONS_FORCE_LED_STATE_H_
#define eip_eif_PACKETDEFINITIONS_FORCE_LED_STATE_H_

/* Led types and states */

typedef enum CIP_LED_TYPE_Etag
{
  CIP_LED_TYPE_MODULE_STATUS = 0,   /*!< 0: Module status LED */
  CIP_LED_TYPE_NETWORK_STATUS,      /*!< 1: Network status LED */
} CIP_LED_TYPE_E;


typedef enum CIP_LED_FORCE_STATE_Etag
{
  CIP_LED_STATE_NO_FORCING = 0,            /* stack will derive the LED state from the current device state (disable previous forcing of selected LED) */
  CIP_LED_STATE_FORCE_OFF,                 /* force selected LED off */
  CIP_LED_STATE_FORCE_RED,                 /* force selected LED red */
  CIP_LED_STATE_FORCE_GREEN,               /* force selected LED green */
  CIP_LED_STATE_FORCE_RED_FLASH_1HZ,       /* force selected LED red flashing with 1Hz interval */
  CIP_LED_STATE_FORCE_GREEN_FLASH_1HZ,     /* force selected LED green flashing 1Hz interval */
  CIP_LED_STATE_FORCE_RED_GREEN_FLASH_1HZ, /* force selected LED red-green flashing with 1Hz (new for CIPSafety) */
  CIP_LED_STATE_FORCE_RED_GREEN_FLASH_2HZ, /* force selected LED red-green flashing with 2Hz (new for CIPSafety) */
} CIP_LED_FORCE_STATE_E;

#endif
