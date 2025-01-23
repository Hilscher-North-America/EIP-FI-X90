/**************************************************************************************
Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
***************************************************************************************
$Id:  $:

Description:
  Auth_Manager_Results.h
**************************************************************************************/

#ifndef __AUTH_MANAGER_RESULTS_H
#define __AUTH_MANAGER_RESULTS_H

#include <stdint.h>

/***********************************************************************************/
/* Authentication Manager - User Database Error Codes */
/***********************************************************************************/

/** No configuration available. */
#define ERR_AUTH_USRDB_NO_CONFIG         0xC0FB0001L

/** Realm string has invalid length. */
#define ERR_AUTH_USRDB_INVALID_REALM_LEN 0xC0FB0002L

/** User name has invalid length. */
#define ERR_AUTH_USRDB_INVALID_USER_NAME_LEN 0xC0FB0003L

/** Libstorage initialization failed. */
#define ERR_AUTH_USRDB_LIBSTORAGE_INIT_FAIL 0xC0FB0004L

/** Libstorage write operation failed. */
#define ERR_AUTH_USRDB_LIBSTORAGE_WRITE_FAIL 0xC0FB0005L

/** Libstorage read operation failed. */
#define ERR_AUTH_USRDB_LIBSTORAGE_READ_FAIL 0xC0FB0006L

/** Authentication Manager is not initialized. */
#define ERR_AUTH_USRDB_NOT_INITIALIZED   0xC0FB0007L

/** Unexpected group member. */
#define ERR_AUTH_USRDB_UNEXPECTED_GROUP  0xC0FB0008L

/** Incorrect password. */
#define ERR_AUTH_USRDB_WRONG_PASSWORD    0xC0FB0009L

/** Invalid token. */
#define ERR_AUTH_USRDB_INVALID_TOKEN     0xC0FB000AL

/** Option flag is not enabled. */
#define ERR_AUTH_USRDB_OPT_FLAG_NOT_ENABLED 0xC0FB000BL

/** Invalid user name. */
#define ERR_AUTH_USRDB_INVALID_USERNAME  0xC0FB000CL

/** Authentication Manager is already initialized. */
#define ERR_AUTH_USRDB_ALREADY_INITIALIZED 0xC0FB000DL

/** Valid token. */
#define ERR_AUTH_USRDB_VALID_TOKEN       0xC0FB000EL

/** Invalid file name. */
#define ERR_AUTH_USRDB_INVALID_FILENAME  0xC0FB000FL

/** Invalid credentials. Wrong username or password. */
#define ERR_AUTH_USRDB_INVALID_CREDENTIALS 0xC0FB0010L

/** Root user could not be created. */
#define ERR_AUTH_USRDB_ROOT_NOT_CREATED  0xC0FB0011L

/** Password is either empty, too short or too long. */
#define ERR_AUTH_USRDB_INVALID_PASSWORD_LEN 0xC0FB0012L

/** Invalid token buffer. */
#define ERR_AUTH_USRDB_INVALID_TOKEN_BUFFER 0xC0FB0013L

/** Group not allowed. */
#define ERR_AUTH_USRDB_GROUP_NOT_ALLOWED 0xC0FB0014L

/** User already exist. */
#define ERR_AUTH_USRDB_USER_EXISTS       0xC0FB0015L

/** User enumeration index is out of range. */
#define ERR_AUTH_USRDB_IDX_OUT_OF_RANGE  0xC0FB0016L

/** Token is not privileged enough to execute operation. */
#define ERR_AUTH_USRDB_TOKEN_UNPRIVILEGED 0xC0FB0017L

/** The new configuration does not match the configuration the system was initialized with. */
#define ERR_AUTH_MANAGER_CONFIG_MISSMATCH 0xC0FB0018L

/***********************************************************************************/
/* Authentication Manager - Certificate Database Error Codes */
/***********************************************************************************/

/** Certificate database initialization failed. */
#define ERR_AUTH_CRTDB_INITIALIZATION_FAILED 0xC0FB0020L

/** Certificate database module has not been initialized yet. */
#define ERR_AUTH_CRTDB_NOT_INITIALIZED   0xC0FB0021L

/** No entry matching the search criteria could be found in the database. */
#define ERR_AUTH_CRTDB_ENTRY_NOT_FOUND   0xC0FB0022L

/** The Trust Store file seems to be corrupted. */
#define ERR_AUTH_CRTDB_DATA_INCONSISTENT 0xC0FB0023L

/** Failed to store the file. */
#define ERR_AUTH_CRTDB_STORE_FAILED      0xC0FB0024L

/** An invalid security component ID was passed to the CrtDB API. */
#define ERR_AUTH_CRTDB_INVALID_COMPONENT_ID 0xC0FB0025L

/** An invalid parameter was passed to the certificate database API. */
#define ERR_AUTH_CRTDB_INVALID_PARAMETER 0xC0FB0026L

/** An invalid KeyId was passed to the Generate Key API. */
#define ERR_AUTH_CRTDB_INVALID_KEY_ID    0xC0FB0027L

/** Unable to parse the key. */
#define ERR_AUTH_CRTDB_PARSE_KEY_FAILED  0xC0FB0028L

/** Failure during key generation. */
#define ERR_AUTH_CRTDB_KEY_GENERATION_FAILED 0xC0FB0029L

/** Failed to load the encrypted key. */
#define ERR_AUTH_CRTDB_LOAD_KEY_FAILED   0xC0FB002AL

/** Failure during the key encryption. */
#define ERR_AUTH_CRTDB_KEY_ENCRYPTION_FAILED 0xC0FB002BL

/** Failure during the key decryption. */
#define ERR_AUTH_CRTDB_KEY_DECRYPTION_FAILED 0xC0FB002CL

/** Certificate signing is failed. */
#define ERR_AUTH_CRTDB_CERT_SIGNING_FAILED 0xC0FB002DL

/** Error during generation of a certificate signing request. */
#define ERR_AUTH_CRTDB_CSR_GENERATION_FAILED 0xC0FB002EL

/** Error occurred during parsing of a x509 certificate. */
#define ERR_AUTH_CRTDB_PARSE_X509_FAILED 0xC0FB002FL

/** Unable to load the requested certificate. */
#define ERR_AUTH_CRTDB_LOAD_CRT_FAILED   0xC0FB0030L

/** The trust store is out of resource. */
#define ERR_AUTH_CRTDB_TRUST_STORE_OUT_OF_RESOURCE 0xC0FB0031L

/** The certificate chain is not complete. */
#define ERR_AUTH_CRTDB_INCOMPLETE_CERTIFICATE_CHAIN 0xC0FB0032L

/** Error occurred while setting the extension to the CSR or to the certificate. */
#define ERR_AUTH_CRTDB_SET_EXTENSION_FAILED 0xC0FB0033L

/** The file doesn't exist in the given path or the path is invalid. */
#define ERR_AUTH_CRTDB_FILE_NOT_FOUND    0xC0FB0034L

#endif  /* __AUTH_MANAGER_RESULTS_H */
