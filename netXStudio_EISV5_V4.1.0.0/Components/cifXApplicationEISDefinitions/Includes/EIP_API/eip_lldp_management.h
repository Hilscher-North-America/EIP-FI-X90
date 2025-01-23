/**************************************************************************************
Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
***************************************************************************************
$Id: eip_lldp_management.h 80332 2019-01-27 10:35:04Z MarcBommert $:

Description:
  EtherNet/IP - EtherNet/IP LLDP Management Object
**************************************************************************************/

#ifndef EIP_LLDP_MGMT_H
#define EIP_LLDP_MGMT_H

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

  #define EIP_LLDP_MGMT_CLASS_NUMBER       0x109
  #define EIP_LLDP_MGMT_REVISION           0x01
  #define EIP_LLDP_MGMT_MAX_INSTANCE       0x01
  #define EIP_LLDP_MGMT_MAX_INSTANCE_ATTR  5

  /* attributes of the object */
  #define EIP_LLDP_MGMT_ATTR_1_LLDP_ENABLE       1
  #define EIP_LLDP_MGMT_ATTR_2_MSG_TX_INTERVAL   2
  #define EIP_LLDP_MGMT_ATTR_3_MSG_TX_HOLD       3
  #define EIP_LLDP_MGMT_ATTR_4_LLDP_DATASTORE    4
  #define EIP_LLDP_MGMT_ATTR_5_DSCP_LAST_CHANGE  5

  /*#####################################################################################*/

  #define EIP_LLDP_MGMT_INST_ATTR_1_GLOBAL_ENABLE          0x01
  #define EIP_LLDP_MGMT_INST_ATTR_1_PORT_1_ENABLE          0x02
  #define EIP_LLDP_MGMT_INST_ATTR_1_PORT_2_ENABLE          0x04

  #define EIP_LLDP_MGMT_INST_ATTR_2_MSG_TX_INTERVAL_MIN       1 /* seconds */
  #define EIP_LLDP_MGMT_INST_ATTR_2_MSG_TX_INTERVAL_DEFAULT  30 /* seconds *//* default as defined in 802.1AB */
  #define EIP_LLDP_MGMT_INST_ATTR_2_MSG_TX_INTERVAL_MAX    3600 /* seconds */

  #define EIP_LLDP_MGMT_INST_ATTR_3_MSG_TX_HOLD_MIN           1
  #define EIP_LLDP_MGMT_INST_ATTR_3_MSG_TX_HOLD_DEFAULT       4 /* default as defined in 802.1AB */
  #define EIP_LLDP_MGMT_INST_ATTR_3_MSG_TX_HOLD_MAX         100

  #define EIP_LLDP_MGMT_INST_ATTR_4_LLDP_DATA_TABLE        0x01
  #define EIP_LLDP_MGMT_INST_ATTR_4_SNMP                   0x02
  #define EIP_LLDP_MGMT_INST_ATTR_4_NETCONF_YANG           0x04
  #define EIP_LLDP_MGMT_INST_ATTR_4_RESTCONF_YANG          0x08

  /*#####################################################################################*/

  typedef __HIL_PACKED_PRE struct EIP_LLDP_MGMT_INST_ATTR1_LLDP_ENABLE_Ttag
  {
    uint16_t usLength;
    uint8_t  bEnableArray; /* We only need one byte as we only have 2 Ethernet ports
                              This byte could hold up to 7 Ethernet ports (bit 0 is global enable bit). */
  }__HIL_PACKED_POST EIP_LLDP_MGMT_INST_ATTR1_LLDP_ENABLE_T;

typedef struct EIP_LLDP_MGMT_INST_ATTR_STORED_Ttag
{
  EIP_LLDP_MGMT_INST_ATTR1_LLDP_ENABLE_T tLldpEnable;     /* Attribute 1 */
  uint16_t                               usMsgTxInterval; /* Attribute 2 */
  uint8_t                                bMsgTxHold;      /* Attribute 3 */

} EIP_LLDP_MGMT_INST_ATTR_STORED_T;

  /*#####################################################################################*/

struct LWIP_NETIF_Ttag;   /* forward declaration */

typedef struct EIP_LLDP_MGMT_INIT_PARAM_Ttag
{
  void*    hDrvEth;
  void*    hLldpWorker;

  /* SNMP related parameters */
  void*    hSnmpNetif;
  void*    hSnmpWorker;
} EIP_LLDP_MGMT_INIT_PARAM_T;


  /*
  ************************************************************
  *  Task Function Prototypes
  ************************************************************
  */

  struct EIP_RESOURCES_Ttag;    /* forward declaration */

    /** Creates the LLDP Management object.
   *
   * \param pvRsc   [in] Task Resources
   *        ptParam [in] settings for LLDP Management object
   *
   * \return Description of return value, if any
   */
  uint32_t
  EipLldpMgmt_ObjectCreate( struct EIP_RESOURCES_Ttag* hEip, void *pvParam );

  /*#####################################################################################*/

#endif /* EIP_LLDP_MGMT_H */
