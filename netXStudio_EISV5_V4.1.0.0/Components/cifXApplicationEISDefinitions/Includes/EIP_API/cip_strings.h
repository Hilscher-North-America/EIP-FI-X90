/**************************************************************************************
Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
***************************************************************************************
$Id: cip_strings.h 104459 2022-01-21 11:10:11Z OKompani $:

Description:
  EtherNet/IP - CIP Strings

 CIP string data types and functions.
 Concerning the ISTRING type, it needs to be mentioned that third party tools, e.g.
 Rockwell's RSLinx, can only handle SHORT_STRING types encapsulated in the ISTRING.
**************************************************************************************/
#ifndef CIP_STRINGS_H
#define CIP_STRINGS_H

#include <stdint.h>

#include "Hil_Compiler.h"
#include "cip_common.h"

/* string types:
 * STRING         (0xD0): [CharCount[2 Bytes]]-[StringContents[1-Byte organized]]
 * STRING2        (0xD5): [CharCount[2 Bytes]]-[StringContents[2-Bytes organized]]
 * STRINGN        (0xD9): [CharSize[2 Bytes]]-[CharCount[2 Bytes]]-[StringContents[N-Bytes organized]]
 * SHORT_STRING   (0xDA): [CharCount[1 Byte]]-[StringContents[1-Byte organized]]
 */
#define   CIP_STRINGS_STRING_FORMAT                 0xD0
#define   CIP_STRINGS_STRING2_FORMAT                0xD5
#define   CIP_STRINGS_STRINGN_FORMAT                0xD9
#define   CIP_STRINGS_SHORT_STRING_FORMAT           0xDA

/*
 * #define   CIP_STRINGS_SUPPORT_STRING_FORMAT
 * #define   CIP_STRINGS_SUPPORT_STRING2_FORMAT
 * #define   CIP_STRINGS_SUPPORT_STRINGN_FORMAT
 */

#define   CIP_STRINGS_CHARSET_ISO_8859_1            4
#define   CIP_STRINGS_CHARSET_ISO_8859_2            5
#define   CIP_STRINGS_CHARSET_ISO_8859_3            6
#define   CIP_STRINGS_CHARSET_ISO_8859_4            7
#define   CIP_STRINGS_CHARSET_ISO_8859_5            8
#define   CIP_STRINGS_CHARSET_ISO_8859_6            9
#define   CIP_STRINGS_CHARSET_ISO_8859_7            10
#define   CIP_STRINGS_CHARSET_ISO_8859_8            11
#define   CIP_STRINGS_CHARSET_ISO_8859_9            12

#define   CIP_STRINGS_DEFAULT_CHARSET               CIP_STRINGS_CHARSET_ISO_8859_1
/* The ISO_8859 variants all have the same standard character encodings(32..127) Only the extended encodings(128..255)
 * have partly differences.
   As a CIP embedded device is only a media to transfer strings and doesn't print the strings itself,
   this is no matter which charset is in the stringi. We only check the charset value to be in the range. */
#define   CIP_STRINGS_SUPPORTED_CHARSET             ( (1 << CIP_STRINGS_CHARSET_ISO_8859_1) | \
                                                      (1 << CIP_STRINGS_CHARSET_ISO_8859_2) | \
                                                      (1 << CIP_STRINGS_CHARSET_ISO_8859_3) | \
                                                      (1 << CIP_STRINGS_CHARSET_ISO_8859_4) | \
                                                      (1 << CIP_STRINGS_CHARSET_ISO_8859_5) | \
                                                      (1 << CIP_STRINGS_CHARSET_ISO_8859_6) | \
                                                      (1 << CIP_STRINGS_CHARSET_ISO_8859_7) | \
                                                      (1 << CIP_STRINGS_CHARSET_ISO_8859_8) | \
                                                      (1 << CIP_STRINGS_CHARSET_ISO_8859_9) )
#define   CIP_STRINGS_MAX_STRING_CHARSIZE           1
#define   CIP_STRINGS_MAX_STRING_CHARCOUNT          32 /* CharCount * char-size (Metadata is not counted) */

#define   CIP_STRINGS_MAX_STRING2_CHARSIZE          2
#define   CIP_STRINGS_MAX_STRING2_CHARCOUNT         32 /* CharCount * char-size (Metadata is not counted) */

#define   CIP_STRINGS_MAX_STRINGN_CHARSIZE          1
#define   CIP_STRINGS_MAX_STRINGN_CHARCOUNT         32 /* CharCount * char-size (Metadata is not counted) */

#define   CIP_STRINGS_MAX_SHORT_STRING_CHARSIZE     1
#define   CIP_STRINGS_MAX_SHORT_STRING_CHARCOUNT    32 /* CharCount * char-size (Metadata is not counted) */

#define   CIP_STRINGS_MAX_STRINGI_ENTRIES           1


typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST CIP_SHORT_STRING_Ttag {
    CIP_USINT_T bCharCount;
    CIP_USINT_T abString[CIP_STRINGS_MAX_SHORT_STRING_CHARCOUNT * CIP_STRINGS_MAX_SHORT_STRING_CHARSIZE];
} CIP_SHORT_STRING_T;

#ifdef CIP_STRINGS_SUPPORT_STRING_FORMAT
    typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST CIP_STRING_Ttag {
        CIP_UINT_T  usCharCount;
        CIP_USINT_T abString[CIP_STRINGS_MAX_STRING_CHARCOUNT * CIP_STRINGS_MAX_STRING_CHARSIZE];
    } CIP_STRING_T;
#endif

#ifdef CIP_STRINGS_SUPPORT_STRING2_FORMAT
    typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST CIP_STRING2_Ttag {
        CIP_UINT_T usCharCount;
        CIP_UINT_T ausString[CIP_STRINGS_MAX_STRING2_CHARCOUNT * CIP_STRINGS_MAX_STRING2_CHARSIZE];
    } CIP_STRING2_T;
#endif

#ifdef CIP_STRINGS_SUPPORT_STRINGN_FORMAT
    typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST CIP_STRINGN_Ttag {
        CIP_UINT_T  usCharSize;
        CIP_UINT_T  usCharCount;
        CIP_USINT_T abString[CIP_STRINGS_MAX_STRINGN_CHARCOUNT * CIP_STRINGS_MAX_STRINGN_CHARSIZE];
    } CIP_STRINGN_T;
#endif

#ifdef __HIL_PRAGMA_PACK_ENABLE
#pragma __HIL_PRAGMA_PACK_1(CIP_STRINGS)
#endif

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST CIP_STRINGI_Ttag {
  CIP_USINT_T bNumOfEntries;

  __HIL_PACKED_PRE struct __HIL_PACKED_POST {
    CIP_USINT_T abLangCode[3];   /* three characters as of ISO 639-2/T, e.g. "eng" */
    CIP_USINT_T bStringFormat;   /* limited to one byte (0xd0, 0xd5, 0xd9 or 0xda), identifying the string format*/
    CIP_UINT_T  usCharset;       /* charset identifier as of RFC 1759 IANA MIB printer codes */
    union {

      CIP_SHORT_STRING_T  tShortString;

#ifdef CIP_STRINGS_SUPPORT_STRING_FORMAT
    CIP_STRING_T  tString;
#endif
#ifdef CIP_STRINGS_SUPPORT_STRING2_FORMAT
    CIP_STRING2_T tString2;
#endif
#ifdef CIP_STRINGS_SUPPORT_STRINGN_FORMAT
    CIP_STRINGN_T tStringN;
#endif
    } uFormat;
  } atEntry[CIP_STRINGS_MAX_STRINGI_ENTRIES];

} CIP_STRINGI_T;

#ifdef __HIL_PRAGMA_PACK_ENABLE
#pragma __HIL_PRAGMA_UNPACK_1(CIP_STRINGS)
#endif

/* function declarations */
bool CipStrings_IsValidStringi(const CIP_STRINGI_T* ptStringi);
void CipStrings_UpdateStringi(CIP_STRINGI_T* ptStringi, uint8_t bStringFormat, char* szLangCode, char* szString);
void CipStrings_ClearStringi(CIP_STRINGI_T*  ptStringi);
uint32_t CipStrings_GetStringiLen(const CIP_STRINGI_T* ptStringi);
uint32_t CipStrings_GetShortStringLen(const CIP_SHORT_STRING_T* ptShortString);

#endif
