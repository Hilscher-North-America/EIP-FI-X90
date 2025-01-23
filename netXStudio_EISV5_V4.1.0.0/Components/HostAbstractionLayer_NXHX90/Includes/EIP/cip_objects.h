#ifndef CIP_OBJECTS_H_
#define CIP_OBJECTS_H_

#include "device_id.h"
#include "eip_tcp.h"
#include "eip_en.h"
#include "eip_qos.h"

#define IP_ADDRESS(a,b,c,d)       ((((uint32_t)(a)&0xFF)<<24)|(((uint32_t)(b)&0xFF)<<16)|(((uint32_t)(c)&0xFF)<<8)|((uint32_t)(d)&0xFF))

/*************************************************************************************/
/* Assembly instances that shall be supported                                        */
/*************************************************************************************/

#define EIP_CONSUMING_ASSEMBLY_INSTANCE            0x64
#define EIP_CONSUMING_ASSEMBLY_INSTANCE_SIZE       sizeof(APP_PROCESS_DATA_INPUT_T)

#define EIP_PRODUCING_ASSEMBLY_INSTANCE            0x65
#define EIP_PRODUCING_ASSEMBLY_INSTANCE_SIZE       sizeof(APP_PROCESS_DATA_OUTPUT_T)

#define EIP_CONFIGURATION_ASSEMBLY_INSTANCE        0x66
#define EIP_CONFIGURATION_ASSEMBLY_INSTANCE_SIZE   4

#define EIP_LISTENONLY_ASSEMBLY_INSTANCE           0xC0
#define EIP_INPUTONLY_ASSEMBLY_INSTANCE            0xC1

/*****************************************************************************/
/*! Vendor Specific Objcets supported                                        */
/*****************************************************************************/
#define EIP_OBJECT_CLASS_REG                       0x64
#define EIP_OBJECT_CLASS_MFG                       0x65

/*****************************************************************************/
/*! External Global Variables                                                */
/*****************************************************************************/

/*****************************************************************************************************************************************/
/* CIP Identity Object - Class 0x01 - Configuration                                                                                      */
/*****************************************************************************************************************************************/
extern uint16_t usVendor;
extern uint16_t usDeviceType;
extern uint16_t usProductCode;
extern uint16_t usRevision;

/* Default data of configuration assembly --> Adapt as needed! */
extern uint8_t s_abConfigAssemblyData[EIP_CONFIGURATION_ASSEMBLY_INSTANCE_SIZE];

/*****************************************************************************************************************************************/
/* TCP/IP Interface Object - Class 0xF5 - Configuration                                                                                  */
/*****************************************************************************************************************************************/

extern uint32_t                    g_ulConfigCapability;
extern uint32_t                    g_ulConfigControl;
extern EIP_TCP_INTERFACE_CONFIG_T  g_tInterfaceConfig;
extern uint8_t                     g_abHostname[EIP_TCP_MAX_HOSTNAME_LENGTH + 2];
extern EIP_TCP_MCAST_CONFIG_T      g_tMcastConfig;
extern EIP_TCP_ACD_LAST_CONFLICT_T g_tLastConflictDetected;
extern uint8_t                     g_bTTLValue;
extern uint8_t                     g_bSelectAcd;
extern uint16_t                    g_usEncapInactivityTimeout;

/*****************************************************************************************************************************************/
/* Ethernet Link Object - Class 0xF6 - Configuration                                                                                      */
/*****************************************************************************************************************************************/

extern uint32_t g_ulInterfaceControl0;
extern uint32_t g_ulInterfaceControl1;

extern uint8_t  g_bInterfaceType0;
extern uint8_t  g_bInterfaceType1;

extern uint8_t  g_bAdminState0;
extern uint8_t  g_bAdminState1;

extern uint8_t  g_abInterfaceLabel1[16];
extern uint8_t  g_abInterfaceLabel2[16];
extern uint8_t  g_bMDIXCfg;

/*****************************************************************************************************************************************/
/* QoS Object - Configuration                                                                                                            */
/*****************************************************************************************************************************************/

extern uint8_t g_bDSCP_PTP_Event;
extern uint8_t g_bDSCP_PTP_General;
extern uint8_t g_bDSCP_Urgent;
extern uint8_t g_bDSCP_Scheduled;
extern uint8_t g_bDSCP_High;
extern uint8_t g_bDSCP_Low;
extern uint8_t g_bDSCP_Explicit;

extern char    g_abProductName[DEVICE_MODEL_NAME_LENGTH];

#endif // CIP_OBJECTS_H_
