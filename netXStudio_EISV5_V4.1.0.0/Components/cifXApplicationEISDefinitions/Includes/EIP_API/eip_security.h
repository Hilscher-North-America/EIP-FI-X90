#ifndef EIP_SECURITY_H
#define EIP_SECURITY_H


#include "cip_object_dictionary_api.h"

/* -------------------------------------------------------------------------- */

#define EIP_SECURITY_CLASS_NUMBER                       0x5E
#define EIP_SECURITY_REVISION                           7

#define EIP_SECURITY_MAX_INSTANCE                       1
#define EIP_SECURITY_NUM_INSTANCE                       1
#define EIP_SECURITY_MAX_INSTANCE_ATTR                  0x300
#define EIP_SECURITY_NUMBER_OF_INSTANCE_ATTRIBUTES      17 /* Number of standard attributes + 1 vendor specific attribute 768 */

#define EIP_SECURITY_CLASS_ATTR_8_NUMBER_OF_PSKS_SUPPORTED  8

#define EIP_SECURITY_ATTR_1_STATE                           1
#define EIP_SECURITY_ATTR_2_CAPABILITY_FLAGS                2
#define EIP_SECURITY_ATTR_3_AVL_CIPHER_SUITES               3
#define EIP_SECURITY_ATTR_4_ALW_CIPHER_SUITES               4
#define EIP_SECURITY_ATTR_5_PRE_SHARED_KEYS                 5
#define EIP_SECURITY_ATTR_6_ACTIVE_CERTIFICATES             6
#define EIP_SECURITY_ATTR_7_TRUSTED_AUTHORITIES             7
#define EIP_SECURITY_ATTR_8_CERT_REVOC_LIST                 8
#define EIP_SECURITY_ATTR_9_VERIFY_CLIENT_CERT              9
#define EIP_SECURITY_ATTR_10_SEND_CERT_CHAIN                10
#define EIP_SECURITY_ATTR_11_CHECK_EXPIRATION               11
#define EIP_SECURITY_ATTR_12_TRUSTED_IDENTITIES             12
#define EIP_SECURITY_ATTR_13_PULL_MODEL_EN                  13
#define EIP_SECURITY_ATTR_14_PULL_MODEL_STATUS              14
#define EIP_SECURITY_ATTR_15_DTLS_TIMEOUT                   15
#define EIP_SECURITY_ATTR_16_UDP_ONLY_POLICY                16
#define EIP_SECURITY_ATTR_17_CHECK_SUBJECT_ALTERNATIVE_NAME 17
#define EIP_SECURITY_ATTR_768_PRE_SHARED_KEYS               768

#define EIP_SECURITY_STATE_0_FACTORY_DEFAULT_CONFIG     0
#define EIP_SECURITY_STATE_1_CONFIGURATION_IN_PROGRESS  1
#define EIP_SECURITY_STATE_2_CONFIGURED                 2
#define EIP_SECURITY_STATE_3_PULL_MODEL_IN_PROGRESS     3
#define EIP_SECURITY_STATE_4_PULL_MODEL_COMPLETED       4
#define EIP_SECURITY_STATE_5_PULL_MODEL_DISABLED        5

#define EIP_SECURITY_PULL_MODEL_STATUS_PULL_MODEL_DISABLED 0xFFFF

#define EIP_SECURITY_APPLY_CONFIG_BIT0_DROP_CONNECTIONS 0
#define EIP_SECURITY_APPLY_CONFIG_BIT1_CLEANUP_OBJECTS  1

#define EIP_SECURITY_PSK_USAGE_SERVER                   0
#define EIP_SECURITY_PSK_USAGE_CLIENT                   1

#define EIP_SECURITY_CMD_BEGIN_CONFIG                   0x4B
#define EIP_SECURITY_CMD_KICK_TIMER                     0x4C
#define EIP_SECURITY_CMD_APPLY_CONFIG                   0x4D
#define EIP_SECURITY_CMD_ABORT_CONFIG                   0x4E

#define EIP_SECURITY_MAX_NUM_CIPHER_SUITES              20
#define EIP_SECURITY_MAX_CIPHER_SUITES_SIZE             ( 1 + ( (EIP_SECURITY_MAX_NUM_CIPHER_SUITES) * 2 ))

#define EIP_SECURITY_MAX_NUM_PSK_ENTRY                  1
#define EIP_SECURITY_MAX_PSK_ID_SIZE                    128
#define EIP_SECURITY_MAX_PSK_KEY_SIZE                   64
#define EIP_SECURITY_MAX_PSK_STRUCTURE_SIZE             ( 1 /* Size field */ + EIP_SECURITY_MAX_PSK_ID_SIZE  + \
                                                          1 /* Size field */ + EIP_SECURITY_MAX_PSK_KEY_SIZE + \
                                                          1 /* Usage field */ )

#define EIP_SECURITY_MAX_NUM_ACTIVE_DEVICE_CERTIFICATES 1
#define EIP_SECURITY_MAX_NUM_TRUSTED_AUTHORITIES        1
#define EIP_SECURITY_MAX_NUM_TRUSTED_IDENTITIES         1

#define EIP_SECURITY_MAX_DTLS_TIMEOUT_SEC               3600
#define EIP_SECURITY_DEFAULT_DTLS_TIMEOUT_SEC           12
#define EIP_SECURITY_CONFIG_SESSION_TIMEOUT_MS          10000 /* Configuration session timeout is refer to spec-vol8, fixed and equal to 10 seconds */

#define EIP_SECURITY_ESR_01_NO_PSK_CONFIGURED                             0x0001
#define EIP_SECURITY_ESR_02_NO_CERTIFICATES_CONFIGURED                    0x0002
#define EIP_SECURITY_ESR_03_CONFIGURED_CIPHER_SUITES_NOT_SUPPORTED        0x0003
#define EIP_SECURITY_ESR_04_NO_ACTIVE_CIPHER_SUITES_CONFIGURED            0x0004

/* CIP security Supported TLS cipher suites */
#define EIP_SECURITY_TLS_RSA_WITH_NULL_SHA256                                     0x003B
#define EIP_SECURITY_TLS_RSA_WITH_AES_128_CBC_SHA256                              0x003C
#define EIP_SECURITY_TLS_RSA_WITH_AES_256_CBC_SHA256                              0x003D
#define EIP_SECURITY_TLS_ECDHE_ECDSA_WITH_NULL_SHA                                0xC006 /* Required by CIP security */
#define EIP_SECURITY_TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256                      0xC023 /* Required by CIP security */
#define EIP_SECURITY_TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384                      0xC024 /* Required by CIP security */
#define EIP_SECURITY_TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256                      0xC02B /* Required by CIP security */
#define EIP_SECURITY_TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384                      0xC02C /* Required by CIP security */
#define EIP_SECURITY_TLS_ECDHE_PSK_WITH_AES_128_CBC_SHA256                        0xC037 /* Required by CIP security */
#define EIP_SECURITY_TLS_ECDHE_PSK_WITH_NULL_SHA256                               0xC03A /* Required by CIP security */
#define EIP_SECURITY_TLS_ECDHE_ECDSA_WITH_CHACHA20_POLY1305_SHA256                0xCCA9 /* Required by CIP security */
#define EIP_SECURITY_TLS_ECDHE_PSK_WITH_CHACHA20_POLY1305_SHA256                  0xCCAC /* Required by CIP security */
#define EIP_SECURITY_TLS_ECDHE_PSK_WITH_AES_128_GCM_SHA256                        0xD001 /* Required by CIP security - Not supported by mbedTLS */

/* Tuples of TLS cipher suites in BE order (H_Byte, L_Byte)*/
#define EIP_SECURITY_TLS_RSA_WITH_NULL_SHA256_BYTE_TUPLE_BE                       0x00, 0x3B
#define EIP_SECURITY_TLS_RSA_WITH_AES_128_CBC_SHA256_BYTE_TUPLE_BE                0x00, 0x3C
#define EIP_SECURITY_TLS_RSA_WITH_AES_256_CBC_SHA256_BYTE_TUPLE_BE                0x00, 0x3D

#define EIP_SECURITY_TLS_ECDHE_ECDSA_WITH_NULL_SHA_BYTE_TUPLE_BE                  0xC0, 0x06
#define EIP_SECURITY_TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256_BYTE_TUPLE_BE        0xC0, 0x23
#define EIP_SECURITY_TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384_BYTE_TUPLE_BE        0xC0, 0x24
#define EIP_SECURITY_TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256_BYTE_TUPLE_BE        0xC0, 0x2B
#define EIP_SECURITY_TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384_BYTE_TUPLE_BE        0xC0, 0x2C
#define EIP_SECURITY_TLS_ECDHE_ECDSA_WITH_CHACHA20_POLY1305_SHA256_BYTE_TUPLE_BE  0xCC, 0xA9

#define EIP_SECURITY_TLS_ECDHE_PSK_WITH_AES_128_CBC_SHA256_BYTE_TUPLE_BE          0xC0, 0x37
#define EIP_SECURITY_TLS_ECDHE_PSK_WITH_NULL_SHA256_BYTE_TUPLE_BE                 0xC0, 0x3A
#define EIP_SECURITY_TLS_ECDHE_PSK_WITH_CHACHA20_POLY1305_SHA256_BYTE_TUPLE_BE    0xCC, 0xAC
#define EIP_SECURITY_TLS_ECDHE_PSK_WITH_AES_128_GCM_SHA256_BYTE_TUPLE_BE          0xD0, 0x01


typedef union IANA_CIPHER_SUITE_ID_Ttag
{
  uint16_t  usValue; /* Note that the 16bit value is BE. To locally use it, you have to convert to LE */
  /* to comply with the standard format from iana.org
   * i.e  0xC0,0x23 TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256 */
  struct {
    uint8_t  bValueH;
    uint8_t  bValueL;
  } tBytes;

} IANA_CIPHER_SUITE_ID_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST IANA_CIPHER_SUITES_Ttag
{
  uint8_t                bNumOfCipherSuites;
  IANA_CIPHER_SUITE_ID_T atIanaCipherSuiteId[EIP_SECURITY_MAX_NUM_CIPHER_SUITES];
} IANA_CIPHER_SUITES_T;


typedef struct EIP_SECURITY_ACTIVE_DEVICE_CERTIFICATES_Ttag
{
  uint8_t       bNumOfEntries;
  CIP_EPATH_T   atCertMngInstancePath[EIP_SECURITY_MAX_NUM_ACTIVE_DEVICE_CERTIFICATES];

} EIP_SECURITY_ACTIVE_DEVICE_CERTIFICATES_T;

typedef struct EIP_SECURITY_PSK_Ttag
{
  uint8_t   bNumOfEntries;
  uint8_t   abPsk[EIP_SECURITY_MAX_NUM_PSK_ENTRY * (EIP_SECURITY_MAX_PSK_STRUCTURE_SIZE)];
} EIP_SECURITY_PSK_T;


typedef struct EIP_SECURITY_INST_ATTR_STORED_Ttag
{
  uint8_t                 bState;                         /* Attribute 1 */
  uint8_t                 fVerifyClientCertificate;       /* Attribute 9 */
  uint8_t                 fSendCertifacteChain;           /* Attribute 10 */
  uint8_t                 fCheckExpiration;               /* Attribute 11 */
  IANA_CIPHER_SUITES_T    tAllowedCipherSuites;           /* Attribute 4 */

  uint16_t                usDtlsTimeout;                  /* Attribute 15 */ /* 1-3600 seconds */
  EIP_SECURITY_ACTIVE_DEVICE_CERTIFICATES_T tActiveDeviceCertificates;        /* Attribute 6 */

  struct
  {
    uint8_t     bNumOfEntries;
    CIP_EPATH_T atCertFilePath[EIP_SECURITY_MAX_NUM_TRUSTED_AUTHORITIES];
  } tTrustedAuthorities;                      /* Attribute 7 */

  CIP_EPATH_T tCertificateRevocationList;     /* Attribute 8 */

  struct
  {
    uint8_t     bNumOfEntries;
    CIP_EPATH_T atEntryPath[EIP_SECURITY_MAX_NUM_TRUSTED_IDENTITIES];
  } tTrustedIdentities;                       /* Attribute 12 */

  /* As in Cip spec. vol8:
   *    "When accessed via Get_Attributes_All or Get_Attribute_Single, should only the Size of PSK element shall be 0,
   *     and 0 bytes of PSK value shall be returned. This ensures that the PSK value cannot be read out of the device,
   *     as it is a confidential piece of information."
   * Respect to the above instructions the tAttr768_PreSharedKeys(0x300) is added as a member of NV structure. All the
   * SetAttributeSingle to Attr 5, will be redirected to tAttr768_PreSharedKeys as the real container of the PSK.
   */
  EIP_SECURITY_PSK_T tAttr768_PreSharedKeys;                  /* Attribute 0x300 */

} EIP_SECURITY_INST_ATTR_STORED_T;


uint32_t EipSecurity_ObjectCreate( struct EIP_RESOURCES_Ttag* hEip, void* pvParam );


#endif


