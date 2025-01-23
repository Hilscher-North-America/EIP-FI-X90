/**************************************************************************************
Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
***************************************************************************************
$Id: eip_qos.h 80332 2019-01-27 10:35:04Z MarcBommert $:

Description:
  EtherNet/IP - CIP QoS Object
**************************************************************************************/

#ifndef _EIP_QOS_H_
#define _EIP_QOS_H_

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

  #define EIP_QOS_CLASS_NUMBER       0x48
  #define EIP_QOS_REVISION           0x01
  #define EIP_QOS_MAX_INSTANCE       0x01

  #define EIP_QOS_MAX_INSTANCE_ATTR  8


  /* attributes of identity object */
  #define EIP_QOS_ATTR_1_TAG_ENABLE       1
  #define EIP_QOS_ATTR_2_DSCP_PTP_EVENT   2
  #define EIP_QOS_ATTR_3_DSCP_PTP_GENERAL 3
  #define EIP_QOS_ATTR_4_DSCP_URGENT      4
  #define EIP_QOS_ATTR_5_DSCP_SCHEDULED   5
  #define EIP_QOS_ATTR_6_DSCP_HIGH        6
  #define EIP_QOS_ATTR_7_DSCP_LOW         7
  #define EIP_QOS_ATTR_8_DSCP_EXPLICIT    8

  /*#####################################################################################*/

  #define EIP_QOS_TAG_ENABLED               1
  #define EIP_QOS_TAG_DISABLED              0
  #define EIP_QOS_DSCP_PTP_EVENT_DEFAULT   59
  #define EIP_QOS_DSCP_PTP_GENERAL_DEFAULT 47
  #define EIP_QOS_DSCP_URGENT_DEFAULT      55
  #define EIP_QOS_DSCP_SCHEDULED_DEFAULT   47
  #define EIP_QOS_DSCP_HIGH_DEFAULT        43
  #define EIP_QOS_DSCP_LOW_DEFAULT         31
  #define EIP_QOS_DSCP_EXPLICIT_DEFAULT    27

  /* defines for valid value ranges */
  #define EIP_QOS_DSCP_PTP_EVENT_MAXIMUM_VALUE      63
  #define EIP_QOS_DSCP_PTP_GENERAL_MAXIMUM_VALUE    63
  #define EIP_QOS_DSCP_URGENT_MAXIMUM_VALUE         63
  #define EIP_QOS_DSCP_SCHEDULED_MAXIMUM_VALUE      63
  #define EIP_QOS_DSCP_HIGH_MAXIMUM_VALUE           63
  #define EIP_QOS_DSCP_LOW_MAXIMUM_VALUE            63
  #define EIP_QOS_DSCP_EXPLICIT_MAXIMUM_VALUE       63


typedef struct EIP_QOS_INST_ATTR_STORED_Ttag
{
  uint8_t b802_1TagEnable;
  uint8_t bDSCP_PTP_Event;
  uint8_t bDSCP_PTP_General;
  uint8_t bDSCP_Urgent;
  uint8_t bDSCP_Scheduled;
  uint8_t bDSCP_High;
  uint8_t bDSCP_Low;
  uint8_t bDSCP_Explicit;
} EIP_QOS_INST_ATTR_STORED_T;

  /*#####################################################################################*/

  /*
  ************************************************************
  *  Task Function Prototypes
  ************************************************************
  */

  struct EIP_RESOURCES_Ttag;    /* forward declaration */

    /** Creates the Quality of Service object (QoS).
   *
   * \param pvRsc   [in] Task Resources
   *        ptParam [in] settings for QoS object
   *
   * \return Description of return value, if any
   */
  uint32_t
  EipQoS_ObjectCreate( struct EIP_RESOURCES_Ttag* hEip, void *pvParam );

  /*#####################################################################################*/

#endif /* _EIP_QOS_H_ */
