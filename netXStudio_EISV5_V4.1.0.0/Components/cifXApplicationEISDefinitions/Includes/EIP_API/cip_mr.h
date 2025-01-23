/**************************************************************************************
Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
***************************************************************************************
$Id: cip_mr.h 108920 2022-09-14 15:34:25Z OKompani $:

Description:
  EtherNet/IP - CIP Message Router Object
**************************************************************************************/

#ifndef CIP_MR_H
#define CIP_MR_H

#include <stdint.h>
#include "cip_common.h"
#include "cip_object_dictionary_api.h"

#define CIP_MR_MAX_SERVICES         10    /* number of parallel CIP services        */

/************************************************************
*  Object specific defines
************************************************************
*/
#define CIP_MR_CLASS_NUMBER            0x02
#define CIP_MR_REVISION                1
#define CIP_MR_MAX_INSTANCE            1

#define CIP_MR_MAX_INSTANCE_ATTR       0

#define CIP_MR_CMD_SEND_RECEIVE_FRAGMENT                                  ( 0x4c )

#define CIP_MR_ESR_RESOURCE_REQUEST_TOO_LARGE                              0x0001
#define CIP_MR_ESR_RESOURCE_OUT_OF_FRAGMENTATION_SESSIONS                  0x0002
#define CIP_MR_ESR_RESOURCE_ASSEMBLED_RESPONSE_TOO_LARGE                   0x0003
#define CIP_MR_ESR_FRAGMENTATION_NO_ACTIVE_SESSION                         0x0001
#define CIP_MR_ESR_FRAGMENTATION_MIDDLE_FRAGMENT_WHEN_EXPECTED_LAST        0x0002
#define CIP_MR_ESR_FRAGMENTATION_LAST_FRAGMENT_WHEN_EXPECTED_MIDDLE_PHASE1 0x0003
#define CIP_MR_ESR_FRAGMENTATION_LAST_FRAGMENT_WHEN_EXPECTED_MIDDLE_PHASE2 0x0004
#define CIP_MR_ESR_FRAGMENTATION_REQUEST_INFO_OUT_OF_SEQUENCE              0x0005
#define CIP_MR_ESR_FRAGMENTATION_INVALID_REQUEST_INFO                      0x0006
#define CIP_MR_ESR_FRAGMENTATION_INVALID_SIZE_TOO_MUCH_DATA                0x0007
#define CIP_MR_ESR_FRAGMENTATION_INVALID_SIZE_NON_FRAGMENTED_SIZE_MISMATCH 0x0008
#define CIP_MR_ESR_VERSION_UNSUPPORTED_VERSION                             0x0001



struct CIP_MR_RSC_Ttag; /* forward declaration */

typedef struct EIP_MR_INIT_PARAM_Ttag
{
  uint16_t usFragmentationBufferSize;
  bool (*fnSendReceiveFragment)(struct CIP_MR_RSC_Ttag *ptRsc, CIP_OBJ_REQUEST_T *ptRequest, CIP_OBJ_RESPONSE_T *ptResponse, CIP_OBJ_RESPONSE_CB fnCallback, void* pvParam );
} CIP_MR_INIT_PARAM_T;

bool
CipMR_SendReceiveFragment(struct CIP_MR_RSC_Ttag *ptRsc, CIP_OBJ_REQUEST_T *ptRequest, CIP_OBJ_RESPONSE_T *ptResponse, CIP_OBJ_RESPONSE_CB fnCallback, void* pvParam );

typedef void(*CIP_MR_SENDRESP_FUNC)(void * pvParam, CIP_OBJ_RESPONSE_T *tCIPResponse);

bool
CipMRHandleRequest(
  struct CIP_MR_RSC_Ttag* ptRsc,
  uint16_t                usLength,
  uint8_t*                pbData,
  CIP_MR_SENDRESP_FUNC    pfFinalizeCallback,
  void*                   pvFinalizeParam,
  bool                    fCalledOverDtls);

uint32_t
CipMR_ObjectCreate(struct EIP_RESOURCES_Ttag *hEip, void* pvParam);

void CipMr_TerminateFragmentationSession(EIP_H hEip, void *pvHandle);

  /*#####################################################################################*/


#endif /* CIP_MR_H */
