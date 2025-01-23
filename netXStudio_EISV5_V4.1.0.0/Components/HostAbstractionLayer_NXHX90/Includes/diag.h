#ifndef DRV_DIAG_H_
#define DRV_DIAG_H_

#include "App_Application.h"
#include "App_PacketCommunication.h"

/* keep in sync with s_tDiag */
typedef enum
{
  DIAG_SPI1_LOCKED = 0,
  DIAG_SPI1_BUSY,
  DIAG_SPI1_BUFFER,
  DIAG_SPI1_ERROR,
  DIAG_SPI1_CS_ERR,
  DIAG_SPI1_OK,
  
  DIAG_SPI2_LOCKED,
  DIAG_SPI2_BUSY,
  DIAG_SPI2_BUFFER,
  DIAG_SPI2_ERROR,
  DIAG_SPI2_CS_ERR,
  DIAG_SPI2_OK,

  DIAG_IO_READ_ERROR,
  DIAG_IO_READ_SYNC_ERR,
  DIAG_IO_READ_ERR_VAL,
  DIAG_IO_READ_ERR_RETRY,
  //DIAG_IO_READ_MAX,
  DIAG_IO_READ_FORCE_HS,
  DIAG_IO_READ_OK,

  DIAG_IO_WRITE_ERROR,
  DIAG_IO_WRITE_SYNC_ERR,
  DIAG_IO_WRITE_ERR_VAL,
  DIAG_IO_WRITE_ERR_RETRY,
  //DIAG_IO_WRITE_MAX,
  DIAG_IO_WRITE_FORCE_HS,
  DIAG_IO_WRITE_OK,

  DIAG_WD_FAIL,
  DIAG_WD_RESUME,
  
  DIAG_PROF_HIST_PACKET_1,
  DIAG_PROF_HIST_PACKET_2,
  DIAG_PROF_HIST_PACKET_3,
  DIAG_PROF_HIST_PACKET_4,
  DIAG_PROF_HIST_PACKET_5,    
  DIAG_PROF_PACKET,

  DIAG_PROF_TERMINAL,
  DIAG_PROF_EVENT,

  DIAG_PROF_HIST_IO_1,
  DIAG_PROF_HIST_IO_2,
  DIAG_PROF_HIST_IO_3,
  DIAG_PROF_HIST_IO_4,
  DIAG_PROF_HIST_IO_5,  
  DIAG_PROF_IO_TIMER,

  DIAG_IO_READ_HSK_ERR,
  DIAG_IO_WRITE_HSK_ERR,
  
  MAX_DIAG_INDEX,
}DIAG_TYPE;

#define DIAG_HIST_IO_START  15
#define DIAG_HIST_PACKET_START  1000

typedef enum
{
  DRV_SPI = 0,
}DRV_TYPE;

/*************************************************************************************/
typedef struct DIAG_DATA_tag
{
  char* sName;
  char* sFormat;
  uint32_t ulDiagValue;
} DIAG_DATA_T;

#define DIAG_MODE

#ifdef DIAG_MODE
#define DIAG_ERR(a,b) DIAG_Error(a, b, __FILE__, __LINE__)
int32_t DIAG_Error(const char* szError, DRV_TYPE eType, uint32_t lError, const char* szFile, int nLine);
void Diag_Inc(uint32_t ulDiagIdx);
void Diag_Latch(uint32_t ulDiagIdx, uint32_t ulValue);
void Diag_Max(uint32_t ulDiagIdx, uint32_t ulValue);
uint32_t Diag_Hist(uint32_t ulDiagIdx, uint32_t ulValue, uint32_t ulStart);
#else
#define DIAG_ERR(...)
#define DIAG_Error(...)
#define Diag_Inc(...)
#define Diag_Latch(...)
#define Diag_Max(...)
#endif // DIAG_MODE

#endif // DRV_DIAG_H_
