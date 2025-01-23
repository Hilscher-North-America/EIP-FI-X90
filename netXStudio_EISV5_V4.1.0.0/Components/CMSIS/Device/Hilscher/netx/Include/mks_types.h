/**
  * \copyright (c) 2022  MKS Instruments, Inc. All rights reserved.
  * The information herein is confidential, and includes
  * trade secrets and other proprietary information.
  *
  * \file mks_types.h
  *
  * Types and enumerations shared by the MKS code.
  *
  * MODIFICATION HISTORY
  *   | Date     | Initials | Notes |
  *   | -------- | -------- | ------------------------------------------------|
  *   | 09/30/22 |  LM      | Initial types and return codes
  */

#ifndef _MKS_TYPES_H_
#define _MKS_TYPES_H_

/** NULL type */
#define MKS_NULL  0L
/** False type */
#define MKS_FALSE 0
/** True type */
#define MKS_TRUE  1

#ifndef TRUE
#define TRUE MKS_TRUE
#endif

#ifndef FALSE
#define FALSE MKS_FALSE
#endif


/*! Mks Return Types */
typedef unsigned char           MksBoolType;
typedef char                    MksInt8Type;
typedef unsigned char           MksUint8Type;
typedef short                   MksInt16Type;
typedef unsigned short          MksUint16Type;
typedef long                    MksInt32Type;
typedef unsigned long           MksUint32Type;
typedef long long unsigned int  MksUint64Type;


/*! Enumeration of return codes for MKS functions */
typedef enum MksReturnCodes
{
  kMksOk = 0,
  kMksInvalidParameter,
  kMksUnsupportedOperation,
  kMksOperationFailed,
  kMksConfigurationError,
  kMksDriverBusy,
  kMksSystemCounterNotRuning,
  kMksSysBusInitError,
  kMksNotInitialized,
  kMksFileNotReady,
  kMksEOF,

  kMksTimeout = 100,
  kMksInvalidPointer,
  kMksInvalidBufferSize,
  kMksMemoryOverrun,

  kMksUnknownError = 0xfffffffful
} MksReturnCodes;


#endif //_MKS_TYPES_H_
