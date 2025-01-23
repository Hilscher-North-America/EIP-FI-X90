/*! \file AuthMgr_UsrDB_Definitions.h */

#ifndef AUTH_USRDB_DEFINITIONS_H
#define AUTH_USRDB_DEFINITIONS_H

#include <stdint.h>

#define SRT_AUTH_USRDB_GROUP_GUEST                    0           /*!< User group "guest" */
#define MSK_AUTH_USRDB_GROUP_GUEST                    0x00000001  /*!< User group "guest" */
#define SRT_AUTH_USRDB_GROUP_USER                     1           /*!< User group "user" */
#define MSK_AUTH_USRDB_GROUP_USER                     0x00000002  /*!< User group "user" */
#define SRT_AUTH_USRDB_GROUP_MANAGER                  2           /*!< User group "manager" */
#define MSK_AUTH_USRDB_GROUP_MANAGER                  0x00000004  /*!< User group "manager" */
#define SRT_AUTH_USRDB_GROUP_FWUPDATE                 3           /*!< User group "firmware update" */
#define MSK_AUTH_USRDB_GROUP_FWUPDATE                 0x00000008  /*!< User group "firmware update" */
#define SRT_AUTH_USRDB_GROUP_RESET                    4           /*!< User group "reset" */
#define MSK_AUTH_USRDB_GROUP_RESET                    0x00000010  /*!< User group "reset" */

#define SRT_AUTH_USRDB_GROUP_PRODUCT_SPECIFIC_0       8           /*!< User group "product specific 0" */
#define MSK_AUTH_USRDB_GROUP_PRODUCT_SPECIFIC_0       0x00000100  /*!< User group "product specific 0" */
#define SRT_AUTH_USRDB_GROUP_PRODUCT_SPECIFIC_1       9           /*!< User group "product specific 1" */
#define MSK_AUTH_USRDB_GROUP_PRODUCT_SPECIFIC_1       0x00000200  /*!< User group "product specific 1" */
#define SRT_AUTH_USRDB_GROUP_PRODUCT_SPECIFIC_2       10          /*!< User group "product specific 2" */
#define MSK_AUTH_USRDB_GROUP_PRODUCT_SPECIFIC_2       0x00000400  /*!< User group "product specific 2" */
#define SRT_AUTH_USRDB_GROUP_PRODUCT_SPECIFIC_3       11          /*!< User group "product specific 3" */
#define MSK_AUTH_USRDB_GROUP_PRODUCT_SPECIFIC_3       0x00000800  /*!< User group "product specific 3" */
#define SRT_AUTH_USRDB_GROUP_PRODUCT_SPECIFIC_4       12          /*!< User group "product specific 4" */
#define MSK_AUTH_USRDB_GROUP_PRODUCT_SPECIFIC_4       0x00001000  /*!< User group "product specific 4" */
#define SRT_AUTH_USRDB_GROUP_PRODUCT_SPECIFIC_5       13          /*!< User group "product specific 5" */
#define MSK_AUTH_USRDB_GROUP_PRODUCT_SPECIFIC_5       0x00002000  /*!< User group "product specific 5" */
#define SRT_AUTH_USRDB_GROUP_PRODUCT_SPECIFIC_6       14          /*!< User group "product specific 6" */
#define MSK_AUTH_USRDB_GROUP_PRODUCT_SPECIFIC_6       0x00004000  /*!< User group "product specific 6" */
#define SRT_AUTH_USRDB_GROUP_PRODUCT_SPECIFIC_7       15          /*!< User group "product specific 7" */
#define MSK_AUTH_USRDB_GROUP_PRODUCT_SPECIFIC_7       0x00008000  /*!< User group "product specific 7" */

/* deprecated, shall not be used anymore
#define SRT_AUTH_USRDB_GROUP_USER_MANAGER             24
#define MSK_AUTH_USRDB_GROUP_USER_MANAGER             0x01000000
*/
#define SRT_AUTH_USRDB_GROUP_USER_ACCOUNT_MANAGER     26         /*!< User group "user account manager" */
#define MSK_AUTH_USRDB_GROUP_USER_ACCOUNT_MANAGER     0x04000000 /*!< User group "user account manager" */
#define SRT_AUTH_USRDB_GROUP_ADMIN                    31         /*!< User group "admin" */
#define MSK_AUTH_USRDB_GROUP_ADMIN                    0x80000000 /*!< User group "admin" */

/*! Bit field of user groups, where each bit represents a well-defined user group
 * (see MSK_AUTH_USRDB_GROUP_* or SRT_AUTH_USRDB_GROUP_*). */
typedef uint32_t AUTH_USRDB_USER_GROUP_BF_T;

#endif /* AUTH_USRDB_DEFINITIONS_H */
