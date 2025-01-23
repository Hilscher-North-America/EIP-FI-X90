#ifndef INGRESS_EGRESS_H
#define INGRESS_EGRESS_H

#include "cip_common.h"
#include <stdint.h>

#define INGRESS_EGRESS_CLASS_NUMBER                             0x03AC
#define INGRESS_EGRESS_REVISION                                 1
#define INGRESS_EGRESS_MAX_INSTANCE                             1

/* class attributes defaults */
#define INGRESS_EGRESS_MAX_CLASS_ATTR                           12
#define INGRESS_EGRESS_MAX_INSTANCE_ATTR                        3

/* specific class attributes for this object */
#define INGRESS_EGRESS_CLASS_ATTR_8_TCP_PORT_RANGES_SUPPORTED   8
#define INGRESS_EGRESS_CLASS_ATTR_9_UDP_PORT_RANGES_SUPPORTED   9
#define INGRESS_EGRESS_CLASS_ATTR_10_MAX_RULES                  10
#define INGRESS_EGRESS_CLASS_ATTR_11_MAX_BUFFER_SIZE_FOR_RULES  11
#define INGRESS_EGRESS_CLASS_ATTR_12_RULES_CHANGE_COUNT         12

/* instance attributes */
#define INGRESS_EGRESS_ATTR_1_PORT_OBJECT_INSTANCE              1
#define INGRESS_EGRESS_ATTR_2_INGRESS_RULES                     2
#define INGRESS_EGRESS_ATTR_3_EGRESS_RULES                      3

/* specific service */
#define INGRESS_EGRESS_CMD_SET_RULES                            0x4B

/* Memory demand */
#define INGRESS_EGRESS_MAX_RULES                                4
#define INGRESS_EGRESS_MAX_RULES_SIZE                           128      /* in byte granularity */

#define INGRESS_EGRESS_MAX_NUM_OF_CIPHER_SUITES                 10
#define INGRESS_EGRESS_CIPHER_SUITES_SIZE                      (sizeof("FFFF") - 1)

#define INGRESS_EGRESS_SET_RULES_DROP_CONNECTIONS_DELAY_MS      250

#define INGRESS_EGRESS_GSR_SYNTAX_ERROR                         0xD0
#define INGRESS_EGRESS_GSR_RULES_CHANGE_COUNT_MISMATCH          0xD2
#define INGRESS_EGRESS_GSR_DUPLICATED_INSTANCE                  0xD3
#define INGRESS_EGRESS_GSR_MAX_RULES_EXCEEDED                   0xD4
#define INGRESS_EGRESS_GSR_MAX_BUFFER_EXCEEDED                  0xD5
#define INGRESS_EGRESS_GSR_INVALID_INGRESS_VALUE                0xD6
#define INGRESS_EGRESS_GSR_INVALID_EGRESS_VALUE                 0xD7

#define INGRESS_EGRESS_ESR_INVALID_VALUE_IN_INGRESS_RULE_SRC_ADDRESS_RANGE  (1 << 0)
#define INGRESS_EGRESS_ESR_INVALID_VALUE_IN_INGRESS_RULE_DST_CIP_PORT       (1 << 1)
#define INGRESS_EGRESS_ESR_INVALID_VALUE_IN_INGRESS_RULE_DST_ADDRESS_RANGE  (1 << 2)
#define INGRESS_EGRESS_ESR_INVALID_VALUE_IN_INGRESS_RULE_PROTOCOL           (1 << 3)
#define INGRESS_EGRESS_ESR_INVALID_VALUE_IN_INGRESS_RULE_ACTION             (1 << 4)

#define INGRESS_EGRESS_ESR_INVALID_VALUE_IN_EGRESS_RULE_ADDRESS_RANGE       (1 << 0)
#define INGRESS_EGRESS_ESR_INVALID_VALUE_IN_EGRESS_RULE_PROTOCOL            (1 << 1)
#define INGRESS_EGRESS_ESR_INVALID_VALUE_IN_EGRESS_RULE_ACTION              (1 << 2)
#define INGRESS_EGRESS_ESR_INVALID_VALUE_IN_EGRESS_RULE_CIPHER_LIST         (1 << 3)

#define INGRESS_EGRESS_ESR_NO_MATCHING_INGRESS_RULE                         0x0918
#define INGRESS_EGRESS_ESR_INGRESS_RULE_DENY                                0x0919
#define INGRESS_EGRESS_ESR_INGRESS_RULE_DENY_NONE_SECURE                    0x091A

#define CIPHIL_CMD_INGRESS_EGRESS_CHECK_INGRESS_REQUEST_PERMISSION          0x3AC1

#define INGRESS_EGRESS_SET_RULES_APPLY_BEHAVIOR_DROP_CONNECTIONS            (1 << 0)

typedef struct INGRESS_EGRESS_INST_ATTR_STORED_Ttag
{
  uint16_t                usPortObjectInstance;             /* attr 1 */
  /* Array size (UINT) in first two bytes followed by n CIP_SHORTSTRING datatypes, one for each rule */
  uint8_t   abIngressRules[INGRESS_EGRESS_MAX_RULES_SIZE];  /* attr 2 */
  /* Array size (UINT) in first two bytes followed by n CIP_SHORTSTRING datatypes, one for each rule */
  uint8_t   abEgressRules[INGRESS_EGRESS_MAX_RULES_SIZE];   /* attr 3 */

} INGRESS_EGRESS_INST_ATTR_STORED_T;


typedef struct INGRESS_EGRESS_CLASS_ATTR_STORED_Ttag
{
  uint32_t ulRulesChangeCount;          /* 12 */

}INGRESS_EGRESS_CLASS_ATTR_STORED_T;

struct EIP_RESOURCES_Ttag;  /* forward declaration */

/** Creates the Ingress/Egress object
 *
 * \param hEip    [in] Handle to EtherNet/IP stack resources
 * \param pvParam [in] Set to NULL (not used yet)
 *
 * \return SUCCESS_HIL_OK on success, ERR_EIP_OBJECT_OUT_OF_MEMORY otherwise.
 */
uint32_t
IngressEgress_ObjectCreate(struct EIP_RESOURCES_Ttag *hEip, void* pvParam );

#endif
