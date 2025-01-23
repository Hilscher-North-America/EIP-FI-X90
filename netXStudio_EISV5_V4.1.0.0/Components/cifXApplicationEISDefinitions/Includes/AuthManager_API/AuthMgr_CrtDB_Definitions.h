/*! \file AuthMgr_CrtDB_Definitions.h */

#ifndef AUTH_CRTDB_DEFINITIONS_H
#define AUTH_CRTDB_DEFINITIONS_H

#include <stdint.h>
#include "Hil_Compiler.h"

/*! Maximum Subject Distinguished Name Length
 *
 * Maximum string length for Subject Distinguished Name (including the termination character) */
#define AUTH_CRTDB_MAX_SDN_LEN             512

/*! Maximum Filename Length
 *
 * Maximum resource file-name(full path) length including the terminating null. */
#define AUTH_CRTDB_MAX_FILENAME_LEN        128

/*! Maximum Key Length
 *
 * Buffer length used for DownloadAndInstallKey service. This limitation is inherited from the largest key algorithm
 * supported in the current version (RSA-4096). */
#define AUTH_CRTDB_KEY_DOWNLOAD_BUF_LEN    4096

#define AUTH_CRTDB_UTCTIME_LEN             sizeof("YYYYMMDDhhmmss")

#define AUTH_CRTDB_RSC_FLAGS_DEFAULT_RSC      0x00000000  /*!< Default resource for a security component. */
#define AUTH_CRTDB_RSC_FLAGS_OPERATIONAL_RSC  0x00000001  /*!< Operational(non-default) resource for a security component. */

#define AUTH_CRTDB_OPTION_FLAGS_DER_ENCODING  0x00000000  /*!< Resource is DER format encoded */
#define AUTH_CRTDB_OPTION_FLAGS_PEM_ENCODING  0x00000001  /*!< Resource is PEM format encoded */

/*! Supported public key IDs. */
typedef enum AUTH_CRTDB_PK_ID_Etag
{
  AUTH_CRTDB_PK_ID_RSA_2048      = 0x10, /*!< RSA key with 2048 bit length. */
  AUTH_CRTDB_PK_ID_RSA_3072      = 0x11, /*!< RSA key with 3072 bit length. */
  AUTH_CRTDB_PK_ID_RSA_4096      = 0x12, /*!< RSA key with 4096 bit length. */
  AUTH_CRTDB_PK_ID_EC_SECP256R1  = 0x50, /*!< EC key based on elliptic curve secp256r1. */
  AUTH_CRTDB_PK_ID_EC_SECP384R1  = 0x51, /*!< EC key based on elliptic curve secp384r1. */
} AUTH_CRTDB_PK_ID_E;

/*! Security component IDs.
 *
 * Each security component gets a range of 16 IDs assigned. This allows having multiple security resources
 * based on different keys for the same component (e.g. different security resources for cyclic and acylic communication).
 * Refer to the security component documentation to check which IDs are used and their purposes
 */
typedef enum AUTH_CRTDB_SECURITY_COMPONENT_ID_Etag
{
  AUTH_CRTDB_SEC_COMPONENT_ID_CIP_SECURITY = 0x10, /*!< CIP Security component uses the IDs 0x00000010-0x0000001F. */
  AUTH_CRTDB_SEC_COMPONENT_ID_HTTPS        = 0x20, /*!< HTTPS component uses the IDs 0x00000020-0x0000002F. */
  AUTH_CRTDB_SEC_COMPONENT_ID_OPCUA        = 0x30, /*!< OPCUA component uses the IDs 0x00000030-0x0000003F. */
  AUTH_CRTDB_SEC_COMPONENT_ID_OMB          = 0x40, /*!< OpenModbus Security component uses the IDs 0x00000040-0x0000004F. */
  AUTH_CRTDB_SEC_COMPONENT_ID_MQTT         = 0x50, /*!< MQTT component uses the IDs 0x00000050-0x0000005F. */
  AUTH_CRTDB_SEC_COMPONENT_ID_NETCONF      = 0x60, /*!< NETCONF component uses the IDs 0x00000060-0x0000006F. */
  AUTH_CRTDB_SEC_COMPONENT_ID_PN_SECURITY  = 0x70, /*!< PROFINET Security component uses the IDs 0x00000070-0x0000007F */
} AUTH_CRTDB_SECURITY_COMPONENT_ID_E;


/*! Security firmware Resource types. */
typedef enum AUTH_CRTDB_SECURITY_RESOURCE_TYPE_Etag
{
    AUTH_CRTDB_RSC_TYPE_NONE = 0,            /*!< Undefined security resource. */
    AUTH_CRTDB_RSC_TYPE_KEY,                 /*!< Private Key that belongs to the security component. */
    AUTH_CRTDB_RSC_TYPE_EE_CERTIFICATE,      /*!< End Entity Certificate. either does not contain a "Basic Constraints extension",
                                              * or contains such an extension with the CA flag set to False. */
    AUTH_CRTDB_RSC_TYPE_CA_CERTIFICATE,      /*!< A Certificate[chain] that belongs to a certificate Authority. This is a part of
                                              * certificate path that proves an end entity certificate has been approved by a
                                              * trusted authority. */
} AUTH_CRTDB_SECURITY_RESOURCE_TYPE_E;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST AUTH_CRTDB_COMPONENT_STAT_Ttag
{
    uint32_t fHasPrivateKey               : 1;  /*!< Flag indicating the existence of a Private Key belonging to the specified security component. */
    uint32_t fHasDefaultEECertificate     : 1;  /*!< Flag indicating the existence of a default EE certificate belonging to the specified security component. */
    uint32_t fHasOperationalEECertificate : 1;  /*!< Flag indicating the existence of an operational certificate belonging to the specified security component. */
    uint32_t fReservedBitFields           : 29; /*!< Reserved bits. */

    uint8_t ulNumOfDefaultCACertificates;       /*!< Number of existing default CA certificates belonging to the specified security component. */
    uint8_t ulNumOfOperationalCACertificates;   /*!< Number of existing operational CA certificates belonging to the specified security component. */
} AUTH_CRTDB_COMPONENT_STAT_T;


#endif /* AUTH_CRTDB_DEFINITIONS_H */
