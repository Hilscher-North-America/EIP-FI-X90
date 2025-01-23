#include "cip_objects.h"

/*****************************************************************************/
/*! Global Variables                                                         */
/*****************************************************************************/

/*****************************************************************************************************************************************/
/* CIP Identity Object - Class 0x01 - Configuration                                                                                      */
/*****************************************************************************************************************************************/
uint16_t usVendor       = VENDOR_ID;                               /* Attribute  1: Hilscher Vendor ID                   */
uint16_t usDeviceType   = DEVICE_TYPE_COMMUNICATION_ADAPTER;       /* Attribute  2: Communications Device                */
uint16_t usProductCode  = PRODUCT_CODE;                            /* Attribute  3: Vendor specific Product Code         */
uint16_t usRevision     = (MAJOR_REVISION << 8) | MINOR_REVISION;  /* Attribute  4: Major: 1, Minor : 1                  */

/* Default data of configuration assembly --> Adapt as needed! */
uint8_t s_abConfigAssemblyData[EIP_CONFIGURATION_ASSEMBLY_INSTANCE_SIZE] = {0x00};


/*****************************************************************************************************************************************/
/* TCP/IP Interface Object - Class 0xF5 - Configuration                                                                                  */
/*****************************************************************************************************************************************/

uint32_t                    g_ulConfigCapability = EIP_TCP_CAP_BOOTP                           /* Attribute  2 */
                                                | EIP_TCP_CAP_DHCP
                                                | EIP_TCP_CAP_CNFSET
                                                | EIP_TCP_CAP_ACD_CAPABLE;

uint32_t                    g_ulConfigControl = EIP_TCP_CNTRL_INTERN;                            /* Attribute  3 */

EIP_TCP_INTERFACE_CONFIG_T  g_tInterfaceConfig         = { IP_ADDRESS(192,168,9,1),     /* Attribute  5                  */
                                                                                        /* IP :           192.168.9.1    */
                                                         IP_ADDRESS(255,255,255,0),     /* Subnet :       255.255.255.0  */
                                                         IP_ADDRESS(0,0,0,0),           /* Gateway:       0.0.0.0.       */
                                                         IP_ADDRESS(0,0,0,0),           /* Name Server1 : 0.0.0.0.       */
                                                         IP_ADDRESS(0,0,0,0),           /* Name Server2 : 0.0.0.0.       */
                                                         {0x0, 0x0} };                  /* Domain Name  : ""             */

uint8_t                     g_abHostname[EIP_TCP_MAX_HOSTNAME_LENGTH + 2];                /* Host name */
EIP_TCP_MCAST_CONFIG_T      g_tMcastConfig;                                               /* Multicast configuration */
EIP_TCP_ACD_LAST_CONFLICT_T g_tLastConflictDetected;                                      /* Last detected IP conflict  */




uint8_t                     g_bTTLValue = 1;                                              /* Attribute  8 */

uint8_t                     g_bSelectAcd = 1;                                             /* Attribute 10 */

uint16_t                    g_usEncapInactivityTimeout = EIP_TCP_ENCAP_TIMEOUT_DEFAULT;                 /* Attribute 13 */

/*****************************************************************************************************************************************/
/* Ethernet Link Object - Class 0xF6 - Configuration                                                                                      */
/*****************************************************************************************************************************************/

uint32_t g_ulInterfaceControl0 = 0x1;                         /* Interface control for port 0: Autoneg */
uint32_t g_ulInterfaceControl1 = 0x1;                         /* Interface control for port 1: Autoneg */

uint8_t  g_bInterfaceType0 = EIP_EN_INTF_TYPE_TWISTEDPAIR;   /* Interface type for port 0 */
uint8_t  g_bInterfaceType1 = EIP_EN_INTF_TYPE_TWISTEDPAIR;   /* Interface type for port 1 */

uint8_t  g_bAdminState0 = 1;         /* Admin state for port 0 : port enabled */
uint8_t  g_bAdminState1 = 1;         /* Admin state for port 1 : port enabled */

uint8_t  g_abInterfaceLabel1[16] = "\x05" "Port0";                 /* Attribute 10 - Port 0                                                                 */
uint8_t  g_abInterfaceLabel2[16] = "\x05" "Port1";                 /* Attribute 10 - Port 1                                                                 */
uint8_t  g_bMDIXCfg = EIP_EN_INTF_MDIX_AUTO;        /* Attribute 300 - vendor specific attribute to set MDI settings (used for port 0 and 1) */

/*****************************************************************************************************************************************/
/* QoS Object - Configuration                                                                                                            */
/*****************************************************************************************************************************************/

uint8_t g_bDSCP_PTP_Event     = EIP_QOS_DSCP_PTP_EVENT_DEFAULT;
uint8_t g_bDSCP_PTP_General   = EIP_QOS_DSCP_PTP_GENERAL_DEFAULT;
uint8_t g_bDSCP_Urgent        = EIP_QOS_DSCP_URGENT_DEFAULT;
uint8_t g_bDSCP_Scheduled     = EIP_QOS_DSCP_SCHEDULED_DEFAULT;
uint8_t g_bDSCP_High          = EIP_QOS_DSCP_HIGH_DEFAULT;
uint8_t g_bDSCP_Low           = EIP_QOS_DSCP_LOW_DEFAULT;
uint8_t g_bDSCP_Explicit      = EIP_QOS_DSCP_EXPLICIT_DEFAULT;

char g_abProductName[] = "\x011" "Factory Interface";

//char    g_abProductName[] = "\x1B" "EIS_V5_EXTENDED_CONFIG_DEMO";

