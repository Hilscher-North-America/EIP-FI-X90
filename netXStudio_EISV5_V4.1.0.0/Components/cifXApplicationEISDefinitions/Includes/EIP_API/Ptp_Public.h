/**************************************************************************************
Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
***************************************************************************************
$Id: Ptp_Public.h 90579 2020-03-02 15:02:27Z MarcBommert $:

Description:
  PTP - Public declarations

Changes:
 Date          Description
 -----------------------------------------------------------------------------------
 2015-03-24    created
**************************************************************************************/

#ifndef __PTP_PUBLIC_H
#define __PTP_PUBLIC_H

/*######################################################################################*/
/*               ____                _              _                                   */
/*              / ___|___  _ __  ___| |_ __ _ _ __ | |_ ___                             */
/*             | |   / _ \| '_ \/ __| __/ _` | '_ \| __/ __|                            */
/*             | |__| (_) | | | \__ \ || (_| | | | | |_\__ \                            */
/*              \____\___/|_| |_|___/\__\__,_|_| |_|\__|___/                            */
/*                                                                                      */
/*######################################################################################*/

#define PTP_PRODUCT_DESCRIPTION_MAX     64
#define PTP_USER_DESCRIPTION_MAX       128
#define PTP_REVISION_MAX                32
#define PTP_PHYSICALLAYERPROTOCOL_MAX   32

/* Range and default values for Sync0 */
#define  PTP_SYNC_PARAMETER_SYNC0_INTERVAL_MIN          5000 /*   5 us */
#define  PTP_SYNC_PARAMETER_SYNC0_INTERVAL_DEFAULT 500000000 /* 500 ms */
#define  PTP_SYNC_PARAMETER_SYNC0_INTERVAL_MAX     500000000 /* 500 ms */

/* Default Sync0 offset */
#define  PTP_SYNC_PARAMETER_SYNC0_OFFSET_DEFAULT           0 /* 0   ns */

/* Range and default values for Sync1 */
#define  PTP_SYNC_PARAMETER_SYNC1_INTERVAL_MIN         50000 /*   5 us */
#define  PTP_SYNC_PARAMETER_SYNC1_INTERVAL_DEFAULT 500000000 /* 500 ms */
#define  PTP_SYNC_PARAMETER_SYNC1_INTERVAL_MAX     500000000 /* 500 ms */

/* Default Sync1 offset */
#define  PTP_SYNC_PARAMETER_SYNC1_OFFSET_DEFAULT         150 /* 150 ns (we need this offset, since sync0 and sync1 cannot be triggered at the exact same time) */

/* Range definitions of pulse length */
#define  PTP_SYNC_PARAMETER_PULSE_LENGTH_MIN               1 /*   1 us */
#define  PTP_SYNC_PARAMETER_PULSE_LENGTH_DEFAULT           4 /*   4 us */
#define  PTP_SYNC_PARAMETER_PULSE_LENGTH_MAX             500 /* 500 us */

/*######################################################################################*/
/* IEEE 1588 PTP related definitions */

typedef enum
{
  PTP_STATE_INITIALIZING = 1,
  PTP_STATE_FAULTY       = 2,
  PTP_STATE_DISABLED     = 3,
  PTP_STATE_LISTENING    = 4,
  PTP_STATE_PRE_MASTER   = 5,
  PTP_STATE_MASTER       = 6,
  PTP_STATE_PASSIVE      = 7,
  PTP_STATE_UNCALIBRATED = 8,
  PTP_STATE_SLAVE        = 9

} PTP_STATE_E ;

/*######################################################################################*/
/* Control commands for use with API function Ptp_Control() */

typedef enum
{
  /* GET commands */
  PTP_CTRLCMD_GET_ISENABLED                      = 0x00000001,
  PTP_CTRLCMD_GET_ISSYNCHRONIZED                 = 0x00000002,
  PTP_CTRLCMD_GET_PTPTIME                        = 0x00000003,
  PTP_CTRLCMD_GET_OFFSETFROMMASTER               = 0x00000004,
  PTP_CTRLCMD_GET_MAXOFFSETFROMMASTER            = 0x00000005,
  PTP_CTRLCMD_GET_MEANPATHDELAY                  = 0x00000006,
  PTP_CTRLCMD_GET_GRANDMASTERCLOCKINFO           = 0x00000007,
  PTP_CTRLCMD_GET_PARENTCLOCKINFO                = 0x00000008,
  PTP_CTRLCMD_GET_LOCALCLOCKINFO                 = 0x00000009,
  PTP_CTRLCMD_GET_DOMAINNUMBER                   = 0x0000000A,
  PTP_CTRLCMD_GET_LOCALCLOCKOFFSET               = 0x0000000B,
  PTP_CTRLCMD_GET_PORTSTATE                      = 0x0000000C,
  PTP_CTRLCMD_GET_NEXTSYNCEVENTTIME              = 0x0000000D,
  PTP_CTRLCMD_GET_PRODUCTDESCRIPTION             = 0x0000000E,
  PTP_CTRLCMD_GET_USERDESCRIPTION                = 0x0000000F,
  PTP_CTRLCMD_GET_REVISION                       = 0x00000010,
  PTP_CTRLCMD_GET_PORTLOGANNOUNCEINTERVAL        = 0x00000011,
  PTP_CTRLCMD_GET_PORTLOGSYNCINTERVAL            = 0x00000012,
  PTP_CTRLCMD_GET_CLOCKTYPE                      = 0x00000013,
  PTP_CTRLCMD_GET_PHYSICALLAYERPROTOCOL          = 0x00000014,
  PTP_CTRLCMD_GET_NETWORKPROTOCOL                = 0x00000015,
  PTP_CTRLCMD_GET_DSCPEVENTVALUE                 = 0x00000016,
  PTP_CTRLCMD_GET_DSCPGENERALVALUE               = 0x00000017,
  PTP_CTRLCMD_GET_SYNC_PARAMETERS                = 0x00000018,
  PTP_CTRLCMD_GET_STEPS_REMOVED                  = 0x00000019, /* Retrieves the "stepsRemoved" variable from the current DS */

  /* SET commands */
  PTP_CTRLCMD_SET_ENABLE                         = 0x00010001,
  PTP_CTRLCMD_SET_DOMAINNUMBER                   = 0x00010002,
  PTP_CTRLCMD_SET_SYNC_CONFIG                    = 0x00010003,
  PTP_CTRLCMD_SET_PRODUCTDESCRIPTION             = 0x00010004,
  PTP_CTRLCMD_SET_USERDESCRIPTION                = 0x00010005,
  PTP_CTRLCMD_SET_REVISION                       = 0x00010006,
  PTP_CTRLCMD_SET_PROFILEIDENTITY                = 0x00010007,
  PTP_CTRLCMD_SET_DSCPEVENTVALUE                 = 0x00010008,
  PTP_CTRLCMD_SET_DSCPGENERALVALUE               = 0x00010009,
  PTP_CTRLCMD_SET_PORTLOGANNOUNCEINTERVAL        = 0x0001000A,
  PTP_CTRLCMD_SET_PORTLOGSYNCINTERVAL            = 0x0001000B,
  PTP_CTRLCMD_SET_SYNC_PARAMETERS                = 0x0001000C,

  PTP_CTRLCMD_SET_IP_ADDRESS                     = 0x0001000D,
  PTP_CTRLCMD_SET_MAC_ADDRESS                    = 0x0001000E,

  /* Special commands */
  PTP_CTRLCMD_RESET_MAXOFFSETFROMMASTER          = 0x00020001,
  PTP_CTRLCMD_START_PATH_RECONFIGURATION         = 0x00020002

} PTP_CTRLCMD_E;

typedef enum
{
  PTP_CTRLCMD_GET_ISENABLED_DATA_DISABLED = 0,
  PTP_CTRLCMD_GET_ISENABLED_DATA_ENABLED  = 1,

} PTP_CTRLCMD_GET_ISENABLED_DATA_E ;

typedef enum
{
  PTP_CTRLCMD_GET_ISSYNCHRONIZED_DATA_NOT_SYNCHRONIZED = 0,
  PTP_CTRLCMD_GET_ISSYNCHRONIZED_DATA_SYNCHRONIZED     = 1,

} PTP_CTRLCMD_GET_ISSYNCHRONIZED_DATA_E ;

typedef enum
{
  PTP_CTRLCMD_SET_ENABLE_DATA_DISABLE = 0,
  PTP_CTRLCMD_SET_ENABLE_DATA_ENABLE  = 1,

} PTP_CTRLCMD_SET_ENABLE_DATA_E ;


typedef enum
{
  PTP_TIME_PROPERTIES_FLAG_LI61    =    0x01, /* Leap 61               */
  PTP_TIME_PROPERTIES_FLAG_LI59    =    0x02, /* Leap 59               */
  PTP_TIME_PROPERTIES_FLAG_UTCV    =    0x04, /* currentUtcOffsetValid */
  PTP_TIME_PROPERTIES_FLAG_PTP     =    0x08, /* fPtpTimeScale         */
  PTP_TIME_PROPERTIES_FLAG_TTRA    =    0x10, /* timeTraceable         */
  PTP_TIME_PROPERTIES_FLAG_FTRA    =    0x20, /* frequencyTraceable    */

} PTP_TIME_PROPERTIES_FLAGS_E;


typedef enum
{
  PTP_NETWORK_PROTOCOL_UDP_IP_V4  =  0x0001,
  PTP_NETWORK_PROTOCOL_UDP_IP_V6  =  0x0002,
  PTP_NETWORK_PROTOCOL_IEEE_802_3 =  0x0003,
  PTP_NETWORK_PROTOCOL_DEVICENET  =  0x0004,
  PTP_NETWORK_PROTOCOL_CONTROLNET =  0x0005,
  PTP_NETWORK_PROTOCOL_PROFINET   =  0x0006,
  /* Reserved                                 0x0007 - 0xFFFD */
  PTP_NETWORK_PROTOCOL_UNKOWN     =  0xFFFE,
  /* Reserved                                 0xFFFF */
} PTP_NETWORK_PROTOCOL_E;


/*######################################################################################*/
/*               ____        _          _____                                           */
/*              |  _ \  __ _| |_ __ _  |_   _|   _ _ __   ___  ___                      */
/*              | | | |/ _` | __/ _` |   | || | | | '_ \ / _ \/ __|                     */
/*              | |_| | (_| | || (_| |   | || |_| | |_) |  __/\__ \                     */
/*              |____/ \__,_|\__\__,_|   |_| \__, | .__/ \___||___/                     */
/*                                           |___/|_|                                   */
/*######################################################################################*/


typedef struct PTP_RESOURCES_Ttag* PTP_H;

typedef void (*PTP_SYNC_CALLBACK)( void* pvUserParam,  /* User parameter registered with
                                                          Ptp_RegisterSyncCallback() */
                                   bool  fFirstSync ); /* Indicates whether this is the
                                                          first Sync event. */

typedef void (*PTP_SYNC_STATUS_CALLBACK)( void*      pvUserParam,    /* User parameter */
                                          bool       fSyncStatus );  /* Signals sync status changes
                                                                          true (1):  in sync
                                                                          false (0): not in sync */

/*######################################################################################*/

#ifdef PRAGMA_PACK_ENABLE
#pragma PRAGMA_PACK_1(__PTP_PACKET)
#endif

/* Structure used to register event callback functions */
typedef struct PTP_EVENT_CALLBACKS_Ttag
{
  PTP_SYNC_CALLBACK        pfnSyncCallback;           /* Function called on every sync event          */
  void*                    pvSyncCallbackParam;       /* User parameter provided with pfnSyncCallback */

  PTP_SYNC_STATUS_CALLBACK pfnSyncStatusCallback;     /* Function called on  */
  void*                    pvSyncStatusCallbackParam; /* User parameter provided with pfnSyncStatusCallback */
} PTP_EVENT_CALLBACKS_T;

typedef struct PTP_SYNC_PARAMETER_Ttag
{
  uint32_t  ulSync0Interval;  /** Sync0 interval in nanoseconds. Range: 0 (sync0 disabled), or 5us <= ulSync0Interval <= 500ms */
  uint32_t  ulSync0Offset;    /** Sync0 offset in nanoseconds. Range: 0 <= ulSync0Offset < ulSync0Interval  */
  uint32_t  ulSync1Interval;  /** Sync1 interval in nanoseconds. Range: 0 (sync0 disabled), or 5us <= ulSync1Interval <= 500ms */
  uint32_t  ulSync1Offset;    /** Sync1 offset in nanoseconds. Range: 0 <= ulSync1Offset < ulSync1Interval  */
} PTP_SYNC_PARAMETER_T;

typedef struct PTP_SYNC_PARAMETERS_Ttag
{
  uint32_t  ulSync0Interval;  /** Sync0 interval in nanoseconds. Range: 0 (sync0 disabled), or 5us <= ulSync0Interval <= 500ms */
  uint32_t  ulSync0Offset;    /** Sync0 offset in nanoseconds. Range: 0 <= ulSync0Offset < ulSync0Interval  */
  uint32_t  ulSync1Interval;  /** Sync1 interval in nanoseconds. Range: 0 (sync0 disabled), or 5us <= ulSync1Interval <= 500ms */
  uint32_t  ulSync1Offset;    /** Sync1 offset in nanoseconds. Range: 0 <= ulSync1Offset < ulSync1Interval  */
  uint32_t  ulPulseLength;    /** Length of sync trigger pulse in microseconds */
} PTP_SYNC_PARAMETERS_T;

typedef struct PTP_IP_ADDRESS_Ttag
{
  uint32_t  ulIpAddress;      /** Local IP address to be used by the PTP subsystem */
} PTP_IP_ADDRESS_T;

typedef struct PTP_MAC_ADDRESS_Ttag
{
  uint8_t  abMacAddress[6];          /** Local MAC address to be used by the PTP subsystem */
} PTP_MAC_ADDRESS_T;

/* pragma unpack */
#ifdef PRAGMA_PACK_ENABLE
#pragma PRAGMA_UNPACK_1(__PTP_PACKET)
#endif

/*######################################################################################*/

typedef struct PTP_CONTROL_Ttag
{
  union
  {
    uint32_t ulIsEnabled;         /* see PTP_CTRLCMD_GET_ISENABLED_DATA_E */

    uint32_t ulIsSynchronized;    /* see PTP_CTRLCMD_GET_ISSYNCHRONIZED_DATA_E */

    struct
    {
      uint32_t lSeconds;
      uint32_t lNanoseconds;
    }tPtpTime;

    struct
    {
      uint32_t lSeconds;
      uint32_t lNanoseconds;
    }tNextSyncEventTime;

    int32_t    lOffsetFromMaster;
    long long  llMaxOffsetFromMaster;
    uint32_t   ulMeanPathDelay;

    struct
    {
      uint16_t usCurrentUTCOffset;
      uint8_t  bPriority1;
      uint8_t  bPriority2;
      uint8_t  bGrandMasterClockClass;      /* see PTP_CLOCK_CLASS_E */
      uint8_t  bGrandMasterClockAccuracy;   /* see PTP_CLOCK_ACCURACY_E */
      uint16_t usGrandMasterClockVariance;
      uint8_t  abGrandMasterClockIdentity[8];
      uint8_t  bTimeSource;                 /* see PTP_TIME_SOURCE_E */
      uint16_t usTimePropertyFlags;         /* see PTP_TIME_PROPERTIES_FLAGS_E */
    }tGrandmasterClockInfo;

    struct
    {
      uint8_t abClockIdentity[8];
      uint16_t usPortNumber;
    }tParentClockInfo;

    struct
    {
      uint8_t  abClockIdentity[8];
      uint8_t  bClockClass;                 /* see PTP_CLOCK_CLASS_E */
      uint8_t  bClockAccuracy;              /* see PTP_CLOCK_ACCURACY_E */
      uint16_t usClockVariance;
      uint16_t usCurrentUTCOffset;
      uint8_t  bTimeSource;                 /* see PTP_TIME_SOURCE_E */
      uint16_t usProperties;                /* see PTP_TIME_PROPERTIES_FLAGS_E */
    }tLocalClockInfo;

    uint8_t  bDomainNumber;

    struct
    {
      uint32_t lSeconds;
      uint32_t lNanoseconds;
    }tLocalClockOffset;

    uint32_t           ulPortState;         /* see PTP_STATE_E                   */
    uint32_t           ulEnable;            /* see PTP_CTRLCMD_SET_ENABLE_DATA_E */

    PTP_SYNC_PARAMETER_T  tSyncConfig;
    PTP_SYNC_PARAMETERS_T tSyncParameters;

    struct
    {
      uint32_t ulLength;
      uint8_t  abDescription[PTP_PRODUCT_DESCRIPTION_MAX];
    }tProductDescription;

    struct
    {
      uint32_t ulLength;
      uint8_t  abDescription[PTP_USER_DESCRIPTION_MAX];
    }tUserDescription;

    struct
    {
      uint32_t ulLength;
      uint8_t  abRevision[PTP_REVISION_MAX];
    }tRevision;

    uint8_t abProfileIdentity[6];

    uint8_t  bPortLogAnnounceInterval;
    uint8_t  bLogSyncInterval;
    uint16_t usClockType;       /* see PTP_CLOCK_TYPE_E */

    struct
    {
      uint32_t ulLength;
      uint8_t  abName[PTP_PHYSICALLAYERPROTOCOL_MAX];
    }tPhysicalLayerProtocol;

    uint16_t usNetworkProtocol; /* see PTP_NETWORK_PROTOCOL_E */
    uint8_t  bDscpEventValue;
    uint8_t  bDscpGeneralValue;
    uint16_t usStepsRemoved;     /* PTP_CTRLCMD_GET_STEPS_REMOVED */

    PTP_IP_ADDRESS_T  tIpAddress;
    PTP_MAC_ADDRESS_T tMacAddress;

  }uCommand;

} PTP_CONTROL_T;

/*######################################################################################*/
/*             ____            _        _                                               */
/*             |  _ \ _ __ ___ | |_ ___ | |_ _   _ _ __   ___ ___                       */
/*             | |_) | '__/ _ \| __/ _ \| __| | | | '_ \ / _ Y __|                      */
/*             |  __/| | | (_) | || (_) | |_| |_| | |_) |  __|__ \                      */
/*             |_|   |_|  \___/ \__\___/ \__|\__, | .__/ \___|___/                      */
/*                                          |___/|_|                                    */
/*######################################################################################*/

/** This function can be used to control the PTP stack or to read or write configuration.
 *
 * \param hPtpHandle    [in]     Handle to PTP interface
 * \param ulPtpCommand  [in]     Command for the requested function (use enum PTP_CTRLCMD_E)
 * \param ptData        [in,out] Command specific data.
 *                               Input if the command is a "set" command.
 *                               Output, if the command is a "get" command
 *
 * \return   0 (OK)
 *           TLR_E_PTP_UNKNOWN_COMMAND
 *           TLR_E_PTP_INVALID_HANDLE
 *           ...
 */
uint32_t     Ptp_Control( PTP_H          hPtpHandle,
                          uint32_t       ulPtpCommand,  /* See PTP_CTRLCMD_E                      */
                          PTP_CONTROL_T* ptData);       /* control data depending on ulPtpCommand */

/****************************************************************************************/

/** This function can be used to register event callback functions.
 *
 * \param hPtpHandle        [in] Handle to PTP interface
 * \param ptEventCallbacks  [in] Callback functions
 *
 * \return   0 (OK)
 *           TLR_E_PTP_INVALID_HANDLE
 */
uint32_t     Ptp_RegisterEventCallbacks( PTP_H                  hPtpHandle,
                                         PTP_EVENT_CALLBACKS_T* ptEventCallbacks );

/****************************************************************************************/

#endif /* #ifndef __PTP_PUBLIC_H */
