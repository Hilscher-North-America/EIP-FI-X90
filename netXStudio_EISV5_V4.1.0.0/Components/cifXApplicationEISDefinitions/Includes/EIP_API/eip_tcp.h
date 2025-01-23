/**************************************************************************************
Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
***************************************************************************************
$Id: eip_tcp.h 114330 2023-06-21 10:32:49Z MBommert $:

Description:
  EtherNet/IP - CIP TCP/IP Interface Object
**************************************************************************************/

#ifndef EIP_TCP_H
#define EIP_TCP_H

#include <stdint.h>

#include "Hil_Compiler.h"
#include "Hil_Results.h"
#include "cip_common.h"
#include "cip_strings.h"

  /*#####################################################################################*/


  /*
  ************************************************************
  *  Object specific defines
  ************************************************************
  */

  #define EIP_TCP_CLASS_NUMBER            0xF5
  #define EIP_TCP_REVISION                4
  #define EIP_TCP_MAX_INSTANCE            1

  #define EIP_TCP_MAX_INSTANCE_ATTR       0x301
  #define EIP_TCP_NUMBER_OF_INSTANCE_ATTRIBUTES   16 /* Number of standard attributes (14) + 2 vendor attributes */

  #define EIP_TCP_LINK_PATH_LENGTH        4
  #define EIP_TCP_MAX_HOSTNAME_LENGTH     64
  #define EIP_TCP_MAX_DOMAINNAME_LENGTH   48


  /* attributes of identity object */
  #define EIP_TCP_ATTR_1_STATUS                  1
  #define EIP_TCP_ATTR_2_CFG_CAPABILITY          2
  #define EIP_TCP_ATTR_3_CFG_CONTROL             3
  #define EIP_TCP_ATTR_4_PHY_LINK_OBJ            4
  #define EIP_TCP_ATTR_5_INTERFACE_CFG           5
  #define EIP_TCP_ATTR_6_HOST_NAME               6
  #define EIP_TCP_ATTR_7_SAFETY_NETWORK_NUMBER   7
  #define EIP_TCP_ATTR_8_TTL_VALUE               8
  #define EIP_TCP_ATTR_9_MCAST_CFG               9
  #define EIP_TCP_ATTR_10_SELECT_ACD             10
  #define EIP_TCP_ATTR_11_LAST_CONFLICT_DETECTED 11
  #define EIP_TCP_ATTR_12_EIP_QUICK_CONNECT      12
  #define EIP_TCP_ATTR_13_ENCAP_TIMEOUT          13
  #define EIP_TCP_ATTR_14_IANA_PORT_ADMIN        14
  #define EIP_TCP_ATTR_OBJECT_SPECIFIC_MAX       14

  #define EIP_TCP_ATTR_768_DHCP_CLIENT_ID        768
  #define EIP_TCP_ATTR_769_DHCP_TRANSMIT_RATE    769
  #define EIP_TCP_ATTR_VENDOR_SPECIFIC_MAX       769

  /* Configuration Capility Flags */
  #define EIP_TCP_CAP_BOOTP                      0x01
  #define EIP_TCP_CAP_DNS                        0x02
  #define EIP_TCP_CAP_DHCP                       0x04
  #define EIP_TCP_CAP_DHCPDNS                    0x08
  #define EIP_TCP_CAP_CNFSET                     0x10
  #define EIP_TCP_CAP_HW_CONFIG                  0x20
  #define EIP_TCP_CAP_CFG_CHANGE_REQUIRES_RESET  0x40
  #define EIP_TCP_CAP_ACD_CAPABLE                0x80

  /* Configuration Control Flags */
  #define EIP_TCP_CNTRL_START_MSK                0x0F
  #define EIP_TCP_CNTRL_INTERN                   0x00
  #define EIP_TCP_CNTRL_BOOTP                    0x01
  #define EIP_TCP_CNTRL_DHCP                     0x02

  #define EIP_TCP_CNTRL_DNS                      0x10

  #define EIP_TCP_STATUS_CONFIG_MASK             0x0000000F
  #define EIP_TCP_STATUS_CONFIG_NOT_CONFIGURED   0x00000000 /* The Interface Configuration attribute has not been configured. */
  #define EIP_TCP_STATUS_CONFIG_BOOTP_DHCP_NV    0x00000001 /* The Interface Configuration attribute contains
                                                               configuration obtained from BOOTP, DHCP or non- volatile storage */
  #define EIP_TCP_STATUS_CONFIG_HARDWARE         0x00000002 /* The IP address member of the Interface Configuration attribute
                                                               contains configuration, obtained from hardware settings
                                                               (e.g.: pushwheel, thumbwheel, etc.) */

  #define EIP_TCP_STATUS_MCAST_CFG_PENDING       0x00000010
  #define EIP_TCP_STATUS_INTERFACE_CFG_PENDING   0x00000020
  #define EIP_TCP_STATUS_ACD_STATUS              0x00000040
  #define EIP_TCP_STATUS_ACD_FAULT               0x00000080

  #define EIP_TCP_SELECT_ACD_DISABLE             0
  #define EIP_TCP_SELECT_ACD_ENABLE              1 /* Default (see CIP Volume 2) */

  #define EIP_TCP_ENCAP_TIMEOUT_DISABLE          0
  #define EIP_TCP_ENCAP_TIMEOUT_MIN              1
  #define EIP_TCP_ENCAP_TIMEOUT_MAX              3600
  #define EIP_TCP_ENCAP_TIMEOUT_DEFAULT          120

  #define CIPHIL_CMD_TCP_GET_NEXT_MULTICAST_ADDRESS       0xF501

  #define CIPHIL_CMD_TCP_ADD_IANA_PORT_ADMIN_ENTRY        0xF503 /* Adds or updates a port entry in EIP_TCP_IANA_PORT_ADMIN_T
                                                                    Request data has the structure of EIP_TCP_IANA_PORT_ADMIN_ENTRY_T                 */
  #define CIPHIL_CMD_TCP_REMOVE_IANA_PORT_ADMIN_ENTRY     0xF504 /* Removes a port entry in EIP_TCP_IANA_PORT_ADMIN_T  (sets entry to zero)
                                                                    Request data has the structure of EIP_TCP_IANA_PORT_ADMIN_CLEAR_PORT_ENTRY_DATA_T */

  #define CIPHIL_TCP_CMD_FORCE_NETIF_RESET_ON_NEXT_RECONFIG  0xF505  /* Force full netif reprogramming on next reconfig
                                                                        (cold start emulation for the CIP reset) */

  #define EIP_TCP_QUICKCONNECT_ARP_INTERVAL_MS   25  /* milliseconds */
  #define EIP_TCP_MAX_NUM_OF_QUICKCONNECT_ARPS   40

  #define EIP_TCP_DHCP_DISCOVER_INTERVAL_MIN     4   /* seconds */
  #define EIP_TCP_DHCP_DISCOVER_INTERVAL_MAX     60
  #define EIP_TCP_DHCP_DISCOVER_INTERVAL_DEFAULT 60

  /*#####################################################################################*/

  /*
  ************************************************************
  *  Object specific types
  ************************************************************
  */

  /*#####################################################################################*/

#ifdef __HIL_PRAGMA_PACK_ENABLE
#pragma __HIL_PRAGMA_PACK_1(EIP_TCP)
#endif

  typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_TCP_INTERFACE_CONFIG_Ttag
  {
    uint32_t   ulIpAddr;
    uint32_t   ulSubnetMask;
    uint32_t   ulGatewayAddr;
    uint32_t   ulNameServer;
    uint32_t   ulNameServer_2;
    uint8_t    abDomainName[CIP_TYPE_STRING_CHAR_SIZE + EIP_TCP_MAX_DOMAINNAME_LENGTH];
  } EIP_TCP_INTERFACE_CONFIG_T;

#ifdef __HIL_PRAGMA_PACK_ENABLE
#pragma __HIL_PRAGMA_UNPACK_1(EIP_TCP)
#endif

  /*#####################################################################################*/

  /* IP multicast address configuration */
  typedef struct EIP_TCP_MCAST_CONFIG_Ttag
  {
#define EIP_TCP_ALLOCCONTROL_DEFAULT 0
#define EIP_TCP_ALLOCCONTROL_MANUAL  1
    uint8_t    bAllocControl;     /* Multicast address allocation control
                                        word. Determines how addresses are
                                        allocated. */
    uint8_t    bReserved;
    uint16_t   usNumMCast;        /* Number of IP multicast addresses
                                       to allocate for EtherNet/IP */
    uint32_t   ulMcastStartAddr;  /* Starting multicast address from which
                                       to begin allocation */
  } EIP_TCP_MCAST_CONFIG_T;

  /*#####################################################################################*/

  /* Last Detected ACD Conflict  */
  typedef struct EIP_TCP_ACD_LAST_CONFLICT_Ttag
  {
    uint8_t    bAcdActivity;      /* State of ACD activity when last
                                       conflict detected */

    uint8_t    abRemoteMac[6];    /* MAC address of remote node from
                                       the ARP PDU in which a conflict was
                                       detected */

    uint8_t    abArpPdu[28];      /* Copy of the raw ARP PDU in which
                                       a conflict was detected. */
  } EIP_TCP_ACD_LAST_CONFLICT_T;

  /*#####################################################################################*/

  /* Used in conjunction with the object specific service CIPHIL_CMD_TCP_REMOVE_IANA_PORT_ADMIN_ENTRY. */
  typedef struct EIP_TCP_IANA_PORT_ADMIN_CLEAR_PORT_ENTRY_DATA_Ttag
  {
    uint16_t           usPortNumber;
    uint8_t            bProtocol;
  } EIP_TCP_IANA_PORT_ADMIN_CLEAR_PORT_ENTRY_DATA_T;


  #define EIP_TCP_IANA_PORT_ADMIN_MAX_ENTRIES 8

  typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_TCP_IANA_PORT_ADMIN_ENTRY_Ttag
  {
    uint8_t abPortNameEmptyCipShortString[1]; /*  The CIP specification defines the port name member like this:
                                                  The Port Name member is vendor specific but for well-known and
                                                  registered ports it is recommended to use the IANA description.
                                                  It is valid to have a NULL string in the Port Name attribute. */

    uint16_t           usPortNumber;  /* Port number defined by IANA ( www.iana.org) */

    #define EIP_TCP_IANA_PORT_ADMIN_PROTOCOL_TCP     6
    #define EIP_TCP_IANA_PORT_ADMIN_PROTOCOL_UDP    17
    uint8_t            bProtocol;

    #define EIP_TCP_IANA_PORT_ADMIN_STATE_CLOSED    0x00
    #define EIP_TCP_IANA_PORT_ADMIN_STATE_OPEN      0x01
    uint8_t            fAdminState;

    #define EIP_TCP_IANA_PORT_ADMIN_CAPABILITY_CONFIGURABLE    0x01 /* If set, the admin state member can be changed using
                                                                       the Set_Port_Admin_State service. */
    #define EIP_TCP_IANA_PORT_ADMIN_CAPABILITY_RESETREQUIRED   0x02 /* Indicates whether or not  device requires a restart
                                                                       in order for a change to the admin state member to take effect. */
    uint8_t            bAdminCapability;
  } EIP_TCP_IANA_PORT_ADMIN_ENTRY_T;

  typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_TCP_IANA_PORT_ADMIN_Ttag
  {
    uint8_t                         bPortCount; /* Number of entries in atIanaPortAdminEntries */
    EIP_TCP_IANA_PORT_ADMIN_ENTRY_T atIanaPortAdminEntries[EIP_TCP_IANA_PORT_ADMIN_MAX_ENTRIES];

  } EIP_TCP_IANA_PORT_ADMIN_T;

  /*#####################################################################################*/

  /* DHCP option 61 (Client Identifier)  */
  typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_TCP_DHCP_CLIENT_ID_Ttag
  {
    uint8_t    bNumBytes;         /* Number of bytes that will be used for
                                       DHCP option 61 */

    uint8_t    abClientId[48];    /* DHCP option 61 content */

  } EIP_TCP_DHCP_CLIENT_ID_T;

  /*#####################################################################################*/

  typedef struct EIP_TCP_INST_ATTR_STORED_Ttag
  {
    uint32_t                    ulConfigControl;                                                     /* 3 Configuration Control    */
    EIP_TCP_INTERFACE_CONFIG_T  tInterfaceConfig;                                                    /* 5 Interface Configuration  */
    uint8_t                     abHostname[CIP_TYPE_STRING_CHAR_SIZE + EIP_TCP_MAX_HOSTNAME_LENGTH]; /* 6 Host Name                */
    uint8_t                     bTTLValue;                                                           /* 8 TTL Value                */
    EIP_TCP_MCAST_CONFIG_T      tMcastConfig;                                                        /* 9 Mcast Config             */
    uint8_t                     bSelectAcd;                                                          /* 10 Select ACD              */
    EIP_TCP_ACD_LAST_CONFLICT_T tLastConflictDetected;                                               /* 11 Last detected conflict  */
    uint8_t                     bQuickConnectEnable;                                                 /* 12 Enable/Disable of
                                                                                                           QuickConnect feature    */
    uint16_t                    usEncapTimeout;                                                      /* 13 Encapsulation Inactivity
                                                                                                           Timeout                 */
    EIP_TCP_DHCP_CLIENT_ID_T    tClientId;                                                           /* 768 DHCP Client Identifier */
    uint8_t                     bDhcpDiscoverIntv;                                                   /* 769 DHCP Discover Transmit
                                                                                                            Interval               */
  } EIP_TCP_INST_ATTR_STORED_T;

  /*#####################################################################################*/

  struct LWIP_NETIF_Ttag;   /* forward declaration */

  typedef struct EIP_TCP_INIT_PARAM_Ttag
  {
    struct LWIP_NETIF_Ttag* hNetif;
  } EIP_TCP_INIT_PARAM_T;


  struct EIP_RESOURCES_Ttag;    /* forward declaration */

  /** Creates the EtherNet/IP specfic TCP/IP Interface object
   *
   * \param hEip    [in] Handle to EtherNet/IP stack resources
   * \param pvParam [in] Pointer to initialization parameters (EIP_TCP_INIT_PARAM_T)
   *
   * \return SUCCESS_HIL_OK on success, ERR_EIP_OBJECT_OUT_OF_MEMORY otherwise.
   */
  uint32_t
  EipTcp_ObjectCreate( struct EIP_RESOURCES_Ttag* hEip,
                       void* pvParam );

  /** Obtain the TTL value from TCP/IP Interface object (Attribute 8).
   *
   * \param hEip   [in]  Handle to EtherNet/IP stack resources
   * \param pbTTL  [out] Pointer to TTL value
   */
  void
  EipTcp_GetTTL( struct EIP_RESOURCES_Ttag*    hEip,
                 uint8_t* pbTTL );

  /** Obtain the IP address and subnet mask from TCP/IP Interface object (attribute 5).
   *
   * \param hEip       [in]  Handle to EtherNet/IP stack resources
   * \param pulIP      [out] Pointer to IP address
   * \param pulSubnet  [out] Pointer to subnet mask
   */
  void
  EipTcp_GetIpAndSubnet( struct EIP_RESOURCES_Ttag*     hEip,
                         uint32_t* pulIP,
                         uint32_t* pulSubnet );

  /** Provides information of whether QuickConnect is enabled or not (attribute 12).
   *
   * \param hEip       [in]  Handle to EtherNet/IP stack resources
   *
   * \return true, if QuickConnect is enabled. False otherwise.
   */
  bool
  EipTcp_IsQuickConnectEnabled( struct EIP_RESOURCES_Ttag* hEip );

  /** Stops sending QuickConnect ARPs
   */
  void
  EipTcp_StopSendingQuickConnectArps( struct EIP_RESOURCES_Ttag* hEip );

#endif /* EIP_TCP_H */
