/**************************************************************************************
Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
***************************************************************************************
$Id: eip_eif_packetdefinitions_sysflags.h 73908 2018-01-30 09:15:51Z MarcBommert $:

Description:
This header defines system behavior flags provided with packet
EIP_APS_SET_CONFIGURATION_PARAMETERS_REQ.

These definitions contribute to the LFW API of the EtherNet/IP stack and
are applicable to the DPM packet interface.

**************************************************************************************/


#ifndef eip_eif_PACKETDEFINITIONS_SYSFLAGS_H_
#define eip_eif_PACKETDEFINITIONS_SYSFLAGS_H_

/* Configuration system flags  */

#define WSTART_SYSFLAG_START_MSK            0x00000001
#define WSTART_SYSFLAG_START_APPLICATION    0x00000001      /*!< Application-controlled start */
#define WSTART_SYSFLAG_START_AUTO           0x00000000      /*!< Automatic start              */

#endif
