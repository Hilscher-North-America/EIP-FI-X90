/**************************************************************************************
Exclusion of Liability for this software:
  The following software is intended for and must only be used for reference and in an
  evaluation laboratory environment. It is provided without charge and is subject to
  alterations. There is no warranty for the software, to the extent permitted by
  applicable law. Except when otherwise stated in writing the copyright holders and/or
  other parties provide the software "as is" without warranty of any kind, either
  expressed or implied.
  Please refer to the Agreement in README_DISCLAIMER.txt, provided together with this file!
  By installing or otherwise using the software, you accept the terms of this Agreement.
  If you do not agree to the terms of this Agreement, then do not install or use the
  Software!
**************************************************************************************/

/**************************************************************************************

Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.

***************************************************************************************/


/*************************************************************************************
 Note: All features of the EtherNet/IP Host Example Project can enable in this central
       configuration file. Per default most of the are disabled.
*************************************************************************************/

#ifndef APPEIS_APPLICATION_CONFIG_H_
#define APPEIS_APPLICATION_CONFIG_H_

/*************************************************************************************
 Enable this macro is basically required for demonstration of different features which
 can be enabled below. When this macro is enable by its own the stack will forward
 Link State change indications per default.
*************************************************************************************/
#define HOST_APPLICATION_REGISTRATION

/*************************************************************************************
 Enable this macro will forward store remanent data indications, when the host app
 shall handle the remanent data.
*************************************************************************************/
#define HOST_APPLICATION_STORES_REMANENT_DATA

/*************************************************************************************
 Enable this macro allows to simulate rotary switches via terminal handler. For details
 see function Protocol_TerminalHandler_callback().
*************************************************************************************/
#define HOST_APPLICATION_TERMINAL_HANDLER

/*************************************************************************************
 Enable this macro shows how the MAC address can be set by the host application.
**************************************************************************************/
#define HOST_APPLICATION_SETS_MAC_ADDRESS

/*************************************************************************************
 Enable this macro shows how the serial number can be set by the host application.
**************************************************************************************/
//#define HOST_APPLICATION_SETS_SERIAL_NUMBER

/*************************************************************************************
 Enable one of the following macro shows the different use cases for IP address
 configuration.
*************************************************************************************/
// #define HOST_APP_CFG_FIX_IP_ADDRESS
// #define HOST_APP_CFG_DHCP
// #define HOST_APP_CFG_DHCP_LEARNING
// #define HOST_APP_CFG_SWITCH_SIMULATION

/*************************************************************************************
 Enable this macro will force the stack to forward Connection Indication packets to
 host application.
**************************************************************************************/
#define HOST_APPLICATION_CFG_ENABLE_CONNECTION_IND

#ifdef HOST_APPLICATION_STORES_REMANENT_DATA
#ifndef HOST_APPLICATION_REGISTRATION
#error Register_App_Required!!!
#endif
#endif

#ifdef HOST_APPLICATION_CFG_ENABLE_CONNECTION_IND
#ifndef HOST_APPLICATION_REGISTRATION
#error Register_App_Required!!!
#endif

#endif

#endif /* APPEIS_APPLICATION_CONFIG_H_ */
