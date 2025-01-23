/**************************************************************************************
Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
***************************************************************************************
$Id: Ptp_Datatypes.h 80412 2019-01-28 14:17:54Z kai $:

Description:
  PTP - Data types

**************************************************************************************/

#ifndef PTP_DATATYPES_H_
#define PTP_DATATYPES_H_

#include <stdint.h>
#include "Hil_Compiler.h"

/*######################################################################################*/
/*               ____        _          _____                                           */
/*              |  _ \  __ _| |_ __ _  |_   _|   _ _ __   ___  ___                      */
/*              | | | |/ _` | __/ _` |   | || | | | '_ \ / _ \/ __|                     */
/*              | |_| | (_| | || (_| |   | || |_| | |_) |  __/\__ \                     */
/*              |____/ \__,_|\__\__,_|   |_| \__, | .__/ \___||___/                     */
/*                                           |___/|_|                                   */
/*######################################################################################*/

typedef __HIL_PACKED_PRE struct PTP_TIME_Ttag
{
  int32_t lSeconds;
  int32_t lNanoseconds;
} __HIL_PACKED_POST PTP_TIME_T;

/*######################################################################################*/

/* IEEE 1588 - 5.3.2 TimeInterval */

typedef __HIL_PACKED_PRE struct PTP_TIMEINTERVAL_Ttag
{
  int32_t lNanosecondsMsb;
  int32_t lNanosecondsLsb;
} __HIL_PACKED_POST PTP_TIMEINTERVAL_T;

/*######################################################################################*/

/* IEEE 1588 - 5.3.5 PortIdentity */

typedef __HIL_PACKED_PRE struct PTP_PORT_IDENTITY_Ttag
{
  uint8_t  abClockIdentity[8];
  uint16_t usPortNumber;
} __HIL_PACKED_POST PTP_PORT_IDENTITY_T;

/*######################################################################################*/

/* IEEE 1588 - 5.3.3 Timestamp */

typedef __HIL_PACKED_PRE struct PTP_TIMESTAMP_Ttag
{
    uint16_t usSecondsMsb;
    uint32_t ulSecondsLsb;
    uint32_t ulNanoseconds;
} __HIL_PACKED_POST PTP_TIMESTAMP_T;

/*######################################################################################*/

/* IEEE 1588 - 5.3.7 ClockQuality */

typedef __HIL_PACKED_PRE struct PTP_CLOCK_QUALITY_Ttag
{
  uint8_t  bClockClass;
  uint8_t  bClockAccuracy;
  uint16_t usOffsetScaledLogVariance;
} __HIL_PACKED_POST PTP_CLOCK_QUALITY_T;

/*######################################################################################*/

/* IEEE 1588 - 5.3.9 PTPText */

typedef __HIL_PACKED_PRE struct PTP_TEXT_Ttag
{
  #define PTP_TEXT_MAX_SIZE   256

  uint8_t  bLengthField;
  uint8_t  abTextField[PTP_TEXT_MAX_SIZE];
} __HIL_PACKED_POST PTP_TEXT_T;

/*######################################################################################*/

/* IEEE 1588 - 5.3.6 PortAddress */

typedef __HIL_PACKED_PRE struct PTP_PORT_ADDRESS_Ttag
{
  uint16_t usNetworkProtocol; /* See PTP_NETWORK_PROTOCOL_E */
  uint16_t usAddressLength;
  uint8_t  abAddressField[4];

} __HIL_PACKED_POST PTP_PORT_ADDRESS_T;

/*######################################################################################*/


#endif /* PTP_DATATYPES_H_ */
