/**************************************************************************************
Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
***************************************************************************************
$Id: Ptp_Clock.h 52490 2015-03-24 09:40:36Z kai $:

Description:
  PTP - Clock definitions

**************************************************************************************/

#ifndef PTP_CLOCK_H_
#define PTP_CLOCK_H_

/*######################################################################################*/
/*               ____                _              _                                   */
/*              / ___|___  _ __  ___| |_ __ _ _ __ | |_ ___                             */
/*             | |   / _ \| '_ \/ __| __/ _` | '_ \| __/ __|                            */
/*             | |__| (_) | | | \__ \ || (_| | | | | |_\__ \                            */
/*              \____\___/|_| |_|___/\__\__,_|_| |_|\__|___/                            */
/*                                                                                      */
/*######################################################################################*/

/* IEEE 1588 - 7.6.2.5 clockAccuracy (table 6)*/
typedef enum
{
  /* 0x00 - 0x1F is reserved */
  PTP_CLOCK_ACCURACY_25_NS              = 0x20,
  PTP_CLOCK_ACCURACY_100_NS             = 0x21,
  PTP_CLOCK_ACCURACY_250_NS             = 0x22,
  PTP_CLOCK_ACCURACY_1_US               = 0x23,
  PTP_CLOCK_ACCURACY_2_5_US             = 0x24,
  PTP_CLOCK_ACCURACY_10_US              = 0x25,
  PTP_CLOCK_ACCURACY_25_US              = 0x26,
  PTP_CLOCK_ACCURACY_100_US             = 0x27,
  PTP_CLOCK_ACCURACY_250_US             = 0x28,
  PTP_CLOCK_ACCURACY_1_MS               = 0x29,
  PTP_CLOCK_ACCURACY_2_5_MS             = 0x2A,
  PTP_CLOCK_ACCURACY_10_MS              = 0x2B,
  PTP_CLOCK_ACCURACY_25_MS              = 0x2C,
  PTP_CLOCK_ACCURACY_100_MS             = 0x2D,
  PTP_CLOCK_ACCURACY_250_MS             = 0x2E,
  PTP_CLOCK_ACCURACY_1_S                = 0x2F,
  PTP_CLOCK_ACCURACY_10_S               = 0x30,
  PTP_CLOCK_ACCURACY_GREATER_THAN_10_S  = 0x31,
  /* 0x32 - 0x7F is reserved */
  /* 0x80 - 0xFD for use by alternate PTP profiles */
  PTP_CLOCK_ACCURACY_UNKNOWN            = 0xFE,
  PTP_CLOCK_ACCURACY_RESERVED           = 0xFF

} PTP_CLOCK_ACCURACY_E ;

/*######################################################################################*/
/* IEEE 1588 - 7.6.2.4 clockClass (table 5)*/
typedef enum
{
  PTP_CLOCK_CLASS_PRIMARY_REFERENCE       = 6,
  PTP_CLOCK_CLASS_PRIMARY_REFERENCE_HOLD  = 7,
  PTP_CLOCK_CLASS_DEGRADED_REFERENCE_A    = 52,  /* Master only  */
  PTP_CLOCK_CLASS_DEGRADED_REFERENCE_B    = 187, /* Master/Slave */
  PTP_CLOCK_CLASS_DEFAULT                 = 248,
  PTP_CLOCK_CLASS_SLAVE_ONLY              = 255,

  /* all other see 1588 specification */

} PTP_CLOCK_CLASS_E;

/*######################################################################################*/
/* IEEE 1588 - 7.6.2.6 timeSource (table 7)*/
typedef enum
{
  PTP_TIME_SOURCE_ATOMIC_CLOCK        = 0x10,
  PTP_TIME_SOURCE_GPS                 = 0x20,
  PTP_TIME_SOURCE_TERRESTRIAL_RADIO   = 0x30,
  PTP_TIME_SOURCE_PTP                 = 0x40,
  PTP_TIME_SOURCE_NTP                 = 0x50,
  PTP_TIME_SOURCE_HAND_SET            = 0x60,
  PTP_TIME_SOURCE_OTHER               = 0x90,
  PTP_TIME_SOURCE_INTERNAL_OSCILLATOR = 0xA0
} PTP_TIME_SOURCE_E;

/*######################################################################################*/
/* IEEE 1588 - 15.5.3.1.2.1 clockType (table 42)*/
typedef enum
{
  PTP_MM_TLV_CLOCK_TYPE_SLAVE_ONLY             = 0x0100,
  PTP_MM_TLV_CLOCK_TYPE_ORDINARY_CLOCK         = 0x0080,
  PTP_MM_TLV_CLOCK_TYPE_BOUNDARY_CLOCK         = 0x0040,
  PTP_MM_TLV_CLOCK_TYPE_P2P_TRANSPARENT_CLOCK  = 0x0020,
  PTP_MM_TLV_CLOCK_TYPE_E2E_TRANSPARENT_CLOCK  = 0x0010,
  PTP_MM_TLV_CLOCK_TYPE_MANAGEMENT_NODE        = 0x0008
  /* Reserved: bit 5 - F */
} PTP_CLOCK_TYPE_E;

/*######################################################################################*/

#endif /* PTP_CLOCK_H_ */
