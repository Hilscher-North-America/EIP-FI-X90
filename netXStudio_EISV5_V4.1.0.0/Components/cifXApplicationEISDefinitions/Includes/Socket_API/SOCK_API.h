/**************************************************************************************
Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
***************************************************************************************
$Id:  $:

Description:
  Packet Socket API definitions
**************************************************************************************/
#ifndef SOCK_API_H_
#define SOCK_API_H_

#include "Hil_Packet.h"

#ifdef __HIL_PRAGMA_PACK_ENABLE
  #pragma __HIL_PRAGMA_PACK_1(SOCKAPI)
#endif

/** Maximum number of FDs to poll */
#define SOCK_LIMITS_MAX_POLL       32

/** Maximum data size */
#define SOCK_LIMITS_MAX_DATA_SIZE  1472

/** ulCmd */
enum SOCK_CMD_Etag
{
  /* generic file descriptor commands */

  /* Not implemented, not applicable
  SOCK_CMD_OPEN_REQ     = 0x00009600,
  SOCK_CMD_OPEN_CNF     = 0x00009601,
  */

  SOCK_CMD_CLOSE_REQ    = 0x00009602,
  SOCK_CMD_CLOSE_CNF    = 0x00009603,

  /* Not implemented, see recv,send and poll
  SOCK_CMD_READ_REQ     = 0x00009604,
  SOCK_CMD_READ_CNF     = 0x00009605,

  SOCK_CMD_WRITE_REQ    = 0x00009606,
  SOCK_CMD_WRITE_CNF    = 0x00009607,

  SOCK_CMD_SELECT_REQ   = 0x00009608,
  SOCK_CMD_SELECT_CNF   = 0x00009609,
  */

  SOCK_CMD_POLL_REQ     = 0x0000960A,
  SOCK_CMD_POLL_CNF     = 0x0000960B,

  SOCK_CMD_FCNTL_REQ    = 0x0000960C,
  SOCK_CMD_FCNTL_CNF    = 0x0000960D,

  /* Not implemented, not required
  SOCK_CMD_IOCTL_REQ    = 0x0000960E,
  SOCK_CMD_IOCTL_CNF    = 0x0000960F,
  */

  /* socket commands */
  SOCK_CMD_SOCKET_REQ      = 0x00009610,
  SOCK_CMD_SOCKET_CNF      = 0x00009611,

  SOCK_CMD_CONNECT_REQ     = 0x00009612,
  SOCK_CMD_CONNECT_CNF     = 0x00009613,

  SOCK_CMD_BIND_REQ        = 0x00009614,
  SOCK_CMD_BIND_CNF        = 0x00009615,

  SOCK_CMD_LISTEN_REQ      = 0x00009616,
  SOCK_CMD_LISTEN_CNF      = 0x00009617,

  SOCK_CMD_ACCEPT_REQ      = 0x00009618,
  SOCK_CMD_ACCEPT_CNF      = 0x00009619,

  SOCK_CMD_RECVFROM_REQ    = 0x0000961A,
  SOCK_CMD_RECVFROM_CNF    = 0x0000961B,

  SOCK_CMD_SENDTO_REQ      = 0x0000961C,
  SOCK_CMD_SENDTO_CNF      = 0x0000961D,

  SOCK_CMD_ABORT_REQ       = 0x0000961E,
  SOCK_CMD_ABORT_CNF       = 0x0000961F,

  SOCK_CMD_SETSOCKOPT_REQ  = 0x00009620,
  SOCK_CMD_SETSOCKOPT_CNF  = 0x00009621,

  SOCK_CMD_GETSOCKOPT_REQ  = 0x00009622,
  SOCK_CMD_GETSOCKOPT_CNF  = 0x00009623,

  /* misc commands */
  SOCK_CMD_GETIFADDRS_REQ  = 0x00009630,
  SOCK_CMD_GETIFADDRS_CNF  = 0x00009631,
};

typedef enum SOCK_CMD_Etag SOCK_CMD_E;

typedef struct SOCK_EMPTY_PCK_Ttag SOCK_EMPTY_PCK_T;
/** Empty packet without additional data
 * \cnfhead
 */
__HIL_PACKED_PRE struct __HIL_PACKED_POST SOCK_EMPTY_PCK_Ttag
{
  HIL_PACKET_HEADER_T tHead;
};

/** The handle of a socket.
 *
 * The values 0,1,2 are reserved and will not be used. Usable range starts with 3.
 * Also there are some special values starting from 0xF000 for notification purposes
 */
typedef uint16_t SOCK_H;

/** Socket handle value to be used in conjunction with poll service
 *  to check for ip address changes */
#define SOCK_HANDLE_IPV4CHANGE 0xF000

/**
 * @defgroup FD_Close_Service File Descriptor Close Service
 * The Socket Close service shall be used by the application to close a socket.
 * \details The service will close and destroy the associated socket object.
 * @{
 */
/* tag::SOCK-CMD-CLOSE-REQ[] */
typedef struct SOCK_CLOSE_REQ_DATA_Ttag SOCK_CLOSE_REQ_DATA_T;

__HIL_PACKED_PRE struct __HIL_PACKED_POST SOCK_CLOSE_REQ_DATA_Ttag
{
  /** Socket or file descriptor handle */
  SOCK_H hSocket;
};

typedef struct SOCK_CLOSE_REQ_Ttag SOCK_CLOSE_REQ_T;

/** This packet shall be send by the application in order to close a socket object.
  * \reqhead
  */
__HIL_PACKED_PRE struct __HIL_PACKED_POST SOCK_CLOSE_REQ_Ttag
{
  HIL_PACKET_HEADER_T   tHead;

  SOCK_CLOSE_REQ_DATA_T tData;
};
/* end::SOCK-CMD-CLOSE-REQ[] */

/* tag::SOCK-CMD-CLOSE-CNF[] */
/** The packet will be returned to the application after executing the @ref SOCK_CLOSE_REQ_Ttag packet.
  *
  * \details
  * The associated file or socket object has been destroyed, thus the socket handle is now invalid.
  * \note Closing a TCP socket may take up to 120 seconds delay.
  * This is necessary according TCP protocol specification in some states.
  * \cnfhead
  */
typedef struct SOCK_EMPTY_PCK_Ttag SOCK_CLOSE_CNF_T;
/* end::SOCK-CMD-CLOSE-CNF[] */

typedef union SOCK_CLOSE_PCK_Ttag SOCK_CLOSE_PCK_T;

union SOCK_CLOSE_PCK_Ttag
{
  SOCK_CLOSE_REQ_T tReq;

  SOCK_CLOSE_CNF_T tCnf;
};
/** @} */

/**
 * @defgroup FD_Poll_Service File Descriptor Poll Service
 * The Socket Poll service shall be used by the application to wait for an event on a socket.
 * \details The service will return which file descriptors has pending events.
 * @{
 */

/* tag::SOCK-CMD-POLL-REQ[] */
/** Poll event bitmask, these are defined similar to posix values */
enum SOCK_POLL_EVENT_Etag
{
  SOCK_POLLIN   = 0x0001, /*!< The socket is ready for reading data */
  SOCK_POLLPRI  = 0x0002, /*!< The socket has received high priority data and is ready for reading */
  SOCK_POLLOUT  = 0x0004, /*!< The socket is ready for writing data */
  SOCK_POLLERR  = 0x0008, /*!< The socket had an error (always active) */
  SOCK_POLLHUP  = 0x0010, /*!< The remote peer has closed the connection (always active) */
  SOCK_POLLNVAL = 0x0020, /*!< Invalid socket handle (always active) */
};

typedef enum SOCK_POLL_EVENT_Etag SOCK_POLL_EVENT_E;

typedef struct SOCK_POLL_Ttag SOCK_POLL_T;

__HIL_PACKED_PRE struct __HIL_PACKED_POST SOCK_POLL_Ttag
{
  /** The file handle to poll */
  SOCK_H   hSocket;
  /** @ref SOCK_POLL_EVENT_Etag "Event Mask" to specify for which events to wait.
    * Multiple events should be combined by performing an OR-operation with the corresponding bitmasks to a single value.
    * Some events are always enabled (e.g.: Errors).
    */
  uint16_t usEventMsk;
};

typedef struct SOCK_POLL_REQ_DATA_Ttag SOCK_POLL_REQ_DATA_T;

__HIL_PACKED_PRE struct __HIL_PACKED_POST SOCK_POLL_REQ_DATA_Ttag
{
  /** Polling Timeout in milliseconds.
    * Value 0 means to just check current events and immediate return.
    * Negative Value means infinite wait until event occurs.
    * */
  int32_t     ilTimeout;
  /** Array of poll descriptor. One entry for each file descriptor to poll.
   * Actual array size might be smaller and must be specified in header.
   */
  SOCK_POLL_T atFds[SOCK_LIMITS_MAX_POLL];
};

typedef struct SOCK_POLL_REQ_Ttag SOCK_POLL_REQ_T;

/** This packet shall be sent by the application in order to wait for file descriptors.
  * \details The length of the packet shall be set according the number of sockets to be polled.
  *
  * For example: Polling of two sockets' results in 4+2x4 = 12 Bytes.
  * \reqhead
  */
__HIL_PACKED_PRE struct __HIL_PACKED_POST SOCK_POLL_REQ_Ttag
{
  HIL_PACKET_HEADER_T  tHead;

  SOCK_POLL_REQ_DATA_T tData;
};
/* end::SOCK-CMD-POLL-REQ[] */

 /* tag::SOCK-CMD-POLL-CNF[] */
typedef struct SOCK_POLL_CNF_DATA_Ttag SOCK_POLL_CNF_DATA_T;

__HIL_PACKED_PRE struct __HIL_PACKED_POST SOCK_POLL_CNF_DATA_Ttag
{
  /** Number of file descriptors with events */
  int32_t     ilNumFd;
  /** Array of file descriptors to poll
   * Actual array size might be smaller and must be specified in header
   */
  SOCK_POLL_T atFds[SOCK_LIMITS_MAX_POLL];
};

typedef struct SOCK_POLL_CNF_Ttag SOCK_POLL_CNF_T;

/** The packet will be returned immediately to the application if any of the polled file descriptors has a pending event or a timeout of zero had been specified in the request.
  * The packet will be returned after the specified timeout if no event occurred within the given interval.
  * \cnfhead
  */
__HIL_PACKED_PRE struct __HIL_PACKED_POST SOCK_POLL_CNF_Ttag
{
  /** @ref HIL_PACKET_HEADER_Ttag */
  HIL_PACKET_HEADER_T  tHead;
  /** @ref SOCK_POLL_CNF_DATA_Ttag */
  SOCK_POLL_CNF_DATA_T tData;
};
/* end::SOCK-CMD-POLL-CNF[] */

typedef union SOCK_POLL_PCK_Ttag SOCK_POLL_PCK_T;

union SOCK_POLL_PCK_Ttag
{
  SOCK_POLL_REQ_T tReq;

  SOCK_POLL_CNF_T tCnf;
};
/** @} */

/**
 * @defgroup FD_Fcntl_Service File Descriptor Control Service
 * The Socket Control service shall be used by the application to perform a specific control operation on a socket.
 *
 * \details For example: It is used to switch a socket in non-blocking mode.
 * Currently only setting/getting non-blocking I/O Mode is supported.
 * @{
 */

/* tag::SOCK-CMD-FCNTL-REQ[] */
/** FD Control Command Codes */
typedef enum
{
  /* not implemented
  SOCK_FCNTL_GETFD = 1,
  SOCK_FCNTL_SETFD = 2,
  */
  SOCK_FCNTL_GETFL = 3, /*!< Get status flag */
  SOCK_FCNTL_SETFL = 4, /*!< Set status flag */
} SOCK_FCNTL_E;

/** File descriptor status flag bitmask. */
enum SOCK_STATUS_FLAGS_Etag
{
  /** Makes socket operations except connect non blocking */
  SOCK_FL_O_NONBLOCK = 0x0800,
  /** Makes socket operations including connect non blocking */
  SOCK_FL_O_NONBLOCK_CONNECT = 0x1000,
};

typedef enum SOCK_STATUS_FLAGS_Etag SOCK_STATUS_FLAGS_E;

typedef struct SOCK_FCNTL_COM_Ttag SOCK_FCNTL_COM_T;

__HIL_PACKED_PRE struct __HIL_PACKED_POST SOCK_FCNTL_COM_Ttag
{
  /** The file descriptor to perform the fcntl on */
  SOCK_H   hSocket;
  /**Command code to perform @ref SOCK_FCNTL_E */
  uint16_t usFcntl;
};

typedef struct SOCK_FCNTL_FL_Ttag SOCK_FCNTL_FL_T;

__HIL_PACKED_PRE struct __HIL_PACKED_POST SOCK_FCNTL_FL_Ttag
{
  /** The file descriptor to perform the fcntl on */
  SOCK_H   hSocket;
  /**Command code to perform @ref SOCK_FCNTL_E */
  uint16_t usFcntl;
  /** File descriptor @ref SOCK_STATUS_FLAGS_Etag "status flags bitmask". */
  uint32_t ulStatusFlags;
};

typedef union SOCK_FCNTL_DATA_Ttag SOCK_FCNTL_DATA_T;

union SOCK_FCNTL_DATA_Ttag
{
  /** Union element describing the fields common to all fcntl operations */
  SOCK_FCNTL_COM_T tCom;
  /** Union element for use with F_GETFL/F_SETFL Descriptor Status Flag command code */
  SOCK_FCNTL_FL_T  tFileStatus;
};

typedef struct SOCK_FCNTL_REQ_Ttag SOCK_FCNTL_REQ_T;

/** This packet shall be send by the application in order to perform a socket control.
  * \reqhead
  */
__HIL_PACKED_PRE struct __HIL_PACKED_POST SOCK_FCNTL_REQ_Ttag
{
  HIL_PACKET_HEADER_T tHead;

  SOCK_FCNTL_DATA_T   tData;
};
/* end::SOCK-CMD-FCNTL-REQ[] */

/* tag::SOCK-CMD-FCNTL-CNF[] */
typedef struct SOCK_FCNTL_CNF_Ttag SOCK_FCNTL_CNF_T;

/** The packet will be returned to the application after performing the file descriptor control operation.
  * \cnfhead
  */
__HIL_PACKED_PRE struct __HIL_PACKED_POST SOCK_FCNTL_CNF_Ttag
{
  HIL_PACKET_HEADER_T tHead;

  SOCK_FCNTL_DATA_T   tData;
};
/* end::SOCK-CMD-FCNTL-CNF[] */

typedef union SOCK_FCNTL_PCK_Ttag SOCK_FCNTL_PCK_T;

union SOCK_FCNTL_PCK_Ttag
{
  SOCK_FCNTL_REQ_T tReq;

  SOCK_FCNTL_CNF_T tCnf;
};
/** @}*/

/**
 * @defgroup SOCK_Socket_Service Create a Socket Service
 * The application has to create a new socket for communication.
 *
 * \details Valid combinations for domain, socket and protocol are:
 * Domain            | Socket Type             | Protocol              | Connection
 * ------------------|-------------------------|-----------------------|-----------
 * @ref SOCK_AF_INET | @ref SOCK_SOCKET_STREAM | @ref SOCK_IPPROTO_IP  | TCP IPv4
 * @ref SOCK_AF_INET | @ref SOCK_SOCKET_STREAM | @ref SOCK_IPPROTO_TCP | TCP IPv4
 * @ref SOCK_AF_INET | @ref SOCK_SOCKET_DGRAM  | @ref SOCK_IPPROTO_UDP | UDP IPv4
 * @{
 */

/* tag::SOCK-SOCKET-DOMAIN[] */
enum SOCK_SOCKET_DOMAIN_Etag
{
  SOCK_AF_INET   = 2,
  SOCK_AF_INET6  = 10,
  SOCK_AF_PACKET = 17,
};
/* end::SOCK-SOCKET-DOMAIN[] */

/* tag::SOCK-SOCKET-DOMAIN[] */
typedef enum SOCK_SOCKET_DOMAIN_Etag SOCK_SOCKET_DOMAIN_E;
/* end::SOCK-SOCKET-DOMAIN[] */

/* tag::SOCK-SOCKET-TYPE[] */
/* tag::SOCK-SOCKET-DOMAIN[] */
enum SOCK_SOCKET_TYPE_Etag
{
  SOCK_SOCKET_STREAM = 1,
  SOCK_SOCKET_DGRAM  = 2,
};
/* end::SOCK-SOCKET-DOMAIN[] */
/* end::SOCK-SOCKET-TYPE[] */

/* tag::SOCK-SOCKET-TYPE[] */
typedef enum SOCK_SOCKET_TYPE_Etag SOCK_SOCKET_TYPE_E;
/* end::SOCK-SOCKET-TYPE[] */

/* tag::SOCK-SOCKET-TYPE[] */
enum SOCK_IPPROTO_Etag
{
  SOCK_SOL_SOCKET = 1,

  /** Dummy, alias for TCP */
  SOCK_IPPROTO_IP  = 0,
  /** TCP */
  SOCK_IPPROTO_TCP = 6,
  /** UDP */
  SOCK_IPPROTO_UDP = 17,
};

typedef enum SOCK_IPPROTO_Etag SOCK_IPPROTO_E;

typedef struct SOCK_SOCKET_REQ_DATA_Ttag SOCK_SOCKET_REQ_DATA_T;

__HIL_PACKED_PRE struct __HIL_PACKED_POST SOCK_SOCKET_REQ_DATA_Ttag
{
  /** @ref SOCK_SOCKET_DOMAIN_Etag "The socket domain" */
  uint32_t ulDomain;
  /** @ref SOCK_SOCKET_TYPE_Etag "The socket type" */
  uint32_t ulType;
  /** @ref SOCK_IPPROTO_Etag "The socket protocol" */
  uint32_t ulProtocol;
};

typedef struct SOCK_SOCKET_REQ_Ttag SOCK_SOCKET_REQ_T;

/** The application has to send this packet to create a new socket.
  * \details
  * \reqhead
  */
__HIL_PACKED_PRE struct __HIL_PACKED_POST SOCK_SOCKET_REQ_Ttag
{
  HIL_PACKET_HEADER_T    tHead;

  SOCK_SOCKET_REQ_DATA_T tData;
};
/* end::SOCK-SOCKET-TYPE[] */

/* tag::SOCK-SOCKET-CNF[] */
typedef struct SOCK_SOCKET_CNF_DATA_Ttag SOCK_SOCKET_CNF_DATA_T;

__HIL_PACKED_PRE struct __HIL_PACKED_POST SOCK_SOCKET_CNF_DATA_Ttag
{
  SOCK_H hSocket;
};

typedef struct SOCK_SOCKET_CNF_Ttag SOCK_SOCKET_CNF_T;

/** This packet will be returned to the application as a confirmation to the @ref SOCK_SOCKET_REQ_DATA_Ttag packet.
  * Depending on the status of the operation, the confirmation packet contains the new socket handle or an error code.
  * \details
  * \reqhead
  */
__HIL_PACKED_PRE struct __HIL_PACKED_POST SOCK_SOCKET_CNF_Ttag
{
  HIL_PACKET_HEADER_T    tHead;

  SOCK_SOCKET_CNF_DATA_T tData;
};
/* end::SOCK-SOCKET-CNF[] */

typedef union SOCK_SOCKET_PCK_Ttag SOCK_SOCKET_PCK_T;

union SOCK_SOCKET_PCK_Ttag
{
  SOCK_SOCKET_REQ_T tReq;

  SOCK_SOCKET_CNF_T tCnf;
};
/* end::SOCK-SOCKET-SERVICE[] */
/** @}*/

/* tag::SOCK-SOCKET-DOMAIN[] */
typedef struct SOCK_ADDR_COMMON_Ttag SOCK_ADDR_COMMON_T;

__HIL_PACKED_PRE struct __HIL_PACKED_POST SOCK_ADDR_COMMON_Ttag
{
  /** Socket Family / Domain */
  uint8_t bFamily;
  /** Placeholder for family specific data */
  uint8_t bReserved[14];
  /** Padding for alignment */
  uint8_t bPadding;
};

typedef struct SOCK_ADDR_IP_Ttag SOCK_ADDR_IP_T;

__HIL_PACKED_PRE struct __HIL_PACKED_POST SOCK_ADDR_IP_Ttag
{
  /** Set to @ref SOCK_AF_INET */
  uint8_t  bFamily;
  /** Padding for alignment */
  uint8_t  bPadding;
  /** IP Address of endpoint */
  uint32_t ulIpAddr;
  /** Port of endpoint */
  uint16_t usPort;
};

typedef struct SOCK_ADDR_LL_Ttag SOCK_ADDR_LL_T;

__HIL_PACKED_PRE struct __HIL_PACKED_POST SOCK_ADDR_LL_Ttag
{
  /** Set to @ref SOCK_AF_PACKET */
  uint8_t bFamily;
  /** index of the interface, starts from 1 (Zero means all interfaces) */
  uint8_t bIfIndex;
  /** Padding for alignment */
  uint8_t abPadding[4];
  /** reserved for future usage */
  uint8_t bPktType;
  /** Length of hardware address */
  uint8_t bLlAddrLen;
  /** Up to 8 bytes hardware address */
  uint8_t abLlAddr[8];
};

typedef union SOCK_ADDR_Ttag SOCK_ADDR_T;

union SOCK_ADDR_Ttag
{
  /** @ref SOCK_ADDR_COMMON_Ttag
   *
   * Contains common fields of all socket address structures
   */
  SOCK_ADDR_COMMON_T tCommon;
  /** IPv4 specific socket address */
  SOCK_ADDR_IP_T     tIp;
  /** Packet (Layer 2) socket address */
  SOCK_ADDR_LL_T     tLL;
};
/* end::SOCK-SOCKET-DOMAIN[] */

/* tag::SOCK-CMD-CONNECT-REQ[] */
/* tag::SOCK-CMD-BIND-REQ[] */
typedef struct SOCK_ADDR_DATA_Ttag SOCK_ADDR_DATA_T;

__HIL_PACKED_PRE struct __HIL_PACKED_POST SOCK_ADDR_DATA_Ttag
{
  SOCK_H      hSocket;

  SOCK_ADDR_T tSa;
};
/* end::SOCK-CMD-BIND-REQ[] */
/* end::SOCK-CMD-CONNECT-REQ[] */

/**
 * @defgroup SOCK_Connect_Service Connect Socket Service
 *
 * The application has to use this service to connect a socket to a remote endpoint.
 * \attention This service is mandatory for TCP client applications.
 *
 * @{
 */

/* tag::SOCK-CMD-CONNECT-REQ[] */
typedef struct SOCK_CONNECT_REQ_Ttag SOCK_CONNECT_REQ_T;

/** The application has to send this packet in order to bind the socket to a local endpoint.
  * \reqhead
  */
__HIL_PACKED_PRE struct __HIL_PACKED_POST SOCK_CONNECT_REQ_Ttag
{
  HIL_PACKET_HEADER_T tHead;

  SOCK_ADDR_DATA_T    tData;
};
/* end::SOCK-CMD-CONNECT-REQ[] */

/* tag::SOCK-CMD-CONNECT-CNF[] */
/** The packet will be returned to application for the @ref SOCK_CONNECT_REQ_Ttag packet.
  */
typedef struct SOCK_EMPTY_PCK_Ttag SOCK_CONNECT_CNF_T;
/* end::SOCK-CMD-CONNECT-CNF[] */

typedef union SOCK_CONNECT_PCK_Ttag SOCK_CONNECT_PCK_T;

union SOCK_CONNECT_PCK_Ttag
{
  SOCK_CONNECT_REQ_T tReq;

  SOCK_CONNECT_CNF_T tCnf;
};
/** @}*/

/**
 * @defgroup SOCK_Bind_Service Bind Socket Service
 * The application has to use this service to bind a socket to a local endpoint address.
 *
 * \details This service is mandatory for any server application.
 * In case of Internet Domain, the IP address might be set to zero which means 'any local IP address'.
 *
 * If the port number is set to zero, a port will be automatically selected.
 * The assigned port number can be determined from the confirmation packet.
 * @{
 */
/* tag::SOCK-CMD-BIND-REQ[] */
typedef struct SOCK_BIND_REQ_Ttag SOCK_BIND_REQ_T;

/** The application has to send this packet in order to bind the socket to a local endpoint. */
__HIL_PACKED_PRE struct __HIL_PACKED_POST SOCK_BIND_REQ_Ttag
{
  HIL_PACKET_HEADER_T tHead;

  SOCK_ADDR_DATA_T    tData;
};
/* end::SOCK-CMD-BIND-REQ[] */

/* tag::SOCK-CMD-BIND-CNF[] */
typedef struct SOCK_BIND_CNF_Ttag SOCK_BIND_CNF_T;

/** This packet will be returned to the application as a confirmation to the @ref SOCK_BIND_REQ_Ttag packet.
  * If the IP Port was set to Zero in the request packet, the confirmation will contain the automatically assigned port number value.
  */
__HIL_PACKED_PRE struct __HIL_PACKED_POST SOCK_BIND_CNF_Ttag
{
  HIL_PACKET_HEADER_T tHead;

  SOCK_ADDR_DATA_T    tData;
};
/* end::SOCK-CMD-BIND-CNF[] */

typedef union SOCK_BIND_PCK_Ttag SOCK_BIND_PCK_T;

union SOCK_BIND_PCK_Ttag
{
  SOCK_BIND_REQ_T tReq;

  SOCK_BIND_CNF_T tCnf;
};
/**@}*/

/**
 * @defgroup SOCK_Listen_Service Socket Listen Service
 * The Socket Listen service is used by the application to switch a socket into passive mode.
 *
 * \details This mode is relevant for TCP sockets in order to listen for incoming connections.
 * \mandatorytcp
 * \noudp
 * @{
 */
/* tag::SOCK_CMD_LISTEN_REQ[] */
typedef struct SOCK_LISTEN_REQ_DATA_Ttag SOCK_LISTEN_REQ_DATA_T;

__HIL_PACKED_PRE struct __HIL_PACKED_POST SOCK_LISTEN_REQ_DATA_Ttag
{
  /** Handle of the socket to switch into listen mode */
  SOCK_H   hSocket;
  /** Maximum number of pending and active connections to this socket.
   *
   * \attention For any possible connection a full socket will be preallocated within Socket API component.
   */
  uint16_t usBackLog;
};

typedef struct SOCK_LISTEN_REQ_Ttag SOCK_LISTEN_REQ_T;

/* This packet has to be used by the application to switch the socket into passive mode.
 * In passive mode, the socket will be waiting for incoming connections and create a new socket for each incoming connection.
 * The resources for such a connection socket are taken from the listening socket's backlog. (resource pool).
 * This service is useful for connection oriented sockets only.
 */
__HIL_PACKED_PRE struct __HIL_PACKED_POST SOCK_LISTEN_REQ_Ttag
{
  HIL_PACKET_HEADER_T    tHead;

  SOCK_LISTEN_REQ_DATA_T tData;
};
/* end::SOCK_CMD_LISTEN_REQ[] */

/* tag::SOCK_CMD_LISTEN_CNF[] */
typedef struct SOCK_EMPTY_PCK_Ttag SOCK_LISTEN_CNF_T;
/* end::SOCK_CMD_LISTEN_CNF[] */

typedef union SOCK_LISTEN_PCK_Ttag SOCK_LISTEN_PCK_T;

union SOCK_LISTEN_PCK_Ttag
{
  SOCK_LISTEN_REQ_T tReq;

  SOCK_LISTEN_CNF_T tCnf;
};
/**@}*/

/**
 * @defgroup SOCK_Accept_Service Socket Accept Service
 * The Socket Accept service is used by the application to accept a pending connection request on a connection oriented socket in listen mode.
 * \mandatorytcp
 * \noudp
 * @{
 */
/* tag::SOCK-CMD-ACCEPT-REQ[] */
typedef struct SOCK_ACCEPT_REQ_DATA_Ttag SOCK_ACCEPT_REQ_DATA_T;

__HIL_PACKED_PRE struct __HIL_PACKED_POST SOCK_ACCEPT_REQ_DATA_Ttag
{
  /** Handle of a socket in listen mode */
  SOCK_H   hSocket;
  /** Reserved for future extensions set to zero */
  uint16_t usFlags;
};

/** This packet shall be sent by the application in order to accept a pending connection of a socket.
 * \reqhead
 */
typedef struct SOCK_ACCEPT_REQ_Ttag SOCK_ACCEPT_REQ_T;

__HIL_PACKED_PRE struct __HIL_PACKED_POST SOCK_ACCEPT_REQ_Ttag
{
  HIL_PACKET_HEADER_T    tHead;

  SOCK_ACCEPT_REQ_DATA_T tData;
};
/* end::SOCK-CMD-ACCEPT-REQ[] */

/* tag::SOCK-CMD-ACCEPT-CNF[] */
typedef struct SOCK_ACCEPT_CNF_DATA_Ttag SOCK_ACCEPT_CNF_DATA_T;

__HIL_PACKED_PRE struct __HIL_PACKED_POST SOCK_ACCEPT_CNF_DATA_Ttag
{
  /** the socket handle of the new connection */
  SOCK_H      hAcceptSocket;
  /** address information */
  SOCK_ADDR_T tSa;
};

/** The packet will be returned to the application for the @ref SOCK_ACCEPT_REQ_Ttag packet when a new connection is available.
  * If the socket is in Blocking Mode (default), the packet will not be returned until the socket is closed or a connection request is received.
  * \cnfhead
  */
typedef struct SOCK_ACCEPT_CNF_Ttag SOCK_ACCEPT_CNF_T;

__HIL_PACKED_PRE struct __HIL_PACKED_POST SOCK_ACCEPT_CNF_Ttag
{
  HIL_PACKET_HEADER_T    tHead;

  SOCK_ACCEPT_CNF_DATA_T tData;
};
/* end::SOCK-CMD-ACCEPT-CNF[] */

typedef union SOCK_ACCEPT_PCK_Ttag SOCK_ACCEPT_PCK_T;

union SOCK_ACCEPT_PCK_Ttag
{
  SOCK_ACCEPT_REQ_T tReq;

  SOCK_ACCEPT_CNF_T tCnf;
};
/**@}*/

/**
 * @defgroup SOCK_RecvFrom_Service Socket RecvFrom Service
 * The Receive From service is used by the application to retrieve received data from a socket.
 * @{
 */

/* tag::SOCK-CMD-RECVFROM-REQ[] */
typedef struct SOCK_RECVFROM_REQ_DATA_Ttag SOCK_RECVFROM_REQ_DATA_T;

__HIL_PACKED_PRE struct __HIL_PACKED_POST SOCK_RECVFROM_REQ_DATA_Ttag
{
  /** Handle of a socket to retrieve data from */
  SOCK_H   hSocket;
  /** Reserved for future extensions set to zero */
  uint16_t usFlags;
  /** Maximum Number of Receive Data to return in Confirmation.
    * If this length is smaller than a pending datagram for Datagram Sockets the excess data will be dropped.
    *
    * Range: @ref SOCK_LIMITS_MAX_DATA_SIZE
    */
  uint16_t usMaxLen;
};

/** This packet shall be send by the application in order to retrieve received data from a socket. */
typedef struct SOCK_RECVFROM_REQ_Ttag SOCK_RECVFROM_REQ_T;
__HIL_PACKED_PRE struct __HIL_PACKED_POST SOCK_RECVFROM_REQ_Ttag
{
  HIL_PACKET_HEADER_T      tHead;

  SOCK_RECVFROM_REQ_DATA_T tData;
};
/* end::SOCK-CMD-RECVFROM-REQ[] */

/* tag::SOCK-CMD-RECVFROM-CNF[] */
typedef struct SOCK_RECVFROM_CNF_DATA_Ttag SOCK_RECVFROM_CNF_DATA_T;

__HIL_PACKED_PRE struct __HIL_PACKED_POST SOCK_RECVFROM_CNF_DATA_Ttag
{
  /** Remote Endpoint Address associated with the data. Only valid for datagram sockets (UDP). */
  SOCK_ADDR_T tSa;
  /** Payload Data of received datagram or stream. */
  uint8_t     abPayload[SOCK_LIMITS_MAX_DATA_SIZE];
};

/** The packet will be returned to application for the @ref SOCK_RECVFROM_REQ_Ttag packet when data is available for the socket.
  * If the socket is in Blocking Mode (default), the packet will not be returned until the socket is closed or data is available.
  */
typedef struct SOCK_RECVFROM_CNF_Ttag SOCK_RECVFROM_CNF_T;

__HIL_PACKED_PRE struct __HIL_PACKED_POST SOCK_RECVFROM_CNF_Ttag
{
  HIL_PACKET_HEADER_T      tHead;

  SOCK_RECVFROM_CNF_DATA_T tData;
};
/* end::SOCK-CMD-RECVFROM-CNF[] */

typedef union SOCK_RECVFROM_PCK_Ttag SOCK_RECVFROM_PCK_T;

union SOCK_RECVFROM_PCK_Ttag
{
  SOCK_RECVFROM_REQ_T tReq;

  SOCK_RECVFROM_CNF_T tCnf;
};
/**@}*/

/**
 * @defgroup SOCK_SendTo_Service Socket SendTo Service
 * The Send To service is used by the application to send data to a remote endpoint via a socket.
 * @{
 */
/* tag::SOCK-CMD-SENDTO-REQ[] */
typedef struct SOCK_SENDTO_REQ_DATA_Ttag SOCK_SENDTO_REQ_DATA_T;

__HIL_PACKED_PRE struct __HIL_PACKED_POST SOCK_SENDTO_REQ_DATA_Ttag
{
  SOCK_H      hSocket;
  /** Reserved for future extensions set to zero */
  uint16_t    usFlags;
  /** remote address information */
  SOCK_ADDR_T tSa;
  /** data */
  uint8_t     abBuffer[SOCK_LIMITS_MAX_DATA_SIZE];
};

typedef struct SOCK_SENDTO_REQ_Ttag SOCK_SENDTO_REQ_T;

__HIL_PACKED_PRE struct __HIL_PACKED_POST SOCK_SENDTO_REQ_Ttag
{
  HIL_PACKET_HEADER_T    tHead;

  SOCK_SENDTO_REQ_DATA_T tData;
};
/* end::SOCK-CMD-SENDTO-REQ[] */

/* tag::SOCK-CMD-SENDTO-CNF[] */
typedef struct SOCK_EMPTY_PCK_Ttag SOCK_SENDTO_CNF_T;
/* end::SOCK-CMD-SENDTO-CNF[] */

typedef union SOCK_SENDTO_PCK_Ttag SOCK_SENDTO_PCK_T;

union SOCK_SENDTO_PCK_Ttag
{
  SOCK_SENDTO_REQ_T tReq;

  SOCK_SENDTO_CNF_T tCnf;
};
/**@}*/

/**
 * @defgroup SOCK_Abort_Service Socket Abort Service
 * The Socket Abort service shall be used by the application to close a socket immediately.
 *
 * \details This service is useful in the context of TCP connections.
 *
 * According to the TCP standard, a TCP socket must be held open in a special state for twice the Maximum Segment Lifetime after closing it (2x 60 seconds).
 * This is necessary to ensure that all pending incoming fragments are collected and acknowledged.
 * During this time the socket resource is still in use and cannot be reused by another connection.
 *
 * In several circumstances, it might be required to force a connection close without this timeout and immediately release the associated socket resource.
 * This can be accomplished by using this service. The service will close the socket and releases the socket resource immediately.
 *
 * \note This service is not a general replacement for the @ref FD_Close_Service. \n
 * It might cause a violation of the TCP protocol and result in communication problems with network peers!
 * @{
 */
/* tag::SOCK-CMD-ABORT-REQ[] */
typedef struct SOCK_ABORT_REQ_DATA_Ttag SOCK_ABORT_REQ_DATA_T;

__HIL_PACKED_PRE struct __HIL_PACKED_POST SOCK_ABORT_REQ_DATA_Ttag
{
  /** Socket or file descriptor handle */
  SOCK_H hSocket;
};

typedef struct SOCK_ABORT_REQ_Ttag SOCK_ABORT_REQ_T;

/** This packet shall be sent by the application in order to abort a connection.
 * \reqhead
 */
__HIL_PACKED_PRE struct __HIL_PACKED_POST SOCK_ABORT_REQ_Ttag
{
  HIL_PACKET_HEADER_T   tHead;

  SOCK_ABORT_REQ_DATA_T tData;
};
/* end::SOCK-CMD-ABORT-REQ[] */

/* tag::SOCK-CMD-ABORT-CNF[] */
/** The packet will be returned to the application after executing a @ref SOCK_ABORT_REQ_Ttag` packet.
  * Afterwards the associated file or socket object has been destroyed, thus the socket handle is invalid then.
  * \cnfhead
  */
typedef struct SOCK_EMPTY_PCK_Ttag SOCK_ABORT_CNF_T;
/* end::SOCK-CMD-ABORT-CNF[] */

typedef union SOCK_ABORT_PCK_Ttag SOCK_ABORT_PCK_T;

union SOCK_ABORT_PCK_Ttag
{
  SOCK_ABORT_REQ_T tReq;

  SOCK_ABORT_CNF_T tCnf;
};
/** @} */

/* tag::TCP-SOCKET-OPTIONS[] */
/** TCP socket options */
enum
{
  SOCK_TCP_NODELAY    = 1,    /*!<@ref SOCK_SOCKOPT_TCP_NODELAY_Ttag */
  SOCK_TCP_KEEPCNT    = 0x05, /*!< @ref SOCK_SOCKOPT_TCP_KEEPALIVE_CNT_Ttag */
  SOCK_TCP_KEEPIDLE   = 0x06, /*!< @ref SOCK_SOCKOPT_TCP_KEEPALIVE_IDLE_Ttag */
  SOCK_TCP_KEEPINTVL  = 0x07, /*!< @ref SOCK_SOCKOPT_TCP_KEEPALIVE_INTVL_Ttag */
  SOCK_SO_KEEPALIVE   = 0x08, /*!< @ref SOCK_SOCKOPT_TCP_KEEPALIVE_Ttag */
  SOCK_SO_LINGER      = 0x09, /*!< @ref SOCK_SOCKOPT_TCP_LINGER_Ttag */
  SOCK_SO_REUSEADDR   = 0x0A, /*!< @ref SOCK_SOCKOPT_SO_REUSEADDR_Ttag */
  SOCK_SO_BROADCAST   = 0x0B,
  SOCK_SO_ERROR       = 0xF1, /*!< @ref SOCK_SOCKOPT_SO_ERROR_Ttag */
};

/** Set sockopt tcp no delay */
typedef struct SOCK_SOCKOPT_TCP_NODELAY_Ttag SOCK_SOCKOPT_TCP_NODELAY_T;

__HIL_PACKED_PRE struct __HIL_PACKED_POST SOCK_SOCKOPT_TCP_NODELAY_Ttag
{
  /** Set to true if socket shall immediately send out the data */
  int32_t lEnabled;
  /** Padding to unique size. Set to zero in @ref SOCK_SETSOCKOPT_REQ_Ttag,
   *  ignore in Get Sockopt Confirmation */
  int32_t lReserved;
};

typedef struct SOCK_SOCKOPT_TCP_KEEPALIVE_INTVL_Ttag SOCK_SOCKOPT_TCP_KEEPALIVE_INTVL_T;
__HIL_PACKED_PRE struct __HIL_PACKED_POST SOCK_SOCKOPT_TCP_KEEPALIVE_INTVL_Ttag
{
  /** The time in seconds between two non-acknowledged keep alive probes. */
  int32_t lKeepIntvl;
  /** Padding to unique size. Set to zero in @ref SOCK_SETSOCKOPT_REQ_Ttag,
   *  ignore in Get Sockopt Confirmation */
  int32_t lReserved;
};

typedef struct SOCK_SOCKOPT_TCP_KEEPALIVE_IDLE_Ttag SOCK_SOCKOPT_TCP_KEEPALIVE_IDLE_T;
__HIL_PACKED_PRE struct __HIL_PACKED_POST SOCK_SOCKOPT_TCP_KEEPALIVE_IDLE_Ttag
{
  /** The time in seconds the connection needs to remain idle before starts sending keep alive probes */
  int32_t lKeepIdle;
  /** Padding to unique size. Set to zero in @ref SOCK_SETSOCKOPT_REQ_Ttag,
   *  ignore in Get Sockopt Confirmation */
  int32_t lReserved;
};

typedef struct SOCK_SOCKOPT_TCP_KEEPALIVE_CNT_Ttag SOCK_SOCKOPT_TCP_KEEPALIVE_CNT_T;
__HIL_PACKED_PRE struct __HIL_PACKED_POST SOCK_SOCKOPT_TCP_KEEPALIVE_CNT_Ttag
{
  /** Set the number of keep alive probes send before considering the connection dead. */
  int32_t lKeepCnt;
  /** Padding to unique size. Set to zero in @ref SOCK_SETSOCKOPT_REQ_Ttag,
   *  ignore in Get Sockopt Confirmation */
  int32_t lReserved;
};

typedef struct SOCK_SOCKOPT_TCP_KEEPALIVE_Ttag SOCK_SOCKOPT_TCP_KEEPALIVE_T;
__HIL_PACKED_PRE struct __HIL_PACKED_POST SOCK_SOCKOPT_TCP_KEEPALIVE_Ttag
{
  /** The time in seconds between two non-acknowledged keep alive probes. */
  int32_t lKeepAlive;
  /** Padding to unique size. Set to zero in @ref SOCK_SETSOCKOPT_REQ_Ttag,
   *  ignore in Get Sockopt Confirmation */
  int32_t lReserved;
};

typedef struct SOCK_SOCKOPT_TCP_LINGER_Ttag SOCK_SOCKOPT_TCP_LINGER_T;
__HIL_PACKED_PRE struct __HIL_PACKED_POST SOCK_SOCKOPT_TCP_LINGER_Ttag
{
  /** Configure the linger timeout, in order to set the maximum time of _TIMEWAIT_ state.
   * After timeout, the socket resources are freed and the close packet confirmation will be send.
   *
   * __Remark__: The on/off boolean is in Socket API not relevant.
   * After a close packet has been transmitted, it always “lingers” before transmitting the closing confirmation packet.
   */
  int32_t lOnOff;
  int32_t lLinger;
};

typedef struct SOCK_SOCKOPT_SO_REUSEADDR_Ttag SOCK_SOCKOPT_SO_REUSEADDR_T;
__HIL_PACKED_PRE struct __HIL_PACKED_POST SOCK_SOCKOPT_SO_REUSEADDR_Ttag
{
  int32_t lReuseAddr;
  /** Padding to unique size. Set to zero in @ref SOCK_SETSOCKOPT_REQ_Ttag,
   *  ignore in Get Sockopt Confirmation */
  int32_t lReserved;
};

typedef struct SOCK_SOCKOPT_SO_ERROR_Ttag SOCK_SOCKOPT_SO_ERROR_T;
__HIL_PACKED_PRE struct __HIL_PACKED_POST SOCK_SOCKOPT_SO_ERROR_Ttag
{
  /** Get the last error code. */
  uint32_t ulError;
  /** Padding to unique size. Set to zero in @ref SOCK_SETSOCKOPT_REQ_Ttag,
   *  ignore in Get Sockopt Confirmation */
  int32_t  lReserved;
};

typedef union SOCK_SOCKOPT_Ttag SOCK_SOCKOPT_T;

union SOCK_SOCKOPT_Ttag
{
  SOCK_SOCKOPT_TCP_NODELAY_T tTcpNoDelay;             /*!<@ref SOCK_SOCKOPT_TCP_NODELAY_Ttag */
  SOCK_SOCKOPT_TCP_KEEPALIVE_CNT_T tKeepAliveCnt;     /*!< @ref SOCK_SOCKOPT_TCP_KEEPALIVE_CNT_Ttag */
  SOCK_SOCKOPT_TCP_KEEPALIVE_IDLE_T tKeepAliveIdle;   /*!< @ref SOCK_SOCKOPT_TCP_KEEPALIVE_IDLE_Ttag */
  SOCK_SOCKOPT_TCP_KEEPALIVE_INTVL_T tKeepAliveIntvl; /*!< @ref SOCK_SOCKOPT_TCP_KEEPALIVE_INTVL_Ttag */
  SOCK_SOCKOPT_TCP_KEEPALIVE_T tKeepAlive;            /*!< @ref SOCK_SOCKOPT_TCP_KEEPALIVE_Ttag */
  SOCK_SOCKOPT_TCP_LINGER_T tLinger;                  /*!< @ref SOCK_SOCKOPT_TCP_LINGER_Ttag */
  SOCK_SOCKOPT_SO_REUSEADDR_T tReuseAddr;             /*!< @ref SOCK_SOCKOPT_SO_REUSEADDR_Ttag */
  SOCK_SOCKOPT_SO_ERROR_T tError;                     /*!< @ref SOCK_SOCKOPT_SO_ERROR_Ttag */
};

/* end::TCP-SOCKET-OPTIONS[] */

/**
 * @defgroup SOCK_SetSockOpt_Service Set Socket Options Service
 * The Set Socket Options service can be used by the application to configure socket options.
 * \details Currently the following options for TCP sockets are supported:
 *
 * - Disable and enable the Naggle algorithm
 * - Disable and enable the KeepAlive
 * - Configure the timing values of KeepAlive
 * - Configure the linger timeout
 * @{
 *
 */

/* tag::SOCK-CMD-SETSOCKOPT-REQ[] */
typedef struct SOCK_SETSOCKOPT_REQ_DATA_Ttag SOCK_SETSOCKOPT_REQ_DATA_T;

__HIL_PACKED_PRE struct __HIL_PACKED_POST SOCK_SETSOCKOPT_REQ_DATA_Ttag
{
  /** Socket handle */
  SOCK_H         hSocket;
  /** Option level to set, value should be one of @ref SOCK_IPPROTO_Etag */
  uint16_t       usLevel;
  /** The option to set, this is protocol specific. See @ref SOCK_SOCKOPT_Ttag */
  uint16_t       usOption;
  /** Reserved, set to zero */
  uint16_t       usReserved;
/* Valid union field depends on usOption */
  SOCK_SOCKOPT_T tOpt;
};

typedef struct SOCK_SETSOCKOPT_REQ_Ttag SOCK_SETSOCKOPT_REQ_T;

/** This packet shall be used by the application to configure a socket option.
  * \reqhead
  */
__HIL_PACKED_PRE struct __HIL_PACKED_POST SOCK_SETSOCKOPT_REQ_Ttag
{
  HIL_PACKET_HEADER_T        tHead;

  SOCK_SETSOCKOPT_REQ_DATA_T tData;
};
/* end::SOCK-CMD-SETSOCKOPT-REQ[] */

/* tag::SOCK-CMD-SETSOCKOPT-CNF[] */
/** The packet will be returned to application after applying the socket option to the socket.
  * \cnfhead
  */
typedef struct SOCK_EMPTY_PCK_Ttag SOCK_SETSOCKOPT_CNF_T;
/* end::SOCK-CMD-SETSOCKOPT-CNF[] */

typedef union SOCK_SETSOCKOPT_PCK_Ttag SOCK_SETSOCKOPT_PCK_T;

union SOCK_SETSOCKOPT_PCK_Ttag
{
  SOCK_SETSOCKOPT_REQ_T tReq;

  SOCK_SETSOCKOPT_CNF_T tCnf;
};
/** @}*/

/**
 * @defgroup SOCK_GetSockOpt_Service Get Socket Options Service
 * The Get Socket Options service can be used by the application to retrieve the current setting for a particular socket option.
 * @{
 */

/* tag::SOCK-CMD-GETSOCKOPT-REQ[] */
typedef struct SOCK_GETSOCKOPT_REQ_DATA_Ttag SOCK_GETSOCKOPT_REQ_DATA_T;

__HIL_PACKED_PRE struct __HIL_PACKED_POST SOCK_GETSOCKOPT_REQ_DATA_Ttag
{
  /** Socket handle */
  SOCK_H   hSocket;
  /** Option level to set, value should be one of @ref SOCK_IPPROTO_Etag */
  uint16_t usLevel;
  /** The option to set, this is protocol specific. See @ref SOCK_SOCKOPT_Ttag */
  uint16_t usOption;
  /** Reserved, set to zero */
  uint16_t usReserved;
};

typedef struct SOCK_GETSOCKOPT_REQ_Ttag SOCK_GETSOCKOPT_REQ_T;

/** This packet shall be used by the application to configure a socket option.
  * \reqhead
  */
__HIL_PACKED_PRE struct __HIL_PACKED_POST SOCK_GETSOCKOPT_REQ_Ttag
{
  HIL_PACKET_HEADER_T        tHead;

  SOCK_GETSOCKOPT_REQ_DATA_T tData;
};
/* end::SOCK-CMD-GETSOCKOPT-REQ[] */

/* tag::SOCK-CMD-GETSOCKOPT-CNF[] */
typedef struct SOCK_GETSOCKOPT_CNF_DATA_Ttag SOCK_GETSOCKOPT_CNF_DATA_T;

/** Common fields of every sockopt data */
__HIL_PACKED_PRE struct __HIL_PACKED_POST SOCK_GETSOCKOPT_CNF_DATA_Ttag
{
  /** Socket handle */
  SOCK_H         hSocket;
  /** Option level to set, value should be one of @ref SOCK_IPPROTO_Etag enums */
  uint16_t       usLevel;
  /** The option to set, this is protocol specific. See @ref SOCK_SOCKOPT_Ttag */
  uint16_t       usOption;
  /** Reserved, set to zero */
  uint16_t       usReserved;
/* Valid union field depends on usOption */
  SOCK_SOCKOPT_T tOpt;
};

typedef struct SOCK_GETSOCKOPT_CNF_Ttag SOCK_GETSOCKOPT_CNF_T;

/** This packet will be returned from the stack to the application in response to the application's request.
  * \cnfhead
  */
__HIL_PACKED_PRE struct __HIL_PACKED_POST SOCK_GETSOCKOPT_CNF_Ttag
{
  HIL_PACKET_HEADER_T        tHead;

  SOCK_GETSOCKOPT_CNF_DATA_T tData;
};
/* end::SOCK-CMD-GETSOCKOPT-CNF[] */

typedef union SOCK_GETSOCKOPT_PCK_Ttag SOCK_GETSOCKOPT_PCK_T;

union SOCK_GETSOCKOPT_PCK_Ttag
{
  SOCK_GETSOCKOPT_REQ_T tReq;

  SOCK_GETSOCKOPT_CNF_T tCnf;
};
/** @}*/

/**
 * @defgroup SOCK_GetIfAddrs_Service Get IfAddrs Service
 * The Get Interface Addresses service can be used by the application to retrieve the current interface addresses.
 * For example: IP Address.
 * @{
 */

/* tag::SOCK-CMD-GETIFADDRS-REQ[] */
/** This packet shall be send by the application in order to get the interface addresses
  * \reqhead
  */
typedef struct SOCK_EMPTY_PCK_Ttag SOCK_GETIFADDRS_REQ_T;
/* end::SOCK-CMD-GETIFADDRS-REQ[] */

/* tag::SOCK-CMD-GETIFADDRS-CNF[] */
typedef struct SOCK_GETIFADDRS_ENTRY_Ttag SOCK_GETIFADDRS_ENTRY_T;

__HIL_PACKED_PRE struct __HIL_PACKED_POST SOCK_GETIFADDRS_ENTRY_Ttag
{
  /** Interface Name. */
  uint8_t     szIfName[8];
  /** __Ignore__. Reserved for interface flags for future use. */
  uint32_t    ulFlags;
  /** Address assigned to interface */
  SOCK_ADDR_T tIfAddress;
  /** Netmask assigned to interface, when the entry represents an IPv4 address. */
  SOCK_ADDR_T tNetmask;
  /** Broadcast Address assigned to interface, when the entry represents an IPv4 address. */
  SOCK_ADDR_T tBcastAddr;
};

typedef struct SOCK_GETIFADDRS_CNF_DATA_Ttag SOCK_GETIFADDRS_CNF_DATA_T;

__HIL_PACKED_PRE struct __HIL_PACKED_POST SOCK_GETIFADDRS_CNF_DATA_Ttag
{
#define SOCK_GETIFADDRS_MAX_NB_ENTRIES 8
  /** Array of up to 8 interface entries. One interface could be represented by several entries. */
  SOCK_GETIFADDRS_ENTRY_T atEntries[SOCK_GETIFADDRS_MAX_NB_ENTRIES];
};

typedef struct SOCK_GETIFADDRS_CNF_Ttag SOCK_GETIFADDRS_CNF_T;

/** The packet will be returned to the application for a @ref SOCK_GETIFADDRS_REQ_T packet to retrieve interface addresses.
  *
  * \details The confirmation data is an array of address entries, the data length (``tHead.ulLen``) is used to indicate the number of entries.
  *
  * One interface (identified with the same name ``tEntry.szIfName``) could be associated to several entries:
  * - If ``tEntry.tIfAddress.tCommon.bFamily`` is equal to @ref SOCK_AF_INET, it is an IPv4 address.\n
  * ``tNetmask`` and ``tBcastAddr`` are meaningful.
  * - If ``tEntry.tIfAddress.tCommon.bFamily`` is equal to @ref SOCK_AF_PACKET, it is a layer 2 address (or hardware address).\n
  * ``tNetmask`` and ``tBcastAddr`` are not meaningful.
  *
  * \cnfhead
  */
__HIL_PACKED_PRE struct __HIL_PACKED_POST SOCK_GETIFADDRS_CNF_Ttag
{
  HIL_PACKET_HEADER_T        tHead;

  SOCK_GETIFADDRS_CNF_DATA_T tData;
};
/* end::SOCK-CMD-GETIFADDRS-CNF[] */

typedef union SOCK_GETIFADDRS_PCK_Ttag SOCK_GETIFADDRS_PCK_T;

union SOCK_GETIFADDRS_PCK_Ttag
{
  SOCK_GETIFADDRS_REQ_T tReq;

  SOCK_GETIFADDRS_CNF_T tCnf;
};
/**@}*/

typedef union SOCK_PCK_Ttag SOCK_PCK_T;

/**
 * Packet Union of all supported services.
 */
union SOCK_PCK_Ttag
{
  SOCK_CLOSE_PCK_T      tClose;
  SOCK_POLL_PCK_T       tPoll;
  SOCK_FCNTL_PCK_T      tFcntl;

  SOCK_SOCKET_PCK_T     tSocket;
  SOCK_CONNECT_PCK_T    tConnect;
  SOCK_BIND_PCK_T       tBind;
  SOCK_LISTEN_PCK_T     tListen;
  SOCK_ACCEPT_PCK_T     tAccept;

  SOCK_RECVFROM_PCK_T   tRecvFrom;
  SOCK_SENDTO_PCK_T     tSendTo;

  SOCK_ABORT_PCK_T      tAbort;

  SOCK_SETSOCKOPT_PCK_T tSetSockOpt;
  SOCK_GETSOCKOPT_PCK_T tGetSockOpt;

  SOCK_GETIFADDRS_PCK_T tGetIfAddrs;

  SOCK_EMPTY_PCK_T      tEmpty;
};

#ifdef __HIL_PRAGMA_PACK_ENABLE
  #pragma __HIL_PRAGMA_UNPACK_1(SOCKAPI)
#endif

#endif /* SOCK_API_H_ */
