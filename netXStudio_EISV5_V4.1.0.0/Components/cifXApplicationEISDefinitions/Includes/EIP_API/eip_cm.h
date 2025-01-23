/**************************************************************************************
Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
***************************************************************************************
$Id: eip_cm.h 113886 2023-05-23 05:29:03Z MBommert $:

Description:
  EtherNet/IP - Connection Manager Object
**************************************************************************************/

#include <stdint.h>

#include "Hil_Compiler.h"
#include "Hil_Results.h"

#include "cip_common.h"

#ifndef EIP_CM_H
#define EIP_CM_H

  /*#####################################################################################*/

  /*
    ************************************************************
    *  Object specific defines
    ************************************************************
  */
  #define EIP_CM_CLASS_NUMBER            0x06
  #define EIP_CM_REVISION                1
  #define EIP_CM_MAX_INSTANCE            1

  #define EIP_CM_MAX_INSTANCE_ATTR       0

  /* CIP services specific to Connection Manager Object */
  #define CIP_CMD_FORWARD_CLOSE                 ( 0x4E )
  #define CIP_CMD_UNCONNECTED_SEND              ( 0x52 )
  #define CIP_CMD_FORWARD_OPEN                  ( 0x54 )
  #define CIP_CMD_GET_CONNECTION_DATA           ( 0x56 )
  #define CIP_CMD_SEARCH_CONNECTION_DATA        ( 0x57 )
  #define CIP_CMD_GET_CONNECTION_OWNER          ( 0x5A )
  #define CIP_CMD_LARGE_FORWARD_OPEN            ( 0x5B )

  /* Hilscher services specific to Connection Manager Object */
  #define CIPHIL_CMD_CM_FORWARD_OPEN_COMPLETE                     (0x0601)
  #define CIPHIL_CMD_CM_GET_CONNECTION_SUMMARY                    (0x0602)    /* No request data. For format of response data see CIPHIL_CM_CMD_GET_CONNECTION_SUMMARY_RESPONSE_DATA_T */
  #define CIPHIL_CMD_CM_GET_NUMBER_OF_CL3_CONNECTIONS_ON_TCP_CONN (0x0603)
  #define CIPHIL_CMD_CM_EVENT_TCP_CONN_CLOSING                    (0x0604)

  #define CIPHIL_CMD_CM_CONFIGASSEMBLY_RESPONSE                   (0x0605)
  #define CIPHIL_CMD_CM_FWOPEN_NOTIFICATION_RESPONSE              (0x0606)
  #define CIPHIL_CMD_CM_FWCLOSE_NOTIFICATION_RESPONSE             (0x0607)

  #define CIPHIL_CMD_CM_CONNECTION_STATE_CHANGE_RESPONSE          (0x0608)
  #define CIPHIL_CMD_CM_GEN_NEW_CONNECTION_ID                     (0x0609)

  #define CIPHIL_CMD_CM_ENABLE_DISABLE_TRANSMISSION_TRIGGER_TIMER (0x060A)

  #define CIPHIL_CMD_CM_NULL_FWOPEN_NOTIFICATION_RESPONSE         (0x060B)
  #define CIPHIL_CMD_CM_NULL_FWOPEN_CONFIGASSEMBLY_RESPONSE       (0x060C)

  #define CIPHIL_CMD_CM_ENABLE_DISABLE_NULL_FORWARD_OPEN          (0x060D)

  /* Service (0x060E) is deprecated. Do not use it for future versions */

  #define CIPHIL_CMD_CM_CLOSE_ALL_CONNECTIONS                     (0x060F)

  /* Connection parameter */
  #define EIP_CM_PARAM_REDUNDANT_MASK     0x8000
  #define EIP_CM_PARAM_EXCLUSIVE_OWNER    0x0000
  #define EIP_CM_PARAM_REDUNDANT_OWNER    0x8000

  #define EIP_CM_PARAM_TYPE_MASK          0x6000
  #define EIP_CM_PARAM_TYPE_NULL          0x0000
  #define EIP_CM_PARAM_TYPE_MULTICAST     0x2000
  #define EIP_CM_PARAM_TYPE_POINT         0x4000
  #define EIP_CM_PARAM_TYPE_RESERVED      0x6000

  #define EIP_CM_PARAM_PRIORITY_MASK      0x0C00
  #define EIP_CM_PARAM_PRIORITY_LOW       0x0000
  #define EIP_CM_PARAM_PRIORITY_HIGH      0x0400
  #define EIP_CM_PARAM_PRIORITY_SCHEDULED 0x0800
  #define EIP_CM_PARAM_PRIORITY_URGENT    0x0C00

  #define EIP_CM_PARAM_FIXED_VAR_MASK     0x0200
  #define EIP_CM_PARAM_VARIABLE_SIZE      0x0200
  #define EIP_CM_PARAM_FIXED_SIZE         0x0000

  #define EIP_CM_PARAM_SIZE_MASK          0x01FF

  /* transportClass_Trigger */
  #define EIP_CM_TYPTRIG_DIR_MASK         0x80
  #define EIP_CM_TYPTRIG_DIR_CLIENT       0x00
  #define EIP_CM_TYPTRIG_DIR_SERVER       0x80

  #define EIP_CM_TYPTRIG_TRIG_MASK             0x70
  #define EIP_CM_TYPTRIG_TRIG_CYCLIC           0x00
  #define EIP_CM_TYPTRIG_TRIG_CHANGE_OF_STATE  0x10
  #define EIP_CM_TYPTRIG_TRIG_APP_TRIGGERED    0x20

  #define CIP_CM_IS_TYPTRIG_CYCLIC(bTriggerType)           (((bTriggerType) & EIP_CM_TYPTRIG_TRIG_MASK) == EIP_CM_TYPTRIG_TRIG_CYCLIC)
  #define CIP_CM_IS_TYPTRIG_CHANGE_OF_STATE(bTriggerType)  (((bTriggerType) & EIP_CM_TYPTRIG_TRIG_MASK) == EIP_CM_TYPTRIG_TRIG_CHANGE_OF_STATE)
  #define CIP_CM_IS_TYPTRIG_APP_TRIGGERED(bTriggerType)    (((bTriggerType) & EIP_CM_TYPTRIG_TRIG_MASK) == EIP_CM_TYPTRIG_TRIG_APP_TRIGGERED)

  #define EIP_CM_TYPTRIG_TYP_MASK         0x0F
  #define EIP_CM_TYPTRIG_TYP_CL0          0x00
  #define EIP_CM_TYPTRIG_TYP_CL1          0x01
  #define EIP_CM_TYPTRIG_TYP_CL2          0x02
  #define EIP_CM_TYPTRIG_TYP_CL3          0x03

  #define EIP_DEFAULT_ADDSTATUS_IDX          1

  #define EIP_FWDOPEN_SIZE_MSK            0x01FF
  #define EIP_FWDOPEN_PARAM_MSK           0xFE00
  #define EIP_LFWDOPEN_SIZE_MSK           0x0000FFFF
  #define EIP_LFWDOPEN_PARAM_MSK          0xFE000000

  /*#####################################################################################*/

  /*
  ************************************************************
  *  Object specific types
  ************************************************************
  */

#ifdef __HIL_PRAGMA_PACK_ENABLE
#pragma __HIL_PRAGMA_PACK_1(EIP_CM)
#endif

  /*#####################################################################################*/

  #define EIP_DEFAULT_PATH_LEN              510      /* Maximum connection path size is 255 words == 510 bytes */

  /* Forward Open Service - Request Data */
  typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_CM_FWOPEN_REQ_Ttag
  {
    uint8_t   bPriority;          /* used to calculate request timeout information   */
    uint8_t   bTimeOutTicks;      /* used to calculate request timeout information   */
    uint32_t  ulOTConnID;         /* Network connection ID orginator to target       */
    uint32_t  ulTOConnID;         /* Network connection ID target to orginator       */
    uint16_t  usConnSerialNum;    /* Connection serial number                        */
    uint16_t  usVendorId;         /* Orginator Vendor ID                             */
    uint32_t  ulOSerialNum;       /* Orginator serial number                         */
    uint8_t   bTimeoutMultiple;   /* Connection timeout multiple                     */
    uint8_t   abReserved1[3];     /* reserved                                        */
    uint32_t  ulOTRpi;            /* orginator to target requested packet rate in us */
    uint16_t  usOTConnParam;      /* orginator to target connection parameter        */
    uint32_t  ulTORpi;            /* target to orginator requested packet rate in us */
    uint16_t  usTOConnParam;      /* target to orginator connection parameter        */
    uint8_t   bTriggerType;       /* Transport type/trigger                          */
    uint8_t   bConnPathSize;      /* Connection path size                            */
    uint8_t   bConnPath[EIP_DEFAULT_PATH_LEN];        /* connection path             */
  } EIP_CM_FWOPEN_REQ_T;

  /* Large Forward Open Service - Request Data */
  typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_CM_LARGEFWOPEN_REQ_Ttag
  {
    uint8_t   bPriority;          /* used to calculate request timeout information    */
    uint8_t   bTimeOutTicks;      /* used to calculate request timeout information    */
    uint32_t  ulOTConnID;         /* Network connection ID originator to target       */
    uint32_t  ulTOConnID;         /* Network connection ID target to originator       */
    uint16_t  usConnSerialNum;    /* Connection serial number                         */
    uint16_t  usVendorId;         /* Originator Vendor ID                             */
    uint32_t  ulOSerialNum;       /* Originator serial number                         */
    uint8_t   bTimeoutMultiple;   /* Connection timeout multiple                      */
    uint8_t   abReserved1[3];     /* reserved                                         */
    uint32_t  ulOTRpi;            /* Originator to target requested packet rate in us */
    uint32_t  ulOTConnParam;      /* Originator to target connection parameter        */
    uint32_t  ulTORpi;            /* target to originator requested packet rate in us */
    uint32_t  ulTOConnParam;      /* target to originator connection parameter        */
    uint8_t   bTriggerType;       /* Transport type/trigger                           */
    uint8_t   bConnPathSize;      /* Connection path size                             */
    uint8_t   bConnPath[EIP_DEFAULT_PATH_LEN];        /* connection path              */
  } EIP_CM_LARGEFWOPEN_REQ_T;

  /*#####################################################################################*/

  /* Forward Open Service - Successful Response  */
  typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_CM_FWOPEN_RESP_Ttag
  {
    uint32_t  ulOTConnID;         /* Network connection ID orginator to target      */
    uint32_t  ulTOConnID;         /* Network connection ID target to orginator      */
    uint16_t  usConnSerialNum;    /* Connection serial number                       */
    uint16_t  usVendorId;         /* Orginator Vendor ID                            */
    uint32_t  ulOSerialNum;       /* Orginator serial number                        */
    uint32_t  ulOTApi;            /* orginator to target actual packet rate         */
    uint32_t  ulTOApi;            /* target to orginator actual packet rate         */
    uint8_t   bAppReplySize;      /* Number of 16Bit words in the application reply */
    uint8_t   bReserved1;
    uint16_t  ausAppReply[EIP_DEFAULT_ADDSTATUS_IDX];
  }  EIP_CM_FWOPEN_RESP_T;

  /*#####################################################################################*/

  /* Forward Open/Close Service - Unsuccessful Response  */
  typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_CM_FW_FAIL_RESP_Ttag
  {
    uint16_t  usConnSerialNum;    /* Connection serial number */
    uint16_t  usVendorId;         /* Orginator Vendor ID      */
    uint32_t  ulOSerialNum;       /* Orginator serial number  */
    uint8_t   bConnPathSize;      /* Connection path size     */
    uint8_t   bReserved1;         /* connection path           */
  } EIP_CM_FWOPEN_FAIL_RESP_T;

  /*#####################################################################################*/

  /* Forward Close Service - Request Data  */
  typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_CM_FWCLOSE_REQ_Ttag
  {
    uint8_t   bPriority;          /* used to calculate request timeout information  */
    uint8_t   bTimeOutTicks;      /* used to calculate request timeout information  */
    uint16_t  usConnSerialNum;    /* Connection serial number                       */
    uint16_t  usVendorId;         /* Orginator Vendor ID                            */
    uint32_t  ulOSerialNum;       /* Orginator serial number                        */
    uint8_t   bConnPathSize;      /* Connection path size                           */
    uint8_t   bReserved1;
    uint8_t   bConnPath[EIP_DEFAULT_PATH_LEN];  /* connection path                  */
  } EIP_CM_FWCLOSE_REQ_T;

  /*#####################################################################################*/

  /* Forward Close Service - Response */
  typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_CM_FWCLOSE_RESP_Ttag
  {
    uint16_t  usConnSerialNum;    /* Connection serial number */
    uint16_t  usVendorId;         /* Orginator Vendor ID      */
    uint32_t  ulOSerialNum;       /* Orginator serial number  */

    union
    {
      struct
      {
        uint8_t   bAppReplySize;      /* Number of 16Bit words in the application reply */
        uint8_t   bReserved1;
        uint8_t   abAppReply[EIP_DEFAULT_ADDSTATUS_IDX];
      } tSuccess;

      struct
      {
        uint8_t   bRemainingWordsInConnectionPath;
        uint8_t   bReserved1;
      } tError;
    } u;
  }  EIP_CM_FWCLOSE_RESP_T;

  /* Note: Keep consistent with EIP_OBJECT_FWD_OPEN_FWD_COMPLETION_IND_T ! */
  typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_CM_FWOPENCOMPLETE_IND_Ttag
  {
    uint16_t  usReserved;         /*!< Reserved                           */
    uint16_t  usConnSerialNum;    /*!< Connection serial number           */
    uint16_t  usVendorId;         /*!< Originator Vendor ID               */
    uint32_t  ulOSerialNum;       /*!< Originator serial number           */
    uint32_t  ulGRC;              /*!< General Error Code                 */
    uint32_t  ulERC;              /*!< Extended Error Code                */
  } EIP_CM_FWOPENCOMPLETE_IND_T;

  /*#####################################################################################*/
  /* Response data of command CIPHIL_CM_CMD_GET_CONNECTION_SUMMARY */
  typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST CIPHIL_CM_CMD_GET_CONNECTION_SUMMARY_RESPONSE_DATA_Ttag
  {
    struct
    {
      uint16_t usNumberOfExclusiveOwnerConnectionsInRunMode;
      uint16_t usNumberOfExclusiveOwnerConnectionsInIdleMode;
      uint16_t usNumberOfExclusiveOwnerConnectionsTimedOut;

      uint16_t usNumberOfInputOnly;
      uint16_t usNumberOfListenOnly;
      uint16_t usNumberOfExplicitMessaging;
    } tAdapter;
    struct
    {
      uint16_t usOpenConnectionCount;         /* number of open connections */
      uint16_t usIdleConnectionCount;         /* number of open connection which don't have RUN status in O2T direction */
      uint16_t usFaultedConnectionCount;      /* number of faulted (timed out) connections */

      /* TODO: Add usExplicitMessagingCount */
    } tScanner;
  } CIPHIL_CM_CMD_GET_CONNECTION_SUMMARY_RESPONSE_DATA_T;

  /*#####################################################################################*/
  /* Response data format for extended status code CIP_ESR_RPI_VALUE_NOT_ACCEPTABLE */
  typedef __HIL_PACKED_PRE struct  __HIL_PACKED_POST EIP_CM_RPI_VALUE_NOT_ACCEPTABLE_Ttag
  {
    uint8_t   bAcceptableO2T_RpiType;   /* see EIP_CM_ACCEPTABLE_RPI_TYPES_E for valid values */
    uint8_t   bAcceptableT2O_RpiType;   /* see EIP_CM_ACCEPTABLE_RPI_TYPES_E for valid values */
    uint32_t  bAcceptableO2T_RpiValue;
    uint32_t  bAcceptableT2O_RpiValue;
  } EIP_CM_RPI_VALUE_NOT_ACCEPTABLE_T;


#ifdef __HIL_PRAGMA_PACK_ENABLE
#pragma __HIL_PRAGMA_UNPACK_1(EIP_CM)
#endif

  /* Acceptable RPI types (from Volume 1, Edition 3.20, Table 3-5.33) */
  typedef enum EIP_CM_ACCEPTABLE_RPI_TYPES_Etag
  {
    EIP_CM_ACCEPTABLE_RPI_TYPE_ACCEPTED   = 0,  /* the RPI specified in the forward open was acceptable          */
    EIP_CM_ACCEPTABLE_RPI_TYPE_UNSEPCIFIED,     /* unspecified (used to suggest an alternate RPI, e.g. default)  */
    EIP_CM_ACCEPTABLE_RPI_TYPE_MINIMUM_RPI,     /* minimum acceptable RPI (used when RPI was too fast for range) */
    EIP_CM_ACCEPTABLE_RPI_TYPE_MAXIMUM_RPI,     /* maximum acceptable RPI (used when RPI was too slow for range) */
    EIP_CM_ACCEPTABLE_RPI_TYPE_REQUIRED_RPI     /* required RPI to correct mismatch (used when data already
                                                   being consumed at a different interval)                       */
  } EIP_CM_ACCEPTABLE_RPI_TYPES_E;

  /*#####################################################################################*/

  typedef struct EIP_CM_Ttag
  {
    CIP_CLASS_ATTR_DIR_T tClass;
  } EIP_CM_T;

  typedef struct EIP_CM_INIT_PARAM_Ttag
  {
    uint16_t usMaxConnections;
  } EIP_CM_INIT_PARAM_T;

  /*#####################################################################################*/

  struct EIP_RESOURCES_Ttag;  /* forward declaration */

  uint32_t
  EipCm_ObjectCreate(struct EIP_RESOURCES_Ttag *hEip, void* pvParam );

  bool
  CipCm_IsTransmissionTriggerTimerEnabled(struct EIP_RESOURCES_Ttag *hEip);

  /*#####################################################################################*/

#endif /* EIP_CM_H */
