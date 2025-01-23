/**************************************************************************************
Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
***************************************************************************************
$Id: eip_eif_packetdefinitions_parameters.h 73908 2018-01-30 09:15:51Z MarcBommert $:

Description:
This header defines parameter flags applicable to the packets
EIP_OBJECT_SET_PARAMETER_REQ and EIP_APS_SET_PARAMETER_REQ.
They dynamically enable or disable certain optional features.

These definitions contribute to the LFW API of the EtherNet/IP stack and are
applicable to the DPM packet interface.

**************************************************************************************/


#ifndef eip_eif_PACKETDEFINITIONS_PARAMETERS_H_
#define eip_eif_PACKETDEFINITIONS_PARAMETERS_H_

/* Option-Flags for EIP_OBJECT_SET_PARAMETER_REQ */
#define EIP_OBJECT_PRM_FWRD_OPEN_CLOSE_FORWARDING                0x00000001 /*!< Setting this flag the host application will
                                                                                 receive the complete forward_open/close message
                                                                                 from the stack. */
#define EIP_OBJECT_PRM_DISABLE_FLASH_LEDS_SERVICE                0x00000002 /*!< Setting this flag the host application will
                                                                                 disable the Flash_LEDs service of the Identity object */

#define EIP_OBJECT_PRM_DISABLE_TRANSMISSION_TRIGGER_TIMER        0x00000004 /*!< This flag affects the timing of data production
                                                                                 in case of "Application Object" or "Change Of State"
                                                                                 triggered data.\n
                                                                                 Setting of this flag will turn off the transmission trigger timer
                                                                                 for all application-triggered and change-of-state connections.
                                                                                 Data production is then only triggered by the EtherNet/IP Application
                                                                                 when providing new data to the protocol stack (e.g. xChannelIoWrite()).
                                                                                 For connections of type "cyclic", the transmission trigger timer will
                                                                                 not be disabled.  */

#define EIP_OBJECT_PRM_HOST_CONTROLS_IDENTITY_STATE_ATTRIBUTE_8  0x00000008 /*!< This flag affects the handling of Identity object's attribute 5 (State).
                                                                                 Usually, the protocol stack controls this attribute autonomously.
                                                                                 However, there are types of host applications that need to control
                                                                                 this attribute them self (e.g. CIP Safety).

                                                                                 If set, the protocol stack will stop controlling the state attribute.
                                                                                 Instead, the host application must take care of the attribute's content.
                                                                                 Send set_attribute_single service (0x0E) to attribute 8 to set state values.

                                                                                 Note:
                                                                                 The designer of the application must decide whether or not it needs this feature. Enabling and after some
                                                                                 time disabling the write access must be avoided as this might lead to invalid state attribute values. */
#define EIP_OBJECT_PRM_ENABLE_NULL_FWRD_OPEN                     0x00000010 /*<! This flag, if set, enables NULL ForwardOpen support. If enabled, NULL-ForwardOpen requests, i.e.
                                                                                 ForwardOpen requests which have the transport type NULL for both directions, O2T and T2O, are processed.
                                                                                 Please refer to teh API manual for further information about possible use cases and behavioral effects
                                                                                 at the host application interface.
                                                                            */

/* Option-Flags for EIP_APS_SET_PARAMETER_REQ */
#define EIP_APS_PRM_SIGNAL_MS_NS_CHANGE              0x00000001 /*!< This flag enables or disables the notification of
                                                                     the network and module status. Every time the status
                                                                     of the module or network changes packet
                                                                     EIP_APS_MS_NS_CHANGE_IND will be sent to the
                                                                     registered EtherNet/IP Application task.    */
#endif


