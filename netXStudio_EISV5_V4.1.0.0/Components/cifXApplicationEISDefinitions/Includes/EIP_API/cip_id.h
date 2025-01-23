/**************************************************************************************
Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
***************************************************************************************
$Id: cip_id.h 109543 2022-10-14 13:37:29Z MBommert $:

Description:
  EtherNet/IP - CIP Identity Object
**************************************************************************************/

#ifndef CIP_ID_H
#define CIP_ID_H

#include <stdint.h>
#include "cip_common.h"

  /*#####################################################################################*/

  /*
  ************************************************************
  *  Object specific defines
  ************************************************************
  */

  #define CIP_ID_SERIAL_NUMBER      0x12345678L
  #define CIP_ID_CLASS_NUMBER       0x01
  #define CIP_ID_REVISION           0x02
  #define CIP_ID_MAX_INSTANCE       0x01

  #define CIP_ID_MAX_INSTANCE_ATTR  19

  #define CIP_ID_MAX_PRODUKTNAME_LEN  32

  /* Maximum number of bytes for get_attribute_all response data:
     Includes instance attributes 1 - 19. */
  #define CIP_ID_MAX_RESPONSE_DATA_LENGTH_OF_GET_ATTRIBUTE_ALL   64

  /*
  ** Attribute 8: State defines
  */

  #define CIP_ID_STATE_NONEXISTENT        0
  #define CIP_ID_STATE_DEVICESELFTEST     1
  #define CIP_ID_STATE_STANDBY            2
  #define CIP_ID_STATE_OPERATIONAL        3
  #define CIP_ID_STATE_MAJRECOVERFLT      4
  #define CIP_ID_STATE_MAJUNRECOVERFLT    5
  #define CIP_ID_STATE_DEFAULT          255

  /*
  ** Attribute 5: Defines of the status bits
  */

  #define CIP_ID_STATUS_OWNED_MASK                 0x0001
  #define CIP_ID_STATUS_OWNED                      0x0001
  #define CIP_ID_STATUS_UNOWNED                    0x0000

  #define CIP_ID_STATUS_CONFIGURED_MASK            0x0004
  #define CIP_ID_STATUS_UNCONFIGURED               0x0000
  #define CIP_ID_STATUS_CONFIGURED                 0x0004

  /* Device state bits.*/
  #define CIP_ID_STATUS_STATE_MASK                          0x00F0
  #define CIP_ID_STATUS_SELF_TESTING                        0x0000
  #define CIP_ID_STATUS_FIRMWARE_UPDATE_IN_PROGRESS         0x0010
  #define CIP_ID_STATUS_AT_LEAST_ONE_FAULTED_IO_CONNECTION  0x0020
  #define CIP_ID_STATUS_NO_IO_CONNECTION_ESTABLISHED        0x0030
  #define CIP_ID_STATUS_NON_VOLATILE_CONFIGURATION_IS_BAD   0x0040
  #define CIP_ID_STATUS_MAJOR_FAULT                         0x0050
  #define CIP_ID_STATUS_AT_LEAST_ONE_CONNECTION_IN_RUN_MODE 0x0060
  #define CIP_ID_STATUS_ALL_CONNECTIONS_IN_IDLE_MODE        0x0070

  /* Attribute 19: Protection Mode */
  #define CIP_ID_PROTECTION_MODE_NONE                       0x0000

  #define CIP_ID_PROTECTION_MODE_IMPLICIT_PROTECTION_MASK   0x0007
  #define CIP_ID_PROTECTION_MODE_IMPLICIT_PROTECTION        0x0001
  #define CIP_ID_PROTECTION_MODE_EXPLICIT_PROTECTION_MASK   0x0008
  #define CIP_ID_PROTECTION_MODE_EXPLICIT_PROTECTION        0x0008

#ifdef EIP_DO_LIKE_ROCKWELL
#warning manipulate Status for RSNETWORX
  #define CIP_ID_STATUS_NO_CONNECTION              CIP_ID_STATUS_ALL_CONNECTIONS_IN_IDLE_MODE
#else
  #define CIP_ID_STATUS_NO_CONNECTION              CIP_ID_STATUS_NO_IO_CONNECTION_ESTABLISHED
#endif

  /* Fault bits.*/
  #define CIP_ID_STATUS_FAULT_MASK                 0x0F00
  #define CIP_ID_STATUS_MINOR_RECOVERABLE_FAULT    0x0100
  #define CIP_ID_STATUS_MINOR_UNRECOVERABLE_FAULT  0x0200
  #define CIP_ID_STATUS_MAJOR_RECOVERABLE_FAULT    0x0400
  #define CIP_ID_STATUS_MAJOR_UNRECOVERABLE_FAULT  0x0800

  /* attributes of identity object */
  #define CIP_ID_ATTR_1_VENDOR_ID                   1
  #define CIP_ID_ATTR_2_DEVICE_TYPE                 2
  #define CIP_ID_ATTR_3_PRODUCT_CODE                3
  #define CIP_ID_ATTR_4_REVISION                    4
  #define CIP_ID_ATTR_5_STATUS                      5
  #define CIP_ID_ATTR_6_SERIAL_NUMBER               6
  #define CIP_ID_ATTR_7_PRODUCT_NAME                7
  #define CIP_ID_ATTR_8_STATE                       8
  #define CIP_ID_ATTR_9_CFG_CONSIST_VALUE           9
  #define CIP_ID_ATTR_10_HEARTBEAT_INTERVAL         10
  #define CIP_ID_ATTR_19_PROTECTION_MODE            19


  /* CIP services specific to Identity Object */
  #define CIP_CMD_ID_FLASH_LEDS                               (0x4B)

  /* Hilscher services specific to Identity Object */
  #define CIPHIL_CMD_ID_ENABLE_DISABLE_FLASH_LEDS_SERVICE     ( 0x0101 )
  #define CIPHIL_CMD_ID_ENABLE_DISABLE_HOST_CONTROL_OF_ATTR_8 ( 0x0102 ) /* Enables/disables the control of attribute 8 via the host.
                                                                            Enabling the host control also decreases the attribute's set option flags to "USER".
                                                                            In this case, only set services with set access "USER" will be able to change the attribute's value.
                                                                            Set services with access flag "ADMIN" will be silently ignored (service will not be rejected with GRC != 0). */
  /* Valid value ranges */

  #define CIP_ID_MAJOR_REV_MINIMUM_VALUE            1
  #define CIP_ID_MAJOR_REV_MAXIMUM_VALUE            127

  #define CIP_ID_MINOR_REV_MINIMUM_VALUE            1
  #define CIP_ID_MINOR_REV_MAXIMUM_VALUE            255

  /*#####################################################################################*/

  /*
  ************************************************************
  *  Object specific types
  ************************************************************
  */

  /*#####################################################################################*/


typedef struct CIP_ID_INST_ATTR_STORED_Ttag
{
  uint8_t   bHeartBeatInterval;
} CIP_ID_INST_ATTR_STORED_T;


  /*#####################################################################################*/

  /*
  ****************************************************************************
  *  Object specific macros
  ****************************************************************************
  */
  #define CIPID_STATUS_MODIFY_OWNED(        hEip, value )  CipId_ModifyStatus( hEip, 0x0001, value );
  #define CIPID_STATUS_MODIFY_CONFIGURED(   hEip, value )  CipId_ModifyStatus( hEip, 0x0004, value );
  #define CIPID_STATUS_MODIFY_EXTDEVSTATUS( hEip, value )  CipId_ModifyStatus( hEip, 0x00F0, value );
  #define CIPID_STATUS_MODIFY_MINORRECOV(   hEip, value )  CipId_ModifyStatus( hEip, 0x0100, value );
  #define CIPID_STATUS_MODIFY_MINORUNRECOV( hEip, value )  CipId_ModifyStatus( hEip, 0x0200, value );
  #define CIPID_STATUS_MODIFY_MAJORRECOV(   hEip, value )  CipId_ModifyStatus( hEip, 0x0400, value );
  #define CIPID_STATUS_MODIFY_MAJORUNRECOV( hEip, value )  CipId_ModifyStatus( hEip, 0x0800, value );
  #define CIPID_STATUS_MODIFY_EXTDEVSTAT2(  hEip, value )  CipId_ModifyStatus( hEip, 0xF000, value );

  /*#####################################################################################*/


  struct EIP_RESOURCES_Ttag;  /* forward declaration */

  /*
  ************************************************************
  *  Task Function Prototypes
  ************************************************************
  */

  /** Creates the CIP Identity object
   *
   * \param hEip    [in] Handle to EtherNet/IP stack resources
   * \param pvParam [in] Set to NULL (not used yet)
   *
   * \return SUCCESS_HIL_OK on success, ERR_EIP_OBJECT_OUT_OF_MEMORY otherwise.
   */
  uint32_t
  CipId_ObjectCreate(struct EIP_RESOURCES_Ttag *hEip, void* pvParam );

  /*#####################################################################################*/

  /** Modify status attribute 5.
   *
   * \param hEip    [in] Handle to EtherNet/IP stack resources
   * \param usMask  [in] Mask used in conjunction with usValue to set the new status attribute value.
   * \param usValue [in] Status value that shall be set. Use proper mask value.
   *                     Only bits that a masked with 1 are taken into account..
   */
  void
  CipId_ModifyStatus(struct EIP_RESOURCES_Ttag *hEip, uint16_t usMask, uint16_t usValue );

  /*#####################################################################################*/

  uint16_t CipId_GetProtectionMode(struct EIP_RESOURCES_Ttag * hEip);

#endif /* CIP_ID_H */
