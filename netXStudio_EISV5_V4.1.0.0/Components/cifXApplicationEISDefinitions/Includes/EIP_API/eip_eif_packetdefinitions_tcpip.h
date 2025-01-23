/**************************************************************************************
Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
***************************************************************************************
$Id: eip_eif_packetdefinitions_tcpip.h 73908 2018-01-30 09:15:51Z MarcBommert $:

Description:
This header defines TCP/IP configuration flags provided with packet
EIP_APS_SET_CONFIGURATION_PARAMETERS_REQ.

These definitions contribute to the LFW API of the EtherNet/IP stack and are
applicable to the DPM packet interface.

**************************************************************************************/

#ifndef eip_eif_PACKETDEFINITIONS_TCPIP_H_
#define eip_eif_PACKETDEFINITIONS_TCPIP_H_

/* TCP/IP configuration flags */

/**
 * \defgroup eip_eif_packetdefinitions_tcpip_flags EtherNet/IP Basic TCP/IP Flags
 */
/**@{*/

#define IP_CFG_FLAG_IP_ADDR             (0x0001)      /*!< Specify/use a static IP address */
#define IP_CFG_FLAG_NET_MASK            (0x0002)      /*!< Specify/use a static netmask */
#define IP_CFG_FLAG_GATEWAY             (0x0004)      /*!< Specify/use a static gateway address */
#define IP_CFG_FLAG_BOOTP               (0x0008)      /*!< Use BOOTP to retrive the IP config */
#define IP_CFG_FLAG_DHCP                (0x0010)      /*!< Use DHCP to retrive the IP config */

/*! @} */

#define IP_CFG_FLAG_ETHERNET_ADDR       (0x0020)
#define IP_CFG_FLAG_HTTP_SOCKETS        (0x0040)
#define IP_CFG_FLAG_FQDN_NAME           (0x0080)

/*** #define IP_CFG_FLAG_AUTO_DETECT         (0x0100) ***/
#define IP_CFG_FLAG_INTF_TP             (0x0200)
#define IP_CFG_FLAG_AUTO_NEGOTIATE      (0x0400)
#define IP_CFG_FLAG_FULL_DUPLEX         (0x0800)
#define IP_CFG_FLAG_SPEED_100MBIT       (0x1000)
#define IP_CFG_FLAG_EXTENDED_FLAGS      (0x8000)

#define IP_CFG_FLAG_PORT1_AUTO_NEGOTIATE (0x0400)
#define IP_CFG_FLAG_PORT1_FULL_DUPLEX    (0x0800)
#define IP_CFG_FLAG_PORT1_SPEED_100MBIT  (0x1000)

#endif
