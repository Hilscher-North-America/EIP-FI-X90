/**************************************************************************************
Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
***************************************************************************************
$Id: Eip_Indicators.h 75747 2018-05-09 05:34:11Z kai $:

Description:
  EtherNet/IP Indicators
**************************************************************************************/

#ifndef EIP_INDICATORS_H_
#define EIP_INDICATORS_H_

  /*
  ************************************************************
  *  CIP specific defines
  ************************************************************
  */

/*#####################################################################################*/

enum EIP_HW_MODUL_STATUS_E      /*!  List of module states */
{
  EIP_MS_NO_POWER = 0,          /*!< No Power\n
                                     If no power is supplied to the device, the module
                                     status indicator is steady off. */

  EIP_MS_SELFTEST,              /*!< Self-Test\n
                                     While the device is performing its power up testing,
                                     the module status indicator flashes green/red */

  EIP_MS_STANDBY,               /*!< Standby\n
                                     If the device has not been configured, the module
                                     status indicator flashes green. */

  EIP_MS_OPERATE,               /*!< Device operational\n
                                     If the device is operating correctly, the module
                                     status indicator is steady green. */

  EIP_MS_MAJOR_RECOVERABLE_FAULT,   /*!< Major recoverable fault\n
                                         If the device has detected a major recoverable fault,
                                         the module status indicator flashes red.
                                         NOTE: An incorrect or inconsistent configuration
                                         would be considered a major recoverable fault. */

  EIP_MS_MAJOR_UNRECOVERABLE_FAULT, /*!< Major unrecoverable fault\n
                                         If the device has detected a major unrecoverable fault,
                                         the module status indicator is steady red. */
};


enum EIP_HW_NET_STATUS_E        /*!  List of network states */
{
  EIP_NS_NO_POWER = 0,          /*!< Not powered, no IP address\n
                                     The device is powered off, or is powered on but with no
                                     IP address configured. */

  EIP_NS_NO_CONNECTION,         /*!< No connections\n
                                     An IP address is configured, but no CIP connections are
                                     established, and an Exclusive Owner connection has not timed
                                     out. */

  EIP_NS_CONNECTED,             /*!< Connected\n
                                     At least one CIP connection (any transport class) is established,
                                     and an Exclusive Owner connection has not timed out. */

  EIP_NS_TIMEOUT,               /*!< Connection timeout\n
                                     An Exclusive Owner connection for
                                     which this device is the target has timed out. The network status indicator
                                     returns to steady green only when all timed out Exclusive Owner
                                     connections are reestablished.\n
                                     The Network LED transitions from flashing red to steady green only when
                                     all connections to the previously timed-out O->T connection points are
                                     reestablished. Timeout of connections other than Exclusive Owner connections
                                     do not cause the indicator to flash red.\n
                                     The Flashing Red state applies to target connections only. */

  EIP_NS_DUPIP,                 /*!< Duplicate IP\n
                                     The device has detected that its IP address is already in use. */

  EIP_NS_SELFTEST               /*!< Self-Test\n
                                     The device is performing its power-on self-test (POST). During POST
                                     the network status indicator alternates flashing green and red. */
};

#endif

