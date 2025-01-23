/**************************************************************************************
Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
***************************************************************************************
$Id: cip_file.h 105015 2022-02-18 23:16:54Z OKompani $:
**************************************************************************************/

#ifndef CIP_FILE_H
#define CIP_FILE_H

#include <stdint.h>

#include "Hil_Compiler.h"
#include "cip_strings.h"

/* -------------------------------------------------------------------------- */
#define CIP_FILE_CLASS_NUMBER                       0x37
#define CIP_FILE_NUM_STD_INSTANCE_ATTR              12
#define CIP_FILE_MAX_CLASS_ATTR                     768
#define CIP_FILE_MAX_INSTANCE_ATTR                  769

#define CIP_FILE_VENDOR_INSTANCE_ATTR               2
#define CIP_FILE_REVISION                           3
#define CIP_FILE_MAX_INSTANCE                       0 /* default: calculated dynamically */

/* CIP specific file instances */
#define CIP_FILE_EDS_INSTANCE_ID                    0x00C8
#define CIP_FILE_EDS_COLLECTION_INSTANCE_ID         0x00C9
/* Vendor specific static instances */
#define CIP_FILE_DEVICE_CONFIG_INSTANCE_ID          0x0100
#define CIP_FILE_DEFAULT_DEVICE_CERT_INSTANCE_ID    0x0101
#define CIP_FILE_DEFAULT_CA_CERT_INSTANCE_ID        0x0102
/* Vendor specific dynamic instances */
#define CIP_FILE_DYN_INSTANCE_1_ID                  0x0110
#define CIP_FILE_DYN_INSTANCE_2_ID                  0x0111
#define CIP_FILE_DYN_INSTANCE_3_ID                  0x0112
#define CIP_FILE_DYN_INSTANCE_4_ID                  0x0113
#define CIP_FILE_DYN_INSTANCE_5_ID                  0x0114
#define CIP_FILE_DYN_INSTANCE_6_ID                  0x0115
#define CIP_FILE_DYN_INSTANCE_7_ID                  0x0116
#define CIP_FILE_DYN_INSTANCE_8_ID                  0x0117

/* -------------------------------------------------------------------------- */
/* Instance/File names */
/* Static instances */

/* define these to either 1 or 0 */
#define USE_CIP_FILE_EDS_INSTANCE                     1
#define USE_CIP_FILE_EDS_COLLECTION_INSTANCE          1
#define USE_CIP_FILE_DEVICE_CONFIG_INSTANCE           1
#define USE_CIP_FILE_DEFAULT_DEVICE_CERT_INSTANCE     1
#define USE_CIP_FILE_DEFAULT_CA_CERT_INSTANCE         1


#define CIP_FILE_MAX_NUM_OF_DYNAMIC_INSTANCES               5
#define CIP_FILE_NUM_OF_STATIC_INSTANCES                    ( USE_CIP_FILE_EDS_INSTANCE +\
                                                              USE_CIP_FILE_EDS_COLLECTION_INSTANCE +\
                                                              USE_CIP_FILE_DEVICE_CONFIG_INSTANCE +\
                                                              USE_CIP_FILE_DEFAULT_DEVICE_CERT_INSTANCE +\
                                                              USE_CIP_FILE_DEFAULT_CA_CERT_INSTANCE )

#define CIP_FILE_MAX_NUM_OF_INSTANCES                       ( CIP_FILE_NUM_OF_STATIC_INSTANCES + CIP_FILE_MAX_NUM_OF_DYNAMIC_INSTANCES )


#define CIP_FILE_DIRECTORY_PATH                             "file://SYSVOLUME/PORT_0/"
#define CIP_FILE_MAX_FILENAME_LEN                           ( sizeof("12345678.123") )
                                                            /* -1 to exclude one of NUL terminations from size */
#define CIP_FILE_MAX_PATH_LEN                               (( sizeof(CIP_FILE_DIRECTORY_PATH) + CIP_FILE_MAX_FILENAME_LEN ) - 1)

#define CIP_FILE_MAX_TRANSFER_SIZE                          0x80
                                                            /* This is the size of data that triggers a Save to NV */
#define CIP_FILE_INCREMENTAL_BURN_SIZE                      ( (CIP_FILE_MAX_TRANSFER_SIZE) * 16 )
                                                            /* This is the actual size of the buffer.
                                                             * Plus extra capacity to protect the buffer against overflow */
#define CIP_FILE_BURN_BUFFER_SIZE                           ( CIP_FILE_INCREMENTAL_BURN_SIZE + CIP_FILE_MAX_TRANSFER_SIZE )

/* -------------------------------------------------------------------------- */


/* attributes of file object instance */
#define CIP_FILE_ATTR_1_STATE                                   1
#define CIP_FILE_ATTR_2_INSTANCE_NAME                           2
#define CIP_FILE_ATTR_3_FILE_FORMAT_VERSION                     3
#define CIP_FILE_ATTR_4_FILE_NAME                               4
#define CIP_FILE_ATTR_5_FILE_REVISION                           5
#define CIP_FILE_ATTR_6_FILE_SIZE                               6
#define CIP_FILE_ATTR_7_FILE_CHECKSUM                           7
#define CIP_FILE_ATTR_8_INVOCATION_METHOD                       8
#define CIP_FILE_ATTR_9_FILE_SAVE_PARAMETERS                    9
#define CIP_FILE_ATTR_10_FILE_ACCESS_RULE                       10
#define CIP_FILE_ATTR_11_FILE_ENCODING_FORMAT                   11
#define CIP_FILE_ATTR_12_TRANSFER_SESSION_TIMEOUT               12

#define CIP_FILE_ATTR_768_LOCAL_FILE_NAME                       768 /* 0x300 */
#define CIP_FILE_ATTR_769_INSTANCE_ID                           769 /* 0x301 */

/* definitions for attribute 1 of file object instance: "State of the object instance" */
#define CIP_FILE_INSTANCE_STATE_0_NONEXISTENT                   0
#define CIP_FILE_INSTANCE_STATE_1_EMPTY                         1
#define CIP_FILE_INSTANCE_STATE_2_LOADED                        2
#define CIP_FILE_INSTANCE_STATE_3_UPLOAD_INITIATED              3
#define CIP_FILE_INSTANCE_STATE_4_DOWNLOAD_INITIATED            4
#define CIP_FILE_INSTANCE_STATE_5_UPLOAD_IN_PROGRESS            5
#define CIP_FILE_INSTANCE_STATE_6_DOWNLOAD_IN_PROGRESS          6
#define CIP_FILE_INSTANCE_STATE_7_STORING                       7

/* specific attributes of file object class */
#define CIP_FILE_CLASS_ATTR_32_DICTIONARY                       32
#define CIP_FILE_CLASS_ATTR_768_EXISTING_INSTANCES              768 /* 0x300 */

/* transfer packet types */
#define CIP_FILE_TRANSFER_PACKET_TYPE_0_FIRST_PACKET            0
#define CIP_FILE_TRANSFER_PACKET_TYPE_1_MIDDLE_PACKET           1
#define CIP_FILE_TRANSFER_PACKET_TYPE_2_LAST_PACKET             2
#define CIP_FILE_TRANSFER_PACKET_TYPE_3_ABORT_TRANSFER          3
#define CIP_FILE_TRANSFER_PACKET_TYPE_4_FIRST_AND_LAST_PACKET   4

/* Download service, Save attribute values  */
#define CIP_FILE_SAVE_PARAMETERS_SAVE_METHOD_MASK               0x0F
#define CIP_FILE_SAVE_METHOD_0_AUTOMATIC                        0 /* Auto save only supported by Read/write instances      */
#define CIP_FILE_SAVE_METHOD_1_SAVE_SERVICE                     1 /* save service should be implemented     */
#define CIP_FILE_SAVE_METHOD_2_VOLATILE                         2 /* Doesn't save the data to a NV storage  */
#define CIP_FILE_SAVE_METHOD_3_CUSTOM_SAVE                      3
#define CIP_FILE_SAVE_METHOD_4_CUSTOM_AND_SERVICE_SAVE          4

#define CIP_FILE_SAVE_PARAMETERS_SAVE_SATAUS_MASK               0x10
#define CIP_FILE_SAVE_STATUS                                    0x10  /* bit 4 of save parameters attribute */
/* File access rules  */
#define CIP_FILE_ACCESS_RULE_0_READ_WRITE                       0
#define CIP_FILE_ACCESS_RULE_1_READ_ONLY                        1
/* File invocation method */
#define CIP_FILE_INVOCATION_METHOD_0_NO_ACTION                  0
#define CIP_FILE_INVOCATION_METHOD_1_RESET                      1
#define CIP_FILE_INVOCATION_METHOD_2_POWER_CYCLE                2
#define CIP_FILE_INVOCATION_METHOD_255_NOT_APPLICABLE           255
/* Instance file encoding format  */
#define CIP_FILE_ENCODING_FORMAT_0_BINARY                       0
#define CIP_FILE_ENCODING_FORMAT_1_COMPRESSED                   1
#define CIP_FILE_ENCODING_FORMAT_2_PEM_CERT                     2
#define CIP_FILE_ENCODING_FORMAT_3_PKCS7_CERT                   3
#define CIP_FILE_ENCODING_FORMAT_4_PEM_CRL                      4
#define CIP_FILE_ENCODING_FORMAT_5_PKCS7_CRL                    5
#define CIP_FILE_ENCODING_FORMAT_11_ASCII_TEXT                  11
#define CIP_FILE_ENCODING_FORMAT_12_WORD                        12
#define CIP_FILE_ENCODING_FORMAT_13_EXCEL                       13
#define CIP_FILE_ENCODING_FORMAT_14_PDF                         14
/* specific services of the file object instance */
#define CIP_FILE_CMD_INITIATE_UPLOAD                            0x4B
#define CIP_FILE_CMD_INITIATE_DOWNLOAD                          0x4C
#define CIP_FILE_CMD_UPLOAD_TRANSFER                            0x4F
#define CIP_FILE_CMD_DOWNLOAD_TRANSFER                          0x50
#define CIP_FILE_CMD_CLEAR_FILE                                 0x51

#define CIPHIL_CMD_CIP_FILE_CREATE_WITH_CONTENTS                0x3701
                                                                /* Request parameters:
                                                                 *  Instance name
                                                                 *  File name
                                                                 *  File size
                                                                 *  File format version
                                                                 *  File revision
                                                                 *  File encoding format
                                                                 *  File contents
                                                                 */
#define CIP_FILE_CREATE_WITH_CONTENTS_MAX_PARAMETER_SIZE        ( (sizeof(CIP_STRINGI_T) * 2) + \
                                                                   sizeof(uint32_t) + \
                                                                   sizeof(uint16_t) + \
                                                                   sizeof(uint16_t) + \
                                                                   sizeof(uint8_t)  + \
                                                                   sizeof(void*) )
#define CIPHIL_CMD_CIP_FILE_LOAD_CONTENTS                       0x3702
                                                                /* Request parameters:
                                                                 *  *Buffer
                                                                 *  Buffer size
                                                                 *
                                                                 * Response parameters
                                                                 *  File size
                                                                 */
/* -------------------------------------------------------------------------------------------- */
/* specific error codes for initiate/transfer services      */
/* -------------------------------------------------------- */
/* Extended error codes for CIP_GSR_BAD_OBJ_MODE(0x0C)      */
#define CIP_FILE_ESR_FILE_IS_READ_ONLY                          0x0100  /* File is read-only */
/* -------------------------------------------------------- */
/* Extended error codes for CIP_GSR_CONFIG_TOO_BIG(0x15)    */
#define CIP_FILE_ESR_FILENAME_TOO_LONG                          0x0001  /* File name too long */
#define CIP_FILE_ESR_TOO_MANY_LANGUAGES_IN_FILENAME             0x0002  /* Too many languages in File Name */
/* -------------------------------------------------------- */
/* Extended error codes for CIP_GSR_INVALID_PARAMETER(0x20) */
#define CIP_FILE_ESR_FILE_OFFSET_OUT_OF_RANGE                   0x0002  /* File Offset out of range */
#define CIP_FILE_ESR_READ_WRITE_BEYOND_FILE_END                 0x0003  /* Read/Write Size goes beyond end of file */
#define CIP_FILE_ESR_FILE_SIZE_TOO_LARGE                        0x0004  /* File Size too large */
#define CIP_FILE_ESR_FILE_FORMAT_VERSION_NOT_COMPATIBLE         0x0005  /* File format version not compatible */
#define CIP_FILE_ESR_FAIL_ON_TRANSFER_OUT_OF_SEQUENCE           0x0006  /* Fail on transfer – out of sequence */
#define CIP_FILE_ESR_FAIL_ON_TRANSFER_OTHER                     0x0007  /* Fail on transfer – other  */
#define CIP_FILE_ESR_FAIL_ON_TRANSFER_ZERO_SIZE                 0x0008  /* Fail on transfer - zero size */
#define CIP_FILE_ESR_FILENAME_EMPTY                             0x000A  /* File name empty */
#define CIP_FILE_ESR_FAIL_ON_TRANSFER_DUPLICATE                 0x000B  /* Fail on transfer – duplicate */

#define CIP_FILE_GSR_FILE_EMPTY                                 0x0C
#define CIP_FILE_GSR_INVALID_CHECKSUM                           0xD0    /* Fail on checksum */
#define CIP_FILE_GSR_FAIL_ON_SAVE_TO_NV_STORAGE                 0x19    /* Fail on save to NV storage  */
#define CIP_FILE_GSR_FAIL_ON_TRANSFER                           0x20    /* Instance not deletable */

/* -------------------------------------------------------------------------- */
typedef struct CIP_FILE_CLASS_ATTR_STORED_Ttag
{
  uint16_t ausAttr768_InstanceMapping[CIP_FILE_MAX_NUM_OF_INSTANCES]; /* Attribute 0x300, List of all instances (statics + dynamics)
                                                                       * ausInstanceId[Index] either contains an existing InstanceId
                                                                       * or zero for a non-existing instance. */
} CIP_FILE_CLASS_ATTR_STORED_T;


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST CIP_FILE_INST_ATTR_STORED_STATIC_INSTANCETtag
{
  uint8_t         bTransferSessionTimeout;  /* attribute 12 */  /* Static: tNV - Dynamic: tNV */

} CIP_FILE_INST_ATTR_STORED_STATIC_INSTANCE_T;


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST CIP_FILE_INST_ATTR_STORED_DYNAMIC_INSTANCE_Ttag
{
  CIP_STRINGI_T   tInstanceName;            /* attribute 2 */   /* Static: tV - Dynamic: tNV */
  uint16_t        usFileFormatVersion;      /* attribute 3 */   /* Static: tV - Dynamic: tNV */
  CIP_STRINGI_T   tFileName;                /* attribute 4 */   /* Static: tV - Dynamic: tNV */
  struct
  {
      uint8_t     bMajorRevision;
      uint8_t     bMinorRevision;
  } tFileRevision;                          /* attribute 5 */   /* Static: tV - Dynamic: tNV */
  uint8_t         bInvocationMethod;        /* attribute 8 */   /* Static: tV - Dynamic: tNV */
  uint8_t         bFileAccessRule;          /* attribute 10 */  /* Static: tV - Dynamic: tNV */
  uint8_t         bFileEncodingFormat;      /* attribute 11 */  /* Static: tV - Dynamic: tNV */
  uint8_t         bTransferSessionTimeout;  /* attribute 12 */  /* Static: tNV - Dynamic: tNV */

} CIP_FILE_INST_ATTR_STORED_DYNAMIC_INSTANCE_T;


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST CIP_FILE_INITIATE_DOWNLOAD_REQUEST_PARAMETERS_Ttag
{
  uint32_t  ulFileSize;
  uint16_t  usFileFormatVersion;
  struct
  {
      uint8_t bMajorRevision;
      uint8_t bMinorRevision;
  } tFileRevision;
  CIP_STRINGI_T tFileName;
} CIP_FILE_INITIATE_DOWNLOAD_REQUEST_PARAMETERS_T;


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST CIP_FILE_INITIATE_DOWNLOAD_RESPONSE_PARAMETERS_Ttag
{
  uint32_t  ulIncrementalBurnSize; /* Capacity of the intermediate buffer. The buffer will be flushed into destination file if:
                                      it is full or if it may overflow in the next transfer cycle. */
  uint16_t  usIncrementalBurnTime; /* Number of seconds to perform a write to NV storage */
  uint8_t   bTransferSize;
} CIP_FILE_INITIATE_DOWNLOAD_RESPONSE_PARAMETERS_T;


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST CIP_FILE_DOWNLOAD_TRANSFER_REQUEST_PARAMETERS_Ttag
{
  uint8_t  bTransferNumber;
  uint8_t  bTransferPacketType;
  uint8_t  abFileData[];
} CIP_FILE_DOWNLOAD_TRANSFER_REQUEST_PARAMETERS_T;


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST CIP_FILE_DOWNLOAD_TRANSFER_RESPONSE_PARAMETERS_Ttag
{
  uint8_t bTransferNumber;
} CIP_FILE_DOWNLOAD_TRANSFER_RESPONSE_PARAMETERS_T;


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST CIP_FILE_INITIATE_UPLOAD_RESPONSE_PARAMETERS_Ttag
{
  uint32_t  ulFileSize;
  uint8_t   bMaxTransferSize;
} CIP_FILE_INITIATE_UPLOAD_RESPONSE_PARAMETERS_T;


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST CIP_FILE_UPLOAD_TRANSFER_RESPONSE_PARAMETERS_Ttag
{
  uint8_t bTransferNumber;
  uint8_t bTransferPacketType;
  uint8_t abTransferBuffer[CIP_FILE_MAX_TRANSFER_SIZE + 2]; /* buffer used for file upload, plus two bytes for checksum on last packet */
} CIP_FILE_UPLOAD_TRANSFER_RESPONSE_PARAMETERS_T;


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST CIP_FILE_INITIATE_UPLOAD_REQUEST_PARAMETERS_Ttag
{
  uint8_t bMaxTransferSize;
} CIP_FILE_INITIATE_UPLOAD_REQUEST_PARAMETERS_T;


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST CIP_FILE_UPLOAD_TRANSFER_REQUEST_PARAMETERS_Ttag
{
  uint8_t bTransferNumber;
} CIP_FILE_UPLOAD_TRANSFER_REQUEST_PARAMETERS_T;


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST CIP_FILE_LOAD_CONTENTS_REQUEST_PARAMETERS_Ttag
{
  void*    pvBuffer;
  uint32_t ulBufferSize;
} CIP_FILE_LOAD_CONTENTS_REQUEST_PARAMETERS_T;


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST CIP_FILE_LOAD_CONTENTS_RESPONSE_PARAMETERS_Ttag
{
  uint32_t ulFileSize;
} CIP_FILE_LOAD_CONTENTS_RESPONSE_PARAMETERS_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST CIP_FILE_CREATE_WITH_CONTENTS_REQUEST_PARAMETERS_Ttag
{
  uint32_t            ulFileSize;
  uint16_t            usFileFormatVersion;
  struct
  {
    uint8_t bMajor;
    uint8_t bMinor;
  }                   tFileRevision;
  uint8_t             bFileEncodingFormat;
  uint8_t             bPadding[3];
  uint8_t*            pbFileContents;
  CIP_STRINGI_T       tInstanceName;
  CIP_STRINGI_T       tFileName;
} CIP_FILE_CREATE_WITH_CONTENTS_REQUEST_PARAMETERS_T;

/*
 ************************************************************
 *  Task Function Prototypes
 ************************************************************
 */
struct EIP_RESOURCES_Ttag;  /* forward declaration */

uint32_t CipFile_ObjectCreate( struct EIP_RESOURCES_Ttag *hEip, void* pvParam );

#endif
