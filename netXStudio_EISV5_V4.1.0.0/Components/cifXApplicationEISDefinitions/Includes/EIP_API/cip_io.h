/**************************************************************************************
Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
***************************************************************************************
$Id: cip_io.h 106851 2022-05-17 10:38:52Z MBommert $:

Description:
  EtherNet/IP - IO Object (Hilscher specific)
**************************************************************************************/

#ifndef CIP_IO_H
#define CIP_IO_H

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

#define CIP_IO_CLASS_NUMBER             0x402
#define CIP_IO_REVISION                 0x01
#define CIP_IO_MAX_INSTANCE             0x01

#define CIP_IO_MAX_INSTANCE_ATTR        3

/* attributes of predefined connection object */
#define CIP_IO_ATTR_1_STATUS            1
#define CIP_IO_ATTR_2_LEN               2
#define CIP_IO_ATTR_3_DATA              3

#define CIPHIL_CMD_IO_CREATE_INSTANCE     0xFF01 /* See CIP_IO_CREATE_INSTANCE_REQ/RES_T for request/response data structure. */
#define CIPHIL_CMD_IO_DELETE_INSTANCE     0xFF02 /* This service does not have request/response data.                         */

/* bit definitions for attribute 1: status */
#define CIP_IO_STATUS_PRODUCER  0x01
#define CIP_IO_STATUS_CONSUMER  0x02
#define CIP_IO_STATUS_DIRECTION_MASK  0x0F
#define CIP_IO_STATUS_CONNECTED 0x10

/*#####################################################################################*/

  /*
  ************************************************************
  *  Object specific types
  ************************************************************
  */

#ifdef __HIL_PRAGMA_PACK_ENABLE
#pragma __HIL_PRAGMA_PACK_1(CIP_IO)
#endif

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST CIP_IO_CREATE_INSTANCE_REQ_Ttag
{
  uint16_t  usDataLen;
  uint8_t*  pbData;

  uint8_t   bDirection;
  #define CIP_IO_DIRECTION_PRODUCER  0x01
  #define CIP_IO_DIRECTION_CONSUMER  0x02

  uint16_t  usOptions; /* Options flag field */
  #define CIP_IO_OPTION_DO_NOT_CHECK_FOR_OVERLAPPING_MEMORY_REGIONS    0x0001 /* Usually, the IO Object checks that the newly created
                                                                                  IO instance does not overlap with other already existing IO instances.
                                                                                  This option disables that check. */

} CIP_IO_CREATE_INSTANCE_REQ_T;


typedef struct CIP_IO_DPM_DIMENSION_CHANGE_NOTIFICATION_Ttag
{
  uint32_t ulMaximumAddressProduced;
  uint32_t ulMaximumAddressConsumed;

} CIP_IO_DPM_DIMENSION_CHANGE_NOTIFICATION_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST CIP_IO_CREATE_INSTANCE_RES_Ttag
{
  uint32_t  ulCreatedInstanceNumber;

} CIP_IO_CREATE_INSTANCE_RES_T;

#ifdef __HIL_PRAGMA_PACK_ENABLE
#pragma __HIL_PRAGMA_UNPACK_1(CIP_IO)
#endif

typedef struct CIP_IO_INIT_PARAM_Ttag
{
  uint16_t usMaxInstance;     /* maximum number of instances which can be created */
} CIP_IO_INIT_PARAM_T;

  /*#####################################################################################*/

struct EIP_RESOURCES_Ttag;  /* forward declaration */

  uint32_t
  CipIO_ObjectCreate(struct EIP_RESOURCES_Ttag *hEip, void* pvParam );

#endif /* CIP_IO_H */
