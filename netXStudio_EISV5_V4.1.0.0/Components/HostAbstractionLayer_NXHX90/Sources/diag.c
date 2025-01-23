#include "diag.h"
#include "OS_Includes.h"

#ifdef DIAG_MODE

/* keep in sync with DIAG_TYPE */
static DIAG_DATA_T s_tDiag[] = {
  {"SPI Input Locked  ", "%10u", 0},
  {"SPI Input Busy    ", "%10u", 0},
  {"SPI Input Buffer  ", "%10u", 0},
  {"SPI Input Error   ", "%10u", 0},
  {"SPI Input CS Err  ", "%10u", 0},
  {"SPI Input OK      ", "%10u", 0},

  {"SPI Output Locked ", "%10u", 0},
  {"SPI Output Busy   ", "%10u", 0},
  {"SPI Output Buffer ", "%10u", 0},
  {"SPI Output Error  ", "%10u", 0},
  {"SPI Output CS Err ", "%10u", 0},
  {"SPI Output OK     ", "%10u", 0},

  {"Read Err Cnt      ", "%10u", 0},
  {"Read Sync Err     ", "%10u", 0},
  {"Read Last Err     ", "0x%08X", 0},
  {"Read Max Sync Cnt ", "%10u", 0},
  //{"IO Read Max us    ", "%10u", 0},
  {"IO Read Force HS  ", "%10u", 0},
  {"IO Read OK        ", "%10u", 0},

  {"Write Err Cnt     ", "%10u", 0},
  {"Write Sync Err    ", "%10u", 0},
  {"Write Last Err    ", "0x%08X", 0},
  {"Write Max Sync Cnt", "%10u", 0},
  //{"IO Write Max us   ", "%10u", 0},
  {"IO Write Force HS ", "%10u", 0},
  {"IO Write OK       ", "%10u", 0},

  {"WD Failures       ", "%10u", 0},
  {"WD Resumes        ", "%10u", 0},
  
  {"Packet Handler  1000 ", "%10u", 0},
  {"Packet Handler  2000 ", "%10u", 0},
  {"Packet Handler  4000 ", "%10u", 0},
  {"Packet Handler  8000 ", "%10u", 0},
  {"Packet Handler 16000 ", "%10u", 0},  
  {"Packet Max        ", "%10u", 0},

  {"Terminal Max      ", "%10u", 0},
  {"Event Max         ", "%10u", 0},

  {"IO Handler 15     ", "%10u", 0},
  {"IO Handler 30     ", "%10u", 0},
  {"IO Handler 60     ", "%10u", 0},
  {"IO Handler 120    ", "%10u", 0},
  {"IO Handler 240    ", "%10u", 0},  
  {"IO Handler Max    ", "%10u", 0},

  {"IO Read HSHK Err  ", "%10u", 0},
  {"IO Write HSHK Err ", "%10u", 0},
  
  };


void Diag_Inc(uint32_t ulDiagIdx)
{
  if (ulDiagIdx < MAX_DIAG_INDEX)
  {
    s_tDiag[ulDiagIdx].ulDiagValue++;
  }
}

void Diag_Latch(uint32_t ulDiagIdx, uint32_t ulValue)
{
  if (ulDiagIdx < MAX_DIAG_INDEX)
  {
    s_tDiag[ulDiagIdx].ulDiagValue = ulValue;
  }
}

void Diag_Max(uint32_t ulDiagIdx, uint32_t ulValue)
{
  if (ulDiagIdx < MAX_DIAG_INDEX)
  {
    s_tDiag[ulDiagIdx].ulDiagValue = HIL_MAX(ulValue, s_tDiag[ulDiagIdx].ulDiagValue);
  }  
}

uint32_t Diag_Hist(uint32_t ulDiagIdx, uint32_t ulValue, uint32_t ulStart)
{  
  
  if (ulValue < ulStart)
    s_tDiag[ulDiagIdx].ulDiagValue++;
  else if (ulValue < ulStart * 2)
    s_tDiag[ulDiagIdx + 1].ulDiagValue++;
  else if (ulValue < ulStart * 4)
    s_tDiag[ulDiagIdx + 2].ulDiagValue++;
  else if (ulValue < ulStart * 8)
    s_tDiag[ulDiagIdx + 3].ulDiagValue++;
  else if (ulValue < ulStart * 16)
    s_tDiag[ulDiagIdx + 4].ulDiagValue++;
  
  return 0;
}

uint32_t Show_Diag (uint32_t argc, const char * argv[])
{  
  uint32_t i;  

  PRINTF("========================================" NEWLINE);
  PRINTF("---------- MKS FI Diagnostics ----------" NEWLINE);  

  for (i = 0; i < MAX_DIAG_INDEX; i++)
  {    
    PRINTF("%s:", s_tDiag[i].sName);
    PRINTF(s_tDiag[i].sFormat, s_tDiag[i].ulDiagValue);
    PRINTF(NEWLINE);
  }

  return 0;
}

uint32_t Clear_Diag (uint32_t argc, const char * argv[])
{  
  uint32_t i;  
  
  for (i = 0; i < MAX_DIAG_INDEX; i++)
  {
    s_tDiag[i].ulDiagValue = 0;
  }
  
  Show_Diag(argc, argv);

  return 0;
}

int32_t DIAG_Error(const char* szError, DRV_TYPE eType, uint32_t lError, const char* szFile, int nLine)
{
  PRINTF("%s 0x%08X File: %s: Line: %u" NEWLINE, szError, (unsigned int) lError, szFile, nLine);
  
  return CIFX_NO_ERROR;
}


#endif
