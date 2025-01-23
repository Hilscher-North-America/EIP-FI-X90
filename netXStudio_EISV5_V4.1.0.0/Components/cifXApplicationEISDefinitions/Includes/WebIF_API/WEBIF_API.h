/************************************************************************************
copyright (c) hilscher gesellschaft fuer systemautomation mbh. all rights reserved.
***************************************************************************************
$id: $:

description:
 Web packet interface definition header file
**************************************************************************************/

#ifndef WEBIF_API_H_
#define WEBIF_API_H_

#include <stdint.h>
#include <Hil_CommandRange.h>
#include <Hil_Packet.h>
#include "Public/Http_Defs.h"

#ifdef __HIL_PRAGMA_PACK_ENABLE
  #pragma __HIL_PRAGMA_PACK_1(HTTP_IF_API)
#endif

/*
 * The following values are defined according the internal implementations:
 * - WEBIF_PCK_CONTENT_SIZE is maximally set to the sum of TX Buffer sizes (defined TcpBuf_MemPool.h).
 * - WEBIF_FIELD_MAX_SIZE is dependent of string storage implemented in Http.c and Utils_HashMap.c.
 * These values can be bigger, but there is no concret benefit.
 */

/*! Maximal response or request content size */
#define WEBIF_PCK_CONTENT_SIZE 1024

/*! Maximal field name and content size */
#define WEBIF_FIELD_MAX_SIZE 128

/*! \defgroup WEBIF_Cmds WebIf Commands
*  \{ */

/*! Indication which indicate the HTTP request reception */
#define WEBIF_HANDLE_HTTP_REQUEST_IND                   0x0000AF00
#define WEBIF_HANDLE_HTTP_REQUEST_RSP                   0x0000AF01

/*! Indication which indicate the HTTP request body reception */
#define WEBIF_HANDLE_HTTP_REQUEST_CONTENT_IND           0x0000AF02
#define WEBIF_HANDLE_HTTP_REQUEST_CONTENT_RSP           0x0000AF03

/*! Indication which indicate the end of HTTP request reception */
#define WEBIF_FINISH_HANDLING_HTTP_REQUEST_IND          0x0000AF04
#define WEBIF_FINISH_HANDLING_HTTP_REQUEST_RSP          0x0000AF05

/*! Request to get a field in the HTTP request */
#define WEBIF_GET_HTTP_REQUEST_FIELD_REQ                0x0000AF06
#define WEBIF_GET_HTTP_REQUEST_FIELD_CNF                0x0000AF07

/*! Request to begin the HTTP response transmission */
#define WEBIF_GENERATE_HTTP_RESPONSE_REQ                0x0000AF08
#define WEBIF_GENERATE_HTTP_RESPONSE_CNF                0x0000AF09

/*! Request to set a field in HTTP response */
#define WEBIF_GENERATE_HTTP_RESPONSE_FIELD_REQ          0x0000AF0A
#define WEBIF_GENERATE_HTTP_RESPONSE_FIELD_CNF          0x0000AF0B

/*! Request to transmit body in the HTTP response */
#define WEBIF_GENERATE_HTTP_RESPONSE_CONTENT_REQ        0x0000AF0C
#define WEBIF_GENERATE_HTTP_RESPONSE_CONTENT_CNF        0x0000AF0D

/*! Request to finalize the transmission of HTTP response body */
#define WEBIF_FINISH_GENERATION_HTTP_RESPONSE_REQ       0x0000AF0E
#define WEBIF_FINISH_GENERATION_HTTP_RESPONSE_CNF       0x0000AF0F

/*! Request to be registered */
#define WEBIF_ENABLE_REQUEST_HANDLING_REQ               0x0000AF10
#define WEBIF_ENABLE_REQUEST_HANDLING_CNF               0x0000AF11

/*! Retro-compatibility definitions */
#define WEBIF_REGISTER_REQ                              WEBIF_ENABLE_REQUEST_HANDLING_REQ
#define WEBIF_REGISTER_CNF                              WEBIF_ENABLE_REQUEST_HANDLING_CNF

/*! Request to be registered */
#define WEBIF_DISABLE_REQUEST_HANDLING_REQ              0x0000AF20
#define WEBIF_DISABLE_REQUEST_HANDLING_CNF              0x0000AF21

/*! Retro-compatibility definitions */
#define WEBIF_UNREGISTER_REQ                            WEBIF_DISABLE_REQUEST_HANDLING_REQ
#define WEBIF_UNREGISTER_CNF                            WEBIF_DISABLE_REQUEST_HANDLING_CNF

/*! WebServer Start Request*/
#define WEBIF_START_REQ                                 0x0000AF22
#define WEBIF_START_CNF                                 0x0000AF23

/*! WebServer Stop Request*/
#define WEBIF_STOP_REQ                                  0x0000AF24
#define WEBIF_STOP_CNF                                  0x0000AF25

/*! Set TCP Ports Request*/
#define WEBIF_SET_TCP_PORTS_REQ                         0x0000AF26
#define WEBIF_SET_TCP_PORTS_CNF                         0x0000AF27

/*! Enable or disable a dispatch entry. */
#define WEBIF_SET_DISPATCH_ENTRY_ENABLED_REQ            0x0000AF28
#define WEBIF_SET_DISPATCH_ENTRY_ENABLED_CNF            0x0000AF29

/*! Change the URL of a dispatch entry. */
#define WEBIF_SET_DISPATCH_ENTRY_URL_REQ                0x0000AF2A
#define WEBIF_SET_DISPATCH_ENTRY_URL_CNF                0x0000AF2B


/*!  \} */

/*! \defgroup WEBIF_Data WebIf Data Packets
*  \{ */

/*! Special value to mark the chunked transfer */
#define WEBIF_CONTENT_LENGTH_CHUNK      0xFFFFFFFF

/*! Maximum value of the request body (content */
#define WEBIF_CONTENT_LENGTH_MAX        (WEBIF_CONTENT_LENGTH_CHUNK - 1)


/*! Data about an eventual authenticated client */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST
{
#define WEBIF_AUTH_USERNAME_SIZE              32

  uint32_t ulGroupBitfield;     /* A user group bit-field,
                                   see Authentication Manager's AUTH_USRDB_USER_GROUP_BF_T type,
                                   0 = user has no rights. */
  uint8_t  szUserName[WEBIF_AUTH_USERNAME_SIZE];
} WEBIF_AUTH_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST WEBIF_IP_ADDR_Ttag
{
  uint32_t   ulIpAddr;          /* Client IP address. */
  uint16_t   usPort;            /* Client TCP port. */
  uint16_t   usReserved;
} WEBIF_IP_ADDR_T;

/*! Data of WEBIF_HANDLE_HTTP_REQUEST_IND packet */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST
{
  uint32_t        ulHttpMethod;                     /*! HTTP Request method, see HTTP_METHOD_T in Http_Defs.h */
  uint32_t        ulContentLength;                  /*! Size of the request body */
  char            aPathName[WEBIF_FIELD_MAX_SIZE];  /*! Request pathname */
  WEBIF_IP_ADDR_T tClientAddr;                      /*! Client Address (V2 only), all fields are
                                                        equal to zero if the remote address is unknown. */
  WEBIF_AUTH_T    tAuth;                            /*! V2 Only: Information about the authentication */
} WEBIF_HANDLE_HTTP_REQUEST_IND_DATA_T;

#define WEBIF_GENERATE_HTTP_REQUEST_IND_DATA_SIZE_V1 ((sizeof(uint32_t) * 2) + (sizeof(char) * WEBIF_FIELD_MAX_SIZE))
#define WEBIF_GENERATE_HTTP_REQUEST_IND_DATA_SIZE_V2 (sizeof(WEBIF_HANDLE_HTTP_REQUEST_IND_DATA_T))

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST
{
  HIL_PACKET_HEADER_T                   tHead;
  WEBIF_HANDLE_HTTP_REQUEST_IND_DATA_T  tData;
} WEBIF_HANDLE_HTTP_REQUEST_IND_T;


/*! Data of WEBIF_HANDLE_HTTP_REQUEST_CONTENT_IND packet */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST
{
  uint32_t  ulOffset;                       /*! Offset of the content part */
  uint32_t  ulDataSize;
  uint8_t   aData[WEBIF_PCK_CONTENT_SIZE];  /*! data */
} WEBIF_HANDLE_HTTP_REQUEST_CONTENT_IND_DATA_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST
{
  HIL_PACKET_HEADER_T                           tHead;
  WEBIF_HANDLE_HTTP_REQUEST_CONTENT_IND_DATA_T  tData;
} WEBIF_HANDLE_HTTP_REQUEST_CONTENT_IND_T;

/*! Data of WEBIF_GET_HTTP_REQUEST_FIELD_REQ packet */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST
{
  uint32_t  uiIsInUri;                    /*! The field is in the URI or in the HTTP header */
  char      aName[WEBIF_FIELD_MAX_SIZE];  /*! Name of the field */
} WEBIF_GET_HTTP_REQUEST_FIELD_REQ_DATA_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST
{
  HIL_PACKET_HEADER_T                     tHead;
  WEBIF_GET_HTTP_REQUEST_FIELD_REQ_DATA_T tData;
} WEBIF_GET_HTTP_REQUEST_FIELD_REQ_T;

/*! Data of WEBIF_GET_HTTP_REQUEST_FIELD_CNF packet */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST
{
  char aContent[WEBIF_FIELD_MAX_SIZE];    /*! Field content */
} WEBIF_GET_HTTP_REQUEST_FIELD_CNF_DATA_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST
{
  HIL_PACKET_HEADER_T                     tHead;
  WEBIF_GET_HTTP_REQUEST_FIELD_CNF_DATA_T tData;
} WEBIF_GET_HTTP_REQUEST_FIELD_CNF_T;


/*! Data of WEBIF_GENERATE_HTTP_RESPONSE_REQ packet */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST
{
  uint32_t  ulStatusCode;                         /*! HTTP status code */
  uint32_t  ulContentLength;                      /*! Size of the response body */
  char      aReasonPhrase[WEBIF_FIELD_MAX_SIZE];  /*! OPTIONAL (V2 only) reason phrase */
} WEBIF_GENERATE_HTTP_RESPONSE_REQ_DATA_T;

#define WEBIF_GENERATE_HTTP_RESPONSE_REQ_DATA_SIZE_V1 (sizeof(uint32_t) * 2)
#define WEBIF_GENERATE_HTTP_RESPONSE_REQ_DATA_SIZE_V2 (sizeof(WEBIF_GENERATE_HTTP_RESPONSE_REQ_DATA_T))

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST
{
  HIL_PACKET_HEADER_T                     tHead;
  WEBIF_GENERATE_HTTP_RESPONSE_REQ_DATA_T tData;
} WEBIF_GENERATE_HTTP_RESPONSE_REQ_T;

/*! Data of WEBIF_GENERATE_HTTP_RESPONSE_FIELD_RE packet */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST
{
  char aName[WEBIF_FIELD_MAX_SIZE];     /*! Field name */
  char aContent[WEBIF_FIELD_MAX_SIZE];  /*! Field content */
} WEBIF_GENERATE_HTTP_RESPONSE_FIELD_REQ_DATA_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST
{
  HIL_PACKET_HEADER_T                           tHead;
  WEBIF_GENERATE_HTTP_RESPONSE_FIELD_REQ_DATA_T tData;
} WEBIF_GENERATE_HTTP_RESPONSE_FIELD_REQ_T;

/*! Data of WEBIF_GENERATE_HTTP_RESPONSE_CONTENT_REQ packet */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST
{
  uint32_t  ulDataSize;
  uint8_t   aData[WEBIF_PCK_CONTENT_SIZE]; /*! Data */
} WEBIF_GENERATE_HTTP_RESPONSE_CONTENT_REQ_DATA_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST
{
  HIL_PACKET_HEADER_T                             tHead;
  WEBIF_GENERATE_HTTP_RESPONSE_CONTENT_REQ_DATA_T tData;
} WEBIF_GENERATE_HTTP_RESPONSE_CONTENT_REQ_T;


/*! WebServer Start request packet. */
typedef HIL_EMPTY_PACKET_T          WEBIF_START_REQ_T;

/*! WebServer Start confirmation packet. */
typedef HIL_EMPTY_PACKET_T          WEBIF_START_CNF_T;

/*! WebServer Stop request packet. */
typedef HIL_EMPTY_PACKET_T          WEBIF_STOP_REQ_T;

/*! WebServer Stop confirmation packet. */
typedef HIL_EMPTY_PACKET_T          WEBIF_STOP_CNF_T;

/*! Data of WEBIF_SET_TCP_PORTS_REQ packet */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST
{
  uint16_t usHttpPort;    /*!< Tcp listening port for non-encrypted transfers.  */
                          /*!< 0 = Disable HTTP Server. */
  uint16_t usHttpsPort;   /*!< Tcp listening port for encrypted transfers.  */
                          /*!< 0 = Disable HTTPS Server. */
  uint32_t ulReserved;    /*!< Reserved for future use. */
} WEBIF_SET_TCP_PORTS_REQ_DATA_T;

/*! Data of WEBIF_SET_DISPATCH_ENTRY_ENABLED_REQ packet */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST
{
  uint8_t   bDispatchId;  /* Target dispatch entry id. */
  uint8_t   bEnabled;     /* Non-zero to enable, zero to disable. */
} WEBIF_SET_DISPATCH_ENTRY_ENABLED_REQ_DATA_T;

/*! Data of WEBIF_SET_DISPATCH_ENTRY_URL_REQ packet */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST
{
  uint8_t   bDispatchId;                      /* Target dispatch entry id. */
  char      szUrl[WEBIF_FIELD_MAX_SIZE];      /* New URL string. Actual string size is determined
                                                 by the packet length. */
} WEBIF_SET_DISPATCH_ENTRY_URL_REQ_DATA_T;

#define WEBIF_SET_DISPATCH_ENTRY_URL_REQ_MIN_LEN  (HIL_OFFSETOF(WEBIF_SET_DISPATCH_ENTRY_URL_REQ_DATA_T, szUrl))
/*!  \} */

/*! \defgroup WEBIF_Packets WebIf Packets
 *  \{ */

/*! Union representing all packet data */
typedef union
{
  WEBIF_HANDLE_HTTP_REQUEST_IND_DATA_T            tRequestIndData;
  WEBIF_HANDLE_HTTP_REQUEST_CONTENT_IND_DATA_T    tRequestContentIndData;

  WEBIF_GET_HTTP_REQUEST_FIELD_REQ_DATA_T         tGetRequestFieldReqData;
  WEBIF_GET_HTTP_REQUEST_FIELD_CNF_DATA_T         tGetRequestFieldCnfData;

  WEBIF_GENERATE_HTTP_RESPONSE_REQ_DATA_T         tResponseReqData;
  WEBIF_GENERATE_HTTP_RESPONSE_FIELD_REQ_DATA_T   tResponseFieldReqData;
  WEBIF_GENERATE_HTTP_RESPONSE_CONTENT_REQ_DATA_T tResponseContentReqData;

  WEBIF_SET_TCP_PORTS_REQ_DATA_T                  tSetTcpPortsReqData;
  WEBIF_SET_DISPATCH_ENTRY_ENABLED_REQ_DATA_T     tSetDispatchEntryEnabledReqData;
  WEBIF_SET_DISPATCH_ENTRY_URL_REQ_DATA_T         tSetDispatchEntryUrlReqData;
} WEBIF_PCK_DATA_T;

/*! WebIf packet */
typedef __HIL_PACKED_PRE struct WEBIF_PCK_Ttag
{
  HIL_PACKET_HEADER_T tHead; /*! Packet header */
  WEBIF_PCK_DATA_T    tData; /*! Packet data */
} __HIL_PACKED_POST WEBIF_PCK_T;

/*!  \} */

#ifdef __HIL_PRAGMA_PACK_ENABLE
  #pragma __HIL_PRAGMA_UNPACK_1(HTTP_IF_API)
#endif

#endif /* WEBIF_API_H_ */
