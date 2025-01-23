/**************************************************************************************
Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
***************************************************************************************
$Id: eip_en.h 97517 2021-01-11 10:52:11Z MBommert $:

Description:
  EtherNet/IP - Ethernet Link  Object
**************************************************************************************/

#ifndef EIP_EN_H
#define EIP_EN_H

#include <stdint.h>
#include <stdbool.h>

#include "Hil_Compiler.h"
#include "Hil_Results.h"
#include "cip_common.h"

  /*#####################################################################################*/

/*
  ************************************************************
  *  Object specific defines
  ************************************************************
  */
  #define EIP_EN_CLASS_NUMBER            0xF6
  #define EIP_EN_REVISION                4
  #define EIP_EN_MAX_INSTANCE            2

  #define EIP_EN_MAX_INSTANCE_ATTR       768

  #define EIP_MAC_ADDR_LENGTH             6
  #define EIP_INTF_LABEL_LENGTH           64
  #define EIP_EN_MAX_SPEED_DUPLEX_OPTIONS 4

  /* attributes of identity object */
  #define EIP_EN_ATTR_1_INTERFACE_SPEED                  1
  #define EIP_EN_ATTR_2_INTERFACE_FLAGS                  2
  #define EIP_EN_ATTR_3_PHYSICAL_ADDRESS                 3
  #define EIP_EN_ATTR_4_INTERFACE_COUNTERS               4
  #define EIP_EN_ATTR_5_MEDIA_COUNTERS                   5
  #define EIP_EN_ATTR_6_INTERFACE_CONTROL                6
  #define EIP_EN_ATTR_7_INTERFACE_TYPE                   7
  #define EIP_EN_ATTR_8_INTERFACE_STATE                  8
  #define EIP_EN_ATTR_9_ADMIN_STATE                      9
  #define EIP_EN_ATTR_10_INTERFACE_LABEL                 10
  #define EIP_EN_ATTR_11_INTERFACE_CAPABILITY            11

  #define EIP_EN_ATTR_768_MDIX_CONFIG                   768


  #define EIP_EN_INTF_LSTATUS             0x01
  #define EIP_EN_INTF_FULLDUBLEX          0x02
  #define EIP_EN_INTF_NEGSTATUS_MSK       0x1C
  #define EIP_EN_INTF_NS_AUTONEQ_PROCESS  0x00
  #define EIP_EN_INTF_NS_FAILED1          0x40
  #define EIP_EN_INTF_NS_FAILED2          0x80
  #define EIP_EN_INTF_NS_SUCCESS          0xC0
  #define EIP_EN_INTF_NS_NOTATTEMPTED     0x10

  #define EIP_EN_INTF_MANSETTING          0x20
  #define EIP_EN_INTF_HW_FAULT            0x40

  #define EIP_EN_INTF_TYPE_UNKNOWN        0x00
  #define EIP_EN_INTF_TYPE_INTERNAL       0x01
  #define EIP_EN_INTF_TYPE_TWISTEDPAIR    0x02
  #define EIP_EN_INTF_TYPE_OPTICAL        0x03

  #define EIP_EN_INTF_STATE_UNKNOWN       0x00
  #define EIP_EN_INTF_STATE_ENABLE        0x01
  #define EIP_EN_INTF_STATE_DISABLE       0x02
  #define EIP_EN_INTF_STATE_TEST          0x03

  #define EIP_EN_ADMIN_STATE_ENABLE       0x01
  #define EIP_EN_ADMIN_STATE_DISABLE      0x02

  #define EIP_EN_INTF_MDIX_AUTO           0x01
  #define EIP_EN_INTF_MDIX_MDI            0x02
  #define EIP_EN_INTF_MDIX_MDIX           0x03

  /* CIP services specific to Ethernet Link Object */
  #define CIP_CMD_EN_GET_AND_CLEAR             ( 0x4C )

  /*#####################################################################################*/

/*
  ************************************************************
  *  Object specific types
  ************************************************************
  */

#ifdef __HIL_PRAGMA_PACK_ENABLE
#pragma __HIL_PRAGMA_PACK_1(EIP_EN)
#endif

  /*#####################################################################################*/

  typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_EN_INTERFACE_CNT_Ttag
  {
    uint32_t   lInOctets;        /* Number of octets received on the interface    */
    uint32_t   lInUcastPkts;     /* Number of subnetwork-unicast packets
                                    delivered to a higher-layer protocol          */
    uint32_t   lInNUcastPkts;    /* Number of non-unicast packets delivered
                                    to a higher layer protocol                    */
    uint32_t   lInDiscards;      /* Number of inbound packets discarded           */
    uint32_t   lInErrors;        /* Inbound packets that contained errors         */
    uint32_t   lInUnknownProtos; /* Inbound packets discarded due to unknown
                                    or unsupported protocol                       */
    uint32_t   lOutOctets;       /* Number of octets transmitted on the interface */
    uint32_t   lOutUcastPkts;    /* Number of packets transmitted to subnet
                                    unicast address                               */
    uint32_t   lOutNUcastPkts;   /* Number of packets sent to non-unicast
                                    address                                       */
    uint32_t   lOutDiscards;     /* Number of outbound packets discarded          */
    uint32_t   lOutErrors;       /* Number of outbound packets that could not     */
  } EIP_EN_INTERFACE_CNT_T;

  /*#####################################################################################*/


  typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_EN_MEDIA_CNT_Ttag
  {
    uint32_t   ulAlignmentErrors;
    uint32_t   ulFcsErrors;
    uint32_t   ulSingleCollisions;
    uint32_t   ulMultipleCollisions;
    uint32_t   ulSqeTestErrors;
    uint32_t   ulDeferredXmission;
    uint32_t   ulLateCollisions;
    uint32_t   ulExcessiveCollisions;
    uint32_t   ulInternalMacXmitErrors;
    uint32_t   ulCarrierSenseErrors;
    uint32_t   ulFrameTooLong;
    uint32_t   ulInternalMacReadErrors;
  } EIP_EN_MEDIA_CNT_T;

  /*#####################################################################################*/

  typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST EIP_EN_INTERFACE_CAPABILITY_Ttag
  {
    /* Configuration Capility Flags */
    #define EIP_EN_CAP_CFG_CHANGE_REQUIRES_RESET  0x01
    #define EIP_EN_CAP_AUTO_NEGOTIATE_SUPP        0x02
    #define EIP_EN_CAP_AUTO_MDIX_SUPP             0x04
    #define EIP_EN_CAP_MANUAL_SPEED_DUPLEX_SUPP   0x08
    uint32_t ulCapabilityBits;
    __HIL_PACKED_PRE struct __HIL_PACKED_POST
    {
      uint8_t bNumElements;
      __HIL_PACKED_PRE struct __HIL_PACKED_POST
      {
        uint16_t usInterfaceSpeed;
        #define EIP_EN_DUPLEX_MODE_HALF 0
        #define EIP_EN_DUPLEX_MODE_FULL 1
        uint8_t  bInterfaceDuplexMode;
      } atSpeedDuplexOption[EIP_EN_MAX_SPEED_DUPLEX_OPTIONS];
    } tSpeedDuplexOptions;
  } EIP_EN_INTERFACE_CAPABILITY_T;

#ifdef __HIL_PRAGMA_PACK_ENABLE
#pragma __HIL_PRAGMA_UNPACK_1(EIP_EN)
#endif

  /*#####################################################################################*/

  typedef struct EIP_EN_INST_ATTR_STORED_Ttag
  {
#define EIP_EN_CONTROL_AUTONEG              0x00000001
#define EIP_EN_CONTROL_FULLDUPLEX           0x00000002
#define EIP_EN_CONTROL_SPEED_MSK            0xFFFF0000
    uint32_t               ulInterfaceControl;                          /*  6 */
    uint8_t                bAdminState;                                 /*  9 */
    uint8_t                bMDIXCfg;                                    /* 0x300 (vendor specific range) */
  } EIP_EN_INST_ATTR_STORED_T;


  typedef struct EIP_EN_INIT_PARAM_Ttag
  {
    #define EIP_ETH_TYPE_DLR       1
    #define EIP_ETH_TYPE_STD_ETH   2
    #define EIP_ETH_TYPE_2PORT_SW  3

    uint8_t bEthType;
    void*   hDrvEth;
    void*   ahPhy[2];

  } EIP_EN_INIT_PARAM_T;


  struct EIP_RESOURCES_Ttag;  /* forward declaration */

  /** Creates the Ethernet Link object.
   *
   * \param pvRsc [in]  Task Resources
   *
   * \return Description of return value, if any
   */
  uint32_t CipEn_ObjectCreate(struct EIP_RESOURCES_Ttag *hEip, void* pvParam );

#endif /* EIP_EN_H */
