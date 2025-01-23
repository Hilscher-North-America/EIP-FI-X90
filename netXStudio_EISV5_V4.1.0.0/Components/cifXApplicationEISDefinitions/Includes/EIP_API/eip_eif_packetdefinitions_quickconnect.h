/**************************************************************************************
Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
***************************************************************************************
$Id: eip_eif_packetdefinitions_quickconnect.h 73908 2018-01-30 09:15:51Z MarcBommert $:

Description:
This header defines flags applicable to the QuickConnect configuration provided
with packet EIP_APS_SET_CONFIGURATION_PARAMETERS_REQ.

These definitions contribute to the LFW API of the EtherNet/IP stack and are
applicable to the DPM packet interface.

Please note that the QuickConnect feature is not yet available for the
EtherNet/IP stack V3.

**************************************************************************************/


#ifndef eip_eif_PACKETDEFINITIONS_QUICKCONNECT_H_
#define eip_eif_PACKETDEFINITIONS_QUICKCONNECT_H_

/* Quick connect flags */

#define EIP_OBJECT_QC_FLAGS_ACTIVATE_ATTRIBUTE    0x00000001  /*!< If set, the Quick Connect Attribute 12
                                                                   of the Tcp Interface Object (0xF5) is
                                                                   activated */

#define EIP_OBJECT_QC_FLAGS_ENABLE_QC             0x00000002  /*!< This flags sets the value of the Quick Connect Attribute.\n\n

                                                                   If set, Quick Connect is enabled.\n
                                                                   If not set, Quick Connect is disabled\n\n

                                                                   <b>Note:</b>\n

                                                                   This flag has only an effect if EIP_OBJECT_QC_FLAGS_ACTIVATE_ATTRIBUTE is set simultaneously. */


#endif

