/**************************************************************************************
Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
***************************************************************************************
$Id: eip_eif_packetdefinitions_packets.h 73908 2018-01-30 09:15:51Z MarcBommert $:

Description:
This header provides all packet definitions specific to the EtherNet/IP stack.

These definitions contribute to the LFW API of the EtherNet/IP stack and are
applicable to the DPM packet interface.

**************************************************************************************/


#ifndef eip_eif_PACKETDEFINITIONS_PACKETS_H_
#define eip_eif_PACKETDEFINITIONS_PACKETS_H_

#include <Hil_Packet.h>

#define EIP_OBJECT_MAX_PACKET_LEN               1520  /*!< Maximum packet length */

/* pragma pack */
#ifdef __HIL_PRAGMA_PACK_ENABLE
#pragma __HIL_PRAGMA_PACK_1(EIP_DPMINTF_PUBLIC)
#endif

typedef enum EIP_OBJECT_MR_REGISTER_OPTION_FLAGS_Etag
{
  EIP_OBJECT_MR_REGISTER_OPTION_FLAGS_USE_OBJECT_PROVIDED_BY_STACK = 1, /* Activate a stack internal CIP object.
                                                                           This option can currently be used for the following CIP objects

                                                                             - Time Sync object (class code 0x43)
                                                                           */
} EIP_OBJECT_MR_REGISTER_OPTION_FLAGS_E;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_DPMINTF_TI_ACD_LAST_CONFLICT_Ttag
{
  uint8_t    bAcdActivity;      /*!< State of ACD activity when last
                                       conflict detected */

  uint8_t    abRemoteMac[6];    /*!< MAC address of remote node from
                                       the ARP PDU in which a conflict was
                                       detected */

  uint8_t    abArpPdu[28];      /*!< Copy of the raw ARP PDU in which
                                       a conflict was detected. */
} EIP_DPMINTF_TI_ACD_LAST_CONFLICT_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_DPMINTF_TI_MCAST_CONFIG_Ttag
{
  uint8_t    bAllocControl;     /* Multicast address allocation control
                                    word. Determines how addresses are
                                    allocated. */
  uint8_t    bReserved;
  uint16_t   usNumMCast;        /* Number of IP multicast addresses
                                   to allocate for EtherNet/IP */
  uint32_t   ulMcastStartAddr;  /* Starting multicast address from which */
} EIP_DPMINTF_TI_MCAST_CONFIG_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST  EIP_DPMINTF_QOS_CONFIG_Ttag
{
  uint32_t    ulQoSFlags;           /* Deprecated. Set to 0. */
  uint8_t     bTag802Enable;        /* QoS Attribute 1 */
  uint8_t     bDSCP_PTP_Event;      /* QoS Attribute 2 */
  uint8_t     bDSCP_PTP_General;    /* QoS Attribute 3 */
  uint8_t     bDSCP_Urgent;         /* QoS Attribute 4 */
  uint8_t     bDSCP_Scheduled;      /* QoS Attribute 5 */
  uint8_t     bDSCP_High;           /* QoS Attribute 6 */
  uint8_t     bDSCP_Low;            /* QoS Attribute 7 */
  uint8_t     bDSCP_Explicit;       /* QoS Attribute 8 */
} EIP_DPMINTF_QOS_CONFIG_T;


/*#####################################################################################*/


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_APS_SET_PARAMETER_REQ_Ttag
{
  uint32_t  ulParameterFlags;    /*!< Parameter flags \n

                                       \description
                                       \ref EIP_APS_PRM_SIGNAL_MS_NS_CHANGE\n
                                       If set, this flag enables the notification of
                                       the network and module status. Every time the status
                                       of the module or network changes packet
                                       EIP_APS_MS_NS_CHANGE_IND will be sent to the
                                       registered EtherNet/IP Application task. */

} EIP_APS_SET_PARAMETER_REQ_T;

#define EIP_APS_SET_PARAMETER_REQ_SIZE (sizeof(EIP_APS_SET_PARAMETER_REQ_T))

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_APS_PACKET_SET_PARAMETER_REQ_Ttag
{
  HIL_PACKET_HEADER_T             tHead;
  EIP_APS_SET_PARAMETER_REQ_T     tData;
} EIP_APS_PACKET_SET_PARAMETER_REQ_T;

 #define EIP_APS_SET_PARAMETER_CNF_SIZE 0

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_APS_PACKET_SET_PARAMETER_CNF_Ttag
{
  HIL_PACKET_HEADER_T tHead;
} EIP_APS_PACKET_SET_PARAMETER_CNF_T;

/*#####################################################################################*/

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_APS_MS_NS_CHANGE_IND_Ttag
{
  uint32_t  ulModuleStatus;     /*!< Module Status \n

                                       \description
                                       The module status describes the current state of the
                                       corresponding MS-LED (provided that it is connected).
                                       See \ref EIP_HW_MODUL_STATUS_E for more details. */

  uint32_t  ulNetworkStatus;     /*!< Network Status \n

                                       \description
                                       The network status describes the current state of the
                                       corresponding NS-LED (provided that it is connected).
                                       See \ref EIP_HW_NET_STATUS_E for more details. */

} EIP_APS_MS_NS_CHANGE_IND_T;

#define EIP_APS_MS_NS_CHANGE_IND_SIZE (sizeof(EIP_APS_MS_NS_CHANGE_IND_T))

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_APS_PACKET_MS_NS_CHANGE_IND_Ttag
{
  HIL_PACKET_HEADER_T             tHead;
  EIP_APS_MS_NS_CHANGE_IND_T      tData;
} EIP_APS_PACKET_MS_NS_CHANGE_IND_T;


#define EIP_APS_MS_NS_CHANGE_RES_SIZE 0

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_APS_PACKET_MS_NS_CHANGE_RES_Ttag
{
  HIL_PACKET_HEADER_T             tHead;
} EIP_APS_PACKET_MS_NS_CHANGE_RES_T;

/*#####################################################################################*/


#define EIP_APS_GET_MS_NS_REQ_SIZE       0

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_APS_PACKET_GET_MS_NS_REQ_Ttag
{
  HIL_PACKET_HEADER_T             tHead;
} EIP_APS_PACKET_GET_MS_NS_REQ_T;


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_APS_GET_MS_NS_CNF_Ttag
{
  uint32_t  ulModuleStatus;     /*!< Module Status \n

                                       \description
                                       The module status describes the current state of the
                                       corresponding MS-LED (provided that it is connected).
                                       See \ref EIP_HW_MODUL_STATUS_E for more details. */

  uint32_t  ulNetworkStatus;     /*!< Network Status \n

                                       \description
                                       The network status describes the current state of the
                                       corresponding NS-LED (provided that it is connected).
                                       See \ref EIP_HW_NET_STATUS_E for more details. */

} EIP_APS_GET_MS_NS_CNF_T;

#define EIP_APS_GET_MS_NS_CNF_SIZE  (sizeof(EIP_APS_GET_MS_NS_CNF_T))

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_APS_PACKET_GET_MS_NS_CNF_Ttag
{
  HIL_PACKET_HEADER_T      tHead;
  EIP_APS_GET_MS_NS_CNF_T  tData;

} EIP_APS_PACKET_GET_MS_NS_CNF_T;

/*#####################################################################################*/

typedef  __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_MR_REGISTER_REQ_Ttag
{
  uint32_t ulReserved1;              /*!< Reserved, set to 0x00 */

  uint32_t ulClass;                 /*!< Object class identifier

                                           \valueRange \n\n
                                           Instances of the Assembly Object are divided into the following address ranges to \n
                                           provide for extensions to device profiles \n\n
                                           0x0001 - 0x0063 : Open                            \n
                                           0x0064 - 0x00C7 : Vendor Specific                 \n
                                           0x00C8 - 0x00EF : Reserved by ODVA for future use \n
                                           0x00F0 - 0x02FF : Open                            \n
                                           0x0300 - 0x04FF : Vendor Specific                 \n
                                           0x0500 - 0xFFFF : Reserved by ODVA for future use \n */

  uint32_t ulOptionFlags;           /*!< Option flags

                                           see EIP_OBJECT_MR_REGISTER_OPTION_FLAGS_E
                                           */

} EIP_OBJECT_MR_REGISTER_REQ_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_MR_PACKET_REGISTER_REQ_Ttag
{
  HIL_PACKET_HEADER_T           tHead;
  EIP_OBJECT_MR_REGISTER_REQ_T  tData;
} EIP_OBJECT_MR_PACKET_REGISTER_REQ_T;

#define EIP_OBJECT_MR_REGISTER_REQ_SIZE   (sizeof(EIP_OBJECT_MR_REGISTER_REQ_T) )

typedef  __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_MR_PACKET_REGISTER_CNF_Ttag
{
  HIL_PACKET_HEADER_T           tHead;
} EIP_OBJECT_MR_PACKET_REGISTER_CNF_T;

#define EIP_OBJECT_MR_REGISTER_CNF_SIZE   0

/*#####################################################################################*/

typedef  __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_AS_REGISTER_REQ_Ttag
{
  uint32_t      ulInstance;       /*!< Assembly instance number \n

                                         \valueRange \n\n
                                         Instances of the Assembly Object are divided into the following address ranges to  \n
                                         provide for extensions to device profiles \n\n
                                         0x00000001 - 0x00000063 : Open (static assemblies defined in device profile)       \n
                                         0x00000064 - 0x000000C7 : Vendor Specific static assemblies and dynamic assemblies \n
                                         0x000000C8 - 0x000002FF : Open (static assemblies defined in device profile)       \n
                                         0x00000300 - 0x000004FF : Vendor Specific static assemblies and dynamic assemblies \n
                                         0x00000500 - 0x000FFFFF : Open (static assemblies defined in device profile)       \n
                                         0x00100000 - 0xFFFFFFFF : Reserved by CIP for future use.

                                         \note \n\n
                                         The instance numbers 192 (hex C0) and 193 (hex C1) are usually used as the dummy connection
                                         points for listen only and input only connections. These values <b>must not </b> be used for new
                                         connection points.\n\n
                                         If loadable firmware is used these values are not adaptable. If not, theses values can be changed
                                         within the startup parameters of the object task
                                         (see structure EIP_OBJECT_PARAMETER_T in EipObject_Functionlist.h).
                                         */

  uint32_t      ulDPMOffset;      /*!< Relative offset of the assembly instance data within the DPM input/output area. */
  uint32_t      ulSize;           /*!< Size of the assembly instance data in bytes */
  uint32_t      ulFlags;          /*!< Flags to configure the assembly instance properties \n

                                         \valueRange \n

                                         - Bit  0: <b> \ref EIP_AS_TYPE_INPUT           </b> \n\n
                                         - Bit  3: <b> \ref EIP_AS_OPTION_NO_RUNIDLE    </b> \n\n
                                         - Bit  4: <b> \ref EIP_AS_OPTION_RESERVED      </b> \n\n
                                         - Bit  5: <b> \ref EIP_AS_TYPE_CONFIG          </b> \n\n
                                         - Bit  6: <b> \ref EIP_AS_OPTION_HOLDLASTSTATE </b> \n\n
                                         - Bit  7: <b> \ref EIP_AS_OPTION_FIXEDSIZE     </b> \n\n
                                         - Bit  8: <b> \ref EIP_AS_OPTION_MAP_RUNIDLE   </b> \n\n
                                         - Bit  9: <b> \ref EIP_AS_OPTION_INVISIBLE     </b> \n\n
                                         - Bit 10: <b> \ref EIP_AS_OPTION_MAP_SEQCOUNT  </b> \n\n
                                         - Bit 11: <b> \ref EIP_AS_OPTION_RXTRIGGER     </b> \n\n
                                         - Bit 30: <b> \ref EIP_AS_TYPE_INPUTONLY       </b> \n\n
                                         - Bit 31: <b> \ref EIP_AS_TYPE_LISTENONLY      </b> \n\n */

} EIP_OBJECT_AS_REGISTER_REQ_T;

typedef  __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_AS_PACKET_REGISTER_REQ_Ttag
{
  HIL_PACKET_HEADER_T           tHead;
  EIP_OBJECT_AS_REGISTER_REQ_T  tData;
} EIP_OBJECT_AS_PACKET_REGISTER_REQ_T;

#define EIP_OBJECT_AS_REGISTER_REQ_SIZE   (sizeof(EIP_OBJECT_AS_REGISTER_REQ_T) )

typedef  __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_AS_REGISTER_CNF_Ttag
{
  uint32_t  ulInstance;               /*!< Assembly instance number from the request packet              */
  uint32_t  ulDPMOffset;              /*!< DPM offset for the instance data area from the request packet */
  uint32_t  ulSize;                   /*!< Size of the data area from the request packet                 */
  uint32_t  ulFlags;                  /*!< Assembly flags from the request packet                        */
  uint32_t  ulDataBuf;                /*!< Handle of the triple data buffer                              */
} EIP_OBJECT_AS_REGISTER_CNF_T;

typedef  __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_AS_PACKET_REGISTER_CNF_Ttag
{
  HIL_PACKET_HEADER_T           tHead;
  EIP_OBJECT_AS_REGISTER_CNF_T  tData;
} EIP_OBJECT_AS_PACKET_REGISTER_CNF_T;

#define EIP_OBJECT_AS_REGISTER_CNF_SIZE   (sizeof(EIP_OBJECT_AS_REGISTER_CNF_T) )

/*#####################################################################################*/

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_RESET_IND_Ttag
{
  uint32_t ulDataIdx;                 /*!< Index of the service */
  uint32_t ulResetTyp;                /*!< Type of the reset \n

                                             \valueRange
                                             0, 1

                                             \description
                                             0: Reset is done emulating power cycling of
                                                the device(default)\n
                                             1: Return as closely as possible to the factory
                                                default configuration. Reset is then done
                                                emulating power cycling of the device.<br>
                                                <b>Note</b>:\n Reset type 1 is not supported
                                                by default. It needs to be enabled separately
                                                using the command EIP_OBJECT_SET_PARAMETER_REQ.*/

}EIP_OBJECT_RESET_IND_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_RESET_IND_Ttag
{
  HIL_PACKET_HEADER_T     tHead;
  EIP_OBJECT_RESET_IND_T  tData;
}EIP_OBJECT_PACKET_RESET_IND_T;

#define EIP_OBJECT_RESET_IND_SIZE          (sizeof(EIP_OBJECT_RESET_IND_T))

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_RESET_RES_Ttag
{
  HIL_PACKET_HEADER_T     tHead;
}EIP_OBJECT_PACKET_RESET_RES_T;

#define EIP_OBJECT_RESET_RES_SIZE          0

/*#####################################################################################*/

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_EXT_CONNECTION_INFO_Ttag
{
  uint32_t  ulProConnId;           /*!< Producer Connection ID (T->O)                 */
  uint32_t  ulConConnId;           /*!< Consumer Connection ID (O->T)                 */

  uint32_t  ulConnSerialNum;       /*!< Connection serial number                      */
  uint16_t  usOrigVendorId;        /*!< Originator device vendor ID                   */
  uint32_t  ulOrigDeviceSn;        /*!< Originator device serial number               */

  /* Producer parameters */
  uint32_t  ulProApi;              /*!< Actual packet interval (usecs) (T->O)         */
  uint16_t  usProConnParams;       /*!< Connection parameters (T->O) from ForwardOpen */

  /* Consumer parameters */
  uint32_t  ulConApi;              /*!< Actual packet interval (usecs) (O->T)          */
  uint16_t  usConConnParams;       /*!< Connection parameters (O->T)  from ForwardOpen */

  uint8_t   bTimeoutMultiplier;    /*!< Connection timeout multiplier                  */

}  EIP_OBJECT_EXT_CONNECTION_INFO_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_CONNECTION_IND_Ttag
{

  uint32_t ulConnectionState;

  /* These are for the adapter role */
  uint16_t usNumExclusiveOwner;
  uint16_t usNumInputOnly;
  uint16_t usNumListenOnly;
  uint16_t usNumExplicitMessaging;

  /* These are for the scanner role */
  uint16_t usNumImplicitMessagingOriginator;    /* Number of class 0/1 connections currently opened by us */
  uint16_t usNumExplicitMessagingOriginator;    /* Number of class 3 connections currently opened by us */

  uint8_t   bConnType;
  uint8_t   abReserved[3];

  struct
  {
    uint32_t ulClass;     /* (ulClass == 0) -> application path not present/decoded */
    uint32_t ulInstance;
    uint32_t ulConnPoint;
    uint32_t ulAttribute;
    uint32_t ulMember;
  } tConfigPath;          /* this connection's application path for configuration */

  struct
  {
    uint32_t ulClass;
    uint32_t ulInstance;
    uint32_t ulConnPoint;
    uint32_t ulAttribute;
    uint32_t ulMember;
  } tConsumptionPath;     /* this connection's application path for consumption from the network */

  struct
  {
    uint32_t ulClass;
    uint32_t ulInstance;
    uint32_t ulConnPoint;
    uint32_t ulAttribute;
    uint32_t ulMember;
  } tProductionPath;     /* this connection's application path for production towards the network */

  uint16_t  usConnSerialNum;
  uint16_t  usVendorId;
  uint32_t  ulOSerialNum;

  uint8_t   bPriority;
  uint8_t   bTimeOutTicks;
  uint8_t   bTimeoutMultiple;
  uint8_t   bTriggerType;

  uint32_t  ulOTConnID;
  uint32_t  ulTOConnID;

  uint32_t  ulOTRpi;
  uint16_t  usOTConnParam;
  uint16_t  usOTConnSize;
  uint32_t  ulTORpi;
  uint16_t  usTOConnParam;
  uint16_t  usTOConnSize;

  uint32_t  ulProInhib;


  uint32_t ulExtendedState;          /*!< Extended status (see \ref EIP_EXT_CONNECTION_STATE_Ttag)          */

} EIP_OBJECT_CONNECTION_IND_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_CONNECTION_IND_Ttag
{
  HIL_PACKET_HEADER_T          tHead;
  EIP_OBJECT_CONNECTION_IND_T  tData;
}EIP_OBJECT_PACKET_CONNECTION_IND_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_CONNECTION_RES_Ttag
{
  HIL_PACKET_HEADER_T     tHead;
}EIP_OBJECT_PACKET_CONNECTION_RES_T;

/*#####################################################################################*/

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_CL3_SERVICE_IND_Ttag
{
  uint32_t    ulConnectionId;          /*!< Connection Handle \n

                                           \description \n
                                           The connection handle uniquely identifies
                                           the connection. The same connection handle is
                                           used for all transmissions of a particular
                                           connection. \n\n
                                           <b>Important: \n</b>
                                           This handle must be used for the response to
                                           this indication */

  uint8_t     bService;                /*!< Service \n

                                           \description \n
                                           This parameter holds the requested CIP service
                                           that related to the next parameter (ulObject).\n\n
                                           <b>Important: \n</b>
                                           This parameter must be used again for the response
                                           to this indication */

  uint8_t     abPad0[3];               /*!< Padding, set to zero */

  uint32_t    ulObject;                /*!< Object class \n

                                           \description \n
                                           This parameter holds the CIP object class ID.\n\n
                                           <b>Important: \n</b>
                                           This parameter must be used again for the response
                                           to this indication */

  uint32_t    ulInstance;              /*!< Instance \n

                                           \description \n
                                           This parameter holds the instance number
                                           of the object class specified in ulObject.\n\n
                                           <b>Important: \n</b>
                                           This parameter must be used again for the response
                                           to this indication */

  uint32_t    ulAttribute;             /*!< Attribute \n

                                           \description \n
                                           This parameter holds the attribute number
                                           of the object class instance specified in
                                           ulInstance.\n\n
                                           <b>Important: \n</b>
                                           This parameter must be used again for the response
                                           to this indication */

  uint32_t    ulMember;                /*!< Attribute \n

                                           \description \n
                                           This parameter holds the member number
                                           of the object class instance attribute specified in
                                           ulAttribute.\n\n
                                           <b>Important: \n</b>
                                           This parameter must be used again for the response
                                           to this indication */

  uint8_t     abData[1];
} EIP_OBJECT_CL3_SERVICE_IND_T ;


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_CL3_SERVICE_IND_Ttag
{
  HIL_PACKET_HEADER_T             tHead;
  EIP_OBJECT_CL3_SERVICE_IND_T    tData;
} EIP_OBJECT_PACKET_CL3_SERVICE_IND_T;



#define EIP_OBJECT_CL3_SERVICE_IND_SIZE (sizeof(EIP_OBJECT_CL3_SERVICE_IND_T)-1)


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_CL3_SERVICE_RES_Ttag
{
  uint32_t    ulConnectionId;          /*!< Connection Handle \n

                                           \description \n
                                           The connection handle from the indication packet. */

  uint8_t     bService;                /*!< Service \n

                                           \description \n
                                           The service code from the indication packet. */

  uint8_t     abPad0[3];               /*!< Padding, set to zero */

  uint32_t    ulObject;                /*!< Object class \n

                                           \description \n
                                           The object class ID from the indication packet */

  uint32_t    ulInstance;              /*!< Instance \n

                                           \description \n
                                           The instance number from the indication packet */

  uint32_t    ulAttribute;             /*!< Attribute \n

                                           \description \n
                                           The attribute number from the indication packet */

  uint32_t    ulMember;                /*!< Attribute \n

                                             \description \n
                                             The attribute number from the indication packet */

  uint32_t    ulGRC;                 /*!< Generic Error Code \n

                                           \description \n
                                            This generic error code can be used to
                                            indicate whether the service request was successful
                                            or unsuccessful. */

  uint32_t    ulERC;                 /*!< Extended Error Code \n

                                           \description \n
                                           This extended error code can be used to
                                           describe the occurred error (GRC) in more detail. */

  uint8_t     abData[1];             /*!< Response service  data */

} EIP_OBJECT_CL3_SERVICE_RES_T;


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_CL3_SERVICE_RES_Ttag
{
  HIL_PACKET_HEADER_T            tHead;
  EIP_OBJECT_CL3_SERVICE_RES_T   tData;
} EIP_OBJECT_PACKET_CL3_SERVICE_RES_T;



#define EIP_OBJECT_CL3_SERVICE_RES_SIZE (sizeof(EIP_OBJECT_CL3_SERVICE_RES_T)-1)

/*#####################################################################################*/

typedef  __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_FORCE_LED_STATE_REQ_Ttag
{
  uint32_t ulLedType;       /*!< CIP_LED_TYPE_E */
  uint32_t ulLedState;      /*!< CIP_LED_FORCE_STATE_E */
} EIP_OBJECT_FORCE_LED_STATE_REQ_T;

/* command for register a new object to the message router */
typedef  __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_FORCE_LED_STATE_REQ_Ttag
{
  HIL_PACKET_HEADER_T               tHead;
  EIP_OBJECT_FORCE_LED_STATE_REQ_T  tData;
} EIP_OBJECT_PACKET_FORCE_LED_STATE_REQ_T;

#define EIP_OBJECT_FORCE_LED_STATE_REQ_SIZE (sizeof(EIP_OBJECT_FORCE_LED_STATE_REQ_T))

typedef  __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_FORCE_LED_STATE_CNF_Ttag
{
  HIL_PACKET_HEADER_T           tHead;
} EIP_OBJECT_PACKET_FORCE_LED_STATE_CNF_T;

#define EIP_OBJECT_FORCE_LED_STATE_CNF_SIZE 0

/*#####################################################################################*/

typedef  __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_REGISTER_SERVICE_REQ_Ttag
{
  uint8_t bService;                  /*!< Service code \n

                                      \valueRange \n\n
                                       Instances of the Assembly Object are divided into the following address ranges to \n
                                       provide for extensions to device profiles \n\n

                                       0x00 - 0x31 : Open. These are referred to as Common Services.
                                                     These are defined in the EtherNet/IP spec. Vol.1 Appendix A.\n
                                       0x32 - 0x4A : Vendor Specific                        \n
                                       0x4B - 0x63 : Object Class Specific                  \n
                                       0x64 - 0x7F : Reserved by ODVA for future use        \n
                                       0x80 - 0xFF : Reserved for use as Reply Service Code \n

                                       \description \n\n
                                       Pre-defined service codes (hex): \n\n
                                       00:    Reserved \n
                                       01:    Get_Attributes_All \n
                                       02:    Set_Attributes_All \n
                                       03:    Get_Attribute_List \n
                                       04:    Set_Attribute_List \n
                                       05:    Reset \n
                                       06:    Start \n
                                       07:    Stop \n
                                       08:    Create \n
                                       09:    Delete \n
                                       0A:    Multiple_Service_Packet \n
                                       0B:    Reserved for future use \n
                                       0D:    Apply_Attributes \n
                                       0E:    Get_Attribute_Single \n
                                       0F:    Reserved for future use \n
                                       10:   Set_Attribute_Single \n
                                       11:    Find_Next_Object_Instance \n
                                       12-13: Reserved for future use \n
                                       14:    Error Response (used by DevNet only) \n
                                       15:    Restore \n
                                       16:    Save \n
                                       17:    No Operation (NOP) \n
                                       18:    Get_Member \n
                                       19:    Set_Member \n
                                       1A:    Insert_Member \n
                                       1B:    Remove_Member \n
                                       1C:    GroupSync \n
                                       1D-31: Reserved for additional Common Services \n */


} EIP_OBJECT_REGISTER_SERVICE_REQ_T;

/* command for register a new object to the message router */
typedef  __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_REGISTER_SERVICE_REQ_Ttag
{
  HIL_PACKET_HEADER_T                tHead;
  EIP_OBJECT_REGISTER_SERVICE_REQ_T  tData;
} EIP_OBJECT_PACKET_REGISTER_SERVICE_REQ_T;

#define EIP_OBJECT_REGISTER_SERVICE_REQ_SIZE (sizeof(EIP_OBJECT_REGISTER_SERVICE_REQ_T))

typedef  __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_REGISTER_SERVICE_CNF_Ttag
{
  HIL_PACKET_HEADER_T           tHead;
} EIP_OBJECT_PACKET_REGISTER_SERVICE_CNF_T;

#define EIP_OBJECT_REGISTER_SERVICE_CNF_SIZE 0

/*#####################################################################################*/

/* Set Parameter Request */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_SET_PARAMETER_REQ_Ttag
{
  uint32_t ulParameterFlags;    /*!< Parameter flags \n

                                       \description
                                       \ref EIP_OBJECT_PRM_FWRD_OPEN_CLOSE_FORWARDING \n
                                       Enable forwarding of Forward_Open/Forward_Close frames
                                       to the EtherNet/IP Application.
                                       \ref EIP_OBJECT_PRM_DISABLE_FLASH_LEDS_SERVICE
                                       Disable/Unsupport FLash_LEDs service of Identity object.
                                       \ref EIP_OBJECT_PRM_DISABLE_TRANSMISSION_TRIGGER_TIMER
                                       Disable Transmission Trigger Timer for change-of-state and
                                       application-triggered connections.
                                       \ref EIP_OBJECT_PRM_ENABLE_NULL_FWRD_OPEN
                                       Enable support for NULL-ForwardOpen at ConnectionManager object.
                                   */
} EIP_OBJECT_SET_PARAMETER_REQ_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_SET_PARAMETER_REQ_Ttag
{
  HIL_PACKET_HEADER_T             tHead;
  EIP_OBJECT_SET_PARAMETER_REQ_T  tData;
}EIP_OBJECT_PACKET_SET_PARAMETER_REQ_T;

#define EIP_OBJECT_SET_PARAMETER_REQ_SIZE  (sizeof(EIP_OBJECT_SET_PARAMETER_REQ_T))

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_SET_PARAMETER_CNF_Ttag
{
  HIL_PACKET_HEADER_T          tHead;
}EIP_OBJECT_PACKET_SET_PARAMETER_CNF_T;

#define EIP_OBJECT_SET_PARAMETER_CNF_SIZE  0

/*#####################################################################################*/

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_CIP_SERVICE_REQ_Ttag
{
  uint8_t     bService;                           /*!< CIP service code          */
  uint8_t     abPad0[3];                          /*!< Padding, set to zero      */
  uint32_t    ulClass;                            /*!< CIP class ID              */
  uint32_t    ulInstance;                         /*!< CIP instance number       */
  uint32_t    ulAttribute;                        /*!< CIP attribute number      */
  uint32_t    ulMember;                           /*!< CIP member number         */
  uint8_t     abData[EIP_OBJECT_MAX_PACKET_LEN];  /*!< CIP Service Data. <br><br>
                                                         Number of bytes provided in this field
                                                         must be add to the packet header length field
                                                         ulLen.
                                                         Do the following to set the proper packet length:<br>
                                                         ptReq->tHead.ulLen = EIP_OBJECT_CIP_SERVICE_REQ_SIZE + number of bytes provided in abData */
} EIP_OBJECT_CIP_SERVICE_REQ_T;


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_CIP_SERVICE_REQ_Ttag
{
  HIL_PACKET_HEADER_T            tHead;
  EIP_OBJECT_CIP_SERVICE_REQ_T   tData;
}EIP_OBJECT_PACKET_CIP_SERVICE_REQ_T;

#define EIP_OBJECT_CIP_SERVICE_REQ_SIZE     (sizeof(EIP_OBJECT_CIP_SERVICE_REQ_T) - EIP_OBJECT_MAX_PACKET_LEN)

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_CIP_SERVICE_CNF_Ttag
{
  uint8_t     bService;                           /*!< CIP service code          */
  uint8_t     abPad0[3];                          /*!< padding, set to zero      */
  uint32_t    ulClass;                            /*!< CIP class ID              */
  uint32_t    ulInstance;                         /*!< CIP instance number       */
  uint32_t    ulAttribute;                        /*!< CIP attribute number      */
  uint32_t    ulMember;                           /*!< CIP attribute number      */

  uint32_t    ulGRC;                              /*!< Generic Error Code        */
  uint32_t    ulERC;                              /*!< Extended Error Code       */

  uint8_t     abData[EIP_OBJECT_MAX_PACKET_LEN];  /*!< CIP service data. <br><br>
                                                         Number of bytes provided in this field
                                                         is encoded in the packet header length field
                                                         ulLen. <br><br>
                                                         Do the following to get the data size:<br>
                                                         ulSize = ptCnf->tHead.ulLen - EIP_OBJECT_CIP_SERVICE_CNF_SIZE */

} EIP_OBJECT_CIP_SERVICE_CNF_T;


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_CIP_SERVICE_CNF_Ttag
{
  HIL_PACKET_HEADER_T             tHead;
  EIP_OBJECT_CIP_SERVICE_CNF_T    tData;
} EIP_OBJECT_PACKET_CIP_SERVICE_CNF_T;

#define EIP_OBJECT_CIP_SERVICE_CNF_SIZE     (sizeof(EIP_OBJECT_CIP_SERVICE_CNF_T) - EIP_OBJECT_MAX_PACKET_LEN)

/*#####################################################################################*/

/**
   Packets defined for forwarding of forward close and forward open
   from Stack to host application.
   Packets are defined on the assumption/rule that the host application uses the received
   packet for generating the response.
   That's why the indication reserves some space that are used for response parameters
*/
#define EIP_LFWD_OPEN_DATA_MAX_PATHLEN (1000)

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_LFWD_OPEN_DATA
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
  uint8_t   bConnPath[EIP_LFWD_OPEN_DATA_MAX_PATHLEN];        /* connection path              */
} EIP_LFWD_OPEN_DATA_T;

/* Deliver Forward Open to host application */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_LFWD_OPEN_FWD_IND_Ttag
{
  uint32_t                 ulRouteMsg;             /**< Link to to remember underlying Encapsulation request (must not be modified by app)   */
  uint32_t                 aulReserved[4];         /**< Place holder to be filled by response parameters, see  EIP_OBJECT_LFWD_OPEN_FWD_RES_T */
  EIP_LFWD_OPEN_DATA_T     tFwdOpenData;           /**< Forward Open request data to be delivered to host                                    */
} EIP_OBJECT_LFWD_OPEN_FWD_IND_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_LFWD_OPEN_FWD_IND_Ttag
{
  HIL_PACKET_HEADER_T                  tHead;
  EIP_OBJECT_LFWD_OPEN_FWD_IND_T       tData;
} EIP_OBJECT_PACKET_LFWD_OPEN_FWD_IND_T;

#define EIP_OBJECT_LFWD_OPEN_FWD_IND_SIZE  (sizeof(EIP_OBJECT_LFWD_OPEN_FWD_IND_T) - EIP_LFWD_OPEN_DATA_MAX_PATHLEN)


/**
   Deliver Forward Open to host application - Response
   Contains the potentially modified forward open data (since host application may need to modify connection points e.g. for safety)
   Additional parameters are: Status (host application result) and a reference (size/offset) to the application reply data
   that needs to be appended to the Forward open response generated by the stack
*/
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_LFWD_OPEN_FWD_RES_Ttag
{
  uint32_t                 ulRouteMsg;            /**< Link to underlying Encapsulation request           */
  uint32_t                 ulGRC;                /**< General Error Code                                 */
  uint32_t                 ulERC;                /**< Extended Error Code                                */
  uint32_t                 ulAppReplyOffset;     /**< Offset of Application Reply data                   */
  uint32_t                 ulAppReplySize;       /**< Byte-size of Application Reply data                */
  EIP_LFWD_OPEN_DATA_T     tFwdOpenData;         /**< modified forward open (Note that the application
                                                      reply data is appended, which is not visible here) */
} EIP_OBJECT_LFWD_OPEN_FWD_RES_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_LFWD_OPEN_FWD_RES_Ttag
{
  HIL_PACKET_HEADER_T                       tHead;
  EIP_OBJECT_LFWD_OPEN_FWD_RES_T            tData;
} EIP_OBJECT_PACKET_LFWD_OPEN_FWD_RES_T;

#define EIP_OBJECT_LFWD_OPEN_FWD_RES_SIZE (sizeof(EIP_OBJECT_LFWD_OPEN_FWD_RES_T) - EIP_LFWD_OPEN_DATA_MAX_PATHLEN)

/*#####################################################################################*/

/** Status indication of Forward Open, that was previously processed by the
 * host (see EIP_OBJECT_LFWD_OPEN_FWD_IND)*/
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_FWD_OPEN_FWD_COMPLETION_IND_Ttag
{
  uint16_t  usReserved;         /**< Reserved                           */
  uint16_t  usConnSerialNum;    /**< Connection serial number           */
  uint16_t  usVendorId;         /**< Originator Vendor ID               */
  uint32_t  ulOSerialNum;       /**< Originator serial number           */
  uint32_t  ulGRC;              /**< General Error Code                 */
  uint32_t  ulERC;              /**< Extended Error Code                */
} EIP_OBJECT_FWD_OPEN_FWD_COMPLETION_IND_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_FWD_OPEN_FWD_COMPLETION_IND_Ttag
{
  HIL_PACKET_HEADER_T                       tHead;
  EIP_OBJECT_FWD_OPEN_FWD_COMPLETION_IND_T  tData;
} EIP_OBJECT_PACKET_FWD_OPEN_FWD_COMPLETION_IND_T;

#define EIP_OBJECT_FWD_OPEN_FWD_COMPLETION_IND_SIZE   (sizeof(EIP_OBJECT_FWD_OPEN_FWD_COMPLETION_IND_T))


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_FWD_OPEN_FWD_COMPLETION_RES_Ttag
{
  HIL_PACKET_HEADER_T                       tHead;
} EIP_OBJECT_PACKET_FWD_OPEN_FWD_COMPLETION_RES_T;

#define EIP_OBJECT_FWD_OPEN_FWD_COMPLETION_RES_SIZE   0

/*#####################################################################################*/

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_CM_APP_FWCLOSE_IND_Ttag
{
  uint8_t   bPriority;                             /* Used to calculate request timeout information  */
  uint8_t   bTimeOutTicks;                         /* Used to calculate request timeout information  */
  uint16_t  usConnSerialNum;                       /* Connection serial number                       */
  uint16_t  usVendorId;                            /* Originator Vendor ID                           */
  uint32_t  ulOSerialNum;                          /* Originator serial number                       */
  uint8_t   bConnPathSize;                         /* Connection path size in 16bit words            */
  uint8_t   bReserved1;
  uint8_t   bConnPath[EIP_OBJECT_MAX_PACKET_LEN];  /* connection path                                */
} EIP_CM_APP_FWCLOSE_IND_T;


/* Deliver Forward Close to host application - indication */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_FWD_CLOSE_FWD_IND_Ttag
{
  uint32_t                ulRouteMsg;             /*!< Link to underlying Encapsulation request           */
  uint32_t                aulReserved[2];         /*!< Placeholder to be filled by response parameters,
                                                         see  EIP_OBJECT_FWD_CLOSE_FWD_RES_T                */
  EIP_CM_APP_FWCLOSE_IND_T  tFwdCloseData;          /*!< Forward close request data to be delivered to host */
} EIP_OBJECT_FWD_CLOSE_FWD_IND_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_FWD_CLOSE_FWD_IND_Ttag
{
  HIL_PACKET_HEADER_T             tHead;
  EIP_OBJECT_FWD_CLOSE_FWD_IND_T  tData;
} EIP_OBJECT_PACKET_FWD_CLOSE_FWD_IND_T;

#define EIP_OBJECT_FWD_CLOSE_FWD_IND_SIZE   (sizeof(EIP_OBJECT_FWD_CLOSE_FWD_IND_T) - EIP_OBJECT_MAX_PACKET_LEN)


/*
  Deliver Forward Close to host application - response
  Contains the modified forward close (since application may need to modify connection points e.g. for safety)
  Additional parameters are: Status (host application result)
*/
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_FWD_CLOSE_FWD_RES_Ttag
{
  uint32_t                ulRouteMsg;            /*!< Link to underlying Encapsulation request */
  uint32_t                ulGRC;                 /*!< General Error Code                       */
  uint32_t                ulERC;                 /*!< Extended Error Code                      */
  EIP_CM_APP_FWCLOSE_IND_T  tFwdCloseData;       /*!< Modified forward close                   */
} EIP_OBJECT_FWD_CLOSE_FWD_RES_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_FWD_CLOSE_FWD_RES_Ttag
{
  HIL_PACKET_HEADER_T              tHead;
  EIP_OBJECT_FWD_CLOSE_FWD_RES_T   tData;
} EIP_OBJECT_PACKET_FWD_CLOSE_FWD_RES_T;

#define EIP_OBJECT_FWD_CLOSE_FWD_RES_SIZE   (sizeof(EIP_OBJECT_FWD_CLOSE_FWD_RES_T) - EIP_OBJECT_MAX_PACKET_LEN)

/*#####################################################################################*/

#define EIP_OBJECT_CIP_OBJECT_CHANGE_IND_PROPOSE  (0x10)          /*!< The attribute change is pending
                                                                       and the host is given the chance to decide */
#define EIP_OBJECT_CIP_OBJECT_CHANGE_IND_INFORM    (0x20)         /*!< The attribute change already took place
                                                                       and the indication is simply informed about it */
#define EIP_OBJECT_CIP_OBJECT_CHANGE_NV_STORING_BYPASSED (0x40)   /*!< Remanent (NV) storing of new attribute value was explcitly bypassed by protocol stack */

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_CIP_OBJECT_CHANGE_IND_Ttag
{
  uint32_t    ulInfoFlags;                        /*!< Information flags     */
  uint8_t     bService;                           /*!< CIP service code      */
  uint8_t     abPad0[3];                          /*!< padding, set to zero  */
  uint32_t    ulClass;                            /*!< CIP class ID          */
  uint32_t    ulInstance;                         /*!< CIP instance number   */
  uint32_t    ulAttribute;                        /*!< CIP attribute number  */
  uint32_t    ulMember;                           /*!< CIP member number  */

  uint8_t     abData[EIP_OBJECT_MAX_PACKET_LEN];  /*!< Service Data          */

} EIP_OBJECT_CIP_OBJECT_CHANGE_IND_T;


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_CIP_OBJECT_CHANGE_IND_Ttag
{
  HIL_PACKET_HEADER_T                  tHead;
  EIP_OBJECT_CIP_OBJECT_CHANGE_IND_T   tData;
} EIP_OBJECT_PACKET_CIP_OBJECT_CHANGE_IND_T;

#define EIP_OBJECT_CIP_OBJECT_CHANGE_IND_SIZE     (sizeof(EIP_OBJECT_CIP_OBJECT_CHANGE_IND_T) - EIP_OBJECT_MAX_PACKET_LEN)


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_CIP_OBJECT_CHANGE_RES_Ttag
{
  HIL_PACKET_HEADER_T                  tHead;
  EIP_OBJECT_CIP_OBJECT_CHANGE_IND_T   tData;
} EIP_OBJECT_PACKET_CIP_OBJECT_CHANGE_RES_T;

#define EIP_OBJECT_CIP_OBJECT_CHANGE_RES_SIZE    (sizeof(EIP_OBJECT_CIP_OBJECT_CHANGE_IND_T) - EIP_OBJECT_MAX_PACKET_LEN)

/*#####################################################################################*/

/* command for register a new object to the message router */
typedef  __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_APS_PACKET_CONFIG_DONE_REQ_Ttag
{
  HIL_PACKET_HEADER_T                tHead;
} EIP_APS_PACKET_CONFIG_DONE_REQ_T;

#define EIP_APS_CONFIG_DONE_REQ_SIZE 0

typedef  __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_APS_PACKET_CONFIG_DONE_CNF_Ttag
{
  HIL_PACKET_HEADER_T                tHead;
} EIP_APS_PACKET_CONFIG_DONE_CNF_T;

#define EIP_APS_CONFIG_DONE_CNF_SIZE 0
/*! \}*/
/*#####################################################################################*/

#define EIP_APS_CONFIGURATION_PARAMETER_SET_V3 3

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_APS_CONFIGURATION_PARAMETER_SET_V3_Ttag
{

  uint32_t  ulSystemFlags;    /*!< System start flags

                                    This parameter decides whether the system transists from state CONFIGURED into
                                    the state BUS_ON automatically or not. In the latter case, the user will manually
                                    bring the system into the BUS_ON state.

                                     <table>
                                       <tr>
                                        <th colspan="2">Valid values</th>
                                     </tr>
                                     <tr><td>#WSTART_SYSFLAG_START_AUTO</td><td>Automatic system start</td></tr>
                                     <tr><td>#WSTART_SYSFLAG_START_APPLICATION</td><td>Application-controlled system start</td></tr>
                                     </table>
                              */

  uint32_t  ulWdgTime;        /*!< Watch dog time (in milliseconds)

                                     <table>
                                       <tr>
                                        <th colspan="2">Valid values</th>
                                     </tr>
                                     <tr><td>0</td><td>Disable watchdog</td></tr>
                                     <tr><td>20...65535</td><td>Watchdog time in [ms]</td></tr>
                                     </table>
                              */

  uint32_t  ulInputLen;       /*!< Length of input data (Input Assembly)

                                     The size of the input assembly instance \p ulInputAssInstance
                                     i.e. the size of the connection endpoint that consumes data from the network, in bytes.

                                     <table>
                                       <tr>
                                        <th colspan="2">Valid values</th>
                                     </tr>
                                     <tr><td>0...504</td><td>Input assembly size</td></tr>
                                     </table>
                              */
  uint32_t  ulOutputLen;       /*!< Length of output data (Output Assembly)

                                     The size of the output assembly instance \p ulOutputAssInstance
                                     i.e. the size of the connection endpoint that produces data to the network, in bytes.

                                     <table>
                                       <tr>
                                        <th colspan="2">Valid values</th>
                                     </tr>
                                     <tr><td>0...504</td><td>Output assembly size</td></tr>
                                     </table>
                              */

  uint32_t  ulTcpFlag;        /*!< TCP configuration flags

                                     <table>
                                     <tr>
                                        <th colspan="2">Valid values</th>
                                     </tr>
                                     <tr><td>#IP_CFG_FLAG_IP_ADDR</td><td>Static IP address</td></tr>
                                     <tr><td>#IP_CFG_FLAG_NET_MASK</td><td>Static netmask</td></tr>
                                     <tr><td>#IP_CFG_FLAG_GATEWAY</td><td>Static gateway address</td></tr>
                                     <tr><td>#IP_CFG_FLAG_BOOTP</td><td>BOOTP</td></tr>
                                     <tr><td>#IP_CFG_FLAG_DHCP</td><td>DHCP</td></tr>
                                     </table>

                              */
  uint32_t  ulIpAddr;         /*!< IP address

                                     The device's static IP address. All valid IP addresses are allowed.
                                     Multicast and Broadcast addresses may not be set.
                                     Note that the gateway and the device have to be in the same subnet.
                                     The value remains unused, when DHCP or BOOTP is configured by means of \p ulTcpFlags.
                              */
  uint32_t  ulNetMask;        /*!< Network mask

                                     The device's static netmask. All valid netmasks may be set.
                                     Note that the gateway and the device have to be in the same subnet.
                                     The value remains unused, when DHCP or BOOTP is configured by means of \p ulTcpFlags.
                              */
  uint32_t  ulGateway;        /*!< Gateway address

                                     The device's static gateway address. All valid IP addresses are allowed.
                                     Multicast and Broadcast addresses may not be set.
                                     Note that the gateway and the device have to be in the same subnet.
                                     The value remains unused, when DHCP or BOOTP is configured by means of \p ulTcpFlags.
                              */

  uint16_t  usVendId;         /*!< CIP Vendor ID

                                     This is an identification number for the manufacturer of an EtherNet/IP device.
                                     Vendor IDs are managed by ODVA. They are reflected by the CIP Identity object, attribute 1.
                                     The host application is responsible for setting a nonzero value as defined by the CIP specification.
                                     The protocol stack does not restrict the range of valid values.

                              */
  uint16_t  usProductType;    /*!< CIP Device Type

                                     The list of device types is managed by ODVA.
                                     It is used to identify the device profile that a particular product is using.
                                     Device profiles categorize the device for a specific purpose and define the minimum requirements
                                     a device has to implement for compliance with the profile.

                                     <table>
                                     <tr>
                                        <th colspan="2">Valid values</th>
                                     </tr>
                                     <tr><td>0x00 - 0x64</td><td>Publicly defined</td></tr>
                                     <tr><td>0x64 - 0xC7</td><td>Vendor specific</td></tr>
                                     <tr><td>0xC8 - 0xFF</td><td>Reserved by CIP</td></tr>
                                     <tr><td>0x100 - 0x2FF</td><td>Publicly defined</td></tr>
                                     <tr><td>0x300 - 0x4FF</td><td>Vendor specific</td></tr>
                                     <tr><td>0x500 - 0xFFFF</td><td>Reserved for CIP</td></tr>
                                     </table>

                                     Please refer to the CIP specification for further information. \cite cip_spec

                              */
  uint16_t  usProductCode;    /*!< Product code

                                     The vendor assigned Product Code identifies a
                                     particular product within a device type. Each vendor
                                     assigns this code to each of its products. The Product
                                     Code typically maps to one or more catalog/model
                                     numbers. Products shall have different codes if their
                                     configuration and/or runtime options are different. Such
                                     devices present a different logical view to the network.
                                     On the other hand, for example, two products that are
                                     the same except for their color or mounting feet are the
                                     same logically and may share the same product code.

                                     <table>
                                     <tr>
                                        <th colspan="2">Valid values</th>
                                     </tr>
                                     <tr><td>1...65535</td><td>Nonzero product code</td></tr>
                                     </table>

                              */

  uint32_t  ulSerialNumber;   /*!< Serial number

                                    Deprecated. This value has to be set to zero.
                                    The firmware will apply the serial number as stored in
                                    the Device Data Provider (DDP), which in turn fetches
                                    it from either the SecMem or FDL data sources.
                                    Refer to section 2.6.1 for details.

                                    Always set this value to zero.
                              */

  uint8_t   bMinorRev;        /*!< Minor revision

                                    <table>
                                    <tr>
                                       <th colspan="2">Valid values</th>
                                    </tr>
                                    <tr><td>1...255</td><td>Nonzero minor revision specifier</td></tr>
                                    </table>
                              */

  uint8_t   bMajorRev;        /*!< Major revision

                                    <table>
                                    <tr>
                                       <th colspan="2">Valid values</th>
                                    </tr>
                                    <tr><td>1...255</td><td>Valid major evision specifier</td></tr>
                                    </table>
                               */

  /*! Device name (Product Name)

      The name of the product or product family corresponding to the product code. A product code may have a variety of product names assigned.
      This name will be reflected by the CIP Identity object, attribute 7.\n
      The parameter is in CIP_SHORTSTRING format, with a single byte length specifier in the first byte, and bytes 2-31 containing the actual string.
  */
  uint8_t   abDeviceName[32];

  uint32_t  ulInputAssInstance;   /*!< Instance number of input assembly

                                     This parameter specifies the instance number of the input assembly to create.

                                     <table>
                                     <tr>
                                        <th colspan="2">Valid values</th>
                                     </tr>
                                     <tr><td>0x00000001 - 0x00000063</td><td>Open (static assemblies defined in device profile)</td></tr>
                                     <tr><td>0x00000064 - 0x000000C7</td><td>Vendor Specific static assemblies and dynamic assemblies</td></tr>
                                     <tr><td>0x000000C8 - 0x000002FF</td><td>Open (static assemblies defined in device profile)</td></tr>
                                     <tr><td>0x00000300 - 0x000004FF</td><td>Vendor Specific static assemblies and dynamic assemblies</td></tr>
                                     <tr><td>0x00000500 - 0x000FFFFF</td><td>Open (static assemblies defined in device profile)</td></tr>
                                     <tr><td>0x00100000 - 0xFFFFFFFF</td><td>Reserved by CIP for future use.</td></tr>
                                     </table>

                                     Please refer to the CIP specification for further information. \cite cip_spec

                                     */

  uint32_t  ulInputAssFlags;      /*!< Input assembly flags

                                       Please refer to section "Assembly Flags" (TODO)
                                     */


  uint32_t  ulOutputAssInstance;  /*!< Instance number of output assembly

                                     This parameter specifies the instance number of the output assembly to create.

                                     <table>
                                     <tr>
                                        <th colspan="2">Valid values</th>
                                     </tr>
                                     <tr><td>0x00000001 - 0x00000063</td><td>Open (static assemblies defined in device profile)</td></tr>
                                     <tr><td>0x00000064 - 0x000000C7</td><td>Vendor Specific static assemblies and dynamic assemblies</td></tr>
                                     <tr><td>0x000000C8 - 0x000002FF</td><td>Open (static assemblies defined in device profile)</td></tr>
                                     <tr><td>0x00000300 - 0x000004FF</td><td>Vendor Specific static assemblies and dynamic assemblies</td></tr>
                                     <tr><td>0x00000500 - 0x000FFFFF</td><td>Open (static assemblies defined in device profile)</td></tr>
                                     <tr><td>0x00100000 - 0xFFFFFFFF</td><td>Reserved by CIP for future use.</td></tr>
                                     </table>

                                     Please refer to the CIP specification for further information. \cite cip_spec

                                     */

  uint32_t   ulOutputAssFlags;     /*!< Output assembly flags

                                       Please refer to section "Assembly Flags" (TODO)
                                     */

  EIP_DPMINTF_QOS_CONFIG_T tQoS_Config; /*!< Quality of Service Configuration

                                           This parameter set configures the Quality of Service Object
                                           (CIP class ID 0x48).
                                           For more information about specific parameters refer to XXX (TODO).

                                         */

  uint32_t   ulNameServer;         /*!< Name Server 1

                                        Any valid IPv4 address may be set. Multicast addresses are rejected.
                                        Set to zero if no name server is to be used.
                                     */

  uint32_t   ulNameServer_2;       /*!< Name Server 2

                                        Any valid IPv4 address may be set. Multicast addresses are rejected.
                                        Set to zero if no name server is to be used.
                                      */
 /*! Domain Name

    This parameter corresponds to an entry of attribute 5 of the TCP Interface Object (CIP Class ID 0xF5).
    The parameter is in CIP_STRING format, with the 16-Bit length specifier in the first two bytes, and bytes 2-49 containing the actual string.
  */
  uint8_t    abDomainName[48 + 2];

  /*! Host Name

    This parameter corresponds to attribute 6 of the TCP Interface Object (CIP Class ID 0xF5).
    The parameter is in CIP_STRING format, with the 16-Bit length specifier in the first two bytes, and bytes 2-65 containing the actual string.
  */
  uint8_t    abHostName[64+2];

  uint8_t    bSelectAcd;           /*!< Select ACD

                                        This parameter corresponds to attribute 10 of the TCP Interface Object (CIP Class ID 0xF5).
                                        It selects whether or not the Address Conflict Detection mechanism shall be enabled or disabled.
                                        <table>
                                        <tr>
                                          <th colspan="2">Valid values</th>
                                        </tr>
                                        <tr><td>0</td><td>ACD disabled</td></tr>
                                        <tr><td>1</td><td>ACD enabled</td></tr>
                                        </table>

                                      */

  EIP_DPMINTF_TI_ACD_LAST_CONFLICT_T  tLastConflictDetected; /*!< Last detected conflict

                                                               This parameter corresponds to attribute 11 of the TCP Interface Object (CIP Class ID 0xF5).
                                                               Set to zero with system initialization.
                                                               \todo Check if this can be removed.
                                                             */

  uint8_t                           bQuickConnectFlags; /*!< Quick Connect flags

                                                               \description
                                                               This parameter enables/disables the Quick Connect functionality within the stack.
                                                               It affects TCP Interface Object (0xF5) attribute 12.

                                                               This functionality needs special hardware settings (e.g. fast flash)

                                                               <table>
                                                               <tr>
                                                                 <th colspan="2">Valid bits, any OR-combination allowed</th>
                                                               </tr>
                                                               <tr><td>#EIP_OBJECT_QC_FLAGS_ACTIVATE_ATTRIBUTE</td><td>Support QuickConnect. Activate the CIP QuickConnect attribute of the object.</td></tr>
                                                               <tr><td>#EIP_OBJECT_QC_FLAGS_ENABLE_QC</td><td>Enable QuickConnect. Only effective when set in conjunction with EIP_OBJECT_QC_FLAGS_ACTIVATE_ATTRIBUTE</td></tr>
                                                               </table>
                                                             */

  uint8_t                           abAdminState[2];       /*!< Admin State

                                                               This parameter corresponds to attribute 9 of the Ethernet Link Object (CIP Class ID 0xF6).
                                                               The Admin State attribute shall allow administrative
                                                               setting of the interface state. The interface (PHYs) can be
                                                               enabled or disabled.
                                                               Each array entry stands for one ethernet port.
                                                               This attribute shall be stored in non-volatile memory.

                                                               <table>
                                                                 <tr>
                                                                  <th colspan="2">Valid values</th>
                                                               </tr>
                                                               <tr><td>0x01</td><td>Interface enabled</td></tr>
                                                               <tr><td>0x02</td><td>Interface disabled</td></tr>
                                                               </table>
                                                             */

  uint8_t                           bTTLValue;             /*!< TTL Value

                                                               This parameter corresponds to attribute 8 of the TCP/IP Interface Object (CIP Class ID 0xF5).
                                                               The TTL value attribute is used as IP-Header Time-to-Live for UDP multicast (I/0) frames.

                                                               <table>
                                                                 <tr>
                                                                  <th colspan="2">Valid values</th>
                                                               </tr>
                                                               <tr><td>1...255</td><td>Valid TTL values</td></tr>
                                                               </table>
                                                             */
  EIP_DPMINTF_TI_MCAST_CONFIG_T       tMCastConfig;          /*!< Multicast Configuration

                                                               This parameter corresponds to attribute 9 of the TCP/IP Interface Object (CIP Class ID 0xF5).
                                                               The MCast Config sets the used multicast addresses arbitration mechanism and thus defines the
                                                               range for multicast addresses used for I/O communication.
                                                               Set to zero if in doubt.
                                                             */

  uint16_t                          usEncapInactivityTimer;/*!< Encapsulation Inactivity Timeout \n

                                                               This parameter corresponds to attribute 13 of the TCP/IP Interface Object (CIP Class ID 0xF5).
                                                               Any Encapsulation session without any activity for the specified timeout interval (in seconds) will be closed.

                                                              <table>
                                                                 <tr>
                                                                  <th colspan="2">Valid values</th>
                                                               </tr>
                                                               <tr><td>0</td><td>Timeout disabled</td></tr>
                                                               <tr><td>1 - 3600</td><td>Valid timoeut intervals in [s]</td></tr>
                                                               </table>
                                                             */

} EIP_APS_CONFIGURATION_PARAMETER_SET_V3_T;




#define EIP_APS_CONFIGURATION_PARAMETER_SET_V3_SIZE (sizeof(EIP_APS_CONFIGURATION_PARAMETER_SET_V3_T) )

/*#####################################################################################*/

/* Request Packet */

typedef __HIL_PACKED_PRE union __HIL_PACKED_POST EIP_APS_SET_CONFIGURATION_PARAMETERS_REQ_CONFIGSET_UNION_Ttag
{
  EIP_APS_CONFIGURATION_PARAMETER_SET_V3_T tV3;
} EIP_APS_SET_CONFIGURATION_PARAMETERS_REQ_CONFIGSET_UNION_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_APS_SET_CONFIGURATION_PARAMETERS_REQ_Ttag
{
  uint32_t  ulParameterVersion;  /*!< Version related to the following configuration union,
                                      only EIP_APS_CONFIGURATION_PARAMETER_SET_V3 is supported.
                                 */

  EIP_APS_SET_CONFIGURATION_PARAMETERS_REQ_CONFIGSET_UNION_T unConfig;

} EIP_APS_SET_CONFIGURATION_PARAMETERS_REQ_T;


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_APS_PACKET_SET_CONFIGURATION_PARAMETERS_REQ_Ttag
{
  HIL_PACKET_HEADER_T                        tHead;
  EIP_APS_SET_CONFIGURATION_PARAMETERS_REQ_T tData;

} EIP_APS_PACKET_SET_CONFIGURATION_PARAMETERS_REQ_T;

#define EIP_APS_SET_CONFIGURATION_PARAMETERS_REQ_SIZE   4 /* Basic size of request packet.
                                                             The size of the following parameter union needs to be added. */

/* Confirmation Packet */

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_APS_PACKET_SET_CONFIGURATION_PARAMETERS_CNF_Ttag
{
  HIL_PACKET_HEADER_T                        tHead;
} EIP_APS_PACKET_SET_CONFIGURATION_PARAMETERS_CNF_T;

#define EIP_APS_SET_CONFIGURATION_PARAMETERS_CNF_SIZE   0

/*#####################################################################################*/

/*#####################################################################################*/

#define PID_EIP_IP_CONFIGURATION  0x3000A001 /* EtherNet/IP Address Settings      */
#define PID_EIP_IP_CONFIGCONTROL  0x3000A002 /* EtherNet/IP Configuration Control */

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST PID_EIP_IP_CONFIGURATION_Ttag
{
  uint32_t ulIP;
  uint32_t ulNetmask;
  uint32_t ulGateway;
} PID_EIP_IP_CONFIGURATION_T;

#define PRM_CFGCTRL_STORED_CFG  0
#define PRM_CFGCTRL_DHCP        1
#define PRM_CFGCTRL_BOOTP       2
#define PRM_CFGCTRL_FIXIP       3

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST PID_EIP_IP_CONFIGCONTROL_Ttag
{
  uint32_t ulConfigurationControl;
} PID_EIP_IP_CONFIGCONTROL_T;

/*#####################################################################################*/
/* Interface to Encapsulation Layer                                                    */
/*#####################################################################################*/
#define EIP_ENCAP_DATA_PCKT_LEN 1520

/* ####################################################### */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_ENCAP_LISTIDENTITY_REQ_Ttag
{
  uint32_t  ulIPAddr;
  uint32_t  ulTimeout;
} EIP_ENCAP_LISTIDENTITY_REQ_T;

typedef  __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_ENCAP_PACKET_LISTIDENTITY_REQ_Ttag
{
  HIL_PACKET_HEADER_T  tHead;
  EIP_ENCAP_LISTIDENTITY_REQ_T tData;
} EIP_ENCAP_PACKET_LISTIDENTITY_REQ_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_ENCAP_PACKET_LISTIDENTITY_CNF_Ttag
{
  HIL_PACKET_HEADER_T  tHead;
} EIP_ENCAP_PACKET_LISTIDENTITY_CNF_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_ENCAP_LISTIDENTITY_IND_IDENTITYITEM_SOCKETADDRESS_Ttag
{
  int16_t  sin_family;
  uint16_t sin_port;
  uint32_t sin_addr;
  uint8_t  sin_zero[8];
} EIP_ENCAP_LISTIDENTITY_IND_IDENTITYITEM_SOCKETADDRESS_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_ENCAP_LISTIDENTITY_IND_IDENTITYITEM_Ttag
{
  uint16_t usEncapProtocolVersion;
  EIP_ENCAP_LISTIDENTITY_IND_IDENTITYITEM_SOCKETADDRESS_T tSocketAddress;

  uint16_t usVendorId;
  uint16_t usDeviceType;
  uint16_t usProductCode;
  uint8_t  abRevision[2];
  uint16_t usStatus;
  uint32_t ulSerialNumber;
  uint8_t  abProductName[1];                 /* CIP short string: one byte
                                                length specifier */
} EIP_ENCAP_LISTIDENTITY_IND_IDENTITYITEM_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_ENCAP_LISTIDENTITY_IND_Ttag
{
  uint16_t usItemCount;          /* CPF Format: Number of items in message */
  uint16_t usIdentityItemId;     /* CPF Format: The first item is always the identity
                                    item, code 0x0C */
  uint16_t usIdentityItemLength; /* CPF Format: Length in bytes (varies depending
                                    on product name string length */

  EIP_ENCAP_LISTIDENTITY_IND_IDENTITYITEM_T tIdentityItem;
  uint8_t abData[EIP_OBJECT_MAX_PACKET_LEN];  /* Remaining (variable size) data
                                                 (actual product name, state field,
                                                  and possibly further CPF items) */
} EIP_ENCAP_LISTIDENTITY_IND_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_ENCAP_PACKET_LISTIDENTITY_IND_Ttag
{
  HIL_PACKET_HEADER_T  tHead;
  EIP_ENCAP_LISTIDENTITY_IND_T tData;
} EIP_ENCAP_PACKET_LISTIDENTITY_IND_T;

/* ####################################################### */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_ENCAP_LISTSERVICES_REQ_Ttag
{
  uint32_t  ulIPAddr;
  uint32_t  ulTimeout;
} EIP_ENCAP_LISTSERVICES_REQ_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_ENCAP_PACKET_LISTSERVICES_REQ_Ttag
{
  HIL_PACKET_HEADER_T  tHead;
  EIP_ENCAP_LISTSERVICES_REQ_T tData;
} EIP_ENCAP_PACKET_LISTSERVICES_REQ_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_ENCAP_PACKET_LISTSERVICES_CNF_Ttag
{
  HIL_PACKET_HEADER_T  tHead;
} EIP_ENCAP_PACKET_LISTSERVICES_CNF_T;


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_ENCAP_LISTSERVICES_IND_SERVICESITEM_Ttag
{
  uint16_t usEncapProtocolVersion; /* shall be set to 1 */
  uint16_t usCapabilityFlags;      /* expect bit 5 (EIP encap), bit 8 (class 0/1) */
  uint8_t  abNameOfService[16];    /* expect NUL-term. ASCII: "Communications" */
} EIP_ENCAP_LISTSERVICES_IND_SERVICESITEM_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_ENCAP_LISTSERVICES_IND_Ttag
{
  uint16_t usItemCount;          /* CPF Format: Number of items in message */
  uint16_t usServicesItemId;     /* CPF Format: The first item is the service
                                    item, code 0x0100 */
  uint16_t usServicesItemLength; /* CPF Format: Length in bytes (varies depending
                                    on product name string length */
  EIP_ENCAP_LISTSERVICES_IND_SERVICESITEM_T tServicesItem;
  uint8_t abData[EIP_OBJECT_MAX_PACKET_LEN];  /* Remaining (variable size) data
                                                 (for, possibly, further CPF items) */
} EIP_ENCAP_LISTSERVICES_IND_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_ENCAP_PACKET_LISTSERVICES_IND_Ttag
{
  HIL_PACKET_HEADER_T  tHead;
  EIP_ENCAP_LISTSERVICES_IND_T tData;
} EIP_ENCAP_PACKET_LISTSERVICES_IND_T;

/* ####################################################### */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_ENCAP_LISTINTERFACES_REQ_Ttag
{
  uint32_t  ulIPAddr;
  uint32_t  ulTimeout;
} EIP_ENCAP_LISTINTERFACES_REQ_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_ENCAP_PACKET_LISTINTERFACES_REQ_Ttag
{
  HIL_PACKET_HEADER_T  tHead;
  EIP_ENCAP_LISTINTERFACES_REQ_T tData;
} EIP_ENCAP_PACKET_LISTINTERFACES_REQ_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_ENCAP_PACKET_LISTINTERFACES_CNF_Ttag
{
  HIL_PACKET_HEADER_T  tHead;
} EIP_ENCAP_PACKET_LISTINTERFACES_CNF_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_ENCAP_LISTINTERFACES_IND_Ttag
{
  uint16_t usItemCount;                     /* CPF Format: Number of items in message      */
  uint8_t  abData[EIP_ENCAP_DATA_PCKT_LEN]; /* As of Jan/2019, no public items are defined */
} EIP_ENCAP_LISTINTERFACES_IND_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_ENCAP_PACKET_LISTINTERFACES_IND_Ttag
{
  HIL_PACKET_HEADER_T  tHead;
  EIP_ENCAP_LISTINTERFACES_IND_T tData;
} EIP_ENCAP_PACKET_LISTINTERFACES_IND_T;

/*#####################################################################################*/

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_UNCONNECT_MESSAGE_REQ_Ttag
{
  uint32_t ulIPAddr;                  /*!< Destination IP address */
  uint8_t  bService;                  /*!< CIP service code       */
  uint8_t  abPad0[3];                 /*!< Padding, set to zero   */
  uint32_t ulClass;                   /*!< CIP class ID           */
  uint32_t ulInstance;                /*!< CIP Instance           */
  uint32_t ulAttribute;               /*!< CIP Attribute          */
  uint32_t ulMember;                  /*!< CIP member             */

  uint32_t ulTimeout;                 /*!< Timeout value in [ms]. Note that this timeout is only applicable after the peer has successfully received the UCMM request.
                                           For the possible steps of TCP-connecting the peer and establishing an encapsulation session with it, hardcoded timeouts apply (TCP connect ~20s, Encapsulation session timeouts according to CIP object attribute) */

  uint8_t  abData[EIP_OBJECT_MAX_PACKET_LEN]; /*!< Service Data   */
} EIP_OBJECT_UNCONNECT_MESSAGE_REQ_T;


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_UNCONNECT_MESSAGE_REQ_Ttag
{
  HIL_PACKET_HEADER_T     tHead;
  EIP_OBJECT_UNCONNECT_MESSAGE_REQ_T  tData;
} EIP_OBJECT_PACKET_UNCONNECT_MESSAGE_REQ_T;


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_UNCONNECT_MESSAGE_CNF_RESPONSEDATA_Ttag
{
  uint8_t bGrc;                              /*!< CIP General status information */
  uint8_t bExtStatusSize;                    /*!< Number of 16 bit words of CIP Extended
                                                  status information */
  uint8_t abData[EIP_OBJECT_MAX_PACKET_LEN]; /*!< CIP Extended Status information, if present,
                                                  followed by the actual response data */
} EIP_OBJECT_UNCONNECT_MESSAGE_CNF_RESPONSEDATA_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_UNCONNECT_MESSAGE_CNF_Ttag
{
  uint32_t ulIPAddr;                  /*!< Destination IP address */
  uint8_t  bService;                  /*!< CIP service code       */
  uint8_t  abPad0[3];                 /*!< Padding, set to zero   */
  uint32_t ulClass;                   /*!< CIP class ID           */
  uint32_t ulInstance;                /*!< CIP Instance           */
  uint32_t ulAttribute;               /*!< CIP Attribute          */
  uint32_t ulMember;                  /*!< CIP member             */

  EIP_OBJECT_UNCONNECT_MESSAGE_CNF_RESPONSEDATA_T tResponse;

} EIP_OBJECT_UNCONNECT_MESSAGE_CNF_T;


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_UNCONNECT_MESSAGE_CNF_Ttag
{
  HIL_PACKET_HEADER_T                 tHead;
  EIP_OBJECT_UNCONNECT_MESSAGE_CNF_T  tData;
}EIP_OBJECT_PACKET_UNCONNECT_MESSAGE_CNF_T;


#define EIP_OBJECT_UNCONNECT_MESSAGE_REQ_SIZE (sizeof(EIP_OBJECT_UNCONNECT_MESSAGE_REQ_T) - EIP_OBJECT_MAX_PACKET_LEN)
#define EIP_OBJECT_UNCONNECT_MESSAGE_CNF_SIZE (sizeof(EIP_OBJECT_UNCONNECT_MESSAGE_CNF_T) - EIP_OBJECT_MAX_PACKET_LEN)


/*#####################################################################################*/

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_OPEN_CL3_REQ_Ttag
{
  uint32_t ulIPAddr;          /*!< Destination IP address                                       */
  uint32_t ulRpi;             /*!< Request packet interval [us], valid range [1000 .. 32767000] */
  uint32_t ulTimeoutMult;     /*!< Timeout Multiplier, valid range [0..7], see spec.            */
} EIP_OBJECT_OPEN_CL3_REQ_T;


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_OPEN_CL3_REQ_Ttag
{
  HIL_PACKET_HEADER_T         tHead;
  EIP_OBJECT_OPEN_CL3_REQ_T   tData;
} EIP_OBJECT_PACKET_OPEN_CL3_REQ_T;


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_OPEN_CL3_CNF_Ttag
{
  uint32_t    ulConnection;          /*!< Connection Handle    */
  uint32_t    ulGRC;                 /*!< Generic Error Code   */
  uint32_t    ulERC;                 /*!< Extended Error Code  */
} EIP_OBJECT_OPEN_CL3_CNF_T;


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_OPEN_CL3_CNF_Ttag
{
  HIL_PACKET_HEADER_T         tHead;
  EIP_OBJECT_OPEN_CL3_CNF_T   tData;
} EIP_OBJECT_PACKET_OPEN_CL3_CNF_T;

#define EIP_OBJECT_OPEN_CL3_REQ_SIZE (sizeof(EIP_OBJECT_OPEN_CL3_REQ_T))
#define EIP_OBJECT_OPEN_CL3_CNF_SIZE (sizeof(EIP_OBJECT_OPEN_CL3_CNF_T))

/*#####################################################################################*/

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_CONNECT_MESSAGE_REQ_Ttag
{
  uint32_t ulConnection;              /*!< Connection Handle      */
  uint8_t  bService;                  /*!< CIP service code       */
  uint8_t  abPad0[3];                 /*!< Padding, set to zero   */
  uint32_t ulClass;                   /*!< CIP class ID           */
  uint32_t ulInstance;                /*!< CIP Instance           */
  uint32_t ulAttribute;               /*!< CIP Attribute          */
  uint32_t ulMember;                  /*!< CIP member             */

  uint8_t  abData[EIP_OBJECT_MAX_PACKET_LEN]; /*!< Service Data   */
} EIP_OBJECT_CONNECT_MESSAGE_REQ_T;


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_CONNECT_MESSAGE_REQ_Ttag
{
  HIL_PACKET_HEADER_T     tHead;
  EIP_OBJECT_CONNECT_MESSAGE_REQ_T  tData;
} EIP_OBJECT_PACKET_CONNECT_MESSAGE_REQ_T;


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_CONNECT_MESSAGE_CNF_RESPONSEDATA_Ttag
{
  uint8_t bGrc;                              /*!< CIP General status information */
  uint8_t bExtStatusSize;                    /*!< Number of 16 bit words of CIP Extended
                                                  status information */
  uint8_t abData[EIP_OBJECT_MAX_PACKET_LEN]; /*!< CIP Extended Status information, if present,
                                                  followed by the actual response data */
} EIP_OBJECT_CONNECT_MESSAGE_CNF_RESPONSEDATA_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_CONNECT_MESSAGE_CNF_Ttag
{
  uint32_t ulConnection;              /*!< Connection Handle      */
  uint8_t  bService;                  /*!< CIP service code       */
  uint8_t  abPad0[3];                 /*!< Padding, set to zero   */
  uint32_t ulClass;                   /*!< CIP class ID           */
  uint32_t ulInstance;                /*!< CIP Instance           */
  uint32_t ulAttribute;               /*!< CIP Attribute          */
  uint32_t ulMember;                  /*!< CIP Member             */

  EIP_OBJECT_CONNECT_MESSAGE_CNF_RESPONSEDATA_T tResponse;

} EIP_OBJECT_CONNECT_MESSAGE_CNF_T;


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_CONNECT_MESSAGE_CNF_Ttag
{
  HIL_PACKET_HEADER_T     tHead;
  EIP_OBJECT_CONNECT_MESSAGE_CNF_T  tData;
} EIP_OBJECT_PACKET_CONNECT_MESSAGE_CNF_T;


#define EIP_OBJECT_CONNECT_MESSAGE_REQ_SIZE (sizeof(EIP_OBJECT_CONNECT_MESSAGE_REQ_T) - EIP_OBJECT_MAX_PACKET_LEN)
#define EIP_OBJECT_CONNECT_MESSAGE_CNF_SIZE (sizeof(EIP_OBJECT_CONNECT_MESSAGE_CNF_T) - EIP_OBJECT_MAX_PACKET_LEN)

/*#####################################################################################*/

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_CLOSE_CL3_REQ_Ttag
{
  uint32_t    ulConnection;          /*!< Connection Handle    */
} EIP_OBJECT_CLOSE_CL3_REQ_T;


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_CLOSE_CL3_REQ_Ttag
{
  HIL_PACKET_HEADER_T         tHead;
  EIP_OBJECT_CLOSE_CL3_REQ_T   tData;
} EIP_OBJECT_PACKET_CLOSE_CL3_REQ_T;


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_CLOSE_CL3_CNF_Ttag
{
  uint32_t    ulGRC;                 /*!< Generic Error Code   */
  uint32_t    ulERC;                 /*!< Extended Error Code  */
} EIP_OBJECT_CLOSE_CL3_CNF_T;


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_CLOSE_CL3_CNF_Ttag
{
  HIL_PACKET_HEADER_T         tHead;
  EIP_OBJECT_CLOSE_CL3_CNF_T   tData;
} EIP_OBJECT_PACKET_CLOSE_CL3_CNF_T;

#define EIP_OBJECT_CLOSE_CL3_REQ_SIZE (sizeof(EIP_OBJECT_CLOSE_CL3_REQ_T))
#define EIP_OBJECT_CLOSE_CL3_CNF_SIZE (sizeof(EIP_OBJECT_CLOSE_CL3_CNF_T))

/*#####################################################################################*/


typedef  __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_CC_SLAVE_ACTIVATE_REQ_Ttag
{
  uint32_t ulSlaveHandle;           /*!< Handle of the connection to the slave \n
                                      \valueRange \n\n
                                      \description \n\n
                                       none \n\n */

  uint32_t ulActivate;              /*!< Action to the slave \n
                                      \valueRange \n\n
                                       0x01 : activate slave at scanlist             \n
                                       0x02 : deactivate slave from scanlist         \n
                                      \description \n\n
                                       none \n\n */

  uint32_t ulDelayTime;             /*!< Delay to execute start the communication \n

                                      \valueRange \n\n
                                       0x0000 - 0x7FFF : time at ms \n
                                       0xFFFFFFFF : default time from configuration \n

                                      \description \n\n
                                       none \n\n */

} EIP_OBJECT_CC_SLAVE_ACTIVATE_REQ_T;

/* command for register a new object to the message router */
typedef  __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_CC_PACKET_SLAVE_ACTIVATE_REQ_Ttag
{
  HIL_PACKET_HEADER_T                 tHead;
  EIP_OBJECT_CC_SLAVE_ACTIVATE_REQ_T  tData;
} EIP_OBJECT_CC_PACKET_SLAVE_ACTIVATE_REQ_T;

#define EIP_OBJECT_CC_SLAVE_ACTIVATE_REQ_SIZE (sizeof(EIP_OBJECT_CC_SLAVE_ACTIVATE_REQ_T))

typedef  __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_CC_SLAVE_ACTIVATE_CNF_Ttag
{
  uint32_t ulSlaveHandle;           /*!< Handle of the connection to the slave \n

                                      \valueRange \n\n

                                      \description \n\n
                                       none \n\n */

  uint32_t ulActivate;              /*!< Action to the slave \n

                                      \valueRange \n\n
                                       0x01 : activate slave at scanlist             \n
                                       0x02 : deactivate slave from scanlist         \n

                                      \description \n\n
                                       none \n\n */

} EIP_OBJECT_CC_SLAVE_ACTIVATE_CNF_T;

/* command for register a new object to the message router */
typedef  __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_CC_PACKET_SLAVE_ACTIVATE_CNF_Ttag
{
  HIL_PACKET_HEADER_T                 tHead;
  EIP_OBJECT_CC_SLAVE_ACTIVATE_CNF_T  tData;
} EIP_OBJECT_CC_PACKET_SLAVE_ACTIVATE_CNF_T;

#define EIP_OBJECT_CC_SLAVE_ACTIVATE_CNF_SIZE (sizeof(EIP_OBJECT_CC_SLAVE_ACTIVATE_CNF_T))

typedef  __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_CREATE_CC_INSTANCE_ENDPOINT_CONFIG_EXPLICIT_ENDPOINTS_Ttag
{
  uint32_t ulOtConnectionPoint;          /* use this explicit (pre-registered) producing assembly as connection endpoint */
  uint32_t ulToConnectionPoint;          /* use this explicit (pre-registered) consuming assembly as connection endpoint */
} EIP_OBJECT_CREATE_CC_INSTANCE_ENDPOINT_CONFIG_EXPLICIT_ENDPOINTS_T;

typedef  __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_CREATE_CC_INSTANCE_ENDPOINT_CONFIG_IMPLICIT_ENDPOINTS_EXPLICIT_OFFSETS_Ttag
{
  uint16_t usOutputDpmOffset;            /* create endpoint implicitly, but use this offset in the DPM output area */
  uint16_t usInputDpmOffset;             /* create endpoint implicitly, but use this offset in the DPM input area */
  uint16_t usAdditionalAssemblyFlagsOT;  /* attribute 0x300 of the CC instance: Additional (user-specified) assembly flags for the producing assembly (O2T) */
  uint16_t usAdditionalAssemblyFlagsTO;  /* attribute 0x300 of the CC instance: Additional (user-specified) assembly flags for the consuming assembly (T2O) */
} EIP_OBJECT_CREATE_CC_INSTANCE_ENDPOINT_CONFIG_IMPLICIT_ENDPOINTS_EXPLICIT_OFFSETS_T;

typedef  __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_CREATE_CC_INSTANCE_ENDPOINT_CONFIG_IMPLICIT_ENDPOINTS_Ttag
{
  uint16_t usAdditionalAssemblyFlagsOT;  /* attribute 0x300 of the CC instance: Additional (user-specified) assembly flags for the producing assembly (O2T) */
  uint16_t usAdditionalAssemblyFlagsTO;  /* attribute 0x300 of the CC instance: Additional (user-specified) assembly flags for the consuming assembly (T2O) */
} EIP_OBJECT_CREATE_CC_INSTANCE_ENDPOINT_CONFIG_IMPLICIT_ENDPOINTS_T;

typedef  __HIL_PACKED_PRE union __HIL_PACKED_POST EIP_OBJECT_CREATE_CC_INSTANCE_ENDPOINT_CONFIG_UNION_Ttag
{

  /* will be used if bEndPointSelect == 0 */
  EIP_OBJECT_CREATE_CC_INSTANCE_ENDPOINT_CONFIG_EXPLICIT_ENDPOINTS_T tExplicitEndpoints;
  /* will be used if bEndPointSelect == 1 */
  EIP_OBJECT_CREATE_CC_INSTANCE_ENDPOINT_CONFIG_IMPLICIT_ENDPOINTS_EXPLICIT_OFFSETS_T tImplicitEndpointsExplicitOffsets;
  /* will be used if bEndPointSelect == 2 */
  EIP_OBJECT_CREATE_CC_INSTANCE_ENDPOINT_CONFIG_IMPLICIT_ENDPOINTS_T tImplicitEndpoints;

} EIP_OBJECT_CREATE_CC_INSTANCE_ENDPOINT_CONFIG_UNION_T;

typedef  __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_CREATE_CC_INSTANCE_ENDPOINT_CONFIG_Ttag
{
  uint8_t bEndPointSelect;                   /* 0 - explicit endpoints (pre-registered assemblies)
                                                1 - implicit endpoints with explicit DPM offsets (assemblies will be registered internally, offsets are explicitly chosen)
                                                2 - implicit endpoints with implicit DPM offsets (assemblies will be registered internally, offsets will be arbitrarily chosen)
                                             */
  EIP_OBJECT_CREATE_CC_INSTANCE_ENDPOINT_CONFIG_UNION_T uConnectionEndPoints;

} EIP_OBJECT_CREATE_CC_INSTANCE_ENDPOINT_CONFIG_T;



typedef  __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_CREATE_CC_INSTANCE_REQ_FWOPEN_CONNPARAMS_Ttag
{
  uint8_t bConnectionTimeoutMultiplier;  /* Connection timeout multiplier. Refer to the specification. Only applicable when parameters \p ulRpiOT, \p ulRpiTO, \p usNetOT and \p usNetOT are different from zero. */
  uint8_t bTransportClassAndTrigger;     /* Transport class and trigger type. Refer to the specification. Only applicable when parameters \p ulRpiOT, \p ulRpiTO, \p usNetOT and \p usNetOT are different from zero. */
  uint32_t ulRpiOT;                      /* Requested O->T packet interval in microseconds. Only applicable when parameters \p ulRpiOT, \p ulRpiTO, \p usNetOT and \p usNetOT are different from zero. */
  uint32_t ulRpiTO;                      /* Requested T->O packet interval in microseconds. Only applicable when parameters \p ulRpiOT, \p ulRpiTO, \p usNetOT and \p usNetOT are different from zero. */
  uint16_t usNetOT;                      /* O->T Network connection paramters. Only applicable when parameters \p ulRpiOT, \p ulRpiTO, \p usNetOT and \p usNetOT are different from zero. */
  uint16_t usNetTO;                      /* T->O Network connection paramters. Only applicable when parameters \p ulRpiOT, \p ulRpiTO, \p usNetOT and \p usNetOT are different from zero. */
} EIP_OBJECT_CREATE_CC_INSTANCE_REQ_FWOPEN_CONNPARAMS_T;

typedef  __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_CREATE_CC_INSTANCE_REQ_LFWOPEN_CONNPARAMS_Ttag
{
  uint8_t bConnectionTimeoutMultiplier;  /* Connection timeout multiplier. Refer to the specification. Only applicable when parameters \p ulRpiOT, \p ulRpiTO, \p usNetOT and \p usNetOT are different from zero. */
  uint8_t bTransportClassAndTrigger;     /* Transport class and trigger type. Refer to the specification. Only applicable when parameters \p ulRpiOT, \p ulRpiTO, \p usNetOT and \p usNetOT are different from zero. */
  uint32_t ulRpiOT;                      /* Requested O->T packet interval in microseconds. Only applicable when parameters \p ulRpiOT, \p ulRpiTO, \p usNetOT and \p usNetOT are different from zero. */
  uint32_t ulRpiTO;                      /* Requested T->O packet interval in microseconds. Only applicable when parameters \p ulRpiOT, \p ulRpiTO, \p usNetOT and \p usNetOT are different from zero. */
  uint32_t ulNetOT;                      /* O->T large Network connection paramters. Only applicable when parameters \p ulRpiOT, \p ulRpiTO, \p ulNetOT and \p ulNetOT are different from zero. */
  uint32_t ulNetTO;                      /* T->O large Network connection paramters. Only applicable when parameters \p ulRpiOT, \p ulRpiTO, \p ulNetOT and \p ulNetOT are different from zero. */
} EIP_OBJECT_CREATE_CC_INSTANCE_REQ_LFWOPEN_CONNPARAMS_T;

typedef  __HIL_PACKED_PRE union __HIL_PACKED_POST EIP_OBJECT_CREATE_CC_INSTANCE_REQ_CONNPARAM_UNION_Ttag
{
  EIP_OBJECT_CREATE_CC_INSTANCE_REQ_FWOPEN_CONNPARAMS_T tFwOpen;        /* Attribute 5 of the CC object: FwOpen connection parameters */
  EIP_OBJECT_CREATE_CC_INSTANCE_REQ_LFWOPEN_CONNPARAMS_T tLargeFwOpen;  /* Attribute 19 of the CC object: large FwOpen connection parameters */
} EIP_OBJECT_CREATE_CC_INSTANCE_REQ_CONNPARAM_UNION_T;


typedef  __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_CREATE_CC_INSTANCE_REQ_Ttag
{
  uint16_t usConnFlags;                      /* Connection flags. Attribute 2 of the ConnectionConfiguration Object. Specifies real time transfer formats. Refer to the specification. */
  uint32_t ulTargetIpAddress;                /* Connection target IP address. This will be ASCII-encoded into the connection path in terms of a port segment. */

  uint8_t bConnParamSelect;                  /* Attribute 18 of the CC object: Select either the FwOpen connection parameters or large FwOpen connection parameters to be used. Also selects the type of FwOpen to use */
  EIP_OBJECT_CREATE_CC_INSTANCE_REQ_CONNPARAM_UNION_T uConnParams;

  uint16_t usConnPathSize;                   /* in bytes */
  uint16_t usProxyConfigDataSize;            /* in bytes */
  uint16_t usTargetConfigDataSize;           /* in bytes */

  EIP_OBJECT_CREATE_CC_INSTANCE_ENDPOINT_CONFIG_T tEndPointConfig;    /* defines the endpoints of the connection */

  uint32_t ulReserved1;                      /* reserved, set to zero */
  uint32_t ulReserved2;                      /* reserved, set to zero */

  uint8_t abData[EIP_OBJECT_MAX_PACKET_LEN]; /* The actual connection path (without port segment), the proxy config data and the target config data as concatenated byte array */

} EIP_OBJECT_CREATE_CC_INSTANCE_REQ_T;

/* command for register a new object to the message router */
typedef  __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_CREATE_CC_INSTANCE_REQ_Ttag
{
  HIL_PACKET_HEADER_T                  tHead;
  EIP_OBJECT_CREATE_CC_INSTANCE_REQ_T  tData;
} EIP_OBJECT_PACKET_CREATE_CC_INSTANCE_REQ_T;

#define EIP_OBJECT_CREATE_CC_INSTANCE_REQ_SIZE (sizeof(EIP_OBJECT_CREATE_CC_INSTANCE_REQ_T) - EIP_OBJECT_MAX_PACKET_LEN)


typedef  __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_CREATE_CC_INSTANCE_CNF_Ttag
{
  uint32_t ulGrc;
  uint32_t ulInstanceId;
} EIP_OBJECT_CREATE_CC_INSTANCE_CNF_T;

typedef  __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_CREATE_CC_INSTANCE_CNF_Ttag
{
  HIL_PACKET_HEADER_T                  tHead;
  EIP_OBJECT_CREATE_CC_INSTANCE_CNF_T  tData;
} EIP_OBJECT_PACKET_CREATE_CC_INSTANCE_CNF_T;

#define EIP_OBJECT_CREATE_CC_INSTANCE_CNF_SIZE sizeof(EIP_OBJECT_CREATE_CC_INSTANCE_CNF_T)


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_ENABLE_ATTRIBUTE_REQ_Ttag
{
  uint32_t    ulClass;                            /*!< CIP class ID              */
  uint32_t    ulInstance;                         /*!< CIP instance number       */
  uint32_t    ulAttribute;                        /*!< CIP attribute number      */

} EIP_OBJECT_ENABLE_ATTRIBUTE_REQ_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_ENABLE_ATTRIBUTE_REQ_Ttag
{
  HIL_PACKET_HEADER_T               tHead;
  EIP_OBJECT_ENABLE_ATTRIBUTE_REQ_T tData;
} EIP_OBJECT_PACKET_ENABLE_ATTRIBUTE_REQ_T;

#define EIP_OBJECT_ENABLE_ATTRIBUTE_REQ_SIZE     (sizeof(EIP_OBJECT_ENABLE_ATTRIBUTE_REQ_T))


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_ENABLE_ATTRIBUTE_CNF_Ttag
{
  uint8_t                      bGrc;
} EIP_OBJECT_ENABLE_ATTRIBUTE_CNF_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_ENABLE_ATTRIBUTE_CNF_Ttag
{
  HIL_PACKET_HEADER_T               tHead;
  EIP_OBJECT_ENABLE_ATTRIBUTE_CNF_T tData;
} EIP_OBJECT_PACKET_ENABLE_ATTRIBUTE_CNF_T;

#define EIP_OBJECT_ENABLE_ATTRIBUTE_CNF_SIZE     (sizeof(EIP_OBJECT_ENABLE_ATTRIBUTE_CNF_T))

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_SET_ATTRIBUTE_PERMISSION_REQ_Ttag
{
  uint32_t    ulClass;                            /*!< CIP class ID              */
  uint32_t    ulInstance;                         /*!< CIP instance number       */
  uint32_t    ulAttribute;                        /*!< CIP attribute number      */

  uint8_t        fAllowBusSetAccess;
  uint8_t        fAllowBusGetAccess;
} EIP_OBJECT_SET_ATTRIBUTE_PERMISSION_REQ_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_SET_ATTRIBUTE_PERMISSION_Ttag
{
  HIL_PACKET_HEADER_T               tHead;
  EIP_OBJECT_SET_ATTRIBUTE_PERMISSION_REQ_T tData;
} EIP_OBJECT_PACKET_SET_ATTRIBUTE_PERMISSION_REQ_T;

#define EIP_OBJECT_SET_ATTRIBUTE_PERMISSION_REQ_SIZE     (sizeof(EIP_OBJECT_SET_ATTRIBUTE_PERMISSION_REQ_T))


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_SET_ATTRIBUTE_PERMISSION_CNF_Ttag
{
  uint8_t                      bGrc;
} EIP_OBJECT_SET_ATTRIBUTE_PERMISSION_CNF_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_SET_ATTRIBUTE_PERMISSION_CNF_Ttag
{
  HIL_PACKET_HEADER_T                       tHead;
  EIP_OBJECT_SET_ATTRIBUTE_PERMISSION_CNF_T tData;
} EIP_OBJECT_PACKET_SET_ATTRIBUTE_PERMISSION_CNF_T;

#define EIP_OBJECT_SET_ATTRIBUTE_PERMISSION_CNF_SIZE     (sizeof(EIP_OBJECT_ENABLE_ATTRIBUTE_CNF_T))


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_ENABLE_ATTRIBUTE_NOTIFICATION_REQ_Ttag
{
  uint32_t    ulClass;                            /*!< CIP class ID              */
  uint32_t    ulInstance;                         /*!< CIP instance number       */
  uint32_t    ulAttribute;                        /*!< CIP attribute number      */

} EIP_OBJECT_ENABLE_ATTRIBUTE_NOTIFICATION_REQ_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_ENABLE_ATTRIBUTE_NOTIFICATION_REQ_Ttag
{
  HIL_PACKET_HEADER_T               tHead;
  EIP_OBJECT_ENABLE_ATTRIBUTE_NOTIFICATION_REQ_T tData;
} EIP_OBJECT_PACKET_ENABLE_ATTRIBUTE_NOTIFICATION_REQ_T;

#define EIP_OBJECT_ENABLE_ATTRIBUTE_NOTIFICATION_REQ_SIZE     (sizeof(EIP_OBJECT_ENABLE_ATTRIBUTE_NOTIFICATION_REQ_T))


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_ENABLE_ATTRIBUTE_NOTIFICATION_CNF_Ttag
{
  uint8_t                      bGrc;
} EIP_OBJECT_ENABLE_ATTRIBUTE_NOTIFICATION_CNF_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_ENABLE_ATTRIBUTE_NOTIFICATION_CNF_Ttag
{
  HIL_PACKET_HEADER_T               tHead;
  EIP_OBJECT_ENABLE_ATTRIBUTE_NOTIFICATION_CNF_T tData;
} EIP_OBJECT_PACKET_ENABLE_ATTRIBUTE_NOTIFICATION_CNF_T;

#define EIP_OBJECT_ENABLE_ATTRIBUTE_NOTIFICATION_CNF_SIZE     (sizeof(EIP_OBJECT_ENABLE_ATTRIBUTE_NOTIFICATION_CNF_T))



typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_ENABLE_DISABLE_ATTRIBUTE_PROTECTION_REQ_Ttag
{
  uint32_t    ulClass;                            /*!< CIP class ID              */
  uint32_t    ulInstance;                         /*!< CIP instance number       */
  uint32_t    ulAttribute;                        /*!< CIP attribute number      */

  uint8_t     fProtected;                         /*!< Set unprotected (== 0)
                                                        or protected   (!= 0)    */

} EIP_OBJECT_ENABLE_DISABLE_ATTRIBUTE_PROTECTION_REQ_T;


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_ENABLE_DISABLE_ATTRIBUTE_PROTECTION_REQ_Ttag
{
  HIL_PACKET_HEADER_T               tHead;
  EIP_OBJECT_ENABLE_DISABLE_ATTRIBUTE_PROTECTION_REQ_T tData;
} EIP_OBJECT_PACKET_ENABLE_DISABLE_ATTRIBUTE_PROTECTION_REQ_T;

#define EIP_OBJECT_ENABLE_DISABLE_ATTRIBUTE_PROTECTION_REQ_SIZE     (sizeof(EIP_OBJECT_ENABLE_DISABLE_ATTRIBUTE_PROTECTION_REQ_T))


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_ENABLE_DISABLE_ATTRIBUTE_PROTECTION_CNF_Ttag
{
  uint8_t                      bGrc;
} EIP_OBJECT_ENABLE_DISABLE_ATTRIBUTE_PROTECTION_CNF_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_OBJECT_PACKET_ENABLE_DISABLE_ATTRIBUTE_PROTECTION_CNF_Ttag
{
  HIL_PACKET_HEADER_T               tHead;
  EIP_OBJECT_ENABLE_DISABLE_ATTRIBUTE_PROTECTION_CNF_T tData;
} EIP_OBJECT_PACKET_ENABLE_DISABLE_ATTRIBUTE_PROTECTION_CNF_T;

#define EIP_OBJECT_ENABLE_DISABLE_ATTRIBUTE_PROTECTION_CNF_SIZE     (sizeof(EIP_OBJECT_ENABLE_DISABLE_ATTRIBUTE_PROTECTION_CNF_T))



/* pragma unpack */
#ifdef __HIL_PRAGMA_PACK_ENABLE
#pragma __HIL_PRAGMA_UNPACK_1(EIP_DPMINTF_PUBLIC)
#endif

#endif
