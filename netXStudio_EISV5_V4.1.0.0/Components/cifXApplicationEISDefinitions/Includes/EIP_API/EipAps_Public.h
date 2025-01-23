/**************************************************************************************
Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
***************************************************************************************
$Id: EipAps_Public.h 79457 2018-12-10 09:59:51Z kai $:

Description:
  EtherNet/IP Public Definitions
**************************************************************************************/

#ifndef EIP_DPMINTF_PUBLIC_H_
#define EIP_DPMINTF_PUBLIC_H_

/* LFW Packet API: Compiler-specific macro definitions */
#include "Hil_Compiler.h"

/* LFW Packet API: Hilscher common packet definitions */
#include "Hil_Packet.h"

/* LFW Packet API: Hilscher common status codes */
#include "Hil_Results.h"

/* LFW Packet API: Hilscher common command codes and packets */
#include "Hil_ApplicationCmd.h"

/* LFW Packet API: Hilscher component identifiers */
#include "Hil_ComponentID.h"

/* LFW Packet API: Assembly registration */
#include "cip_as_flags.h"
/* LFW Packet API: Command codes */
#include "eip_eif_packetdefinitions_commands.h"
/* LFW Packet API: Connection types/states */
#include "eip_eif_packetdefinitions_connections.h"
/* LFW Packet API: Packet definitions */
#include "eip_eif_packetdefinitions_packets.h"
/* LFW Packet API: Protocol Stack Parameters to disable/enable certain features */
#include "eip_eif_packetdefinitions_parameters.h"
/* LFW Packet API: Quality of Service Object flags */
/* LFW Packet API: Quick Connect control flags */
#include "eip_eif_packetdefinitions_quickconnect.h"
/* LFW Packet API: System autostart control flags */
#include "eip_eif_packetdefinitions_sysflags.h"
/* LFW Packet API: TCP/IP control flags */
#include "eip_eif_packetdefinitions_tcpip.h"
/* LFW Packet API: AP task status codes */
#include "EipAps_Results.h"

/* LFW Packet API: Common definitions for all CIP objects */
#include "cip_common.h"
/* LFW Packet API: Definitions for the Quality of Service object */
#include "eip_qos.h"
/* LFW Packet API: Definitions for the TCP/IP Interface object */
#include "eip_tcp.h"
/* LFW Packet API: Definitions for the Predefined Connection object */
#include "cip_pdc.h"
/* LFW Packet API: Definitions for the Assembly object */
#include "cip_as.h"
/* LFW Packet API: Definitions for the EtherNetLink object */
#include "eip_en.h"
/* LFW Packet API: Definitions for the Identity object */
#include "cip_id.h"
/* LFW Packet API: Definitions for the TimeSync object */
#include "cip_timesync.h"
/* LFW Packet API: Definitions for the Device Level Ring object */
#include "eip_dlr.h"
/* LFW Packet API: Definitions for the Hilscher I/O object */
#include "cip_io.h"
/* LFW Packet API: Definitions for the Message Router object */
#include "cip_mr.h"
/* LFW Packet API: Definitions of EtherNet/IP stack status codes */
#include "Eip_Results.h"
/* LFW Packet API: Definitions of EtherNet/IP extended status codes */
#include "eip_public.h"
/* LFW Packet API: Definitions of the attribute flags of the CIP object dictionary */
#include "cip_object_dictionary_api.h"



/***************************************************************************************/
#endif
