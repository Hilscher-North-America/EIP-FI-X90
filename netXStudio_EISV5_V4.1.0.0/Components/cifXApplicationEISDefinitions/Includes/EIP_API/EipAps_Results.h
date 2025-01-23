/**************************************************************************************
Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
***************************************************************************************
$Id: EipAps_Results.h 74458 2018-03-02 17:24:37Z MarcBommert $:

Description:
  EtherNet/IP Results
**************************************************************************************/

#ifndef __EIP_APS_RESULTS_H
#define __EIP_APS_RESULTS_H

/////////////////////////////////////////////////////////////////////////////////////
// Ethernet/IP Application Task
/////////////////////////////////////////////////////////////////////////////////////
//
// MessageId: ERR_EIP_APS_COMMAND_INVALID
//
// MessageText:
//
//  Invalid command received.
//
#define ERR_EIP_APS_COMMAND_INVALID      ((uint32_t)0xC0590001L)

//
// MessageId: ERR_EIP_APS_PACKET_LENGTH_INVALID
//
// MessageText:
//
//  Invalid packet length.
//
#define ERR_EIP_APS_PACKET_LENGTH_INVALID ((uint32_t)0xC0590002L)

//
// MessageId: ERR_EIP_APS_PACKET_PARAMETER_INVALID
//
// MessageText:
//
//  Parameter of the packet are invalid.
//
#define ERR_EIP_APS_PACKET_PARAMETER_INVALID ((uint32_t)0xC0590003L)

//
// MessageId: ERR_EIP_APS_TCP_CONFIG_FAIL
//
// MessageText:
//
//  Configuration of TCP/IP failed.
//
#define ERR_EIP_APS_TCP_CONFIG_FAIL      ((uint32_t)0xC0590004L)

//
// MessageId: ERR_EIP_APS_CONNECTION_CLOSED
//
// MessageText:
//
//  Existing connection is closed.
//
#define ERR_EIP_APS_CONNECTION_CLOSED    ((uint32_t)0xC0590005L)

//
// MessageId: ERR_EIP_APS_ALREADY_REGISTERED
//
// MessageText:
//
//  A application is already registered.
//
#define ERR_EIP_APS_ALREADY_REGISTERED   ((uint32_t)0xC0590006L)

//
// MessageId: ERR_EIP_APS_ACCESS_FAIL
//
// MessageText:
//
//  Command is not allowed.
//
#define ERR_EIP_APS_ACCESS_FAIL          ((uint32_t)0xC0590007L)

//
// MessageId: ERR_EIP_APS_STATE_FAIL
//
// MessageText:
//
//  Command not allowed at this state.
//
#define ERR_EIP_APS_STATE_FAIL           ((uint32_t)0xC0590008L)

//
// MessageId: ERR_EIP_APS_IO_OFFSET_INVALID
//
// MessageText:
//
//  Invalid offset for I/O data.
//
#define ERR_EIP_APS_IO_OFFSET_INVALID    ((uint32_t)0xC0590009L)

//
// MessageId: ERR_EIP_APS_FOLDER_NOT_FOUND
//
// MessageText:
//
//  Folder for database not found.
//
#define ERR_EIP_APS_FOLDER_NOT_FOUND     ((uint32_t)0xC059000AL)

//
// MessageId: ERR_EIP_APS_CONFIG_DBM_INVALID
//
// MessageText:
//
//  Configuration database invalid.
//
#define ERR_EIP_APS_CONFIG_DBM_INVALID   ((uint32_t)0xC059000BL)

//
// MessageId: ERR_EIP_APS_NO_CONFIG_DBM
//
// MessageText:
//
//  Configuration database not found.
//
#define ERR_EIP_APS_NO_CONFIG_DBM        ((uint32_t)0xC059000CL)

//
// MessageId: ERR_EIP_APS_NWID_DBM_INVALID
//
// MessageText:
//
//  network database invalid.
//
#define ERR_EIP_APS_NWID_DBM_INVALID     ((uint32_t)0xC059000DL)

//
// MessageId: ERR_EIP_APS_NO_NWID_DBM
//
// MessageText:
//
//  network database not found.
//
#define ERR_EIP_APS_NO_NWID_DBM          ((uint32_t)0xC059000EL)

//
// MessageId: ERR_EIP_APS_NO_DBM
//
// MessageText:
//
//  no database found.
//
#define ERR_EIP_APS_NO_DBM               ((uint32_t)0xC059000FL)

//
// MessageId: ERR_EIP_APS_NO_MAC_ADDRESS_AVAILABLE
//
// MessageText:
//
//  no MAC address available.
//
#define ERR_EIP_APS_NO_MAC_ADDRESS_AVAILABLE ((uint32_t)0xC0590010L)

//
// MessageId: ERR_EIP_APS_INVALID_FILESYSTEM
//
// MessageText:
//
//  access to file system failed.
//
#define ERR_EIP_APS_INVALID_FILESYSTEM   ((uint32_t)0xC0590011L)

//
// MessageId: ERR_EIP_APS_NUM_AS_INSTANCE_EXCEEDS
//
// MessageText:
//
//  maximum number of assembly instances exceeds.
//
#define ERR_EIP_APS_NUM_AS_INSTANCE_EXCEEDS ((uint32_t)0xC0590012L)

//
// MessageId: ERR_EIP_APS_CONFIGBYDATABASE
//
// MessageText:
//
//  stack is already configured via database.
//
#define ERR_EIP_APS_CONFIGBYDATABASE     ((uint32_t)0xC0590013L)


#endif  //__EIP_APS_RESULTS_H

