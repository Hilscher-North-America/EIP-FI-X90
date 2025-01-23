/**************************************************************************************
Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
***************************************************************************************
$Id: Eip_Results.h 110559 2022-11-24 20:15:24Z OKompani $:

Description:
  EtherNet/IP - Results
**************************************************************************************/

#ifndef __EIP_ERROR_H
#define __EIP_ERROR_H

/////////////////////////////////////////////////////////////////////////////////////
// Ethernet/IP Encapsulation
/////////////////////////////////////////////////////////////////////////////////////
//
// MessageId: ERR_EIP_ENCAP_COMMAND_INVALID
//
// MessageText:
//
//  Invalid command received.
//
#define ERR_EIP_ENCAP_COMMAND_INVALID    ((uint32_t)0xC01E0001L)

//
// MessageId: ERR_EIP_ENCAP_NOT_INITIALIZED
//
// MessageText:
//
//  Encapsulation layer is not initialized.
//
#define ERR_EIP_ENCAP_NOT_INITIALIZED    ((uint32_t)0xC01E0002L)

//
// MessageId: ERR_EIP_ENCAP_OUT_OF_MEMORY
//
// MessageText:
//
//  System is out of memory.
//
#define ERR_EIP_ENCAP_OUT_OF_MEMORY      ((uint32_t)0xC01E0003L)

//
// MessageId: ERR_EIP_ENCAP_OUT_OF_PACKETS
//
// MessageText:
//
//  Task runs out of empty packets at the local packet pool.
//
#define ERR_EIP_ENCAP_OUT_OF_PACKETS     ((uint32_t)0xC01E0010L)

//
// MessageId: ERR_EIP_ENCAP_SEND_PACKET
//
// MessageText:
//
//  Sending a packet failed.
//
#define ERR_EIP_ENCAP_SEND_PACKET        ((uint32_t)0xC01E0011L)

//
// MessageId: ERR_EIP_ENCAP_SOCKET_OVERRUN
//
// MessageText:
//
//  No free socket is available.
//
#define ERR_EIP_ENCAP_SOCKET_OVERRUN     ((uint32_t)0xC01E0012L)

//
// MessageId: ERR_EIP_ENCAP_INVALID_SOCKET
//
// MessageText:
//
//  Socket ID is invalid.
//
#define ERR_EIP_ENCAP_INVALID_SOCKET     ((uint32_t)0xC01E0013L)

//
// MessageId: ERR_EIP_ENCAP_CEP_OVERRUN
//
// MessageText:
//
//  Connection could not be open. No resource for a new Connection Endpoint available.
//
#define ERR_EIP_ENCAP_CEP_OVERRUN        ((uint32_t)0xC01E0014L)

//
// MessageId: ERR_EIP_ENCAP_UCMM_OVERRUN
//
// MessageText:
//
//  Message couldn't send. All Unconnect Message Buffers are in use.
//
#define ERR_EIP_ENCAP_UCMM_OVERRUN       ((uint32_t)0xC01E0015L)

//
// MessageId: ERR_EIP_ENCAP_TRANSP_OVERRUN
//
// MessageText:
//
//  Connection couldn't be opened. All transports are in use.
//
#define ERR_EIP_ENCAP_TRANSP_OVERRUN     ((uint32_t)0xC01E0016L)

//
// MessageId: ERR_EIP_ENCAP_UNKNOWN_CONN_TYP
//
// MessageText:
//
//  Received Message include an unknown connection typ.
//
#define ERR_EIP_ENCAP_UNKNOWN_CONN_TYP   ((uint32_t)0xC01E0017L)

//
// MessageId: ERR_EIP_ENCAP_CONN_CLOSED
//
// MessageText:
//
//  Connection was closed.
//
#define ERR_EIP_ENCAP_CONN_CLOSED        ((uint32_t)0xC01E0000L)

//
// MessageId: ERR_EIP_ENCAP_CONN_RESETED
//
// MessageText:
//
//  Connection is reseted from remote device.
//
#define ERR_EIP_ENCAP_CONN_RESETED       ((uint32_t)0xC01E0019L)

//
// MessageId: SUCCESS_EIP_ENCAP_CONN_UNREGISTER
//
// MessageText:
//
//  We closed the conncetion successful. With an unregister command.
//
#define SUCCESS_EIP_ENCAP_CONN_UNREGISTER ((uint32_t)0x001E001AL)

//
// MessageId: ERR_EIP_ENCAP_CONN_STATE
//
// MessageText:
//
//  Wrong connection state for this service.
//
#define ERR_EIP_ENCAP_CONN_STATE         ((uint32_t)0xC01E001BL)

//
// MessageId: ERR_EIP_ENCAP_CONN_INACTIV
//
// MessageText:
//
//  Encapsulation sesson was deactivated
//
#define ERR_EIP_ENCAP_CONN_INACTIV       ((uint32_t)0xC01E001CL)

//
// MessageId: ERR_EIP_ENCAP_INVALID_IPADDR
//
// MessageText:
//
//  received an invalid IP address.
//
#define ERR_EIP_ENCAP_INVALID_IPADDR     ((uint32_t)0xC01E001DL)

//
// MessageId: ERR_EIP_ENCAP_INVALID_TRANSP
//
// MessageText:
//
//  Invalid transport typ.
//
#define ERR_EIP_ENCAP_INVALID_TRANSP     ((uint32_t)0xC01E001EL)

//
// MessageId: ERR_EIP_ENCAP_TRANSP_INUSE
//
// MessageText:
//
//  Transport is still in use.
//
#define ERR_EIP_ENCAP_TRANSP_INUSE       ((uint32_t)0xC01E001FL)

//
// MessageId: ERR_EIP_ENCAP_TRANSP_CLOSED
//
// MessageText:
//
//  Transport is closed.
//
#define ERR_EIP_ENCAP_TRANSP_CLOSED      ((uint32_t)0xC01E0020L)

//
// MessageId: ERR_EIP_ENCAP_INVALID_MSGID
//
// MessageText:
//
//  The received message has a invalid message ID.
//
#define ERR_EIP_ENCAP_INVALID_MSGID      ((uint32_t)0xC01E0021L)

//
// MessageId: ERR_EIP_ENCAP_INVALID_MSG
//
// MessageText:
//
//  invalid encapsulation message received.
//
#define ERR_EIP_ENCAP_INVALID_MSG        ((uint32_t)0xC01E0022L)

//
// MessageId: ERR_EIP_ENCAP_INVALID_MSGLEN
//
// MessageText:
//
//  Received message with invalid length.
//
#define ERR_EIP_ENCAP_INVALID_MSGLEN     ((uint32_t)0xC01E0023L)

//
// MessageId: ERR_EIP_ENCAP_CL3_TIMEOUT
//
// MessageText:
//
//  Class 3 connection runs into timeout.
//
#define ERR_EIP_ENCAP_CL3_TIMEOUT        ((uint32_t)0xC01E0030L)

//
// MessageId: ERR_EIP_ENCAP_UCMM_TIMEOUT
//
// MessageText:
//
//  Unconnected message gets a timeout.
//
#define ERR_EIP_ENCAP_UCMM_TIMEOUT       ((uint32_t)0xC01E0031L)

//
// MessageId: ERR_EIP_ENCAP_CL1_TIMEOUT
//
// MessageText:
//
//  Timeout of a class 3 connection.
//
#define ERR_EIP_ENCAP_CL1_TIMEOUT        ((uint32_t)0xC01E0032L)

//
// MessageId: WARN_EIP_ENCAP_TIMEOUT
//
// MessageText:
//
//  Encapsulation service is finished by timeout.
//
#define WARN_EIP_ENCAP_TIMEOUT           ((uint32_t)0x801E0033L)

//
// MessageId: ERR_EIP_ENCAP_CMDRUNNING
//
// MessageText:
//
//  Encapsulation service is still running.
//
#define ERR_EIP_ENCAP_CMDRUNNING         ((uint32_t)0xC01E0034L)

//
// MessageId: ERR_EIP_ENCAP_NO_TIMER
//
// MessageText:
//
//  No empty timer available.
//
#define ERR_EIP_ENCAP_NO_TIMER           ((uint32_t)0xC01E0035L)

//
// MessageId: ERR_EIP_ENCAP_INVALID_DATA_IDX
//
// MessageText:
//
//  The data index is unknown by the task. Please ensure that it is the same as at the indication.
//
#define ERR_EIP_ENCAP_INVALID_DATA_IDX   ((uint32_t)0xC01E0036L)

//
// MessageId: ERR_EIP_ENCAP_INVALID_DATA_AREA
//
// MessageText:
//
//  The parameter of the data area are invalid. Please check length and offset.
//
#define ERR_EIP_ENCAP_INVALID_DATA_AREA  ((uint32_t)0xC01E0037L)

//
// MessageId: ERR_EIP_ENCAP_INVALID_DATA_LEN
//
// MessageText:
//
//  Packet length is invalid. Please check length of the packet.
//
#define ERR_EIP_ENCAP_INVALID_DATA_LEN   ((uint32_t)0xC01E0038L)

//
// MessageId: ERR_EIP_ENCAP_TASK_RESETING
//
// MessageText:
//
//  Ethernet/IP Encapsulation Layer runs a reset.
//
#define ERR_EIP_ENCAP_TASK_RESETING      ((uint32_t)0xC01E0039L)

/////////////////////////////////////////////////////////////////////////////////////
// Ethernet/IP Object
/////////////////////////////////////////////////////////////////////////////////////
//
// MessageId: ERR_EIP_OBJECT_COMMAND_INVALID
//
// MessageText:
//
//  Invalid command received.
//
#define ERR_EIP_OBJECT_COMMAND_INVALID   ((uint32_t)0xC01F0001L)

//
// MessageId: ERR_EIP_OBJECT_OUT_OF_MEMORY
//
// MessageText:
//
//  System is out of memory.
//
#define ERR_EIP_OBJECT_OUT_OF_MEMORY     ((uint32_t)0xC01F0002L)

//
// MessageId: ERR_EIP_OBJECT_OUT_OF_PACKETS
//
// MessageText:
//
//  Task runs out of empty packets at the local packet pool.
//
#define ERR_EIP_OBJECT_OUT_OF_PACKETS    ((uint32_t)0xC01F0003L)

//
// MessageId: ERR_EIP_OBJECT_SEND_PACKET
//
// MessageText:
//
//  Sending a packet failed.
//
#define ERR_EIP_OBJECT_SEND_PACKET       ((uint32_t)0xC01F0004L)

//
// MessageId: ERR_EIP_OBJECT_AS_ALLREADY_EXIST
//
// MessageText:
//
//  Assembly instance already exist.
//
#define ERR_EIP_OBJECT_AS_ALLREADY_EXIST ((uint32_t)0xC01F0010L)

//
// MessageId: ERR_EIP_OBJECT_AS_INVALID_INST
//
// MessageText:
//
//  Invalid Assembly Instance.
//
#define ERR_EIP_OBJECT_AS_INVALID_INST   ((uint32_t)0xC01F0011L)

//
// MessageId: ERR_EIP_OBJECT_AS_INVALID_LEN
//
// MessageText:
//
//  Invalid Assembly length.
//
#define ERR_EIP_OBJECT_AS_INVALID_LEN    ((uint32_t)0xC01F0012L)

//
// MessageId: ERR_EIP_OBJECT_CONN_OVERRUN
//
// MessageText:
//
//  No free connection buffer available
//
#define ERR_EIP_OBJECT_CONN_OVERRUN      ((uint32_t)0xC01F0020L)

//
// MessageId: ERR_EIP_OBJECT_INVALID_CLASS
//
// MessageText:
//
//  Object class is invalid.
//
#define ERR_EIP_OBJECT_INVALID_CLASS     ((uint32_t)0xC01F0021L)

//
// MessageId: ERR_EIP_OBJECT_SEGMENT_FAULT
//
// MessageText:
//
//  Segment of the path is invalid.
//
#define ERR_EIP_OBJECT_SEGMENT_FAULT     ((uint32_t)0xC01F0022L)

//
// MessageId: ERR_EIP_OBJECT_CLASS_ALLREADY_EXIST
//
// MessageText:
//
//  Object Class is already used.
//
#define ERR_EIP_OBJECT_CLASS_ALLREADY_EXIST ((uint32_t)0xC01F0023L)

//
// MessageId: ERR_EIP_OBJECT_CONNECTION_FAIL
//
// MessageText:
//
//  Connection failed.
//
#define ERR_EIP_OBJECT_CONNECTION_FAIL   ((uint32_t)0xC01F0024L)

//
// MessageId: ERR_EIP_OBJECT_CONNECTION_PARAM
//
// MessageText:
//
//  Unknown format of connection parameter
//
#define ERR_EIP_OBJECT_CONNECTION_PARAM  ((uint32_t)0xC01F0025L)

//
// MessageId: ERR_EIP_OBJECT_UNKNOWN_CONNECTION
//
// MessageText:
//
//  Invalid connection ID.
//
#define ERR_EIP_OBJECT_UNKNOWN_CONNECTION ((uint32_t)0xC01F0026L)

//
// MessageId: ERR_EIP_OBJECT_NO_OBJ_RESSOURCE
//
// MessageText:
//
//  No resource for creating a new class object available.
//
#define ERR_EIP_OBJECT_NO_OBJ_RESSOURCE  ((uint32_t)0xC01F0027L)

//
// MessageId: ERR_EIP_OBJECT_ID_INVALID_PARAMETER
//
// MessageText:
//
//  Invalid request parameter.
//
#define ERR_EIP_OBJECT_ID_INVALID_PARAMETER ((uint32_t)0xC01F0028L)

//
// MessageId: ERR_EIP_OBJECT_CONNECTION_FAILED
//
// MessageText:
//
//  General connection failure. See also General Error Code and Extended Error Code for more details.
//
#define ERR_EIP_OBJECT_CONNECTION_FAILED ((uint32_t)0xC01F0029L)

//
// MessageId: ERR_EIP_OBJECT_PACKET_LEN
//
// MessageText:
//
//  Packet length of the request is invalid.
//
#define ERR_EIP_OBJECT_PACKET_LEN        ((uint32_t)0xC01F0030L)

//
// MessageId: ERR_EIP_OBJECT_READONLY_INST
//
// MessageText:
//
//  Access denied. Instance is read only.
//
#define ERR_EIP_OBJECT_READONLY_INST     ((uint32_t)0xC01F0031L)

//
// MessageId: ERR_EIP_OBJECT_DPM_USED
//
// MessageText:
//
//  DPM address is already used by an other instance.
//
#define ERR_EIP_OBJECT_DPM_USED          ((uint32_t)0xC01F0032L)

//
// MessageId: ERR_EIP_OBJECT_SET_OUTPUT_RUNNING
//
// MessageText:
//
//  Set Output command is already runnning.
//
#define ERR_EIP_OBJECT_SET_OUTPUT_RUNNING ((uint32_t)0xC01F0033L)

//
// MessageId: ERR_EIP_OBJECT_TASK_RESETING
//
// MessageText:
//
//  Etthernet/IP Object Task is running a reset.
//
#define ERR_EIP_OBJECT_TASK_RESETING     ((uint32_t)0xC01F0034L)

//
// MessageId: ERR_EIP_OBJECT_SERVICE_ALLREADY_EXIST
//
// MessageText:
//
//  The service that shall be registered already exists.
//
#define ERR_EIP_OBJECT_SERVICE_ALLREADY_EXIST ((uint32_t)0xC01F0035L)

//
// MessageId: ERR_EIP_OBJECT_DUPLICATE_SERVICE
//
// MessageText:
//
//  The service is rejected by the application due to a duplicate sequence count.
//
#define ERR_EIP_OBJECT_DUPLICATE_SERVICE ((uint32_t)0xC01F0036L)

//
// MessageId: ERR_EIP_TIMER_INVALID_HANDLE
//
// MessageText:
//
//  Timer function is called with invalid timer handle.
//
#define ERR_EIP_TIMER_INVALID_HANDLE     ((uint32_t)0xC01F0037L)

//
// MessageId: ERR_EIP_INVALID_STACK_MODE
//
// MessageText:
//
//  Setting the operation mode is called with an undefined mode value.
//
#define ERR_EIP_INVALID_STACK_MODE       ((uint32_t)0xC01F0038L)

//
// MessageId: ERR_EIP_OUT_OF_ASSEMBLIES
//
// MessageText:
//
//  No assembly instances free to open a connection.
//
#define ERR_EIP_OUT_OF_ASSEMBLIES        ((uint32_t)0xC01F0039L)

//
// MessageId: ERR_EIP_CALLBACK_REQUIRED
//
// MessageText:
//
//  Function needs callback to provide result data.
//
#define ERR_EIP_CALLBACK_REQUIRED       ((uint32_t)0xC01F003AL)

//
// MessageId: ERR_EIP_SERVICE_NOT_SUPPORTED
//
// MessageText:
//
//  This service is at the actual configuration not supported.
//
#define ERR_EIP_SERVICE_NOT_SUPPORTED    ((uint32_t)0xC01F003BL)

//
// MessageId: ERR_EIP_SERVICE_RUNNING
//
// MessageText:
//
//  This service is running and can not be started twice.
//
#define ERR_EIP_SERVICE_RUNNING          ((uint32_t)0xC01F003CL)

//
// MessageId: EIP_ERR_CC_DATA_IMAGE_ERROR
//
// MessageText:
//
//  The address of the data is not at the range of the data image.
//
#define EIP_ERR_CC_DATA_IMAGE_ERROR      ((uint32_t)0xC01F003DL)

//
// MessageId: EIP_ERR_CC_UNKNOWN_FORMAT
//
// MessageText:
//
//  The format of the data mapping is unknown.
//
#define EIP_ERR_CC_UNKNOWN_FORMAT        ((uint32_t)0xC01F003EL)

//
// MessageId: ERR_EIP_CONNECTION_POINT_CREATE
//
// MessageText:
//
//  Creating the connection point failed.
//
#define ERR_EIP_CONNECTION_POINT_CREATE  ((uint32_t)0xC01F003FL)

/** The CIP Security subsystem is not properly started. */
#define ERR_EIP_SECURITY_SUBSYSTEM_NOT_STARTED ((uint32_t)0xC01F0040L)

#endif  //__EIP_ERROR_H
