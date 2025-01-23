/**************************************************************************************
Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
***************************************************************************************
$Id: cip_timesync.h 109759 2022-10-24 08:38:27Z OKompani $:

Description:
  EtherNet/IP - CIP Time Sync Object
**************************************************************************************/


#ifndef CIP_TIMESYNC_H
#define CIP_TIMESYNC_H

#include <stdint.h>

#include "Hil_Compiler.h"
#include "Hil_Results.h"
#include "cip_common.h"

  /*#####################################################################################*/

/*
  ************************************************************
  *  Object specific defines
  ************************************************************
  */

  #define CIP_TIMESYNC_CLASS_NUMBER                         0x43
  #define CIP_TIMESYNC_REVISION                                3
  #define CIP_TIMESYNC_MAX_INSTANCE                            1

  #define CIP_TIMESYNC_MAX_INSTANCE_ATTR                     768

  #define CIPHIL_CMD_TIMESYNC_GET_PTP_HANDLE            (0x0701)
  #define CIPHIL_CMD_TIMESYNC_DLR_STATE_CHANGE          (0x0702)


  /* attributes of identity object */
  #define CIP_TIMESYNC_ATTR_1_PTP_ENABLE                       1
  #define CIP_TIMESYNC_ATTR_2_IS_SYNCHONIZED                   2
  #define CIP_TIMESYNC_ATTR_3_SYSTEM_TIME_MICROSECONDS         3
  #define CIP_TIMESYNC_ATTR_4_SYSTEM_TIME_NANOSECONDS          4
  #define CIP_TIMESYNC_ATTR_5_OFFSET_FROM_MASTER               5
  #define CIP_TIMESYNC_ATTR_6_MAX_OFFSET_FROM_MASTER           6
  #define CIP_TIMESYNC_ATTR_7_MEAN_PATH_DELAY_TO_MASTER        7
  #define CIP_TIMESYNC_ATTR_8_GRAND_MASTER_CLOCK_INFO          8
  #define CIP_TIMESYNC_ATTR_9_PARENT_CLOCK_INFO                9
  #define CIP_TIMESYNC_ATTR_10_LOCAL_CLOCK_INFO               10
  #define CIP_TIMESYNC_ATTR_11_NUMBER_OF_PORTS                11
  #define CIP_TIMESYNC_ATTR_12_PORT_STATE_INFO                12
  #define CIP_TIMESYNC_ATTR_13_PORT_ENABLE_CFG                13
  #define CIP_TIMESYNC_ATTR_14_PORT_LOG_ANNOUNCE_INTERVAL_CFG 14
  #define CIP_TIMESYNC_ATTR_15_PORT_LOG_SYNC_INTERVAL_CFG     15
  #define CIP_TIMESYNC_ATTR_16_PRIORITY_1                     16
  #define CIP_TIMESYNC_ATTR_17_PRIORITY_2                     17
  #define CIP_TIMESYNC_ATTR_18_DOMAIN_NUMBER                  18
  #define CIP_TIMESYNC_ATTR_19_CLOCK_TYPE                     19
  #define CIP_TIMESYNC_ATTR_20_MANUFACTURE_IDENTITY           20
  #define CIP_TIMESYNC_ATTR_21_PRODUCT_DESCRIPTION            21
  #define CIP_TIMESYNC_ATTR_22_REVISION_DATA                  22
  #define CIP_TIMESYNC_ATTR_23_USER_DESCRIPTION               23
  #define CIP_TIMESYNC_ATTR_24_PORT_PROFILE_IDENTITY_INFO     24
  #define CIP_TIMESYNC_ATTR_25_PORT_PHYSICAL_ADDRESS_INFO     25
  #define CIP_TIMESYNC_ATTR_26_PORT_PROTOCOL_ADDRESS_INFO     26
  #define CIP_TIMESYNC_ATTR_27_STEPS_REMOVED                  27
  #define CIP_TIMESYNC_ATTR_28_SYSTEM_TIME_AND_OFFSET         28

  #define CIP_TIMESYNC_ATTR_768_SYNC_PARAMETER                768

  /* CIP Sync PTP Profile Identifier */
  #define CIP_TIMESYNC_PROFILE_IDENTIFIER_0      0x00
  #define CIP_TIMESYNC_PROFILE_IDENTIFIER_1      0x21
  #define CIP_TIMESYNC_PROFILE_IDENTIFIER_2      0x6C
  #define CIP_TIMESYNC_PROFILE_IDENTIFIER_3      0x00
  #define CIP_TIMESYNC_PROFILE_IDENTIFIER_4      0x01
  #define CIP_TIMESYNC_PROFILE_IDENTIFIER_5      0x00
  #define CIP_TIMESYNC_PROFILE_IDENTIFIER_6      0x00
  #define CIP_TIMESYNC_PROFILE_IDENTIFIER_7      0x00


  #define CIP_TIMESYNC_NUM_OF_PORTS                 1

  /*#####################################################################################*/

/*
  ************************************************************
  *  Object specific types
  ************************************************************
  */

  typedef enum TIMESYNC_ATTR1_PTP_ENABLE_Etag
  {
    TIMESYNC_ATTR1_PTP_ENABLE_DISABLED = 0,
    TIMESYNC_ATTR1_PTP_ENABLE_ENABLED  = 1,
  } TIMESYNC_ATTR1_PTP_ENABLE_E;

  typedef enum TIMESYNC_ATTR2_IS_SYNCHONIZED_Etag
  {
    TIMESYNC_ATTR2_IS_SYNCHONIZED_NOT_SYNCHRONIZED = 0,
    TIMESYNC_ATTR2_IS_SYNCHONIZED_SYNCHRONIZED     = 1,
  } TIMESYNC_ATTR2_IS_SYNCHONIZED_E;


  typedef enum TIMESYNC_ATTR13_PORT_ENABLE_Etag
  {
    TIMESYNC_ATTR13_PORT_ENABLE_DISABLED = 0,
    TIMESYNC_ATTR13_PORT_ENABLE_ENABLED  = 1,
  } TIMESYNC_ATTR13_PORT_ENABLE_E;


#ifdef __HIL_PRAGMA_PACK_ENABLE
#pragma __HIL_PRAGMA_PACK_1(CIP_TIMESYNC)
#endif


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST TIMESYNC_ATTR13_Ttag
{
  uint16_t usNumberOfPorts;
  __HIL_PACKED_PRE struct __HIL_PACKED_POST
  {
    uint16_t usPortNumber;
    uint16_t usPortEnable;
  } tPortEnable[CIP_TIMESYNC_NUM_OF_PORTS];
} TIMESYNC_ATTR13_T;


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST TIMESYNC_ATTR14_Ttag
{
  uint16_t usNumberOfPorts;
  __HIL_PACKED_PRE struct __HIL_PACKED_POST
  {
    uint16_t usPortNumber;
    uint16_t usPortLogAnnounceInterval;
  } tPortLogAnnounceInterval[CIP_TIMESYNC_NUM_OF_PORTS];
} TIMESYNC_ATTR14_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST TIMESYNC_ATTR15_Ttag
{
  uint16_t usNumberOfPorts;
  __HIL_PACKED_PRE struct __HIL_PACKED_POST
  {
    uint16_t usPortNumber;
    int16_t  sPortLogSyncInterval;
  } tPortLogSyncInterval[CIP_TIMESYNC_NUM_OF_PORTS];
} TIMESYNC_ATTR15_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST TIMESYNC_ATTR12_Ttag
{
  uint16_t usNumberOfPorts;
  __HIL_PACKED_PRE struct __HIL_PACKED_POST
  {
    uint16_t usPortNumber;
    uint16_t usPortState;
  } tPortState[CIP_TIMESYNC_NUM_OF_PORTS];

} TIMESYNC_ATTR12_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST TIMESYNC_ATTR24_Ttag
{
  uint16_t usNumberOfPorts;
  __HIL_PACKED_PRE struct __HIL_PACKED_POST
  {
    uint16_t usPortNumber;
    uint8_t  abPortProfileIdentity[8];
  } tPortProfileIdentity[CIP_TIMESYNC_NUM_OF_PORTS];
} TIMESYNC_ATTR24_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST TIMESYNC_ATTR25_Ttag
{
  uint16_t usNumberOfPorts;
  __HIL_PACKED_PRE struct __HIL_PACKED_POST
  {
    uint16_t usPortNumber;
    uint8_t  abPhysicalProtocol[16];
    uint16_t usSizeOfAddress;
    uint8_t  abPortPhysicalAddress[16];
  } tPortPhysicalAddress[CIP_TIMESYNC_NUM_OF_PORTS];

} TIMESYNC_ATTR25_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST TIMESYNC_ATTR26_Ttag
{
  uint16_t usNumberOfPorts;
  __HIL_PACKED_PRE struct __HIL_PACKED_POST
  {
    uint16_t usPortNumber;
    uint16_t usNetworkProtocol;
    uint16_t usSizeOfAddress;
    uint8_t  abPortProtocolAddress[16];
  } tPortProtocolAddress[CIP_TIMESYNC_NUM_OF_PORTS];
} TIMESYNC_ATTR26_T;

#ifdef __HIL_PRAGMA_PACK_ENABLE
#pragma __HIL_PRAGMA_UNPACK_1(CIP_TIMESYNC)
#endif

typedef struct TIMESYNC_SYNC_PARAMETERS_Ttag
{
  uint32_t  ulSync0Interval;  /** Sync0 interval in nanoseconds. Range: 0 (sync0 disabled), or 5us <= ulSync0Interval <= 500ms */
  uint32_t  ulSync0Offset;    /** Sync0 offset in nanoseconds. Range: 0 <= ulSync0Offset < ulSync0Interval  */
  uint32_t  ulSync1Interval;  /** Sync1 interval in nanoseconds. Range: 0 (sync0 disabled), or 5us <= ulSync1Interval <= 500ms */
  uint32_t  ulSync1Offset;    /** Sync1 offset in nanoseconds. Range: 0 <= ulSync1Offset < ulSync1Interval  */
  uint32_t  ulPulseLength;    /** Length of sync trigger pulse in microseconds */
} TIMESYNC_SYNC_PARAMETERS_T;

/** Non-volatile timesync instance attributes */
typedef struct CIP_TIMESYNC_INST_ATTR_STORED_Ttag
{
  uint8_t    bPTPEnable;                             /* Attribute 1  */ /* Use TIMESYNC_ATTR1_PTP_ENABLE_E     */

  TIMESYNC_ATTR13_T tPortEnableConfig;               /* Attribute 13 */
  TIMESYNC_ATTR14_T tPortLogAnnounceIntervalCfg;     /* Attribute 14 */
  TIMESYNC_ATTR15_T tPortLogSyncIntervalCfg;         /* Attribute 15 */

  uint8_t  bPriority1;                                /* Attribute 16 (optional --> not implemented) */
  uint8_t  bPriority2;                                /* Attribute 17 (optional --> not implemented) */
  uint8_t  bDomainNumber;                             /* Attribute 18 */

  TIMESYNC_SYNC_PARAMETERS_T tSyncParameters;         /* Attribute 0x300 */

} CIP_TIMESYNC_INST_ATTR_STORED_T;



  /*#####################################################################################*/



  /*
  ****************************************************************************
  *  Object specific macros
  ****************************************************************************
  */


  /*#####################################################################################*/
#ifdef USE_PTP

  struct EIP_RESOURCES_Ttag;  /* forward declaration */
  /*
  ************************************************************
  *  Task Function Prototypes
  ************************************************************
  */

  struct PTP_STARTUP_Ttag;  /* forward declaration */

  typedef struct PTP_STARTUP_Ttag* CIP_TIMESYNC_INIT_PARAM_T;


  /** Creates the Time Sync object.
   *
   * \param pvRsc [in]  Task Resources
   *
   * \return Description of return value, if any
   */
  uint32_t CipTimeSync_ObjectCreate( struct EIP_RESOURCES_Ttag* hEip,
                                     CIP_TIMESYNC_INIT_PARAM_T  ptParam );

  struct PTP_RESOURCES_Ttag;  /* forward declaration */
  struct PTP_RESOURCES_Ttag* CipTimeSync_RetrievePtpHandle(struct EIP_RESOURCES_Ttag* hEip);

#endif /* USE_PTP */

#endif /* CIP_TIMESYNC_H */
