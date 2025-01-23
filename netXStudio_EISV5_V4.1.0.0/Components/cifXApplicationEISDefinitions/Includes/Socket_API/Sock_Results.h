/**************************************************************************************
Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
***************************************************************************************
$Id:  $:

Description:
  Packet Socket API status code
**************************************************************************************/

#ifndef SOCK_RESULTS_H_
#define SOCK_RESULTS_H_

#include <stdint.h>

/////////////////////////////////////////////////////////////////////////////////////
// Socket Api Error Codes
/////////////////////////////////////////////////////////////////////////////////////
//
// MessageId: ERR_SOCK_UNSUPPORTED_SOCKET
//
// MessageText:
//
// Unsupport socket domain, type and protocol combination.
//
#define ERR_SOCK_UNSUPPORTED_SOCKET      ((uint32_t)0xC0C90001L)

//
// MessageId: ERR_SOCK_INVALID_SOCKET_HANDLE
//
// MessageText:
//
// Invalid socket handle.
//
#define ERR_SOCK_INVALID_SOCKET_HANDLE   ((uint32_t)0xC0C90002L)

//
// MessageId: ERR_SOCK_SOCKET_CLOSED
//
// MessageText:
//
// Socket was closed.
//
#define ERR_SOCK_SOCKET_CLOSED           ((uint32_t)0xC0C90003L)

//
// MessageId: ERR_SOCK_INVALID_OP
//
// MessageText:
//
// The command is invalid for the particular socket.
//
#define ERR_SOCK_INVALID_OP              ((uint32_t)0xC0C90004L)

//
// MessageId: ERR_SOCK_INVALID_ADDRESS_FAMILY
//
// MessageText:
//
// An invalid address family was used for this socket
//
#define ERR_SOCK_INVALID_ADDRESS_FAMILY  ((uint32_t)0xC0C90005L)

//
// MessageId: ERR_SOCK_IN_USE
//
// MessageText:
//
// The specified address is already in use.
//
#define ERR_SOCK_IN_USE                  ((uint32_t)0xC0C90006L)

//
// MessageId: ERR_SOCK_HUP
//
// MessageText:
//
// The remote side closed the connection
//
#define ERR_SOCK_HUP                     ((uint32_t)0xC0C90007L)

//
// MessageId: ERR_SOCK_WOULDBLOCK
//
// MessageText:
//
// The operation would block
//
#define ERR_SOCK_WOULDBLOCK              ((uint32_t)0xC0C90008L)

//
// MessageId: ERR_SOCK_ROUTE
//
// MessageText:
//
// No IP route to destination address.
//
#define ERR_SOCK_ROUTE                   ((uint32_t)0xC0C90009L)

//
// MessageId: ERR_SOCK_IS_CONNECTED
//
// MessageText:
//
// IP socket already connected.
//
#define ERR_SOCK_IS_CONNECTED            ((uint32_t)0xC0C9000AL)

//
// MessageId: ERR_SOCK_CONNECTION_ABORTED
//
// MessageText:
//
// TCP connection aborted.
//
#define ERR_SOCK_CONNECTION_ABORTED      ((uint32_t)0xC0C9000BL)

//
// MessageId: ERR_SOCK_CONNECTION_RESET
//
// MessageText:
//
// TCP connection reset.
//
#define ERR_SOCK_CONNECTION_RESET        ((uint32_t)0xC0C9000CL)

//
// MessageId: ERR_SOCK_CONNECTION_CLOSED
//
// MessageText:
//
// TCP connection closed.
//
#define ERR_SOCK_CONNECTION_CLOSED       ((uint32_t)0xC0C9000DL)

//
// MessageId: ERR_SOCK_NOT_CONNECTED
//
// MessageText:
//
// IP Socket not connected.
//
#define ERR_SOCK_NOT_CONNECTED           ((uint32_t)0xC0C9000EL)

//
// MessageId: ERR_SOCK_NETWORK_INTERFACE
//
// MessageText:
//
// Low-level network interface error.
//
#define ERR_SOCK_NETWORK_INTERFACE       ((uint32_t)0xC0C9000FL)

#endif  /* __SOCK_RESULTS_H */
