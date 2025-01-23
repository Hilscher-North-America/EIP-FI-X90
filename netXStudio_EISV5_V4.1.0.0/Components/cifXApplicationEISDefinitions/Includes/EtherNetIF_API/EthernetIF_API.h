/**************************************************************************************
  Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
***************************************************************************************
  $Id: EthernetIF_API.h 79765 2019-01-07 12:44:14Z ThomasBaranczyk $: *//*!

  \file EthernetIF_API.h

  Application commands used for RAW Ethernet communication.

**************************************************************************************/
#ifndef ETHERNET_IF_API_H_
#define ETHERNET_IF_API_H_

#include <stdint.h>
#include "Hil_Packet.h"
#include "Hil_Types.h"


/*! Ethernet interface commands. */
typedef enum ETHERNET_IF_CMD_Etag
{
  /* Commands from 0x00003B00 to 0x00003B21, are occupied by older implementations.  */

  /*! Send ethernet frame request */
  ETHERNET_IF_SEND_ETH_FRAME_REQ         = 0x00003B22,
  /*! Send ethernet frame confirmation */
  ETHERNET_IF_SEND_ETH_FRAME_CNF         = 0x00003B23,

  /*! Received ethernet frame indication */
  ETHERNET_IF_RECV_ETH_FRAME_IND         = 0x00003B24,
  /*! Received ethernet frame response */
  ETHERNET_IF_RECV_ETH_FRAME_RSP         = 0x00003B25,

  /*! Register for multicast reception for certain group address request */
  ETHERNET_IF_SET_MULTICAST_ADDR_REQ     = 0x00003B26,
  /*! Register for multicast reception for certain group address confirmation */
  ETHERNET_IF_SET_MULTICAST_ADDR_CNF     = 0x00003B27,

  /*! Unregister for multicast reception for certain group address request */
  ETHERNET_IF_CLR_MULTICAST_ADDR_REQ     = 0x00003B28,
  /*! Unregister for multicast reception for certain group address confirmation */
  ETHERNET_IF_CLR_MULTICAST_ADDR_CNF     = 0x00003B29,

  /*! Unregister for multicast reception for all group address request */
  ETHERNET_IF_CLR_ALL_MULTICAST_ADDR_REQ = 0x00003B2A,
  /*! Unregister for multicast reception for all group address confirmation */
  ETHERNET_IF_CLR_ALL_MULTICAST_ADDR_CNF = 0x00003B2B,

  /* The following commands are usually disabled and not indent to be used by
   * host application.
  ETHERNET_IF_GET_HW_STAT_REQ           = 0x00003BFC,
  ETHERNET_IF_GET_HW_STAT_CNF           = 0x00003BFD,

  ETHERNET_IF_PEEK_REGISTER_REQ         = 0x00003BFE,
  ETHERNET_IF_PEEK_REGISTER_CNF         = 0x00003BFF,
   */

} ETHERNET_IF_CMD_E;




#ifdef __HIL_PRAGMA_PACK_ENABLE
  #pragma __HIL_PRAGMA_PACK_1(ETHERNET_IF_API_PUBLIC)
#endif


/*! Size of an ethernet mac address */
#define ETHERNET_IF_ETH_ADDR_SIZE            6

/*! Minimum Ethernet Frame length (without FCS). */
#define ETHERNET_IF_ETH_MIN_FRAME_SIZE      60

/*! Maximum Ethernet Frame length (without FCS). */
#define ETHERNET_IF_ETH_MAX_FRAME_SIZE    1518

/*! Maximum Ethernet Frame data size. */
#define ETHERNET_IF_ETH_MAX_DATA_SIZE     1506


/*! General Ethernet frame data type */
typedef __HIL_PACKED_PRE struct ETHERNET_IF_ETH_FRAME_Ttag
{
  /*! Destination Mac Address */
  uint8_t abDestMacAddr[ETHERNET_IF_ETH_ADDR_SIZE];
  /*! Source Mac Address */
  uint8_t abSrcMacAddr[ETHERNET_IF_ETH_ADDR_SIZE];
  /*! Remaining Data */
  uint8_t  abData[ETHERNET_IF_ETH_MAX_DATA_SIZE];

} __HIL_PACKED_POST ETHERNET_IF_ETH_FRAME_T;

/*! General IPv4 multicast group MAC data type */
typedef __HIL_PACKED_PRE struct ETHERNET_IF_MULTICAST_ADDR_Ttag
{
  /*! IPv4 multicast group MAC address. */
  uint8_t abMacAddr[ETHERNET_IF_ETH_ADDR_SIZE];

} __HIL_PACKED_POST ETHERNET_IF_MULTICAST_ADDR_T;



/*! \defgroup doc_ETHERNET_IF_SEND_ETH_FRAME Send Ethernet Frame
 *
 * This service is used by the Host Application to send an ethernet frame.
 *
 * @{
 */

/*! Send Ethernet frame request packet */
typedef __HIL_PACKED_PRE struct ETHERNET_IF_SEND_ETH_FRAME_REQ_Ttag
{
  HIL_PACKET_HEADER_T       tHead; /*!< Packet header. */
  ETHERNET_IF_ETH_FRAME_T   tData; /*!< Packet data. */

} __HIL_PACKED_POST ETHERNET_IF_SEND_ETH_FRAME_REQ_T;

/*! Send Ethernet frame confirmation packet */
typedef HIL_EMPTY_PACKET_T  ETHERNET_IF_SEND_ETH_FRAME_CNF_T;

/*! Send Ethernet frame packet union */
typedef union ETHERNET_IF_SEND_ETH_FRAME_PCK_Ttag
{
  /*! Request */
  ETHERNET_IF_SEND_ETH_FRAME_REQ_T tReq;
  /*! Confirmation */
  ETHERNET_IF_SEND_ETH_FRAME_CNF_T tCnf;
} ETHERNET_IF_SEND_ETH_FRAME_PCK_T;

/*! @} */


/*! \defgroup doc_ETHERNET_IF_RECV_ETH_FRAME Receive Ethernet Frame
 *
 * This indication is sent to the host application if a new frame has been
 * received.
 *
 * @{
 */

/*! Received Ethernet frame indication packet */
typedef __HIL_PACKED_PRE struct ETHERNET_IF_RECV_ETH_FRAME_IND_Ttag
{
  HIL_PACKET_HEADER_T       tHead; /*!< Packet header. */
  ETHERNET_IF_ETH_FRAME_T   tData; /*!< Packet data. */

} __HIL_PACKED_POST ETHERNET_IF_RECV_ETH_FRAME_IND_T;

/*! Received Ethernet frame response packet */
typedef HIL_EMPTY_PACKET_T  ETHERNET_IF_RECV_ETH_FRAME_RSP_T;

/*! Received Ethernet frame packet union */
typedef union ETHERNET_IF_RECV_ETH_FRAME_PCK_Ttag
{
  /*! Indication */
  ETHERNET_IF_RECV_ETH_FRAME_IND_T tInd;
  /*! Response */
  ETHERNET_IF_RECV_ETH_FRAME_RSP_T tRsp;
} ETHERNET_IF_RECV_ETH_FRAME_PCK_T;

/*! @} */


/*! \defgroup doc_ETHERNET_IF_SET_MULTICAST_ADDR Set Multicast Address Service.
 *
 * This used by host to receive traffic from a specific IPv4
 * multicast group MAC address.
 *
 * @{
 */

/*! Register multicast group request packet */
typedef __HIL_PACKED_PRE struct ETHERNET_IF_SET_MULTICAST_ADDR_REQ_Ttag
{
  HIL_PACKET_HEADER_T           tHead; /*!< Packet header. */
  ETHERNET_IF_MULTICAST_ADDR_T  tData; /*!< Packet data. */

} __HIL_PACKED_POST ETHERNET_IF_SET_MULTICAST_ADDR_REQ_T;

/*! Register multicast group confirmation packet */
typedef HIL_EMPTY_PACKET_T  ETHERNET_IF_SET_MULTICAST_ADDR_CNF_T;

/*! Register multicast group reception packet union */
typedef union ETHERNET_IF_SET_MULTICAST_ADDR_PCK_Ttag
{
  /*! Request */
  ETHERNET_IF_SET_MULTICAST_ADDR_REQ_T tReq;
  /*! Confirmation */
  ETHERNET_IF_SET_MULTICAST_ADDR_CNF_T tCnf;
} ETHERNET_IF_SET_MULTICAST_ADDR_PCK_T;

/*! @} */


/*! \defgroup doc_ETHERNET_IF_CLR_MULTICAST_ADDR Clear Multicast Address Service.
 *
 * This used by host to stop receiving traffic from a specific IPv4
 * multicast group MAC address.
 *
 * @{
 */

/*! Unregister multicast group request packet */
typedef __HIL_PACKED_PRE struct ETHERNET_IF_CLR_MULTICAST_ADDR_REQ_Ttag
{
  HIL_PACKET_HEADER_T           tHead; /*!< Packet header. */
  ETHERNET_IF_MULTICAST_ADDR_T  tData; /*!< Packet data. */

} __HIL_PACKED_POST ETHERNET_IF_CLR_MULTICAST_ADDR_REQ_T;

/*! Unregister multicast group confirmation packet */
typedef HIL_EMPTY_PACKET_T  ETHERNET_IF_CLR_MULTICAST_ADDR_CNF_T;

/*! Unregister multicast group reception packet union */
typedef union ETHERNET_IF_CLR_MULTICAST_ADDR_PCK_Ttag
{
  /*! Request */
  ETHERNET_IF_CLR_MULTICAST_ADDR_REQ_T tReq;
  /*! Confirmation */
  ETHERNET_IF_CLR_MULTICAST_ADDR_CNF_T tCnf;
} ETHERNET_IF_CLR_MULTICAST_ADDR_PCK_T;

/*! @} */


/*! \defgroup doc_ETHERNET_IF_CLR_ALL_MULTICAST_ADDR Clear All Multicast Address Service.
 *
 * This service is used by the host application to stop receiving traffic of all
 * previously enabled multicast group MAC addresses. However, it is possible
 * to receive multicast frames even if explicitly disabled.
 *
 * @{
 */

/*! Unregister all multicast group request packet */
typedef HIL_EMPTY_PACKET_T  ETHERNET_IF_CLR_ALL_MULTICAST_ADDR_REQ_T;

/*! Unregister all multicast group confirmation packet */
typedef HIL_EMPTY_PACKET_T  ETHERNET_IF_CLR_ALL_MULTICAST_ADDR_CNF_T;

/*! Unregister all multicast group reception packet union */
typedef union ETHERNET_IF_CLR_ALL_MULTICAST_ADDR_PCK_Ttag
{
  /*! Request */
  ETHERNET_IF_CLR_ALL_MULTICAST_ADDR_REQ_T tReq;
  /*! Confirmation */
  ETHERNET_IF_CLR_ALL_MULTICAST_ADDR_CNF_T tCnf;
} ETHERNET_IF_CLR_ALL_MULTICAST_ADDR_PCK_T;

/*! @} */



#ifdef __HIL_PRAGMA_PACK_ENABLE
  #pragma __HIL_PRAGMA_UNPACK_1(ETHERNET_IF_API_PUBLIC)
#endif

#endif /* ETHERNET_IF_API_H_ */
