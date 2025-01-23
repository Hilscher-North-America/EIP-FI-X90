/**************************************************************************************
Exclusion of Liability for this demo software:
  The following software is intended for and must only be used for reference and in an
  evaluation laboratory environment. It is provided without charge and is subject to
  alterations. There is no warranty for the software, to the extent permitted by
  applicable law. Except when otherwise stated in writing the copyright holders and/or
  other parties provide the software "as is" without warranty of any kind, either
  expressed or implied.
  Please refer to the Agreement in README_DISCLAIMER.txt, provided together with this file!
  By installing or otherwise using the software, you accept the terms of this Agreement.
  If you do not agree to the terms of this Agreement, then do not install or use the
  Software!
**************************************************************************************/

/**************************************************************************************

Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.

***************************************************************************************/

/*****************************************************************************/
/*! General Inclusion Area                                                   */
/*****************************************************************************/

//#include <stdint.h>
#include "App_PacketCommunication.h"
#include "App_SystemPackets.h"
#include "Hil_SystemCmd.h"
#include "Hil_Packet.h"
#include "App_FileSystemPackets.h"
#include "nvram.h"

/*****************************************************************************/
/*! Definitions                                                              */
/*****************************************************************************/
#define NVRAM_FILE_NAME "PORT_1/NVRAM.DAT"
#define NVRAM_FILE_NAME_SIZE strlen(NVRAM_FILE_NAME) + 1
#define NVRAM_ID 1

#define INI_BLK_NUM 0x0000

/*****************************************************************************/
/*! Global Variables                                                         */
/*****************************************************************************/
extern APP_DATA_T g_tAppData;

// from 'cifXHelper.c'
static const uint32_t Crc32Table[256] = {
  0x00000000UL, 0x77073096UL, 0xee0e612cUL, 0x990951baUL, 0x076dc419UL,
  0x706af48fUL, 0xe963a535UL, 0x9e6495a3UL, 0x0edb8832UL, 0x79dcb8a4UL,
  0xe0d5e91eUL, 0x97d2d988UL, 0x09b64c2bUL, 0x7eb17cbdUL, 0xe7b82d07UL,
  0x90bf1d91UL, 0x1db71064UL, 0x6ab020f2UL, 0xf3b97148UL, 0x84be41deUL,
  0x1adad47dUL, 0x6ddde4ebUL, 0xf4d4b551UL, 0x83d385c7UL, 0x136c9856UL,
  0x646ba8c0UL, 0xfd62f97aUL, 0x8a65c9ecUL, 0x14015c4fUL, 0x63066cd9UL,
  0xfa0f3d63UL, 0x8d080df5UL, 0x3b6e20c8UL, 0x4c69105eUL, 0xd56041e4UL,
  0xa2677172UL, 0x3c03e4d1UL, 0x4b04d447UL, 0xd20d85fdUL, 0xa50ab56bUL,
  0x35b5a8faUL, 0x42b2986cUL, 0xdbbbc9d6UL, 0xacbcf940UL, 0x32d86ce3UL,
  0x45df5c75UL, 0xdcd60dcfUL, 0xabd13d59UL, 0x26d930acUL, 0x51de003aUL,
  0xc8d75180UL, 0xbfd06116UL, 0x21b4f4b5UL, 0x56b3c423UL, 0xcfba9599UL,
  0xb8bda50fUL, 0x2802b89eUL, 0x5f058808UL, 0xc60cd9b2UL, 0xb10be924UL,
  0x2f6f7c87UL, 0x58684c11UL, 0xc1611dabUL, 0xb6662d3dUL, 0x76dc4190UL,
  0x01db7106UL, 0x98d220bcUL, 0xefd5102aUL, 0x71b18589UL, 0x06b6b51fUL,
  0x9fbfe4a5UL, 0xe8b8d433UL, 0x7807c9a2UL, 0x0f00f934UL, 0x9609a88eUL,
  0xe10e9818UL, 0x7f6a0dbbUL, 0x086d3d2dUL, 0x91646c97UL, 0xe6635c01UL,
  0x6b6b51f4UL, 0x1c6c6162UL, 0x856530d8UL, 0xf262004eUL, 0x6c0695edUL,
  0x1b01a57bUL, 0x8208f4c1UL, 0xf50fc457UL, 0x65b0d9c6UL, 0x12b7e950UL,
  0x8bbeb8eaUL, 0xfcb9887cUL, 0x62dd1ddfUL, 0x15da2d49UL, 0x8cd37cf3UL,
  0xfbd44c65UL, 0x4db26158UL, 0x3ab551ceUL, 0xa3bc0074UL, 0xd4bb30e2UL,
  0x4adfa541UL, 0x3dd895d7UL, 0xa4d1c46dUL, 0xd3d6f4fbUL, 0x4369e96aUL,
  0x346ed9fcUL, 0xad678846UL, 0xda60b8d0UL, 0x44042d73UL, 0x33031de5UL,
  0xaa0a4c5fUL, 0xdd0d7cc9UL, 0x5005713cUL, 0x270241aaUL, 0xbe0b1010UL,
  0xc90c2086UL, 0x5768b525UL, 0x206f85b3UL, 0xb966d409UL, 0xce61e49fUL,
  0x5edef90eUL, 0x29d9c998UL, 0xb0d09822UL, 0xc7d7a8b4UL, 0x59b33d17UL,
  0x2eb40d81UL, 0xb7bd5c3bUL, 0xc0ba6cadUL, 0xedb88320UL, 0x9abfb3b6UL,
  0x03b6e20cUL, 0x74b1d29aUL, 0xead54739UL, 0x9dd277afUL, 0x04db2615UL,
  0x73dc1683UL, 0xe3630b12UL, 0x94643b84UL, 0x0d6d6a3eUL, 0x7a6a5aa8UL,
  0xe40ecf0bUL, 0x9309ff9dUL, 0x0a00ae27UL, 0x7d079eb1UL, 0xf00f9344UL,
  0x8708a3d2UL, 0x1e01f268UL, 0x6906c2feUL, 0xf762575dUL, 0x806567cbUL,
  0x196c3671UL, 0x6e6b06e7UL, 0xfed41b76UL, 0x89d32be0UL, 0x10da7a5aUL,
  0x67dd4accUL, 0xf9b9df6fUL, 0x8ebeeff9UL, 0x17b7be43UL, 0x60b08ed5UL,
  0xd6d6a3e8UL, 0xa1d1937eUL, 0x38d8c2c4UL, 0x4fdff252UL, 0xd1bb67f1UL,
  0xa6bc5767UL, 0x3fb506ddUL, 0x48b2364bUL, 0xd80d2bdaUL, 0xaf0a1b4cUL,
  0x36034af6UL, 0x41047a60UL, 0xdf60efc3UL, 0xa867df55UL, 0x316e8eefUL,
  0x4669be79UL, 0xcb61b38cUL, 0xbc66831aUL, 0x256fd2a0UL, 0x5268e236UL,
  0xcc0c7795UL, 0xbb0b4703UL, 0x220216b9UL, 0x5505262fUL, 0xc5ba3bbeUL,
  0xb2bd0b28UL, 0x2bb45a92UL, 0x5cb36a04UL, 0xc2d7ffa7UL, 0xb5d0cf31UL,
  0x2cd99e8bUL, 0x5bdeae1dUL, 0x9b64c2b0UL, 0xec63f226UL, 0x756aa39cUL,
  0x026d930aUL, 0x9c0906a9UL, 0xeb0e363fUL, 0x72076785UL, 0x05005713UL,
  0x95bf4a82UL, 0xe2b87a14UL, 0x7bb12baeUL, 0x0cb61b38UL, 0x92d28e9bUL,
  0xe5d5be0dUL, 0x7cdcefb7UL, 0x0bdbdf21UL, 0x86d3d2d4UL, 0xf1d4e242UL,
  0x68ddb3f8UL, 0x1fda836eUL, 0x81be16cdUL, 0xf6b9265bUL, 0x6fb077e1UL,
  0x18b74777UL, 0x88085ae6UL, 0xff0f6a70UL, 0x66063bcaUL, 0x11010b5cUL,
  0x8f659effUL, 0xf862ae69UL, 0x616bffd3UL, 0x166ccf45UL, 0xa00ae278UL,
  0xd70dd2eeUL, 0x4e048354UL, 0x3903b3c2UL, 0xa7672661UL, 0xd06016f7UL,
  0x4969474dUL, 0x3e6e77dbUL, 0xaed16a4aUL, 0xd9d65adcUL, 0x40df0b66UL,
  0x37d83bf0UL, 0xa9bcae53UL, 0xdebb9ec5UL, 0x47b2cf7fUL, 0x30b5ffe9UL,
  0xbdbdf21cUL, 0xcabac28aUL, 0x53b39330UL, 0x24b4a3a6UL, 0xbad03605UL,
  0xcdd70693UL, 0x54de5729UL, 0x23d967bfUL, 0xb3667a2eUL, 0xc4614ab8UL,
  0x5d681b02UL, 0x2a6f2b94UL, 0xb40bbe37UL, 0xc30c8ea1UL, 0x5a05df1bUL,
  0x2d02ef8d
};

/*****************************************************************************/
/*! FUNCTIONS                                                                */
/*****************************************************************************/

/*****************************************************************************/
/*! Create CRC32 for the File Write command.
 *
 * \param ulCRC      [in]  CRC  Packet resource used for assembling.
 * \param pabBuffer  [in]  Data buffer.
 * \param ulLenght   [in]  Data size.
 */
/*****************************************************************************/
static uint32_t CreateCRC32(uint32_t ulCRC, uint8_t* pabBuffer, uint32_t ulLength)
{
  if( (0 == pabBuffer) || (0 == ulLength) ) return ulCRC;
  ulCRC = ulCRC ^ 0xffffffff;
  for(;ulLength > 0; --ulLength)
  {
    ulCRC = (Crc32Table[((ulCRC) ^ (*(pabBuffer++)) ) & 0xff] ^ ((ulCRC) >> 8));
  }
  return ulCRC ^ 0xffffffff;
}


/*******************************************************************************
 *  ____  _____ ___       __   ____ _   _ _____
 * |  _ \| ____/ _ \     / /  / ___| \ | |  ___|
 * | |_) |  _|| | | |   / /  | |   |  \| | |_
 * |  _ <| |__| |_| |  / /   | |___| |\  |  _|
 * |_| \_\_____\__\_\ /_/     \____|_| \_|_|
 *
 ******************************************************************************/

/*****************************************************************************/
/*! Assembles a HIL_VOLUME_GET_ENTRY_REQ packet.
 *
 * \param ptPkt  [in]  Packet resource used for assembling.
 */
/*****************************************************************************/
static void App_SysPkt_AssembleVolumeGetEntryReq( CIFX_PACKET* ptPkt )
{
  HIL_VOLUME_GET_ENTRY_REQ_T* ptReq = (HIL_VOLUME_GET_ENTRY_REQ_T*)ptPkt;
 
  memset( ptReq, 0, sizeof(HIL_VOLUME_GET_ENTRY_REQ_T) );

  ptReq->tHead.ulDest = HIL_PACKET_DEST_SYSTEM;
  ptReq->tHead.ulCmd  = HIL_VOLUME_GET_ENTRY_REQ;
  ptReq->tHead.ulLen  = sizeof (ptReq->tData);
}

/*****************************************************************************/
/*! Assembles a HIL_FORMAT_REQ packet.
 *
 * \param ptPkt  [in]  Packet resource used for assembling.
 */
/*****************************************************************************/
static void App_SysPkt_AssembleFileFormatReq( CIFX_PACKET* ptPkt, bool fFormat )
{
  HIL_FORMAT_REQ_T* ptReq = (HIL_FORMAT_REQ_T*)ptPkt;
 
  memset( ptReq, 0, sizeof(HIL_FORMAT_REQ_T) );

  ptReq->tHead.ulDest = HIL_PACKET_DEST_SYSTEM;
  ptReq->tHead.ulCmd  = HIL_FORMAT_REQ;
  ptReq->tHead.ulLen  = sizeof (ptReq->tData);

  ptReq->tData.ulFlags = fFormat ? HIL_FORMAT_REQ_DATA_FLAGS_FULLFORMAT : HIL_FORMAT_REQ_DATA_FLAGS_QUICKFORMAT;
}

/*****************************************************************************/
/*! Assembles a HIL_DIR_LIST_REQ packet.
 *
 * \param ptPkt  [in]  Packet resource used for assembling.
 */
/*****************************************************************************/
static void App_SysPkt_FileSystem_ListReq( CIFX_PACKET* ptPkt, bool firstPkt, const char *ptStrDirPath)
{
  HIL_DIR_LIST_REQ_T * ptDiListReq;
#ifdef USE_DIRNAME
  char *ptCpyDst;
#endif
  uint16_t StrLenP1;
  uint16_t ExtByte;

  ptDiListReq = (HIL_DIR_LIST_REQ_T*)ptPkt;
  memset( ptDiListReq, 0, sizeof(HIL_DIR_LIST_REQ_T) );
  StrLenP1 = 0;

  if (firstPkt) {
#ifdef USE_DIRNAME
    PRINTF("->1st TxFileListReq. for '%s'\n", ptStrDirPath);
    StrLenP1 = strlen(ptStrDirPath) + 1;
    ptCpyDst = (char *) ((uint8_t *) ptPkt) + sizeof(HIL_DIR_LIST_REQ_T);  // point to 'EndOfPacket' Beginning of 'StringData'
    strcpy(ptCpyDst, ptStrDirPath);
#endif
    ExtByte = 0; 
  }
  else {
    ExtByte = 0xC0; 
  }
      
  ptDiListReq->tHead.ulDest = HIL_PACKET_DEST_SYSTEM;
  ptDiListReq->tHead.ulCmd  = HIL_DIR_LIST_REQ;
  ptDiListReq->tHead.ulLen  = sizeof(HIL_DIR_LIST_REQ_DATA_T) + StrLenP1; // Note: sizeof(HIL_DIR_LIST_REQ_DATA_T) is '6'
  ptDiListReq->tHead.ulExt  = ExtByte; 

  ptDiListReq->tData.usDirNameLength = StrLenP1;
//ptDiListReq->tData.ulChannelNo     = 0xFFFFFFFF;
  ptDiListReq->tData.ulChannelNo     = 1;

#ifdef ADD_EDDIE_CMD_DUMP
  (void) EU_MD_MemDump_U8("Tx.ListDirData", (void *) ptDiListReq, 0x40, 0);
  PRINTF("\n");
#endif
}

/*****************************************************************************/
/*! Assembles a HIL_FILE_UPLOAD_REQ packet.
 *
 * \param ptPkt  [in]  Packet resource used for setting file info.
 * \param ulId   [in]  Packet identifier.
 */
/*****************************************************************************/
static void App_SysPkt_FileReadReq( CIFX_PACKET* ptPkt, char* szFileName, uint32_t ulId )
{
  HIL_FILE_UPLOAD_REQ_T* ptFileUploadReq = (HIL_FILE_UPLOAD_REQ_T*)ptPkt;  
  char * pFileName = (char *) ((uint8_t *) ptPkt) + sizeof(HIL_FILE_UPLOAD_REQ_T);  // point to file name end of the HIL_FILE_UPLOAD_REQ_T
  
  memset( ptFileUploadReq, 0, sizeof(HIL_FILE_UPLOAD_REQ_T) );
  
  PRINTF("==>Setup FileRead.UploadReq FileName Size %d %s" NEWLINE, NVRAM_FILE_NAME_SIZE, szFileName);
  ptFileUploadReq->tHead.ulDest       = HIL_PACKET_DEST_SYSTEM;
  ptFileUploadReq->tHead.ulCmd        = HIL_FILE_UPLOAD_REQ;
  ptFileUploadReq->tHead.ulLen        = (sizeof(HIL_FILE_UPLOAD_REQ_DATA_T)) + NVRAM_FILE_NAME_SIZE;
  ptFileUploadReq->tHead.ulExt        = HIL_PACKET_SEQ_NONE;
  ptFileUploadReq->tHead.ulId         = ulId;

  ptFileUploadReq->tData.ulXferType       = HIL_FILE_XFER_FILE;
  ptFileUploadReq->tData.ulMaxBlockSize   = CIFX_MAX_DATA_SIZE - sizeof(HIL_FILE_UPLOAD_REQ_DATA_T);
  ptFileUploadReq->tData.ulChannelNo      = 0xFFFFFFFF;
  ptFileUploadReq->tData.usFileNameLength = NVRAM_FILE_NAME_SIZE;
  
  strcpy(pFileName, szFileName);  
}

/*****************************************************************************/
/*! Assembles a HIL_FILE_UPLOAD_DATA_REQ packet.
 *
 * \param ptPkt  [in]  Packet resource used for setting file info.
 * \param ulId   [in]  Packet identifier.
 */
/*****************************************************************************/
static void App_SysPkt_FileReadDataReq( CIFX_PACKET* ptPkt, uint32_t ulId)
{
  HIL_FILE_UPLOAD_DATA_REQ_T* ptFileUploadReq = (HIL_FILE_UPLOAD_DATA_REQ_T*)ptPkt;
  memset( ptFileUploadReq, 0, sizeof(HIL_FILE_UPLOAD_DATA_REQ_T) );

  ptFileUploadReq->tHead.ulDest       = HIL_PACKET_DEST_SYSTEM;
  ptFileUploadReq->tHead.ulCmd        = HIL_FILE_UPLOAD_DATA_REQ;
  ptFileUploadReq->tHead.ulDestId     = ulId + 1;
  ptFileUploadReq->tHead.ulExt        = HIL_PACKET_SEQ_NONE;
}

/*****************************************************************************/
/*! Assembles a HIL_FILE_UPLOAD_ABORT_REQ packet.
 *
 * \param ptPkt  [in]  Packet resource used for aborting file read request.
 */
/*****************************************************************************/
static void App_SysPkt_FileReadAbortReq( CIFX_PACKET* ptPkt, uint32_t ulId)
{
  HIL_FILE_UPLOAD_ABORT_REQ_T* ptFileUploadAbort = (HIL_FILE_UPLOAD_ABORT_REQ_T*)ptPkt;
  memset( ptFileUploadAbort, 0, sizeof(HIL_FILE_UPLOAD_ABORT_REQ_T) );

  ptFileUploadAbort->tHead.ulDest         = HIL_PACKET_DEST_SYSTEM;
  ptFileUploadAbort->tHead.ulCmd          = HIL_FILE_UPLOAD_ABORT_REQ;
  ptFileUploadAbort->tHead.ulId           = ulId;
}

/*****************************************************************************/
/*! Assembles a HIL_FILE_DOWNLOAD_REQ packet.
 *
 * \param ptPkt  [in]  Packet resource used for aborting file read request.
 * \param ulId   [in]  Packet identifier.
 */
/*****************************************************************************/
static void App_SysPkt_FileWriteReq( CIFX_PACKET* ptPkt, char* szFileName, uint32_t ulId, uint32_t ulSize)
{
  HIL_FILE_DOWNLOAD_REQ_T* ptFiDwldReq = (HIL_FILE_DOWNLOAD_REQ_T*)ptPkt;  
  
  char *ptCpyDst;

  memset( ptFiDwldReq, 0, sizeof(HIL_FILE_DOWNLOAD_REQ_T) );
  
  PRINTF("==>Setup FileWr.DownloadReq {Strk.Len:%02X} %s" NEWLINE,  (sizeof(HIL_FILE_DOWNLOAD_REQ_DATA_T)), szFileName);

  ptFiDwldReq->tHead.ulDest       = HIL_PACKET_DEST_SYSTEM;
  ptFiDwldReq->tHead.ulCmd        = HIL_FILE_DOWNLOAD_REQ ;
  ptFiDwldReq->tHead.ulLen        = (sizeof(HIL_FILE_DOWNLOAD_REQ_DATA_T)) + NVRAM_FILE_NAME_SIZE;
  ptFiDwldReq->tHead.ulExt        = HIL_PACKET_SEQ_NONE;
  ptFiDwldReq->tHead.ulId         = ulId;

  ptFiDwldReq->tData.ulXferType       = HIL_FILE_XFER_FILE;
  ptFiDwldReq->tData.ulMaxBlockSize   = CIFX_MAX_DATA_SIZE - sizeof(HIL_FILE_UPLOAD_REQ_DATA_T);
  ptFiDwldReq->tData.ulFileLength     = ulSize;
  ptFiDwldReq->tData.ulChannelNo      = 0xFFFFFFFF;
  ptFiDwldReq->tData.usFileNameLength = NVRAM_FILE_NAME_SIZE;

  /* file name */
  ptCpyDst = (char *) ((uint8_t *) ptPkt) + sizeof(HIL_FILE_DOWNLOAD_REQ_T);
  strcpy(ptCpyDst, szFileName);
}

/*****************************************************************************/
/*! Assembles a HIL_FILE_DOWNLOAD_DATA_REQ REQ packet.
 *
 * \param ptPkt  [in]  Packet resource used for aborting file read request.
 * \param ulId   [in]  Packet identifier.
 */
/*****************************************************************************/
static void App_SysPkt_FileWriteDataReq( CIFX_PACKET* ptPkt, uint32_t ulId, uint8_t* pbData, uint32_t ulSize)
{
  HIL_FILE_DOWNLOAD_DATA_REQ_T* ptFiDwDaReq = (HIL_FILE_DOWNLOAD_DATA_REQ_T*)ptPkt;                  // s.a. HIL_FILE_DOWNLOAD_DATA_REQ_DATA_T  
  char *ptCpyDst = (char *) ((uint8_t *) ptPkt) + sizeof(HIL_FILE_DOWNLOAD_DATA_REQ_T);  // point to 'EndOfPacket' Beginning of '(to be)WriteData'

  PRINTF("==>Setup FileWr_DwldDataReq [DatHdSz:%04X] ActWrLen:%04X" NEWLINE, sizeof(HIL_FILE_DOWNLOAD_DATA_REQ_DATA_T), (unsigned int)ulSize);
  
  memset( ptFiDwDaReq, 0, sizeof(HIL_FILE_DOWNLOAD_DATA_REQ_T) );  

  ptFiDwDaReq->tHead.ulDest     = HIL_PACKET_DEST_SYSTEM;
  ptFiDwDaReq->tHead.ulCmd      = HIL_FILE_DOWNLOAD_DATA_REQ; 
  ptFiDwDaReq->tHead.ulLen      = sizeof(HIL_FILE_DOWNLOAD_DATA_REQ_DATA_T) + ulSize;
  ptFiDwDaReq->tHead.ulExt      = HIL_PACKET_SEQ_NONE;
  ptFiDwDaReq->tHead.ulId       = ulId;

  ptFiDwDaReq->tData.ulBlockNo = INI_BLK_NUM;
  ptFiDwDaReq->tData.ulChksum  = CreateCRC32( 0, pbData, ulSize);
  
  memcpy(ptCpyDst, pbData, ulSize);
}

/*****************************************************************************/
static void App_SysPkt_FileDeleteReq( CIFX_PACKET* ptPkt, char* pbData, uint32_t ulId, uint32_t ulSize)
{
  HIL_FILE_DELETE_REQ_T* ptFileDelReq = (HIL_FILE_DELETE_REQ_T*)ptPkt;
  char *ptCpyDst = (char *) ((uint8_t *) ptPkt) + sizeof(HIL_FILE_DELETE_REQ_T);  // point to 'EndOfPacket' Beginning of 'file name'
  
  memset( ptFileDelReq, 0, sizeof(HIL_FILE_DELETE_REQ_T) );  

  ptFileDelReq->tHead.ulDest     = HIL_PACKET_DEST_SYSTEM;
  ptFileDelReq->tHead.ulCmd      = HIL_FILE_DELETE_REQ; 
  ptFileDelReq->tHead.ulLen      = sizeof(HIL_FILE_DELETE_REQ_DATA_T) + ulSize;  

  ptFileDelReq->tData.ulChannelNo = ulId;
  ptFileDelReq->tData.usFileNameLength  = (uint16_t) ulSize;
  
  memcpy(ptCpyDst, pbData, ulSize);
}

/*****************************************************************************/
/*! Assembles a HIL_FILE_DOWNLOAD_ABORT_REQ packet.
 *
 * \param ptPkt  [in]  Packet resource used for aborting file write request.
 */
/*****************************************************************************/
static void App_SysPkt_FileWriteAbortReq( CIFX_PACKET* ptPkt, uint32_t ulId)
{
  HIL_FILE_DOWNLOAD_ABORT_REQ_T* ptFileUploadAbort = (HIL_FILE_DOWNLOAD_ABORT_REQ_T*)ptPkt;
  memset( ptFileUploadAbort, 0, sizeof(HIL_FILE_DOWNLOAD_ABORT_REQ_T) );

  ptFileUploadAbort->tHead.ulDest         = HIL_PACKET_DEST_SYSTEM;
  ptFileUploadAbort->tHead.ulCmd          = HIL_FILE_DOWNLOAD_ABORT_REQ;
  ptFileUploadAbort->tHead.ulId           = ulId;
}

/*####################################################################################
 *  _   _ _____ _     ____  _____ ____  ____
 * | | | | ____| |   |  _ \| ____|  _ \/ ___|
 * | |_| |  _| | |   | |_) |  _| | |_) \___ \
 * |  _  | |___| |___|  __/| |___|  _ < ___) |
 * |_| |_|_____|_____|_|   |_____|_| \_\____/
 *
 #####################################################################################*/

/*****************************************************************************/
/*! Show file info.
 *
 * \param ptPkt  [in]  Packet resource used for retrieving file info.
 */
/*****************************************************************************/
static uint8_t App_SysPkt_FileSystem_ShowFileInfo( CIFX_PACKET* ptPkt )
{
  const char *ptStrFDType;
  uint8_t *ptStrDiFiName;
  HIL_DIR_LIST_CNF_T *ptLstCnf;
  HIL_DIR_LIST_CNF_DATA_T *ptLstDat;
  uint32_t SizeInfo;
  uint16_t FlagDoNot;
  uint8_t FlgContinue, ubExtByte;

  ptLstCnf = (HIL_DIR_LIST_CNF_T *) ptPkt;
#ifdef ADD_EDDIE_CMD_DUMP
  (void) EU_MD_MemDump_U8("RxListDirData", (void *) ptLstCnf, 0x40, 0);
  TIO_TX_PrStrHexStr(CMX_TIO_BF_NLEL|4, "Note: OfsData:", sizeof(HIL_PACKET_HEADER_T), 0);   // s.a. HIL_PACKET_HEADER_Ttag
#endif

  SizeInfo = 0;
  FlgContinue = 0;
  ubExtByte = (uint8_t) ptLstCnf->tHead.ulExt;
  FlagDoNot = ptLstCnf->tHead.ulSta;
  PRINTF("Packet fragment Ext:%02X" NEWLINE, ubExtByte);
  
  if (!FlagDoNot) 
  {                             
    if (ubExtByte == HIL_PACKET_SEQ_LAST) 
    {
      FlagDoNot = 1; 
      PRINTF(" {No More Files}" NEWLINE);
    }
  }

  if (!FlagDoNot) 
  {                             // if 'no error'
    if ((ubExtByte == HIL_PACKET_SEQ_MIDDLE) || (ubExtByte == HIL_PACKET_SEQ_FIRST)) {     // if 'continue-codes'
      FlgContinue = 1;                          // -> there will be more
    }
    ptLstDat = (HIL_DIR_LIST_CNF_DATA_T *) &ptLstCnf->tData; // pDat;
    ptStrDiFiName = 0;
    if (ptLstDat->bFileType == HIL_DIR_LIST_CNF_FILE_TYPE_FILE)
    {
      ptStrFDType =  "FILE";
      ptStrDiFiName = ptLstDat->szName;
      SizeInfo      = ptLstDat->ulFileSize;
    }
    else if (ptLstDat->bFileType == HIL_DIR_LIST_CNF_FILE_TYPE_DIRECTORY)
    {
      ptStrFDType =  "DIR";
      ptStrDiFiName = ptLstDat->szName;
    }
    else
    {
      ptStrFDType = "Not Available";
    }

    PRINTF(NEWLINE "=> %s", ptStrFDType);
    if (ptStrDiFiName) {
      PRINTF(NEWLINE "====>[FileSz:%04X] '%s' <====", (unsigned int)SizeInfo, ptStrDiFiName); }
    PRINTF(NEWLINE);
  }

  return FlgContinue;
}

/*****************************************************************************/
/*! Show file content.
 *
 * \param ptPkt  [in]  Packet resource used for retrieving file info.
 */
/*****************************************************************************/
static void App_SysPkt_PrintReadFileData(CIFX_PACKET * ptCifPack)
{
  HIL_FILE_UPLOAD_DATA_CNF_T *pCnf;
  uint32_t DataSz;

  pCnf = (HIL_FILE_UPLOAD_DATA_CNF_T *) ptCifPack;
  DataSz  =  pCnf->tHead.ulLen;
  DataSz -= sizeof(HIL_FILE_UPLOAD_DATA_CNF_DATA_T);                  // sub header-size
  PRINTF("Rd.FileSize:0x%08lX Crc:0x%08lX" NEWLINE, DataSz, pCnf->tData.ulChksum );
}

/*####################################################################################
*  ____  _   _ ____  _     ___ ____ 
* |  _ \| | | | __ )| |   |_ _/ ___|
* | |_) | | | |  _ \| |    | | |    
* |  __/| |_| | |_) | |___ | | |___ 
* |_|    \___/|____/|_____|___\____|
 *
 #####################################################################################*/

/****************************************************************************/
/*! Verify that file system is formatted.
 *
 * \param iChannelIdx  [in]  Channel index.
 */
/*****************************************************************************/
uint32_t App_SysPkt_IsFormatted(int iChannelIdx)
{                     
  int32_t lRet;

  App_SysPkt_AssembleVolumeGetEntryReq(&g_tAppData.aptChannels[iChannelIdx]->tPacket);

  if (CIFX_NO_ERROR == (lRet = Pkt_SendReceivePacket(&g_tAppData, iChannelIdx, &g_tAppData.aptChannels[iChannelIdx]->tPacket, TXRX_TIMEOUT)))
  {
    lRet = g_tAppData.aptChannels[iChannelIdx]->tPacket.tHeader.ulState;

    if (CIFX_NO_ERROR != lRet)
      PRINTF ("ERROR: Get Volume Entry packet failed: 0x%0X" NEWLINE, (unsigned int) lRet);
  }

  return lRet;
}
               
/****************************************************************************/
/*! A flash file system init.
 *
 * \param iChannelIdx  [in]  Channel index.
 */
/*****************************************************************************/
uint32_t App_SysPkt_FileSystemInit(int iChannelIdx, bool fFormat)
{
  uint32_t ulRet;

  App_SysPkt_AssembleFileFormatReq(&g_tAppData.aptChannels[iChannelIdx]->tPacket, fFormat);
  PRINTF ("Formatting external memory file system...  Please wait until finished, it might take upto 2 minutes.  The system will restart automatically." NEWLINE);

  if (CIFX_NO_ERROR == (ulRet = Pkt_SendReceivePacket(&g_tAppData, iChannelIdx, &g_tAppData.aptChannels[iChannelIdx]->tPacket, LONG_FS_TXRX_TIMEOUT)))
  {
    ulRet = g_tAppData.aptChannels[iChannelIdx]->tPacket.tHeader.ulState;
  }

  return ulRet;
}

/****************************************************************************/
/*! A flash file system file list.
 *
 * \param iChannelIdx  [in]  Channel index.
 */
/*****************************************************************************/
uint32_t App_SysPkt_FileSystemList(int iChannelIdx)
{
  uint32_t ulRet;
  uint32_t ulFlgFirstTime, ulFlgContinue;
      
  // Send first packet
  ulFlgFirstTime = 1;
  ulFlgContinue  = 1;
  do 
  {
    App_SysPkt_FileSystem_ListReq(&g_tAppData.aptChannels[iChannelIdx]->tPacket, ulFlgFirstTime, "PORT_1");
    ulRet = Pkt_SendReceivePacket(&g_tAppData, 0, &g_tAppData.aptChannels[iChannelIdx]->tPacket, TXRX_TIMEOUT);
    ulFlgFirstTime = 0;
    ulFlgContinue = App_SysPkt_FileSystem_ShowFileInfo(&g_tAppData.aptChannels[iChannelIdx]->tPacket);
  } 
  while (ulFlgContinue);

  return ulRet;
}

/****************************************************************************/
/*! A flash file system file read.
 *
 * \param iChannelIdx  [in]  Channel index.
 */
/*****************************************************************************/
uint32_t App_SysPkt_FileSystemRead(int iChannelIdx, uint8_t* pbData, uint32_t* pulDataSize)
{
  CIFX_PACKET* ptPkt = &g_tAppData.aptChannels[iChannelIdx]->tPacket;
  uint32_t ulRet = CIFX_NO_ERROR;

  *pulDataSize = 0;

  /* setup file name for a read request */
  App_SysPkt_FileReadReq(ptPkt, NVRAM_FILE_NAME, NVRAM_ID);
 
  if (CIFX_NO_ERROR != (ulRet = Pkt_SendReceivePacket(&g_tAppData, 0, ptPkt, TXRX_TIMEOUT)))
  {
    //PRINTF("RetA:%08X: Stat:%08X}", (unsigned int) ulRsltA, (unsigned int) ulRslt);
    return ulRet;
  }

  if (CIFX_NO_ERROR == ptPkt->tHeader.ulState)
  {
    PRINTF("->Now SetReq for Reading File Data" NEWLINE);
    App_SysPkt_FileReadDataReq(ptPkt, NVRAM_ID);
  }
  else
  {
    ulRet = ptPkt->tHeader.ulState;
    PRINTF("Read File Name Error 0x%08lX" NEWLINE, ulRet);
    PRINTF("->Now SetReq for Abort}" NEWLINE);
    App_SysPkt_FileReadAbortReq(ptPkt, NVRAM_ID);    
  }
  
  // process Request File data read or abort
  if (CIFX_NO_ERROR != (ulRet = Pkt_SendReceivePacket(&g_tAppData, 0, ptPkt, TXRX_TIMEOUT)))
  {
    PRINTF("File request Error:%08lX", ulRet);
    return ulRet;
  }  
  
  if (CIFX_NO_ERROR == ptPkt->tHeader.ulState)
  {    
    /* save the read data size */
    if (ptPkt->tHeader.ulLen)
    {
      *pulDataSize = ptPkt->tHeader.ulLen - sizeof (HIL_FILE_UPLOAD_DATA_CNF_DATA_T);
      App_SysPkt_PrintReadFileData(ptPkt);
      /* copy NVRAM data in the local memory */
      memcpy (pbData, ((uint8_t*) ptPkt->abData) + sizeof (HIL_FILE_UPLOAD_DATA_CNF_DATA_T), *pulDataSize);
    }
  }
  else
  {
    ulRet = ptPkt->tHeader.ulState;
    PRINTF("Read File Data Error 0x%08lX" NEWLINE, ulRet);
  }

  return ulRet;
}

/****************************************************************************/
/*! A flash file system file write.
 *
 * \param iChannelIdx  [in]  Channel index.
 */
/*****************************************************************************/
uint32_t App_SysPkt_FileSystemWrite(int iChannelIdx, uint8_t* pbData, uint32_t ulDataSize)
{
  CIFX_PACKET* ptPkt = &g_tAppData.aptChannels[iChannelIdx]->tPacket;
  uint32_t ulRet = CIFX_NO_ERROR;  

  /* setup file name for a write request */
  App_SysPkt_FileWriteReq(ptPkt, NVRAM_FILE_NAME, NVRAM_ID, ulDataSize); //todo test 4 bytes write

  if (CIFX_NO_ERROR != (ulRet = Pkt_SendReceivePacket(&g_tAppData, 0, ptPkt, WRITE_FILE_TIMEOUT)))
  {
    //PRINTF("RetA:%08X: Stat:%08X}", (unsigned int) ulRsltA, (unsigned int) ulRslt);
    return ulRet;
  }

  if (CIFX_NO_ERROR == ptPkt->tHeader.ulState)
  {
    PRINTF(NEWLINE"->Now SetReq for Writing Data (Download)" NEWLINE);
    App_SysPkt_FileWriteDataReq(ptPkt, NVRAM_ID, pbData, ulDataSize);
  }
  else
  {
    ulRet  = ptPkt->tHeader.ulState;
    PRINTF("Write File Name Error 0x%08lX" NEWLINE, ulRet);
    PRINTF("->Now SetReq for Abort}" NEWLINE);
    App_SysPkt_FileWriteAbortReq(ptPkt, NVRAM_ID);    
  }
    
  if (CIFX_NO_ERROR != (ulRet = Pkt_SendReceivePacket(&g_tAppData, 0, ptPkt, WRITE_FILE_TIMEOUT)))
  {
    ulRet  = ptPkt->tHeader.ulState;
    PRINTF("Write File Data Error 0x%08lX" NEWLINE, ulRet);
  }

  return ulRet;
}
 

/****************************************************************************/
/*! A flash file system file delete.
 *
 * \param iChannelIdx  [in]  Channel index.
 */
/*****************************************************************************/
uint32_t App_SysPkt_FileSystemDelete(int iChannelIdx)
{
  CIFX_PACKET* ptPkt = &g_tAppData.aptChannels[iChannelIdx]->tPacket;
  uint32_t ulRet = CIFX_NO_ERROR;

  /* setup file name for a write request */
  App_SysPkt_FileDeleteReq(ptPkt, NVRAM_FILE_NAME, NVRAM_ID, 17);

  if (CIFX_NO_ERROR != (ulRet = Pkt_SendReceivePacket(&g_tAppData, 0, ptPkt, WRITE_FILE_TIMEOUT)))
  {
    PRINTF("Delete File failed: Error %08X}" NEWLINE, (unsigned int) ulRet);
    return ulRet;
  }

  if (CIFX_NO_ERROR != ptPkt->tHeader.ulState)
  {
    ulRet  = ptPkt->tHeader.ulState;
    PRINTF("Delete File state: %08lX" NEWLINE, ptPkt->tHeader.ulState);
  }
  else
  {
    PRINTF("Delete File OK" NEWLINE);
  }

  return ulRet;
}
