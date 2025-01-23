/**************************************************************************************
Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
***************************************************************************************
$Id: cip_object_dictionary_core.h 67791 2017-03-16 07:06:58Z kai $:

Description:
  CIP Object Dictionary - API
**************************************************************************************/

/*! \defgroup CIP_OBJECT_DICTIONARY_Api  Object Dictionary API

* This module provides the API functions for the CIP object dictionary.
* The functions can be used to access the objects registered at the dictionary.
*/

#ifndef CIP_OBJECT_DICTIONARY_API_H
#define CIP_OBJECT_DICTIONARY_API_H


#include "eip_public.h"
#include "cip_common.h"

/*#####################################################################################*/

/*! \addtogroup CIP_OBJECT_DICTIONARY_Api
 * \{ */


 /*#####################################################################################*/

/*
  Flags bitmap (read from the right):
  |  These are only in the request flags              |  These are in the attribute dir and in the                                                                                                                                    |
  | CIP_FLG_PATH_NOT_DECODED | CIP_FLG_DTLS_MESSAGING | ..unused bits.. | CIP_FLG_TREAT_PROTECTED | CIP_FLG_TREAT_DISABLE | CIP_FLG_TREAT_NOTIFY | CIP_FLG_TREAT_NOT_IMPLEMENTED | CIP_FLG_GET_ACCESS_MASK  | CIP_FLG_SET_ACCESS_MASK |
  18                         17                       16                8                         7                       6                      5                               4                          2                         0
*/

/* These are used to flag the attribute dir entry as well as the request */

#define CIP_FLG_SET_ACCESS_MASK     0x0003  /**< Attribute set access rights                      */
#define CIP_FLG_SET_ACCESS_BUS      0x0000  /**< Attribute can be set by fieldbus                 */
#define CIP_FLG_SET_ACCESS_USER     0x0001  /**< Attribute can be set by user                     */
#define CIP_FLG_SET_ACCESS_ADMIN    0x0002  /**< Attribute is only settable from inside the stack */
#define CIP_FLG_SET_ACCESS_NONE     0x0003  /**< Attribute is not settable                        */

#define CIP_FLG_GET_ACCESS_MASK     0x000C  /**< Attribute get access rights                      */
#define CIP_FLG_GET_ACCESS_BUS      0x0000  /**< Attribute can be read by fieldbus                */
#define CIP_FLG_GET_ACCESS_USER     0x0004  /**< Attribute can be read by user                    */
#define CIP_FLG_GET_ACCESS_ADMIN    0x0008  /**< Attribute is only readable from inside the stack */
#define CIP_FLG_GET_ACCESS_NONE     0x000C  /**< Attribute is not readable                        */



/* These are used to flag only the attribute directory entry (not the request) */

#define CIP_FLG_TREAT_NOT_IMPLEMENTED 0x0010  /**< Attribute is not implemented, cannot be enabled */
#define CIP_FLG_TREAT_NOTIFY          0x0020  /**< Change of attribute data will be notified to and
                                                   has to be acknowledged by the user              */
#define CIP_FLG_TREAT_DISABLE         0x0040  /**< Attribute is disabled, can be enabled           */
#define CIP_FLG_TREAT_PROTECTED       0x0080  /**< Attribute is subject to the protection policy   */

/* These are used to flag only the request (not the attribute directory entry) */

#define CIP_FLG_DTLS_MESSAGING        0x10000 /**< The request arrived over a DTLS-secured UDP connection, e.g Forward Open/Close for a secure I/O connection */
#define CIP_FLG_PATH_NOT_DECODED      0x20000 /**< Message router was not able to decode the request path (path is still in the request data) */
#define CIP_FLG_OVERRIDE_PROTECTION   0x30000 /**< Internal set access shall take effect regardless of the current protection mode/policy */

/* And some macros to check flags/permissions */

#define CIP_IS_ATTRIBUTE_DISABLED(attribute_flags) (((attribute_flags) & CIP_FLG_TREAT_DISABLE) == CIP_FLG_TREAT_DISABLE)
#define CIP_IS_ATTRIBUTE_PROTECTED(attribute_flags) (((attribute_flags) & CIP_FLG_TREAT_PROTECTED) == CIP_FLG_TREAT_PROTECTED)
#define CIP_IS_ATTRIBUTE_NOT_IMPLEMENTED(attribute_flags) (((attribute_flags) & CIP_FLG_TREAT_NOT_IMPLEMENTED) == CIP_FLG_TREAT_NOT_IMPLEMENTED)
#define CIP_IS_ATTRIBUTE_NOTIFIED(attribute_flags) (((attribute_flags) & CIP_FLG_TREAT_NOTIFY) == CIP_FLG_TREAT_NOTIFY)

#define CIP_IS_BUS_GET_ACCESS(request_flags) (((request_flags) & CIP_FLG_GET_ACCESS_MASK) == CIP_FLG_GET_ACCESS_BUS)
#define CIP_IS_BUS_SET_ACCESS(request_flags) (((request_flags) & CIP_FLG_SET_ACCESS_MASK) == CIP_FLG_SET_ACCESS_BUS)
#define CIP_IS_USER_SET_ACCESS(request_flags) (((request_flags) & CIP_FLG_SET_ACCESS_MASK) == CIP_FLG_SET_ACCESS_USER)

/* Note how higher permissions include lower ones (i.e. user access possible for bus access permission, admin access possible for user (and bus) permission, etc.) */
#define CIP_IS_GET_PERMISSION_GRANTED(request_flags, attribute_flags) (((attribute_flags) & CIP_FLG_GET_ACCESS_MASK) <= ((request_flags) & CIP_FLG_GET_ACCESS_MASK))
#define CIP_IS_SET_PERMISSION_GRANTED(request_flags, attribute_flags) (((attribute_flags) & CIP_FLG_SET_ACCESS_MASK) <= ((request_flags) & CIP_FLG_SET_ACCESS_MASK))



/**@} */

#define CIP_OBJ_ROUTE_INVALID      0x0000
#define CIP_OBJ_ROUTE_DIRECT_RES   0x0001
#define CIP_OBJ_ROUTE_CALLBACK_RES 0x0002


 /*#####################################################################################*/

 /** CIP Service Request
 *  This structure holds data necessary to perform a CIP service request for an object registered
 *  at the CIP object dictionary.
 */
typedef struct CIP_OBJ_REQUEST_Ttag
{
  void*     pvHandle;           /**< Protocol stack specific handle to identify incoming connection (transport)
                                     When calling CipObj_RequestSync(), this field can set to NULL.             */

  uint32_t  ulClass;            /**< CIP object that is addressed by the service.                               */
  uint32_t  ulInstance;         /**< CIP object instance                                                        */
  uint32_t  ulAttribute;        /**< CIP object Attribute                                                       */
  uint32_t  ulMember;           /**< CIP object attribute's member                                              */

  uint16_t  usService;          /**< CIP service that shall be performed.                                       */

  uint16_t  usReqDataLen;       /**< Number of service data bytes available (pbReqData)                         */
  uint8_t*  pbReqData;          /**< Pointer to service data                                                    */

  uint32_t  ulFlags;            /**< Access flags (relates to the caller)                                       */
} CIP_OBJ_REQUEST_T;


/** CIP error/status code
 *  The structure provides the status/error code of a CIP service
 */
typedef struct CIP_ERROR_Ttag
{
  uint16_t* pusAddError;    /**< [in/out] Pointer to the additional error codes          */
  uint8_t   bAddErrorSize;  /**< [out] Additional Error Code size in words (pusAddError) */
  uint8_t   bGrc;           /**< [out] General Response Code                             */
} CIP_ERROR_T;


/** CIP service response structure
 *  The structure provides the response data of a CIP service.
 */
typedef struct CIP_OBJ_RESPONSE_Ttag
{
  CIP_ERROR_T   tError;                           /**< Status information about the service         */
  uint16_t      usDataLen;                        /**< [out] Length of valid service data in pbData */
  uint8_t*      pbData;                           /**< [in/out] Pointer to service data             */
} CIP_OBJ_RESPONSE_T;

/*#####################################################################################*/
/*! Synchronous Request
 *
 * The function executes a service to an object synchronously, i.e. in the context of the
 * task which calls this function. The service response is returned in the ptResonse buffer.
 *
 * It is mandatory for the caller to provide its own response buffer ptResponse->pbData
 * of sufficient size to store the response data of the requested service.
 *
 * \param hEip            [in]  Handle to the EtherNet/IP stack (obtained from EIS_StackInit() ).
 * \param ptRequest       [in]  Pointer to the CIP service request data.
 * \param ptResponse      [out] Pointer to the service response data.
 */
void
CipObj_RequestSync( EIP_H               hEip,
                    CIP_OBJ_REQUEST_T*  ptRequest,
                    CIP_OBJ_RESPONSE_T* ptResponse );

/* convenience functions */
uint8_t CipObj_SynchronousCall_6(EIP_H hEip, uint16_t usService, uint32_t ulClass, uint32_t ulInstance, uint32_t ulAttribute, uint8_t **ppbReqResData, uint16_t *pusReqResDataLen);
uint8_t CipObj_SynchronousCall_4(EIP_H hEip, uint16_t usService, uint32_t ulClass, uint32_t ulInstance, uint32_t ulAttribute);
uint8_t CipObj_SynchronousCall_3(EIP_H hEip, uint16_t usService, uint32_t ulClass, uint32_t ulInstance);

/*#####################################################################################*/
void CipObj_RequestSyncWithPath( EIP_H                hEip,
                                 uint16_t             usService,
                                 uint32_t             ulAccessFlags,
                                 CIP_EPATH_T*         ptPath,
                                 bool                 fPadded,
                                 uint8_t*             pbReqData,
                                 uint16_t             usReqDataLen,
                                 CIP_OBJ_RESPONSE_T*  ptResponse );
/*#####################################################################################*/

struct CIP_OBJ_ASYNC_REQUEST_Ttag;  /* forward declaration */

 /*! High priority asynchronous request with no response buffer
  *
  * The function executes a service to an object asynchronously via the
  * high-priority PS worker task of the stack.
  * When the requested service has been processed, the given callback function
  * (fnCallback) will be invoked to signal completion.
  *
  * Note that this function does not provide a reply buffer for the requested service.
  * Calling a service which returns data leads to undefined behavior.
  *
  * \param hEip             [in]  Handle to the EtherNet/IP stack (obtained from EIS_StackInit() ).
  * \param ptRequest        [in]  Pointer to the CIP service request. Only tRequest needs to be filled out.
  *                               All other fields are used only internally by the stack.
  * \param fnCallback       [in]  Response callback, called once the requested service has finished.
  * \param pvParam          [in]  Response callback parameter.
  */
void CipObj_RequestAsync_HighPrio_NoResponseData(EIP_H                              hEip,
                                                 struct CIP_OBJ_ASYNC_REQUEST_Ttag* ptRequest,
                                                 CIP_OBJ_RESPONSE_CB                fnCallback,
                                                 void*                              pvParam);


/*! Low priority asynchronous request with response buffer
 *
 * The function executes a service to an object asynchronously via the
 * low-priority PS worker task of the stack.
 * When the requested service has been processed, the given callback function
 * (fnCallback) will be invoked to signal completion. Then, the response buffer
 * which comes as parameter ptResponse->pbData holds the response data for the
 * requested service. Data will be valid until the callback function returns.
 *
 * \param hEip             [in]  Handle to the EtherNet/IP stack (obtained from EIS_StackInit() ).
 * \param ptRequest        [in]  Pointer to the CIP service request. Only tRequest needs to be filled out.
 *                               All other fields are used only internally by the stack.
 * \param fnCallback       [in]  Response callback, called once the requested service has finished.
 * \param pvParam          [in]  Response callback parameter.
 */
void
CipObj_RequestAsync_LowPrio_WithResponse( EIP_H                               hEip,
                                          struct CIP_OBJ_ASYNC_REQUEST_Ttag*  ptRequest,
                                          CIP_OBJ_RESPONSE_CB                 fnCallback,
                                          void*                               pvParam);


/*#####################################################################################*/
/*! Register FwOpen Callback
 *
 * The function is used to register a callback to receive FwOpen services before the stack
 * handles the Forward Open. A NULL callback will disable the service.
 *
 * \param hEip            [in]  Handle to the EtherNet/IP stack.
 * \param pfCallback      [in]  notification callback.
 * \param pvParam         [in]  callback user parameter.
 *
 */
void
CipObj_RegisterFwOpenCallback( EIP_H        hEip,
                               CIP_OBJ_SERVICE_CALLBACK pfCallback,
                               void*                    pvParam );

/*#####################################################################################*/

void
CipObj_PerformServiceToAllObjects(struct EIP_RESOURCES_Ttag*   hEip,
  uint16_t                     usService);


/*#####################################################################################*/

/*! \} */

/*#####################################################################################*/
#endif /* CIP_OBJECT_DICTIONARY_API_H */
