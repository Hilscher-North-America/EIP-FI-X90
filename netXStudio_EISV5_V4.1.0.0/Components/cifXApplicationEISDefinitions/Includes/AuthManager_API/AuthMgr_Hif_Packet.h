/*!************************************************************************************
Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
***************************************************************************************
  $Id:  $:

  \file AuthMgr_Hif_Packet.h
  Authentication Manager Public Packet API.

Changes:
  Date          Description
  -----------------------------------------------------------------------------------
  2019-11-26    created
**************************************************************************************/

#ifndef AUTHMGR_HIF_PACKET_H
#define AUTHMGR_HIF_PACKET_H

#include <stdint.h>
#include "Hil_Compiler.h"
#include "Hil_Packet.h"
#include "AuthMgr_UsrDB_Definitions.h"
#include "AuthMgr_CrtDB_Definitions.h"

#ifdef __HIL_PRAGMA_PACK_ENABLE
  #pragma __HIL_PRAGMA_PACK_1(__AUTHMGR_HIF_PACKET)
#endif

/* UsrDb services (user authentication) */
#define AUTHMGR_USRDB_AUTHENTICATE_USER_REQ             0x0000B000 /*!< Authenticate User Command Request */
#define AUTHMGR_USRDB_AUTHENTICATE_USER_CNF             0x0000B001 /*!< Authenticate User Command Request */

#define AUTHMGR_USRDB_ADD_USER_REQ                      0x0000B002 /*!< Add User Command Request */
#define AUTHMGR_USRDB_ADD_USER_CNF                      0x0000B003 /*!< Add User Command Request */

#define AUTHMGR_USRDB_DELETE_USER_REQ                   0x0000B004 /*!< Delete User Command Request */
#define AUTHMGR_USRDB_DELETE_USER_CNF                   0x0000B005 /*!< Delete User Command Request */

#define AUTHMGR_USRDB_CHANGE_PASSWORD_REQ               0x0000B006 /*!< Change Password Command Request */
#define AUTHMGR_USRDB_CHANGE_PASSWORD_CNF               0x0000B007 /*!< Change Password Command Request */

#define AUTHMGR_USRDB_RESET_TO_FACTORY_DEFAULTS_REQ     0x0000B008 /*!< Reset to Factory Defaults Command Request */
#define AUTHMGR_USRDB_RESET_TO_FACTORY_DEFAULTS_CNF     0x0000B009 /*!< Reset to Factory Defaults Command Request */

/* CrtDb services (certificate management) */
/* NOTE: The CrtDB sub-component is not released yet and its API may change in future versions.
 *       Please do not these services in released products */
#define AUTHMGR_CRTDB_GENERATE_AND_INSTALL_KEY_REQ      0x0000B080 /*!< Generate and Install Key Command Request */
#define AUTHMGR_CRTDB_GENERATE_AND_INSTALL_KEY_CNF      0x0000B081 /*!< Generate and Install Key Command Confirmation */

#define AUTHMGR_CRTDB_GENERATE_CSR_REQ                  0x0000B082 /*!< Generate CSR Command Request */
#define AUTHMGR_CRTDB_GENERATE_CSR_CNF                  0x0000B083 /*!< Generate CSR Command Confirmation */

#define AUTHMGR_CRTDB_SIGN_REQ                          0x0000B084 /*!< Sign Command Request */
#define AUTHMGR_CRTDB_SIGN_CNF                          0x0000B085 /*!< Sign Command Confirmation */

#define AUTHMGR_CRTDB_INSTALL_CERT_REQ                  0x0000B086 /*!< Install Certificate Command Request */
#define AUTHMGR_CRTDB_INSTALL_CERT_CNF                  0x0000B087 /*!< Install Certificate Command Confirmation */

#define AUTHMGR_CRTDB_UNINSTALL_ALL_CERTS_REQ           0x0000B088 /*!< Uninstall Certificates Command Request */
#define AUTHMGR_CRTDB_UNINSTALL_ALL_CERTS_CNF           0x0000B089 /*!< Uninstall Certificates Command Confirmation */

#define AUTHMGR_CRTDB_FACTORY_RESET_REQ                 0x0000B08A /*!< Factory Reset Command Request */
#define AUTHMGR_CRTDB_FACTORY_RESET_CNF                 0x0000B08B /*!< Factory Reset Command Confirmation */

#define AUTHMGR_CRTDB_DOWNLOAD_AND_INSTALL_KEY_REQ      0x0000B090 /*!< Download and Install Key Command Request */
#define AUTHMGR_CRTDB_DOWNLOAD_AND_INSTALL_KEY_CNF      0x0000B091 /*!< Download and Install Key Command Confirmation */

#define AUTHMGR_CRTDB_GET_COMPONENT_STAT_REQ            0x0000B092 /*!< Get Component Stat. Command Request */
#define AUTHMGR_CRTDB_GET_COMPONENT_STAT_CNF            0x0000B093 /*!< Get Component Stat. Command Confirmation */
/******************************************************************************
 *
 * USER DATABASE API
 *
 ******************************************************************************/

/*! Maximum User Name Length. */
#define AUTHMGR_USRDB_USER_NAME_LEN_MAX                28
/*! Maximum User Password Length. */
#define AUTHMGR_USRDB_PASSWORD_LEN_MAX                 28

/*! User structure. */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST AUTHMGR_USRDB_USER_Ttag
{
  char szUserName[AUTHMGR_USRDB_USER_NAME_LEN_MAX];   /*!< User name (non-empty, '\0' terminated string) */
  char szPassword[AUTHMGR_USRDB_PASSWORD_LEN_MAX];    /*!< User password (non-empty, '\0' terminated string) */
} AUTHMGR_USRDB_USER_T;

/******************************************************************************/
/***** request packet *****/

/*! Authenticate User Request data structure. */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST AUTHMGR_USRDB_AUTHENTICATE_USER_REQ_DATA_Ttag
{
  AUTHMGR_USRDB_USER_T tUser;   /*!< User to authenticate (see <<AUTHMGR_USRDB_USER_T, AUTHMGR_USRDB_USER_T>>) */
} AUTHMGR_USRDB_AUTHENTICATE_USER_REQ_DATA_T;

/*! Authenticate User Request Packet Description
 *
 * Authenticate user with user name and password.
 *
 * \param   ulCmd      <<AUTHMGR_USRDB_AUTHENTICATE_USER_REQ, AUTHMGR_USRDB_AUTHENTICATE_USER_REQ>>
 * \param   ulLen      56
 */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST AUTHMGR_USRDB_AUTHENTICATE_USER_REQ_Ttag
{
  HIL_PACKET_HEADER_T                           tHead;  /*!< Packet header. */
  AUTHMGR_USRDB_AUTHENTICATE_USER_REQ_DATA_T    tData;  /*!< Packet data. */
} AUTHMGR_USRDB_AUTHENTICATE_USER_REQ_T;

/*! Packet data size. */
#define AUTHMGR_USRDB_AUTHENTICATE_USER_REQ_SIZE  (sizeof(AUTHMGR_USRDB_AUTHENTICATE_USER_REQ_DATA_T))

/***** request packet *****/

/*! Authenticate user confirmation data structure. */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST AUTHMGR_USRDB_AUTHENTICATE_USER_CNF_DATA_Ttag
{
  AUTH_USRDB_USER_GROUP_BF_T ulGroupMember; /*!< Groups the user belongs to (see <<AUTH_USRDB_USER_GROUP_BF_T, AUTH_USRDB_USER_GROUP_BF_T>>) */
} AUTHMGR_USRDB_AUTHENTICATE_USER_CNF_DATA_T;

/*! Authenticate User Confirmation Packet Description
 *
 * \param   ulCmd      <<AUTHMGR_USRDB_AUTHENTICATE_USER_CNF, AUTHMGR_USRDB_AUTHENTICATE_USER_CNF>>
 * \param   ulLen      4
 */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST AUTHMGR_USRDB_AUTHENTICATE_USER_CNF_Ttag
{
  HIL_PACKET_HEADER_T                           tHead;  /*!< Packet header. */
  AUTHMGR_USRDB_AUTHENTICATE_USER_CNF_DATA_T    tData;  /*!< Packet data. */
} AUTHMGR_USRDB_AUTHENTICATE_USER_CNF_T;

/*! Packet data size. */
#define AUTHMGR_USRDB_AUTHENTICATE_USER_CNF_SIZE  (sizeof(AUTHMGR_USRDB_AUTHENTICATE_USER_CNF_DATA_T))

/******************************************************************************/
/***** request packet *****/

/*! Add user request data structure. */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST AUTHMGR_USRDB_ADD_USER_REQ_DATA_Ttag
{
  AUTHMGR_USRDB_USER_T tUserPrivileged;       /*!< User that is privileged to create the following user (see <<AUTHMGR_USRDB_USER_T, AUTHMGR_USRDB_USER_T>>) */
  AUTHMGR_USRDB_USER_T tUserAdd;              /*!< New user to be created (see <<AUTHMGR_USRDB_USER_T, AUTHMGR_USRDB_USER_T>>) */
  AUTH_USRDB_USER_GROUP_BF_T ulGroupMember;   /*!< Groups the user belongs to (see <<AUTH_USRDB_USER_GROUP_BF_T, AUTH_USRDB_USER_GROUP_BF_T>>) */
} AUTHMGR_USRDB_ADD_USER_REQ_DATA_T;

/*! Add User Request Packet Description
 *
 * Add new user to the User Database
 *
 * \note This is a privileged service and can only be triggered by an user from
 *       the groups "Admin" or "Account Manager".
 *
 * \note It is disallowed to add an user which belongs to a higher group than
 *       the privileged user
 *
 * \param   ulCmd      <<AUTHMGR_USRDB_ADD_USER_REQ, AUTHMGR_USRDB_ADD_USER_REQ>>
 * \param   ulLen      116
 */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST AUTHMGR_USRDB_ADD_USER_REQ_Ttag
{
  HIL_PACKET_HEADER_T                 tHead;  /*!< Packet header. */
  AUTHMGR_USRDB_ADD_USER_REQ_DATA_T   tData;  /*!< Packet data. */
} AUTHMGR_USRDB_ADD_USER_REQ_T;

/*! Packet data size. */
#define AUTHMGR_USRDB_ADD_USER_REQ_SIZE  (sizeof(AUTHMGR_USRDB_ADD_USER_REQ_DATA_T))

/***** confirmation packet *****/

/*! Add User Confirmation Packet Description
 *
 * \param   ulCmd      <<AUTHMGR_USRDB_ADD_USER_CNF, AUTHMGR_USRDB_ADD_USER_CNF>>
 * \param   ulLen      0
 */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST AUTHMGR_USRDB_ADD_USER_CNF_Ttag
{
  HIL_PACKET_HEADER_T                        tHead;  /*!< Packet header */
} AUTHMGR_USRDB_ADD_USER_CNF_T;

/*! Packet data size. */
#define AUTHMGR_USRDB_ADD_USER_CNF_SIZE  (0)

/******************************************************************************/
/***** request packet *****/

/*! Delete user request data structure. */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST AUTHMGR_USRDB_DELETE_USER_REQ_DATA_Ttag
{
  AUTHMGR_USRDB_USER_T tUserPrivileged;             /*!< User that is privileged to delete the following user (see <<AUTHMGR_USRDB_USER_T, AUTHMGR_USRDB_USER_T>>) */
  char szUserName[AUTHMGR_USRDB_USER_NAME_LEN_MAX]; /*!< Name of the user to be deleted (non-empty, '\0' terminated string) */
} AUTHMGR_USRDB_DELETE_USER_REQ_DATA_T;

/*! Delete User Request Packet Description
 *
 * Delete user from the User Database.
 *
 * \note This is a privileged service and can only be triggered by an user from
 *       the groups "Admin" or "Account Manager".
 *
 * \note It is disallowed to delete an user which belongs to a higher group than
 *       the privileged user
 *
 * \param   ulCmd      <<AUTHMGR_USRDB_DELETE_USER_REQ, AUTHMGR_USRDB_DELETE_USER_REQ>>
 * \param   ulLen      84
 */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST AUTHMGR_USRDB_DELETE_USER_REQ_Ttag
{
  HIL_PACKET_HEADER_T                   tHead;  /*!< Packet header. */
  AUTHMGR_USRDB_DELETE_USER_REQ_DATA_T  tData;  /*!< Packet data. */
} AUTHMGR_USRDB_DELETE_USER_REQ_T;

/***** confirmation packet *****/

/*! Packet data size. */
#define AUTHMGR_USRDB_DELETE_USER_REQ_SIZE  (sizeof(AUTHMGR_USRDB_DELETE_USER_REQ_DATA_T))

/***** confirmation packet *****/

/*! Delete User Confirmation Packet Description
 *
 * \param   ulCmd      <<AUTHMGR_USRDB_DELETE_USER_CNF, AUTHMGR_USRDB_DELETE_USER_CNF>>
 * \param   ulLen      0
 */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST AUTHMGR_USRDB_DELETE_USER_CNF_Ttag
{
  HIL_PACKET_HEADER_T                        tHead;  /*!< Packet header */
} AUTHMGR_USRDB_DELETE_USER_CNF_T;

/*! Packet data size. */
#define AUTHMGR_USRDB_DELETE_USER_CNF_SIZE  (0)

/******************************************************************************/
/***** request packet *****/

/*! Change password request data structure. */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST AUTHMGR_USRDB_CHANGE_PASSWORD_REQ_DATA_Ttag
{
  AUTHMGR_USRDB_USER_T tUser;                         /*!< Existing user for whom the password shall be changed (see <<AUTHMGR_USRDB_USER_T, AUTHMGR_USRDB_USER_T>>) */
  char szPasswordNew[AUTHMGR_USRDB_PASSWORD_LEN_MAX]; /*!< New user password (non-empty, '\0' terminated string) */
} AUTHMGR_USRDB_CHANGE_PASSWORD_REQ_DATA_T;

/*! Change Password Request Packet Description
 *
 * Change the user password.
 *
 * \param   ulCmd      <<AUTHMGR_USRDB_CHANGE_PASSWORD_REQ, AUTHMGR_USRDB_CHANGE_PASSWORD_REQ>>
 * \param   ulLen      84
 */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST AUTHMGR_USRDB_CHANGE_PASSWORD_REQ_Ttag
{
  HIL_PACKET_HEADER_T                         tHead;  /*!< Packet header. */
  AUTHMGR_USRDB_CHANGE_PASSWORD_REQ_DATA_T    tData;  /*!< Packet data. */
} AUTHMGR_USRDB_CHANGE_PASSWORD_REQ_T;

/*! Packet data size. */
#define AUTHMGR_USRDB_CHANGE_PASSWORD_REQ_SIZE  (sizeof(AUTHMGR_USRDB_CHANGE_PASSWORD_REQ_DATA_T))

/***** confirmation packet *****/

/*! Change Password Confirmation Packet Description
 *
 * \param   ulCmd      <<AUTHMGR_USRDB_CHANGE_PASSWORD_CNF, AUTHMGR_USRDB_CHANGE_PASSWORD_CNF>>
 * \param   ulLen      0
 */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST AUTHMGR_USRDB_CHANGE_PASSWORD_CNF_Ttag
{
  HIL_PACKET_HEADER_T                        tHead;  /*!< Packet header */
} AUTHMGR_USRDB_CHANGE_PASSWORD_CNF_T;

/*! Packet data size. */
#define AUTHMGR_USRDB_CHANGE_PASSWORD_CNF_SIZE  (0)

/******************************************************************************/
/***** request packet *****/

/*! Reset to factory defaults request data structure. */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST AUTHMGR_USRDB_RESET_TO_FACTORY_DEFAULTS_REQ_DATA_Ttag
{
  AUTHMGR_USRDB_USER_T tUserPrivileged;   /*!< User that is privileged to trigger the reset to factory defaults (see <<AUTHMGR_USRDB_USER_T, AUTHMGR_USRDB_USER_T>>) */
} AUTHMGR_USRDB_RESET_TO_FACTORY_DEFAULTS_REQ_DATA_T;

/*! Reset to Factory Defaults Request Packet Description
 *
 * Reset the User Database to its Factory Defaults.
 * The complete user database is deleted and the root user is re-created from the configuration
 * passed during firmware initialization. The component remains initialized.
 *
 * \note This is a privileged service and can only be triggered by an user from
 *       the groups "Admin" or "Account Manager".
 *
 * \note After the reset, the pre-compiled LFW Default Root User is re-created.
 *       If the application wants to replace it by its own Root User, this has to
 *       be done separately
 *
 * \param   ulCmd      <<AUTHMGR_USRDB_RESET_TO_FACTORY_DEFAULTS_REQ, AUTHMGR_USRDB_RESET_TO_FACTORY_DEFAULTS_REQ>>
 * \param   ulLen      56
 */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST AUTHMGR_USRDB_RESET_TO_FACTORY_DEFAULTS_REQ_Ttag
{
  HIL_PACKET_HEADER_T                                   tHead;  /*!< Packet header. */
  AUTHMGR_USRDB_RESET_TO_FACTORY_DEFAULTS_REQ_DATA_T    tData;  /*!< Packet data. */
} AUTHMGR_USRDB_RESET_TO_FACTORY_DEFAULTS_REQ_T;

/*! Packet data size. */
#define AUTHMGR_USRDB_RESET_TO_FACTORY_DEFAULTS_REQ_SIZE  (sizeof(AUTHMGR_USRDB_RESET_TO_FACTORY_DEFAULTS_REQ_DATA_T))

/***** confirmation packet *****/

/*! Reset to Factory Defaults Confirmation Packet Description
 *
 * \param   ulCmd      <<AUTHMGR_USRDB_RESET_TO_FACTORY_DEFAULTS_CNF, AUTHMGR_USRDB_RESET_TO_FACTORY_DEFAULTS_CNF>>
 * \param   ulLen      0
 */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST AUTHMGR_USRDB_RESET_TO_FACTORY_DEFAULTS_CNF_Ttag
{
  HIL_PACKET_HEADER_T                        tHead;  /*!< Packet header */
} AUTHMGR_USRDB_RESET_TO_FACTORY_DEFAULTS_CNF_T;

/*! Packet data size. */
#define AUTHMGR_USRDB_RESET_TO_FACTORY_DEFAULTS_CNF_SIZE  (0)


/******************************************************************************
 *
 * CERTIFICATE DATABASE API
 *
 ******************************************************************************/

/******************************************************************************/
/***** request packet *****/

/*! Generate and Install Key Request Packet Data */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST AUTHMGR_CRTDB_GENERATE_AND_INSTALL_KEY_REQ_DATA_Ttag
{
  uint32_t     ulComponentId;  /*!< Security component identifier of type <<AUTH_CRTDB_SECURITY_COMPONENT_ID_E, AUTH_CRTDB_SECURITY_COMPONENT_ID_E>> */
  uint32_t     ulKeyId;        /*!< Key type identifier of type <<AUTH_CRTDB_PK_ID_E, AUTH_CRTDB_PK_ID_E>> */
} AUTHMGR_CRTDB_GENERATE_AND_INSTALL_KEY_REQ_DATA_T;

/*! Generate and Install Key Request Packet Description
 *
 * Generate and install an asymmetric key pair for a specific security component.
 * This service replaces an existing key of that security component, so that
 * previous security resources belonging to that security component
 * (e.g. certificates, CSRs, ...) will no longer be valid.
 *
 * \note The service does not verify if the key (type, length) complies to the
 *       corresponding security component requirements.
 *       It is the responsibility of the application to install keys supported
 *       by the security component.
 *
 * \param   ulCmd      <<AUTHMGR_CRTDB_GENERATE_AND_INSTALL_KEY_REQ, AUTHMGR_CRTDB_GENERATE_AND_INSTALL_KEY_REQ>>
 * \param   ulLen      8
 */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST AUTHMGR_CRTDB_GENERATE_AND_INSTALL_KEY_REQ_Ttag
{
  HIL_PACKET_HEADER_T                                   tHead;  /*!< Packet header */
  AUTHMGR_CRTDB_GENERATE_AND_INSTALL_KEY_REQ_DATA_T     tData;  /*!< Packet data */
} AUTHMGR_CRTDB_GENERATE_AND_INSTALL_KEY_REQ_T;

/*! Packet data size */
#define AUTHMGR_CRTDB_GENERATE_AND_INSTALL_KEY_REQ_SIZE  (sizeof(AUTHMGR_CRTDB_GENERATE_AND_INSTALL_KEY_REQ_DATA_T))

/***** confirmation packet *****/

/*! Generate and Install Key Confirmation Packet Description
 *
 * \param   ulCmd      <<AUTHMGR_CRTDB_GENERATE_AND_INSTALL_KEY_CNF, AUTHMGR_CRTDB_GENERATE_AND_INSTALL_KEY_CNF>>
 * \param   ulLen      0
 */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST AUTHMGR_CRTDB_GENERATE_AND_INSTALL_KEY_CNF_Ttag
{
  HIL_PACKET_HEADER_T                        tHead;  /*!< Packet header */
} AUTHMGR_CRTDB_GENERATE_AND_INSTALL_KEY_CNF_T;

/*! Packet data size */
#define AUTHMGR_CRTDB_GENERATE_AND_INSTALL_KEY_CNF_SIZE  (0)

/******************************************************************************/
/***** request packet *****/

/*! Generate CSR Request Packet Data */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST AUTHMGR_CRTDB_GENERATE_CSR_REQ_DATA_Ttag
{
    uint32_t     ulComponentId;                             /*!< Security component identifier (see <<AUTH_CRTDB_SECURITY_COMPONENT_ID_E, AUTH_CRTDB_SECURITY_COMPONENT_ID_E>>) */
    uint32_t     ulOptionFlags;                             /*!< Option flags to customize the process (see <<AUTH_CRTDB_OPTION_FLAGS_, AUTH_CRTDB_OPTION_FLAGS_*>>) */
    char         szFileName[AUTH_CRTDB_MAX_FILENAME_LEN];   /*!< Null-terminated CSR File Name (including file path) (e.g. "file://SYSVOLUME/PORT_0/CSR_0020.pem") (maximum 128 characters, including '\0') */
    char         szSubjectName[AUTH_CRTDB_MAX_SDN_LEN];     /*!< Null-terminated Subject Distinguished Name (maximum 512 characters, including '\0') */
    uint8_t      abExtentions[__HIL_VARIABLE_LENGTH_ARRAY]; /*!< Buffer holding the X509v3 extensions to be set in the CSR.
                                                                 The buffer length is implicitly provided within the packet data length (tHead.ulLen) */
} AUTHMGR_CRTDB_GENERATE_CSR_REQ_DATA_T;

/*! Generate CSR Request Packet Description
 *
 * Generate a PEM/DER encoded Certificate Signing Request for a specific security component (ulComponentId).
 * The CSR is stored as a file on the file system.
 *
 * The Subject Distinguished Name (szSubjectName), includes a sequence of attribute type/value pairs separated by a comma.
 * Syntax: type=value[,type=value]
 *
 * 'type' is one of the following short names:
 *
 * Short form           |  Description              |  ASN.1 OID
 * -------------------- | ------------------------- | ------------
 * CN                   |  commonName               |  2.5.4.3
 * SN                   |  surName                  |  2.5.4.4
 * serialNumber         |  serialNumber             |  2.5.4.5
 * C                    |  countryName              |  2.5.4.6 (2 letter ISO 3166 Country Code)
 * L                    |  localityName             |  2.5.4.7
 * ST                   |  state                    |  2.5.4.8
 * O                    |  organizationName         |  2.5.4.10
 * OU                   |  organizationalUnit       |  2.5.4.11
 * title                |  title                    |  2.5.4.12
 * postalAddress        |  postalAddress            |  2.5.4.16
 * postalCode           |  postalCode               |  2.5.4.17
 * GN                   |  givenName                |  2.5.4.42
 * initials             |  initials                 |  2.5.4.43
 * generationQualifier  |  generationQualifier      |  2.5.4.44
 * uniqueIdentifier     |  uniqueIdentifier         |  2.5.4.45
 * dnQualifier          |  dnQualifier              |  2.5.4.46
 * pseudonym            |  pseudonym                |  2.5.4.47
 * emailAddress         |  emailAddress             |  1.2.840.113549.1.9.1 (Deprecated, use an altName extension instead)
 * DC                   |  domainComponent          |  0.9.2342.19200300.100.1.25
 *
 * **Example:** "CN=My Certificate Authority,C=DE,O=My Company"
 *
 * The extensions buffer holds a sequence of all x509v3 extensions to be included in the CSR. Each extension
 * must be described with the following structure:
 *
 * Element         |  Data type     |  Description
 * --------------- | -------------- | -----------------------------------------
 * Critical        | UINT8          | Indicates if the "critical" flag of the extension has to be set. (0 = FALSE / others = TRUE)
 * OID Tag Length  | UINT32         | The length of the OID tag
 * OID Tag         | Array of UINT8 | The OID Tag of the extension
 * OID Data Length | UINT32         | The length of the OID data
 * OID Data        | Array of UINT8 | The OID data of the extension
 *
 * \note: The value TRUE for the critical flag can only be used for the standard extensions defined in RFC5280
 *
 * \note: The OID Data may contain one single value (e.g. an octet string) or multiple values (e.g. a constructed sequence).
 *        The value data type is coded within the OID Data.
 *        The values follow the ASN.1 syntax "ASN.1 type | length | value".
 *
 * \note: Some helper functions are provided in the file "AuthMgr_CrtDB_X509v3Ext_helpers.h" to simplify the generation of the
 *        extensions buffer
 *
 * \param   ulCmd      <<AUTHMGR_CRTDB_GENERATE_CSR_REQ, AUTHMGR_CRTDB_GENERATE_CSR_REQ>>
 * \param   ulLen      648 + extensions length
 */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST AUTHMGR_CRTDB_GENERATE_CSR_REQ_Ttag
{
  HIL_PACKET_HEADER_T                     tHead;  /*!< Packet header */
  AUTHMGR_CRTDB_GENERATE_CSR_REQ_DATA_T   tData;  /*!< Packet data */
} AUTHMGR_CRTDB_GENERATE_CSR_REQ_T;

/*! Packet data size */
#define AUTHMGR_CRTDB_GENERATE_CSR_REQ_MIN_SIZE  (sizeof(AUTHMGR_CRTDB_GENERATE_CSR_REQ_DATA_T))

/***** confirmation packet *****/

/*! Generate CSR Confirmation Packet Description
 *
 * \param   ulCmd      <<AUTHMGR_CRTDB_GENERATE_CSR_CNF, AUTHMGR_CRTDB_GENERATE_CSR_CNF>>
 * \param   ulLen      0
 */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST AUTHMGR_CRTDB_GENERATE_CSR_CNF_Ttag
{
  HIL_PACKET_HEADER_T                   tHead;  /*!< Packet header */
} AUTHMGR_CRTDB_GENERATE_CSR_CNF_T;

/*! Packet data size */
#define AUTHMGR_CRTDB_GENERATE_CSR_CNF_SIZE  (0)

/******************************************************************************/
/***** request packet *****/

/*! Sign Request Packet Data */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST AUTHMGR_CRTDB_SIGN_REQ_DATA_Ttag
{
    uint32_t     ulComponentId;                               /*!< Security component identifier which is going to sign the certificate (see <<AUTH_CRTDB_SECURITY_COMPONENT_ID_E, AUTH_CRTDB_SECURITY_COMPONENT_ID_E>>)*/
    uint32_t     ulOptionFlags;                               /*!< Option flags to customize the process (see <<AUTH_CRTDB_OPTION_FLAGS_, AUTH_CRTDB_OPTION_FLAGS_*>>) */
    char         szNotValidBefore[AUTH_CRTDB_UTCTIME_LEN];    /*!< Null-terminated validity date notBefore UTC time in "YYYYMMDDhhmmss" format */
    char         szNotValidAfter[AUTH_CRTDB_UTCTIME_LEN];     /*!< Null-terminated validity date notAfter UTC time in "YYYYMMDDhhmmss" format */
    char         szCsrFileName[AUTH_CRTDB_MAX_FILENAME_LEN];  /*!< Null-terminated CSR File Name (including file path) (maximum 128 characters, including '\0') */
    char         szCrtFileName[AUTH_CRTDB_MAX_FILENAME_LEN];  /*!< Null-terminated Certificate File Name (including file path) (maximum 128 characters, including '\0') */
} AUTHMGR_CRTDB_SIGN_REQ_DATA_T;

/*! Sign Request Packet Description
 *
 * Sign a Certificate Signing Request and store the resulted certificated on
 * the file system.
 *
 * \note The CSR must be present in the file system before using this service.
 * \note to indicate that a certificate has no well-defined expiration date, the szNotValidAfter
 *       SHOULD be assigned the GeneralizedTime value of 99991231235959 (RFC5280).
 *
 * \param   ulCmd      <<AUTHMGR_CRTDB_SIGN_REQ, AUTHMGR_CRTDB_SIGN_REQ>>
 * \param   ulLen      294
 */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST AUTHMGR_CRTDB_SIGN_REQ_Ttag
{
  HIL_PACKET_HEADER_T                 tHead;  /*!< Packet header */
  AUTHMGR_CRTDB_SIGN_REQ_DATA_T       tData;  /*!< Packet data */
} AUTHMGR_CRTDB_SIGN_REQ_T;

/*! Packet data size */
#define AUTHMGR_CRTDB_SIGN_REQ_SIZE  (sizeof(AUTHMGR_CRTDB_SIGN_REQ_DATA_T))

/***** confirmation packet *****/

/*! Sign Confirmation Packet Description
 *
 * \param   ulCmd      <<AUTHMGR_CRTDB_SIGN_CNF, AUTHMGR_CRTDB_SIGN_CNF>>
 * \param   ulLen      0
 */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST AUTHMGR_CRTDB_SIGN_CNF_Ttag
{
  HIL_PACKET_HEADER_T                   tHead;  /*!< Packet header */
} AUTHMGR_CRTDB_SIGN_CNF_T;

/*! Packet data size */
#define AUTHMGR_CRTDB_SIGN_CNF_SIZE  (0)

/******************************************************************************/
/***** request packet *****/

/*! Install Certificate Request Packet Data */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST AUTHMGR_CRTDB_INSTALL_CERT_REQ_DATA_Ttag
{
    uint32_t     ulComponentId;                           /*!< Security component identifier (see <<AUTH_CRTDB_SECURITY_COMPONENT_ID_E, AUTH_CRTDB_SECURITY_COMPONENT_ID_E>>) */
    uint16_t     usCertificateType;                       /*!< Certificate type identifier (see <<AUTH_CRTDB_SECURITY_RESOURCE_TYPE_E, AUTH_CRTDB_SECURITY_RESOURCE_TYPE_E>>) */
    uint16_t     usFlags;                                 /*!< bit field certificate attributes (see <<AUTH_CRTDB_RSC_FLAGS_, AUTH_CRTDB_RSC_FLAGS_*>>) */
    char         szFileName[AUTH_CRTDB_MAX_FILENAME_LEN]; /*!< Null-terminated certificate File Name (including file path) (e.g. "file://SYSVOLUME/PORT_0/dflt_crt.cer") (maximum 128 characters, including '\0') */
} AUTHMGR_CRTDB_INSTALL_CERT_REQ_DATA_T;

/*! Install Certificate Request Packet Description
 *
 * Install a certificate (szFileName) for a specific security component (ulComponentId).
 *
 * \note The certificate needs to be available in the file system before using this service.
 *
 * \param   ulCmd      <<AUTHMGR_CRTDB_INSTALL_CERT_REQ, AUTHMGR_CRTDB_INSTALL_CERT_REQ>>
 * \param   ulLen      136
 */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST AUTHMGR_CRTDB_INSTALL_CERT_REQ_Ttag
{
  HIL_PACKET_HEADER_T                         tHead;  /*!< Packet header */
  AUTHMGR_CRTDB_INSTALL_CERT_REQ_DATA_T       tData;  /*!< Packet data */
} AUTHMGR_CRTDB_INSTALL_CERT_REQ_T;

/*! Packet data size */
#define AUTHMGR_CRTDB_INSTALL_CERT_REQ_SIZE  (sizeof(AUTHMGR_CRTDB_INSTALL_CERT_REQ_DATA_T))

/***** confirmation packet *****/

/*! Install Certificate Confirmation Packet Description
 *
 * \param   ulCmd      <<AUTHMGR_CRTDB_INSTALL_CERT_CNF, AUTHMGR_CRTDB_INSTALL_CERT_CNF>>
 * \param   ulLen      0
 */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST AUTHMGR_CRTDB_INSTALL_CERT_CNF_Ttag
{
  HIL_PACKET_HEADER_T                   tHead;  /*!< Packet header */
} AUTHMGR_CRTDB_INSTALL_CERT_CNF_T;

/*! Packet data size */
#define AUTHMGR_CRTDB_INSTALL_CERT_CNF_SIZE  (0)

/******************************************************************************/
/***** request packet *****/

/*! Uninstall All Certificates Request Packet Data */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST AUTHMGR_CRTDB_UNINSTALL_ALL_CERTS_REQ_DATA_Ttag
{
    uint32_t     ulComponentId;                       /*!< Security component identifier (see <<AUTH_CRTDB_SECURITY_COMPONENT_ID_E, AUTH_CRTDB_SECURITY_COMPONENT_ID_E>>) */
    uint16_t     usFlags;                             /*!< Bit field certificate attributes (see <<AUTH_CRTDB_RSC_FLAGS_, AUTH_CRTDB_RSC_FLAGS_*>>) */
} AUTHMGR_CRTDB_UNINSTALL_ALL_CERTS_REQ_DATA_T;

/*! Uninstall All Certificates Request Packet Description
 *
 * Un-install all certificates related to a specific security component.
 *
 * \note This service does not remove the corresponding certificates from the file system.
 *       Manual effort is required to remove the certificates from the file system.
 *
 * \param   ulCmd      <<AUTHMGR_CRTDB_UNINSTALL_ALL_CERTS_REQ, AUTHMGR_CRTDB_UNINSTALL_ALL_CERTS_REQ>>
 * \param   ulLen      6
 */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST AUTHMGR_CRTDB_UNINSTALL_ALL_CERTS_REQ_Ttag
{
  HIL_PACKET_HEADER_T                           tHead;  /*!< Packet header */
  AUTHMGR_CRTDB_UNINSTALL_ALL_CERTS_REQ_DATA_T  tData;  /*!< Packet data */
} AUTHMGR_CRTDB_UNINSTALL_ALL_CERTS_REQ_T;

/*! Packet data size */
#define AUTHMGR_CRTDB_UNINSTALL_ALL_CERTS_REQ_SIZE  (sizeof(AUTHMGR_CRTDB_UNINSTALL_ALL_CERTS_REQ_DATA_T))

/***** confirmation packet *****/

/*! Uninstall All Certificates Confirmation Packet Description
 *
 * \param   ulCmd      <<AUTHMGR_CRTDB_UNINSTALL_ALL_CERTS_CNF, AUTHMGR_CRTDB_UNINSTALL_ALL_CERTS_CNF>>
 * \param   ulLen      0
 */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST AUTHMGR_CRTDB_UNINSTALL_ALL_CERTS_CNF_Ttag
{
  HIL_PACKET_HEADER_T                         tHead;  /*!< Packet header */
} AUTHMGR_CRTDB_UNINSTALL_ALL_CERTS_CNF_T;

/*! Packet data size */
#define AUTHMGR_CRTDB_UNINSTALL_ALL_CERTS_CNF_SIZE  (0)

/******************************************************************************/
/***** request packet *****/

/*! Factory Reset Request Packet Description
 *
 * Remove all certificate and key files from the system
 * and all the entries from the internal database.
 *
 * \note: After the reset is done, the certDB does not contain any
 * security configuration anymore. This may lead to errors or
 * unexpected behaviors in the security components
 *
 * \note: it is recommended to perform a system reset afterwards
 * to run the "out-of-box" behavior and download the (default)
 * security configurations again.
 *
 * \param   ulCmd      <<AUTHMGR_CRTDB_FACTORY_RESET_REQ, AUTHMGR_CRTDB_FACTORY_RESET_REQ>>
 * \param   ulLen      0
 */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST AUTHMGR_CRTDB_FACTORY_RESET_REQ_Ttag
{
  HIL_PACKET_HEADER_T                           tHead;  /*!< Packet header */
} AUTHMGR_CRTDB_FACTORY_RESET_REQ_T;

/*! Packet data size */
#define AUTHMGR_CRTDB_FACTORY_RESET_REQ_SIZE  (0)

/***** confirmation packet *****/

/*! Factory Reset Confirmation Packet Description
 *
 * \param   ulCmd      <<AUTHMGR_CRTDB_FACTORY_RESET_CNF, AUTHMGR_CRTDB_FACTORY_RESET_CNF>>
 * \param   ulLen      0
 */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST AUTHMGR_CRTDB_FACTORY_RESET_CNF_Ttag
{
  HIL_PACKET_HEADER_T                         tHead;  /*!< Packet header */
} AUTHMGR_CRTDB_FACTORY_RESET_CNF_T;

/*! Packet data size */
#define AUTHMGR_CRTDB_FACTORY_RESET_CNF_SIZE  (0)

/******************************************************************************/
/***** request packet *****/

/*! Download and Install Key Request Packet Data */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST AUTHMGR_CRTDB_DOWNLOAD_AND_INSTALL_KEY_REQ_DATA_Ttag
{
    uint32_t  ulComponentId;                          /*!< Security component identifier (see <<AUTH_CRTDB_SECURITY_COMPONENT_ID_E, AUTH_CRTDB_SECURITY_COMPONENT_ID_E>>) */
    uint8_t   abKey[__HIL_VARIABLE_LENGTH_ARRAY];     /*!< Holds a DER or PEM encoded key (maximum <<AUTH_CRTDB_KEY_DOWNLOAD_BUF_LEN, AUTH_CRTDB_KEY_DOWNLOAD_BUF_LEN>> bytes allowed).
                                                           The key length is implicitly provided within the packet data length (tHead.ulLen) */
} AUTHMGR_CRTDB_DOWNLOAD_AND_INSTALL_KEY_REQ_DATA_T;

/*! Download and Install Key Request Packet Description
 *
 * Download and install a key for a specific security component.
 * This service replaces an existing key of that security component, so that
 * previous security resources belonging to that security component
 * (e.g. certificates, CSRs, ...) will no longer be valid.
 *
 * \note The service does not verify if the key (type, length) complies to the
 *       corresponding security component requirements.
 *       It is the responsibility of the application to install keys supported
 *       by the security component.
 *
 * \param   ulCmd      <<AUTHMGR_CRTDB_DOWNLOAD_AND_INSTALL_KEY_REQ, AUTHMGR_CRTDB_DOWNLOAD_AND_INSTALL_KEY_REQ>>
 * \param   ulLen      4 + key length
 */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST AUTHMGR_CRTDB_DOWNLOAD_AND_INSTALL_KEY_REQ_Ttag
{
  HIL_PACKET_HEADER_T                                 tHead;  /*!< Packet header */
  AUTHMGR_CRTDB_DOWNLOAD_AND_INSTALL_KEY_REQ_DATA_T   tData;  /*!< Packet data */
} AUTHMGR_CRTDB_DOWNLOAD_AND_INSTALL_KEY_REQ_T;

/*! Packet data size */
#define AUTHMGR_CRTDB_DOWNLOAD_AND_INSTALL_KEY_REQ_MIN_SIZE  (sizeof(AUTHMGR_CRTDB_DOWNLOAD_AND_INSTALL_KEY_REQ_DATA_T))
#define AUTHMGR_CRTDB_DOWNLOAD_AND_INSTALL_KEY_REQ_MAX_SIZE  (sizeof(AUTHMGR_CRTDB_DOWNLOAD_AND_INSTALL_KEY_REQ_DATA_T) + AUTH_CRTDB_KEY_DOWNLOAD_BUF_LEN)

/***** confirmation packet *****/

/*! Download and Install Key Confirmation Packet Description
 *
 * \param   ulCmd      <<AUTHMGR_CRTDB_DOWNLOAD_AND_INSTALL_KEY_CNF, AUTHMGR_CRTDB_DOWNLOAD_AND_INSTALL_KEY_CNF>>
 * \param   ulLen      0
 */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST AUTHMGR_CRTDB_DOWNLOAD_AND_INSTALL_KEY_CNF_Ttag
{
  HIL_PACKET_HEADER_T                   tHead;  /*!< Packet header */
} AUTHMGR_CRTDB_DOWNLOAD_AND_INSTALL_KEY_CNF_T;

/*! Packet data size */
#define AUTHMGR_CRTDB_DOWNLOAD_AND_INSTALL_KEY_CNF_SIZE  (0)

/******************************************************************************/
/***** request packet *****/

/*! Get Component Stat Request Packet Data */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST AUTHMGR_CRTDB_GET_COMPONENT_STAT_REQ_DATA_Ttag
{
    uint32_t  ulComponentId; /*!< Security component identifier (see <<AUTH_CRTDB_SECURITY_COMPONENT_ID_E, AUTH_CRTDB_SECURITY_COMPONENT_ID_E>>) */
} AUTHMGR_CRTDB_GET_COMPONENT_STAT_REQ_DATA_T;

/*! Get Component Stat Request Packet Description
 *
 * Retrieve basic information(Existence/Cardinality) about the security configuration artifacts for a specific security component.
 * \note Artifacts are considered "existing", when both of the following criteria are met:
 *      1. There is a corresponding file for it on the file system.
 *      2. The artifact has been installed in the AuthMgr Certificate Database.
 *
 * \note No validation/verification of resources are provided within this service.
 *
 * \param   ulCmd      <<AUTHMGR_CRTDB_GET_COMPONENT_STAT_REQ, AUTHMGR_CRTDB_GET_COMPONENT_STAT_REQ>>
 * \param   ulLen      4
 */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST AUTHMGR_CRTDB_GET_COMPONENT_STAT_REQ_Ttag
{
  HIL_PACKET_HEADER_T                           tHead;  /*!< Packet header */
  AUTHMGR_CRTDB_GET_COMPONENT_STAT_REQ_DATA_T   tData;
} AUTHMGR_CRTDB_GET_COMPONENT_STAT_REQ_T;

/*! Packet data size */
#define AUTHMGR_CRTDB_GET_COMPONENT_STAT_REQ_SIZE  (sizeof(AUTHMGR_CRTDB_GET_COMPONENT_STAT_REQ_DATA_T))

/***** confirmation packet *****/

/*! Get Component Stat confirmation Packet Data */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST AUTHMGR_CRTDB_GET_COMPONENT_STAT_CNF_DATA_Ttag
{
  AUTH_CRTDB_COMPONENT_STAT_T  tComopnentStat;
} AUTHMGR_CRTDB_GET_COMPONENT_STAT_CNF_DATA_T;


/*! Get Component Stat Confirmation Packet Description
 *
 * \param   ulCmd      <<AUTHMGR_CRTDB_GET_COMPONENT_STAT_CNF, AUTHMGR_CRTDB_GET_COMPONENT_STAT_CNF>>
 * \param   ulLen      6
 */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST AUTHMGR_CRTDB_GET_COMPONENT_STAT_CNF_Ttag
{
  HIL_PACKET_HEADER_T                         tHead;  /*!< Packet header */
  AUTHMGR_CRTDB_GET_COMPONENT_STAT_CNF_DATA_T tData;
} AUTHMGR_CRTDB_GET_COMPONENT_STAT_CNF_T;

/*! Packet data size */
#define AUTHMGR_CRTDB_GET_COMPONENT_STAT_CNF_SIZE  (sizeof(AUTHMGR_CRTDB_GET_COMPONENT_STAT_CNF_DATA_T))
/******************************************************************************/

/* Backward compatible definitions, those defines will be removed in
 * future versions of AuthManager */
#ifndef DOXYGEN_SKIP_SECTION
  #define AUTHMGR_HIF_AUTHENTICATE_USER_REQ             AUTHMGR_USRDB_AUTHENTICATE_USER_REQ
  #define AUTHMGR_HIF_AUTHENTICATE_USER_CNF             AUTHMGR_USRDB_AUTHENTICATE_USER_CNF
  #define AUTHMGR_HIF_AUTHENTICATE_USER_REQ_DATA_T      AUTHMGR_USRDB_AUTHENTICATE_USER_REQ_DATA_T
  #define AUTHMGR_HIF_AUTHENTICATE_USER_REQ_T           AUTHMGR_USRDB_AUTHENTICATE_USER_REQ_T
  #define AUTHMGR_HIF_AUTHENTICATE_USER_REQ_SIZE        AUTHMGR_USRDB_AUTHENTICATE_USER_REQ_SIZE
  #define AUTHMGR_HIF_AUTHENTICATE_USER_CNF_DATA_T      AUTHMGR_USRDB_AUTHENTICATE_USER_CNF_DATA_T
  #define AUTHMGR_HIF_AUTHENTICATE_USER_CNF_T           AUTHMGR_USRDB_AUTHENTICATE_USER_CNF_T
  #define AUTHMGR_HIF_AUTHENTICATE_USER_CNF_SIZE        AUTHMGR_USRDB_AUTHENTICATE_USER_CNF_SIZE

  #define AUTHMGR_HIF_ADD_USER_REQ                      AUTHMGR_USRDB_ADD_USER_REQ
  #define AUTHMGR_HIF_ADD_USER_CNF                      AUTHMGR_USRDB_ADD_USER_CNF
  #define AUTHMGR_HIF_ADD_USER_REQ_DATA_T               AUTHMGR_USRDB_ADD_USER_REQ_DATA_T
  #define AUTHMGR_HIF_ADD_USER_REQ_T                    AUTHMGR_USRDB_ADD_USER_REQ_T
  #define AUTHMGR_HIF_ADD_USER_REQ_SIZE                 AUTHMGR_USRDB_ADD_USER_REQ_SIZE
  #define AUTHMGR_HIF_ADD_USER_CNF_T                    AUTHMGR_USRDB_ADD_USER_CNF_T
  #define AUTHMGR_HIF_ADD_USER_CNF_SIZE                 AUTHMGR_USRDB_ADD_USER_CNF_SIZE

  #define AUTHMGR_HIF_DELETE_USER_REQ                   AUTHMGR_USRDB_DELETE_USER_REQ
  #define AUTHMGR_HIF_DELETE_USER_CNF                   AUTHMGR_USRDB_DELETE_USER_CNF
  #define AUTHMGR_HIF_DELETE_USER_REQ_DATA_T            AUTHMGR_USRDB_DELETE_USER_REQ_DATA_T
  #define AUTHMGR_HIF_DELETE_USER_REQ_T                 AUTHMGR_USRDB_DELETE_USER_REQ_T
  #define AUTHMGR_HIF_DELETE_USER_REQ_SIZE              AUTHMGR_USRDB_DELETE_USER_REQ_SIZE
  #define AUTHMGR_HIF_DELETE_USER_CNF_T                 AUTHMGR_USRDB_DELETE_USER_CNF_T
  #define AUTHMGR_HIF_DELETE_USER_CNF_SIZE              AUTHMGR_USRDB_DELETE_USER_CNF_SIZE

  #define AUTHMGR_HIF_CHANGE_PASSWORD_REQ               AUTHMGR_USRDB_CHANGE_PASSWORD_REQ
  #define AUTHMGR_HIF_CHANGE_PASSWORD_CNF               AUTHMGR_USRDB_CHANGE_PASSWORD_CNF
  #define AUTHMGR_HIF_CHANGE_PASSWORD_REQ_DATA_T        AUTHMGR_USRDB_CHANGE_PASSWORD_REQ_DATA_T
  #define AUTHMGR_HIF_CHANGE_PASSWORD_REQ_T             AUTHMGR_USRDB_CHANGE_PASSWORD_REQ_T
  #define AUTHMGR_HIF_CHANGE_PASSWORD_REQ_SIZE          AUTHMGR_USRDB_CHANGE_PASSWORD_REQ_SIZE
  #define AUTHMGR_HIF_CHANGE_PASSWORD_CNF_T             AUTHMGR_USRDB_CHANGE_PASSWORD_CNF_T
  #define AUTHMGR_HIF_CHANGE_PASSWORD_CNF_SIZE          AUTHMGR_USRDB_CHANGE_PASSWORD_CNF_SIZE

  #define AUTHMGR_HIF_USER_NAME_LEN_MAX                 AUTHMGR_USRDB_USER_NAME_LEN_MAX
  #define AUTHMGR_HIF_PASSWORD_LEN_MAX                  AUTHMGR_USRDB_PASSWORD_LEN_MAX
  #define AUTHMGR_HIF_GROUP_BF_T                        AUTHMGR_USRDB_GROUP_BF_T
  #define AUTHMGR_HIF_USER_T                            AUTHMGR_USRDB_USER_T
#endif /* DOXYGEN_SKIP_SECTION */

#ifdef __HIL_PRAGMA_PACK_ENABLE
  #pragma __HIL_PRAGMA_UNPACK_1(__AUTHMGR_HIF_PACKET)
#endif

#endif /* AUTHMGR_HIF_PACKET_H */
