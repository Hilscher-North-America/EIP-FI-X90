
#ifndef CIP_SECURITY_H
#define CIP_SECURITY_H

#include "cip_certificate_management.h"
#include "cip_file.h"

/* -------------------------------------------------------------------------- */
#define CIP_SECURITY_CLASS_NUMBER                               0x5D
#define CIP_SECURITY_REVISION                                   4

#define CIP_SECURITY_MAX_INSTANCE                               1
#define CIP_SECURITY_NUM_INSTANCE                               1
#define CIP_SECURITY_MAX_INSTANCE_ATTR                          3


#define CIP_SECURITY_EIP_INTEGRITY_PROFILE                      ( 1 << 0 ) /* obsoleted */
#define CIP_SECURITY_EIP_CONFIDENTIALITY_PROFILE                ( 1 << 1 )
#define CIP_SECURITY_CIP_AUTHORIZATION_PROFILE                  ( 1 << 2 )
#define CIP_SECURITY_USER_AUTHENTICATION_PROFILE                ( 1 << 3 )
#define CIP_SECURITY_RESOURCE_CONSTRAINED_PROFILE               ( 1 << 4 )

#define CIP_SECURITY_ATTR_1_STATE                               1
#define CIP_SECURITY_ATTR_2_SECURITY_PROFILES                   2
#define CIP_SECURITY_ATTR_3_SECURITY_PROFILES_CONFIGURED        3

#define CIP_SECURITY_STATE_0_FACTORY_DEFAULT_CONFIG             0
#define CIP_SECURITY_STATE_1_CONFIGURATION_IN_PROGRESS          1
#define CIP_SECURITY_STATE_2_CONFIGURED                         2
#define CIP_SECURITY_STATE_3_INCOMPLETE_CONFIGURATION           3

#define CIP_SECURITY_CMD_BEGIN_CONFIG                           0x4B
#define CIP_SECURITY_CMD_KICK_TIMER                             0x4C
#define CIP_SECURITY_CMD_END_CONFIG                             0x4D
#define CIP_SECURITY_CMD_OBJECT_CLEANUP                         0x4E

#define CIP_SECURITY_GSR_NO_ORPHAN_OBJECTS                      0xD0

#define CIP_SECURITY_CONFIG_SESSION_TIMEOUT_MS                  10000

#define CIP_SECURITY_MAX_SECURITY_RELATED_CIP_INSTANCES         ( CIP_FILE_MAX_NUM_OF_DYNAMIC_INSTANCES + \
                                                                  CIP_CERT_MNG_MAX_NUM_OF_DYNAMIC_INSTANCES )



/* -------------------------------------------------------------------------- */

  typedef struct CIP_SECURITY_INST_ATTR_STORED_Ttag
  {
    uint8_t   bState;

  } CIP_SECURITY_INST_ATTR_STORED_T;

uint32_t  CipSecurity_ObjectCreate( struct EIP_RESOURCES_Ttag* hEip, void* pvParam );

#endif
