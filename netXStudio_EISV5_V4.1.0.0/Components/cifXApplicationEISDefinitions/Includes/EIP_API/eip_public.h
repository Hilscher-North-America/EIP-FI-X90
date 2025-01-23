/**************************************************************************************
Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
***************************************************************************************
$Id: eip_public.h 111533 2023-01-24 22:58:10Z MBommert $:

Description:
  EtherNet/IP - Public definitions
**************************************************************************************/

#ifndef EIP_PUBLIC_H
#define EIP_PUBLIC_H

#include <stdint.h>
#include <stdbool.h>

/*#####################################################################################*/
  /* extended error codes for the connection manager */
  #define EIP_ESR_ALREADY_USED                             0x100  /* Connection already in use     */
  #define EIP_ESR_BAD_TRANSPORT                            0x103  /* Transport type not supported  */
  #define EIP_ESR_OWNER_CONFLICT                           0x106  /* More than one guy configuring */
  #define EIP_ESR_BAD_CONNECTION                           0x107  /* Trying to close inactive conn */
  #define EIP_ESR_BAD_CONN_TYPE                            0x108  /* Unsupported connection type   */
  #define EIP_ESR_BAD_CONN_SIZE                            0x109  /* Connection size mismatch      */

  #define EIP_ESR_CONN_UNCONFIGURED                        0x110  /* Connection unconfigured       */
  #define EIP_ESR_BAD_RPI                                  0x111  /* Unsupportable RPI             */
  #define EIP_ESR_NO_CM_RESOURCES                          0x113  /* Conn Mgr out of connections   */
  #define EIP_ESR_BAD_VENDOR_PRODUCT                       0x114  /* Mismatch in electronic key    */
  #define EIP_ESR_BAD_DEVICE_TYPE                          0x115  /* Mismatch in electronic key    */
  #define EIP_ESR_BAD_REVISION                             0x116  /* Mismatch in electronic key    */
  #define EIP_ESR_BAD_CONN_POINT                           0x117  /* Nonexistant instance number   */
  #define EIP_ESR_BAD_CONFIGURATION                        0x118  /* Bad config instance number    */
  #define EIP_ESR_CONN_REQ_FAILS                           0x119  /* No controlling connection opn */
  #define EIP_ESR_NO_APP_RESOURCES                         0x11A  /* App out of connections        */

  #define EIP_ESR_BAD_TRANSPORT_CLASS                      0x11C  /* The transport class requested in the
                                                                     Transport Type/Trigger parameter is
                                                                     not supported. */

  #define EIP_ESR_BAD_PRODUCTION_TRIGGER                   0x11D  /* The production trigger requested
                                                                     in the Transport Type/Trigger parameter
                                                                     is not supported. */

  #define EIP_ESR_BAD_DIRECTION                            0x11E  /* The direction requested in the Transport
                                                                     Type/Trigger parameter is not supported. */

  #define EIP_ESR_BAD_O2T_NETWORK_CONN_FIXVAR              0x11F  /* This extended status code shall be returned
                                                                     as the result of specifying an O2T
                                                                     fixed / variable flag that is not supported. */

  #define EIP_ESR_BAD_T2O_NETWORK_CONN_FIXVAR              0x120  /* This extended status code shall be returned
                                                                     as the result of specifying a T2O
                                                                     fixed / variable flag that is not supported. */

  #define EIP_ESR_BAD_02T_NETWORK_CONN_PRIORITY            0x121 /* This extended status code shall be returned
                                                                    as the result of specifying an O2T priority
                                                                    code that is not supported. */

  #define EIP_ESR_BAD_T20_NETWORK_CONN_PRIORITY            0x122 /* This extended status code shall be returned
                                                                     as the result of specifying a T2O priority
                                                                     code that is not supported. */

  #define EIP_ESR_BAD_02T_NETWORK_CONN_TYPE                0x123 /* This extended status code shall be returned
                                                                    as the result of specifying an O2T connection
                                                                    type that is not supported */

  #define EIP_ESR_BAD_T2O_NETWORK_CONN_TYPE                0x124 /* This extended status code shall be returned
                                                                    as the result of specifying a T2O connection
                                                                    type that is not supported */

  #define EIP_ESR_BAD_O2T_NETWORK_CONN_RED_OWNER           0x125 /* This extended status code shall be returned
                                                                    as the result of specifying an O2T Redundant
                                                                    Owner flag that is not supported */

  #define EIP_ESR_BAD_CONFIG_SIZE                          0x126 /* This extended status code is returned
                                                                    when the target device determines that
                                                                    the data segment provided in the
                                                                    Connection_Path parameter did not contain
                                                                    an acceptable number of 16-bit words
                                                                    for the the configuration application
                                                                    path requested.*/

  #define EIP_ESR_BAD_O2T_SIZE                             0x127 /* This extended status code is returned by the
                                                                    target when the size of the consuming object
                                                                    declared in the Forward_Open request and
                                                                    available on the target does not match the
                                                                    size declared in the O->T Network Connection
                                                                    Parameter. */

  #define EIP_ESR_BAD_T2O_SIZE                             0x128 /* This extended status code is returned by the
                                                                    target when the size of the producing object
                                                                    declared in the Forward Open request and
                                                                    available on the target does not match the
                                                                    size declared in the T->O Network Connection
                                                                    Parameter. */

  #define EIP_ESR_BAD_CONFIG_APPL_PATH                     0x129 /* The configuration application path specified
                                                                    in the connection path does not correspond to
                                                                    a valid configuration application path within
                                                                    the target application. This error could also
                                                                    be returned if a configuration application path
                                                                    was required, but not provided by a connection
                                                                    request */

  #define EIP_ESR_BAD_CONSUMING_APPL_PATH                  0x12A /* The consumed application path specified in the
                                                                    connection path does not correspond to a valid
                                                                    consumed application path within the target
                                                                    application. This error could also be returned
                                                                    if a consumed application path was required,
                                                                    but not provided by a connection request */

  #define EIP_ESR_BAD_PRODUCING_APPL_PATH                  0x12B /* The produced application path specified in the
                                                                    connection path does not correspond to a valid
                                                                    produced application path within the target
                                                                    application. This error could also be returned
                                                                    if a produced application path was required,
                                                                    but not provided by a connection request. */

  #define EIP_ESR_BAD_CONFIG_SYMBOL                        0x12C /* Configuration Symbol does not exist. The
                                                                    originator attempts to connect to a configuration
                                                                    tag name, but the name is not on the list of tags
                                                                    defined on the target. */

  #define EIP_ESR_BAD_CONSUMING_SYMBOL                     0x12D /* Consuming Symbol does not exist. The originator
                                                                    attempts to connect to a consuming tag name, but
                                                                    the name is not on the list of tags defined on the
                                                                    target. */

  #define EIP_ESR_BAD_PRODUCING_SYMBOL                     0x12E /* Producing Symbol does not exist. The originator
                                                                    attempts to connect to a producing tag name, but
                                                                    the name is not on the list of tags defined on the
                                                                    target. */

  #define EIP_ESR_BAD_APPL_PATH_COMBINATION                0x12F /* The combination of configuration and/or consume
                                                                    and/or produce application paths specified in the
                                                                    connection path are inconsistent with each other */

  #define EIP_ESR_BAD_CONSUME_DATA_FORMAT                  0x130 /* Information in the data segment is not consistent
                                                                    with the format of the consumed data. For example
                                                                    the configuration data specifies float configuration
                                                                    data while the consumed path specifies integer data */

  #define EIP_ESR_BAD_PRODUCE_DATA_FORMAT                  0x131 /* Information in the data segment is not consistent
                                                                    with the format of the produced data. For example the
                                                                    configuration data specifies float configuration data
                                                                    while the produced path specifies integer data. */

  #define EIP_ESR_CONN_TIMED_OUT                           0x203 /* Using a timed out connection  */
  #define EIP_ESR_UCSEND_TIMED_OUT                         0x204 /* Unconnected Send timed out    */
  #define EIP_ESR_UCSEND_PARAM_ERR                         0x205 /* Unconnected Send param. error */

  #define EIP_ESR_NO_BUFFER                                0x301 /* No buffer memory available    */
  #define EIP_ESR_NO_BANDWIDTH                             0x302 /* Insufficient bandwidth left   */
  #define EIP_ESR_NO_SCREENER                              0x303 /* Out of gen screeners          */
  #define EIP_ESR_NOT_CONF_FOR_RT                          0x304 /* Not configured to send RT data*/
  #define EIP_ESR_SIGNATURE_MISMATCH                       0x305 /* sig does not match sig store in CCM */
  #define EIP_ESR_CCM_NOT_AVAIL                            0x306 /* ccm is not responding to req. */

  #define EIP_ESR_BAD_PORT                                 0x311 /* Nonexistant port              */
  #define EIP_ESR_BAD_LINK_ADDRESS                         0x312 /* Invalid link address in path  */
  #define EIP_ESR_BAD_SEGMENT                              0x315 /* Invalid segment in path       */
  #define EIP_ESR_BAD_CLOSE                                0x316 /* Path & conn not equal in close*/
  #define EIP_ESR_BAD_NET_SEGMENT                          0x317 /* Net seg not present or bad    */
  #define EIP_ESR_BAD_LINK_AD_TO_SELF                      0x318 /* Link address to self invalid  */
  #define EIP_ESR_NO_SEC_RESOURCE                          0x319 /* Resources in secondary unavail*/

  #define EIP_ESR_REDUND_CONN_MISMATCH                     0x31D /* Redundant connection mismatch */

  #define EIP_ESR_NOT_CONFIGURED_FOR_OFF_SUBNET_MULTICAST  0x813  /* A multicast connection has been requested between a
                                                                     producer and a consumer that are on different subnets,
                                                                     and the producer is not configured for off-subnet multicast. */
  /* Vendor specific */
  #define EIP_ESR_ACCESS_CONFLICT                 0x320 /* read write access fail */
  #define EIP_ESR_INTERNAL_ERROR                  0x321 /* Internal error occurred (send/receive packets) */

typedef enum CIP_STACK_MODE_Etag
{
  CIP_STACK_MODE_OFFLINE  = 0x00,
  CIP_STACK_MODE_STOP     = 0x01,
  CIP_STACK_MODE_IDLE     = 0x02,
  CIP_STACK_MODE_RUN      = 0x03,
} CIP_STACK_MODE_E;

  /*#####################################################################################*/

  typedef struct EIP_PARAMETER_Ttag
  {
    uint16_t usMaxUCMMConnections;      /* Maximum number of UCMM server connections */
    uint16_t usMaxCL3Connections;       /* Maximum number of class 3 server connections */
    uint16_t usMaxCL1Connections;       /* Maximum number of class 1 server plus client connections */
    uint16_t usMaxTcpServerConnections; /* Maximum number of TCP server connections aka Encap sessions aka sockets */
    uint16_t usMaxTlsConnections;       /* Maximum number of TLS server connections */
    uint16_t usMaxTcpClientConnections; /* Maximum number of TCP client connections */

    uint8_t  bTcpServerQueueSize;
    uint8_t  bUdpServerQueueSize;
    uint8_t  bIoQueueSize;
  } EIP_PARAMETER_T;

  /*#####################################################################################*/

  typedef struct EIP_CONNECTION_SUMMARY_Ttag
  {
    struct
    {
      uint16_t usNumExclusiveOwner;
      uint16_t usNumInputOnly;
      uint16_t usNumListenOnly;
      uint16_t usNumExplicitMessaging;
    } tAdapter;

    struct
    {
      uint16_t usNumImplicitMessagingOriginator;
      uint16_t usNumExplicitMessagingOriginator;
    } tScanner;

    struct
    {
      #define EIP_MAX_EXCLUSIVE_OWNER_TIMEOUT_CONN    4
      uint8_t                   bOTTimeoutCnt;
      struct
      {
        uint32_t  ulClass;
        uint32_t  ulInstance;
        uint32_t  ulOTConnPoint;
      } atOTConnTimeout[EIP_MAX_EXCLUSIVE_OWNER_TIMEOUT_CONN];
    } tTimedOutConnections;

  } EIP_CONNECTION_SUMMARY_T;

  /*#####################################################################################*/
  typedef struct EIP_APPLICATION_PATH_Ttag
  {
    uint32_t ulClass;     /* (ulClass == 0) -> application path not present/decoded */
    uint32_t ulInstance;
    uint32_t ulConnPoint;
    uint32_t ulAttribute;
    uint32_t ulMember;
    /* Bit addressing is not implemented */

  } EIP_APPLICATION_PATH_T;

  typedef struct EIP_CONNECTION_PARAMETER_Ttag
  {
    uint32_t  ulOSerialNum;       /* Orginator serial number                         */
    uint32_t  ulOTConnID;         /* Network connection ID orginator to target       */
    uint32_t  ulTOConnID;         /* Network connection ID target to orginator       */
    uint32_t  ulOTRpi;            /* orginator to target requested packet rate in us */
    uint32_t  ulTORpi;            /* target to orginator requested packet rate in us */

    EIP_APPLICATION_PATH_T tConfigPath;       /* formerly ulInstance */
    EIP_APPLICATION_PATH_T tConsumptionPath;  /* formerly ulOTConnectionPoints */
    EIP_APPLICATION_PATH_T tProductionPath;   /* formerly ulTOConnectionPoints */

    uint32_t  ulProInhib;         /* Production Inhibit Timeout value in microseconds */
    uint8_t * pbDataSeg;

    uint16_t  usDataSegSize;
    uint16_t  usOTConnParam;      /* orginator to target connection parameter        */
    uint16_t  usTOConnParam;      /* target to orginator connection parameter        */
    uint16_t  usOTConnSize;       /* orginator to target connection size             */
    uint16_t  usTOConnSize;       /* target to orginator connection size             */

    uint16_t  usConnSerialNum;    /* Connection serial number                        */
    uint16_t  usVendorId;         /* Orginator Vendor ID                             */

    uint8_t   bTriggerType;       /* Transport type/trigger                          */
    uint8_t   bTimeoutMultiple;   /* Connection timeout multiple                     */
    uint8_t   bConnType;
    uint8_t   bPriority;          /* used to calculate request timeout information   */
    uint8_t   bTimeOutTicks;      /* used to calculate request timeout information   */

  } EIP_CONNECTION_PARAMETER_T;

  /*#####################################################################################*/

  typedef struct EIP_QOS_CONFIG_Ttag
  {
    uint8_t b802_1TagEnable;
    uint8_t bDSCP_Urgent;
    uint8_t bDSCP_Scheduled;
    uint8_t bDSCP_High;
    uint8_t bDSCP_Low;
    uint8_t bDSCP_Explicit;
  } EIP_QOS_CONFIG_T;

  /*#####################################################################################*/

  typedef struct EIP_STATUS_Ttag
  {
    uint8_t  bGeneralStatus;
    uint8_t  bAddStatusSize;
    uint16_t ausAddStatus[1];
  } EIP_STATUS_T;

  /*#####################################################################################*/

  typedef struct EIP_RESOURCES_Ttag*          EIP_H;

  /*#####################################################################################*/



  typedef uint32_t (*EIP_FEATURE_INIT_FN) (EIP_H hEip, void* pvParam);

  typedef struct EIP_FEATURE_LIST_ENTRY_Ttag
  {
    EIP_FEATURE_INIT_FN fnInit;
    void*               pvParam;
  } EIP_FEATURE_LIST_ENTRY_T;

  typedef struct EIP_FEATURES_Ttag
  {
    /* The first set of objects in this structure is typed and ordered
       because we access each object's startup parameters (pvParam) fields from
       within the stack.
    */
    EIP_FEATURE_LIST_ENTRY_T   tAsObject;
    EIP_FEATURE_LIST_ENTRY_T   tMrObject;
    EIP_FEATURE_LIST_ENTRY_T   tIdObject;
    EIP_FEATURE_LIST_ENTRY_T   tCmObject;
    EIP_FEATURE_LIST_ENTRY_T   tTcpObject;
    EIP_FEATURE_LIST_ENTRY_T   tEnObject;
    EIP_FEATURE_LIST_ENTRY_T   tPdcObject;
    EIP_FEATURE_LIST_ENTRY_T   tIoObject;
    EIP_FEATURE_LIST_ENTRY_T   tDlrObject;
    EIP_FEATURE_LIST_ENTRY_T   tLldpMgmtObject;
    EIP_FEATURE_LIST_ENTRY_T   tCCObject;

    EIP_FEATURE_LIST_ENTRY_T   ptOtherFeaturesList[];

  } EIP_FEATURES_T;


  struct LWIP_NETIF_Ttag;   /* forward declaration */

  #define EIP_TASK_LOWEST_NAME                     "EIP LOWEST"
  #define EIP_TASK_LOW_NAME                        "EIP LOW"
  #define EIP_TASK_HIGH_NAME                       "EIP HIGH"
  #define EIP_TASK_LOWEST_REQUIRED_STACKSIZE       0x800
  #define EIP_TASK_LOW_REQUIRED_STACKSIZE          0x1000
  #define EIP_TASK_HIGH_REQUIRED_STACKSIZE         0x800

  typedef struct EIS_STARTUP_Ttag
  {
    uint16_t                usMaxObjects;
    uint16_t                usMaxRegisteredServices;
    struct LWIP_NETIF_Ttag* hNetif;
    void*                   hDrvEth;
    void*                   pvHwCfg;
    EIP_PARAMETER_T         tEncapParam;
    struct PS_WORKER_Ttag*  hWorker_LowestPrio; /* worker to do long running operations, e.g. flash accesses */
    struct PS_WORKER_Ttag*  hWorker_LowPrio;    /* See EIP_TASK_LOW_REQUIRED_STACKSIZE for required stack size of worker. */
    struct PS_WORKER_Ttag*  hWorker_HighPrio;   /* See EIP_TASK_HIGH_REQUIRED_STACKSIZE for required stack size of worker. */

    const EIP_FEATURES_T    *ptFeatures;
    bool                    fStackStoresRemanentData;  /* True if the stack/GenAP shall handle remanent data, false if the host handles it */
    uint16_t                usMaxHighPrioTimers;

    void                   (*pfnModuleTests)(void*);    /* Hook to module tests entry point, Should be NULL for production firmwares */
    uint32_t                ulLtdStartupTime;          /* Set to nonzero for time limited eval stack. Second granularity */
  } EIP_STARTUP_T;

  /*#####################################################################################*/

  typedef void (*EIP_SESSION_STATE_CALLBACK)(void* pvParam, uint32_t ulEncapSessionHandle, uint32_t ulState);
  typedef int32_t (*EIP_SESSION_RECV_CALLBACK)( void* pvParam, uint32_t ulDataLen, uint8_t* ptData, uint32_t ulEncapSessionHandle );



  typedef void (*EIP_MESSAGESEND_RESPONSE_CALLBACK)( void *pvParam1, void *pvParam2, uint32_t ulCommand, uint32_t ulLen, uint8_t *pbData, uint32_t ulEncapSessionHandle );

  typedef void (*EIP_ENCAP_DATA_CALLBACK) (void *pvParam1, void *pvParam2, uint32_t ulCommand, uint32_t ulLen, uint8_t *pbData);
  typedef void (*CIP_OBJ_STATUS_CALLBACK) (void* pvParam, uint8_t bLedState, uint8_t bState);

  typedef void(*FN_CONNECTION_STATE_CHANGE_CALLBACK)(void *pvParam);

  typedef void (*CIP_CONNECTION_CALLBACK) (
    void* pvParam,
    uint8_t bReason,
    EIP_CONNECTION_SUMMARY_T *ptSummary,
    EIP_CONNECTION_PARAMETER_T *ptConnParam,
    FN_CONNECTION_STATE_CHANGE_CALLBACK fnCallback,
    void *pvCbParam);

  typedef enum
  {
    CIP_OBJECT_HOST_NOTIFICATION_TYPE_RESET,
    CIP_OBJECT_HOST_NOTIFICATION_TYPE_FORWARD_OPEN,
    CIP_OBJECT_HOST_NOTIFICATION_TYPE_SERVICE_REQUEST,
    CIP_OBJECT_HOST_NOTIFICATION_TYPE_OBJECT_CHANGE_INDICATION,
    CIP_OBJECT_HOST_NOTIFICATION_TYPE_OBJECT_CHANGE_INDICATION_BYPASS_NV_STORAGE,
    CIP_OBJECT_HOST_NOTIFICATION_TYPE_DPM_DIMENSION_CHANGED,
    CIP_OBJECT_HOST_NOTIFICATION_TYPE_SLAVE_BITFIELD_CHANGE,

  } CIP_OBJ_HOST_NOTIFICATION_CALLBACK_TYPE_E;

  struct CIP_OBJ_REQUEST_Ttag;    /* forward declaration */
  struct CIP_OBJ_RESPONSE_Ttag;   /* forward declaration */

  typedef void(*CIP_OBJ_RESPONSE_CB)            (void* pvParam, struct CIP_OBJ_RESPONSE_Ttag* ptResponse);

  typedef void(*CIP_OBJ_SERVICE_CALLBACK)     (CIP_OBJ_HOST_NOTIFICATION_CALLBACK_TYPE_E eType,
    void*                                     pvParam,
    struct CIP_OBJ_REQUEST_Ttag*              ptRequest,
    CIP_OBJ_RESPONSE_CB                       fnCallback,
    void*                                     pvCallbackParameter);

  /*#####################################################################################*/
  uint32_t
  EIP_StackInit( EIP_STARTUP_T*                ptPrm,
                 EIP_H*                        phEip,
                 CIP_OBJ_SERVICE_CALLBACK      fnServiceCb,           void* pvServiceCbParam,
                 CIP_OBJ_STATUS_CALLBACK       fnNetworkStatusCb,     void* pvNetworkStatusCbParam,
                 CIP_OBJ_STATUS_CALLBACK       fnModuleStatusCb,      void* pvModuleStatusCbParam,
                 CIP_CONNECTION_CALLBACK       fnConnectionChangedCb, void* pvConnectionChangedCbParam );

  /*#####################################################################################*/
  uint32_t
  Cip_SetStackMode( EIP_H   hEip, CIP_STACK_MODE_E eMode );

  /*#####################################################################################*/
  uint8_t
  Cip_GetStackMode( EIP_H hEip );

  /*#####################################################################################*/
  /*! Reset the EtherNet/IP stack
   *
   * The function resets the stack:
   *  -# Module and network status are set to state "No Power"
   *  -# All Exclusive Owner connections that are in state "timeout" are cleared.
   *  -# All services that have been registered with CipObj_RegisterService() are unregistered.
   *  -# The command CIPHIL_CMD_RESET_OBJECT is sent to all objects that are registered at the dictionary.
   *
   * Note: It is mandatory for the caller to bring the stack into state OFFLINE in advance.
   *
   * \param hEip [in]  Handle to the EtherNet/IP stack (obtained from EIS_StackInit() ).
   *
   * \return SUCCESS_HIL_OK on success.
   *         ERR_EIP_INVALID_STACK_MODE in case the stack is not in mode OFFLINE.
   */
  uint32_t
  Cip_ResetStack( EIP_H hEip );



  /*#####################################################################################*/
  void
  Cip_GetModuleNetworkStatus( EIP_H    hEip,
                             uint32_t* pulModuleStatus,
                             uint32_t* pulNetworkStatus );

  /*#####################################################################################*/
  void EipTimer_CycleCbk( void* pvArg );

  /*#####################################################################################*/
  void Eip_ChangeMode( EIP_H hEip );
  void Eip_TriggerCCObjectActivity( EIP_H hEip );
  void Eip_NotifyTimeSyncAboutIpAddressChange( EIP_H hEip );
  /*#####################################################################################*/

#endif
