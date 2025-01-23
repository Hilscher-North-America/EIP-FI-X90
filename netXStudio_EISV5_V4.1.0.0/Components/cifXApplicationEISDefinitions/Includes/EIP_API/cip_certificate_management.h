#ifndef CIP_CERTIFICATE_MANAGEMENT_H
#define CIP_CERTIFICATE_MANAGEMENT_H

#include "cip_object_dictionary_core.h"
#include "cip_object_dictionary_api.h"
#include "cip_strings.h"
#include "eip_security.h"
/* -------------------------------------------------------------------------- */

#define CIP_CERT_MNG_CLASS_NUMBER                           0x5F

#define CIP_CERT_MNG_REVISION                               2 /* Support for Subject Alternative Name in Create_CSR service */

#define CIP_CERT_MNG_MAX_CLASS_ATTR                         10

#define CIP_CERT_MNG_NUM_OF_STATIC_INSTANCES                1
#define CIP_CERT_MNG_MAX_NUM_OF_DYNAMIC_INSTANCES           2
#define CIP_CERT_MNG_MAX_NUM_OF_INSTANCES                   (CIP_CERT_MNG_NUM_OF_STATIC_INSTANCES + CIP_CERT_MNG_MAX_NUM_OF_DYNAMIC_INSTANCES)

#define CIP_CERT_MNG_MAX_INSTANCE                           123 /* This value depends on number of dynamic instances */
#define CIP_CERT_MNG_MAX_INSTANCE_ATTR                      5

#define CIP_CERT_MNG_CLASS_ATTR_768_EXISTING_INSTANCES      768

#define CIP_CERT_MNG_CLASS_ATTR_8_CAP_FLAGS                 8
#define CIP_CERT_MNG_CLASS_ATTR_9_CERT_LIST                 9
#define CIP_CERT_MNG_CLASS_ATTR_10_CERT_ENCODING_FLAGS      10

#define CIP_CERT_MNG_ATTR_1_NAME                            1
#define CIP_CERT_MNG_ATTR_2_STATE                           2
#define CIP_CERT_MNG_ATTR_3_DEVICE_CERTIFICATE              3
#define CIP_CERT_MNG_ATTR_4_CA_CERTIFICATE                  4
#define CIP_CERT_MNG_ATTR_5_CERTIFICATE_ENCODING            5

#define CIP_CERT_MNG_CERTIFICATE_STATUS_0_NOT_VERIFIED      0
#define CIP_CERT_MNG_CERTIFICATE_STATUS_1_VERIFIED          1
#define CIP_CERT_MNG_CERTIFICATE_STATUS_2_INVALID           2

//#define CIP_CERT_MNG_MAX_PATH_SIZE                          8 /* 8bit classId segment + 32bit InstanceId segment */
#define CIP_CERT_MNG_MAX_INST_NAME_SIZE                     32

#define CIP_CERT_MNG_CAPABILITY_FLAGS_PUSH                  (1 << 0)
#define CIP_CERT_MNG_CAPABILITY_FLAGS_PULL                  (1 << 1)

#define CIP_CERT_MNG_CERTIFICATE_ENCODING_FLAGS_PEM         (1 << 0)
#define CIP_CERT_MNG_CERTIFICATE_ENCODING_FLAGS_PKCS7       (1 << 1)

#define CIP_CERT_MNG_CERTIFICATE_ENCODING_FORMAT_PEM        0
#define CIP_CERT_MNG_CERTIFICATE_ENCODING_FORMAT_PKCS7      1

#define CIP_CERT_MNG_CMD_CREATE_CSR                         0x4B
#define CIP_CERT_MNG_CMD_VERIFY_CERTIFICATE                 0x4C

#define CIP_CERT_MNG_INST_STATE_0_NONE_EXISTENT             0
#define CIP_CERT_MNG_INST_STATE_1_CREATED                   1
#define CIP_CERT_MNG_INST_STATE_2_CONFIGURING               2
#define CIP_CERT_MNG_INST_STATE_3_VERIFIED                  3
#define CIP_CERT_MNG_INST_STATE_4_INVALID                   4

/* Create CSR request parameter definitions */
#define CIP_CERT_MNG_CREATE_CSR_REQUEST_SDN_PARAMETER_COUNT             8
/* Maximum length of Create CSR request SDN tag labels. */
#define CIP_CERT_MNG_CREATE_CSR_SDN_MAX_TAG_LABLE_LENGTH  sizeof("CN=,O=,OU=,L=,S=,C=,emailAddress=,serialNumber=")

/* Set the proper version to automatically set the parameter count */
#if (CIP_CERT_MNG_REVISION < 2)
#define CIP_CERT_MNG_CREATE_CSR_REQUEST_SAN_PARAMETER_COUNT             0
#else
#define CIP_CERT_MNG_CREATE_CSR_REQUEST_SAN_PARAMETER_COUNT             1
#endif
#define CIP_CERT_MNG_CREATE_CSR_REQUEST_PARAMETER_COUNT                 ((CIP_CERT_MNG_CREATE_CSR_REQUEST_SDN_PARAMETER_COUNT) + \
                                                                         (CIP_CERT_MNG_CREATE_CSR_REQUEST_SAN_PARAMETER_COUNT))



#define CIP_CERT_MNG_CSR_BUFFER_LEN         2048
#define CIP_CERT_MNG_SDN_BUFFER_LEN         512
#define CIP_CERT_MNG_SAN_BUFFER_LEN         512
#define CIP_CERT_MNG_CREATE_FILE_REQUEST_DATA_BUFFER_LEN  (CIP_FILE_CREATE_WITH_CONTENTS_MAX_PARAMETER_SIZE)


#define CIP_CERT_MNG_GSR_CERTIFICATE_VERIFICATION_FAILED    0xD0

#define CIP_CERT_MNG_CLASS_ATTR_9_MAX_DATA_LEN              ( sizeof(uint8_t) + \
                                                              CIP_CERT_MNG_MAX_NUM_OF_INSTANCES * \
                                                              ( sizeof(CIP_SHORT_STRING_T) + \
                                                                sizeof(CIP_EPATH_T) ) )

#define CIP_CERT_MNG_MAX_ACCEPTABLE_CERTIFICATE_SIZE        0x1000

/* -------------------------------------------------------------------------- */
typedef struct CIP_CERT_MNG_CLASS_ATTR_STORED_Ttag
{
  uint16_t ausExistingInstances[CIP_CERT_MNG_MAX_NUM_OF_INSTANCES];  /* Attribute 0x300, List of all instances (statics + dynamics)
                                                               * ausInstanceId[Index] either contains an existing InstanceId
                                                               * or zero that means the Instance is not created yet. */

} CIP_CERT_MNG_CLASS_ATTR_STORED_T;

/* -------------------------------------------------------------------------- */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST CIP_CERT_MNG_CERTIFICATE_Ttag
{
  uint8_t     bCertificateStatus;
  CIP_EPATH_T tPath;    /* File object instance */

} CIP_CERT_MNG_CERTIFICATE_T;
/* -------------------------------------------------------------------------- */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST CIP_CERT_MNG_INST_ATTR_STORED_Ttag
{
  CIP_SHORT_STRING_T tName;                          /* Attribute 1 */
  CIP_CERT_MNG_CERTIFICATE_T  tDeviceCertificate;    /* Attribute 3 - Device certificate*/
  CIP_CERT_MNG_CERTIFICATE_T  tCaCertificate;        /* Attribute 4 - CA certificate*/

} CIP_CERT_MNG_INST_ATTR_STORED_T;
uint32_t CipCertMng_ObjectCreate( struct EIP_RESOURCES_Ttag* hEip, void* pvParam );

#endif
