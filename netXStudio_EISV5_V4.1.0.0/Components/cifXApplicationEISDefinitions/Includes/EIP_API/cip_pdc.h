/**************************************************************************************
Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
***************************************************************************************
$Id: cip_pdc.h 97567 2021-01-15 10:13:58Z MBommert $:

Description:
  EtherNet/IP - Predefined Connection Object (Hilscher specific)
**************************************************************************************/

#ifndef CIP_PDC_H
#define CIP_PDC_H

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

  #define CIP_PDC_CLASS_NUMBER       0x401
  #define CIP_PDC_REVISION           0x01
  #define CIP_PDC_MAX_INSTANCE       0x0        /* default value: calculated dynamically */

  #define CIP_PDC_MAX_INSTANCE_ATTR  3

  /* attributes of predefined connection object */
  #define CIP_PDC_ATTR_1_STATE      1
  #define CIP_PDC_ATTR_2_COUNT      2
  #define CIP_PDC_ATTR_3_CONFIG     3

  #define CIP_PDC_DUMMY_INSTANCE    0xFFFFFFFF

  #define CIPHIL_CMD_PDC_OPEN_CONNECTION   0xFF01 /* Request data:  EIP_CONNECTION_T              */
                                                  /* Response data: CIP_PDC_OPEN_CONNECTION_RES_T */

  #define CIPHIL_CMD_PDC_CLOSE_CONNECTION  0xFF02 /* This service does not have request/response data. */


  /*#####################################################################################*/


#ifdef __HIL_PRAGMA_PACK_ENABLE
#pragma __HIL_PRAGMA_PACK_1(CIP_PDC)
#endif

  typedef __HIL_PACKED_PRE struct  CIP_PDC_OPEN_CONNECTION_RES_Ttag
  {
    uint8_t  bConnectionType;

  } __HIL_PACKED_POST CIP_PDC_OPEN_CONNECTION_RES_T;

  typedef __HIL_PACKED_PRE struct CIP_PDC_CONNECTION_CFG_Ttag
  {
    uint32_t ulConsumerPoint;   /* Connection point for consuming direction.       */
    uint32_t ulProducerPoint;   /* Connection point for producing direction.       */
    uint32_t ulConfigPoint;     /* Connection point for configuration data
                                   (typically an configuration assembly instance). */
    uint32_t ulMinOTRpi;        /* Minimum PRI in O2T direction in microseconds.   */
    uint32_t ulMaxOTRpi;        /* Maximum PRI in O2T direction in microseconds.   */
    uint32_t ulMinTORpi;        /* Minimum PRI in T2O direction in microseconds.   */
    uint32_t ulMaxTORpi;        /* Maximum PRI in T2O direction in microseconds.   */

    uint8_t  bSuppTrigTypes;                  /* Supported Trigger Types.
                                                 At least one of the following trigger types must be supported: */
    #define CIP_PDC_TTYPE_CYCLIC         0x01 /* Cyclic                */
    #define CIP_PDC_TTYPE_COS            0x02 /* Change of State       */
    #define CIP_PDC_TTYPE_APPLICATION    0x04 /* Application Triggered */

    uint8_t  bConnType;         /* Connection Type. Use one of the following types:
                                    CIP_CTYPE_EXCLUSIVE_OWNER
                                    CIP_CTYPE_LISTEN_ONLY
                                    CIP_CTYPE_INPUT_ONLY */
  } __HIL_PACKED_POST CIP_PDC_CONNECTION_CFG_T;

  /*#####################################################################################*/


#ifdef __HIL_PRAGMA_PACK_ENABLE
#pragma __HIL_PRAGMA_UNPACK_1(CIP_PDC)
#endif

  typedef struct CIP_PDC_INIT_PARAM_Ttag
  {
    uint16_t usMaxInstance;
  } CIP_PDC_INIT_PARAM_T;

  /*#####################################################################################*/

  /*
  ************************************************************
  *  Task Function Prototypes
  ************************************************************
  */
  struct EIP_RESOURCES_Ttag;  /* forward declaration */

  uint32_t
  CipPDC_ObjectCreate(struct EIP_RESOURCES_Ttag *hEip, void* pvParam );


#endif /* CIP_PDC_H */
