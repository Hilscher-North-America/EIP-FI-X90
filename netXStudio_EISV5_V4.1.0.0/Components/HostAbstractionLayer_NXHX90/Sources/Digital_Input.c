#include <stdio.h>
#include <stdlib.h>

#include "app_application.h"
#include "Digital_Input.h"
#include "hostAbstractionLayer.h"
#include "netx_drv.h"
#include "hal.h"
#include "mks_syscounter.h"
#include "diag.h"

#define   HOSTAL_DEVICE_INPUT_REGISTER_COUNT  64

#define   HOSTAL_DEVICE_RX_BYTE_SIZE          HOSTAL_DEVICE_INPUT_REGISTER_COUNT/8  /* data size sent to PLC */

/* Digital input bits populated, ignore DI 64, 63, 62, 23, 21, 20, 17 */
/* clear bits 63 62 61 22 20 19 16 */
#define DIGITAL_INPUT_MASK 0x1FFFFFFFFFA6FFFFll

// TODO start with true
//bool g_DiAppControl = true;
// TODO start with false - TESTING
bool g_DiAppControl = false;

/*****************************************************************************/
/*! Private variables                                                        */
/*****************************************************************************/
static DRV_SPI_HANDLE_T s_tSPI_InHandle = { { 0 } };   /*! Initializing the SPI Input */

/*****************************************************************************/
/*! Initializes SPI 1 interface                                              */
/* \return kMksOk if initialization succeeds, error code otherwise.          */
/*****************************************************************************/
HOSTAL_RESULT_E SPI_DiInit(void)
{
  DRV_STATUS_E eRet=0;
  uint8_t nCount = 0;

  DIAG_Error("SPI Digital Input Init starts", DRV_SPI, eRet, __FILE__, __LINE__);

  /*! configure the SPI_SPI1 as master */
  /* SPI for inputs */
  do
  {
    if (DRV_SPI_DEVICE_ID_SPI1 == s_tSPI_InHandle.tConfiguration.eSPIDeviceID )
    {
      /*! Disables the DRV_SPI_HANDLE_T */
      DRV_SPI_DeInit(&s_tSPI_InHandle);
    }

    /** configure the SPI peripheral */
    s_tSPI_InHandle.tConfiguration.eSPIDeviceID            = DRV_SPI_DEVICE_ID_SPI1;                       /** spi device ID */
    s_tSPI_InHandle.tConfiguration.eOperationMode          = DRV_OPERATION_MODE_POLL;                      /** Poll/blocking mode */
    s_tSPI_InHandle.tConfiguration.eParallelism            = DRV_SPI_PARALLELISM_1BIT;                     /** parallelism mode */
    s_tSPI_InHandle.tConfiguration.eDuplex                 = DRV_SPI_DUPLEX_FULL;                          /** duplex mode */
    s_tSPI_InHandle.tConfiguration.eFSS                    = DRV_SPI_FSS_NONE;                             /** slave select (FSS) signal pin mask control manually the CS pin */
    s_tSPI_InHandle.tConfiguration.eFSSStatic              = DRV_SPI_FSS_STATIC_CALLER;                    /** FSS is driver controlled */
    s_tSPI_InHandle.tConfiguration.eFrequency              = DRV_SPI_FREQUENCY_10MHz;                      /** spi frequencies */
    s_tSPI_InHandle.tConfiguration.uMode.value             = DRV_SPI_MODE_2;                               /** sampling mode */
    s_tSPI_InHandle.tConfiguration.eBehaviour              = DRV_SPI_BEHAVIOUR_MASTER;                     /** master/slave mode */
    s_tSPI_InHandle.tConfiguration.eSlaveSigEarly          = DRV_SPI_SLV_SIG_EARLY;                        /** early response bit generation */
    s_tSPI_InHandle.tConfiguration.eFILTER                 = DRV_SPI_FILTER_INACTIVE;                        /** filter state */
    s_tSPI_InHandle.tConfiguration.eEndianess              = DRV_SPI_ENDIANESS_LITTLE;                     /** little/big Endianess */
    s_tSPI_InHandle.tConfiguration.eRxFiFoWm               = DRV_SPI_FIFO_WM_DEFAULT;                      /** Receive FiFo watermark */
    s_tSPI_InHandle.tConfiguration.eTxFiFoWm               = DRV_SPI_FIFO_WM_DEFAULT;                      /** Transmit FiFo watermark */
    s_tSPI_InHandle.tConfiguration.eDataSize               = DRV_SPI_DATA_SIZE_SELECT_8b;                  /** data size of 8bit transmitted */
   
  } while ( DRV_OK != (eRet = DRV_SPI_Init(&s_tSPI_InHandle)) && nCount++ < 3); /* driver could be locked */

  //check for errors
  if(DRV_OK != eRet) {
    DIAG_Error("SPI Digital Input Init failed!", DRV_SPI, eRet, __FILE__, __LINE__);
    return HOSTAL_ERROR;
  }

  if (DRV_OK != (eRet = DRV_SPI_ChangeFss(&s_tSPI_InHandle, DRV_SPI_FSS_NONE)))
  {    
    DIAG_Error("SPI Digital Input Chips Select failed!", DRV_SPI, eRet, __FILE__, __LINE__);
  }    

  return HOSTAL_OK;
}

/**************************************************************************************/
/*! Transmit data from SPI Slave -> PLC
 *
 *  \param  pRxSpiData            SPI Master Input Data is SPI Slave Output
 *
 *  \return HOSTAL_OK on success                                                      */
/**************************************************************************************/
HOSTAL_RESULT_E SPI_StartInputDataTransmit(uint8_t* pRxSpiData)
{
  DRV_STATUS_E eRet = DRV_OK;

  uint8_t spiRxdata[HOSTAL_DEVICE_RX_BYTE_SIZE]={0};
  uint8_t i;


  /* enable chip select DI MMIO15 */
  if (DRV_OK != (eRet = DRV_SPI_ChangeFss(&s_tSPI_InHandle, DRV_SPI_FSS_1)))  
  {
    //DIAG_Error("SPI Digital Input Chips Select enable failed!", DRV_SPI, eRet, __FILE__, __LINE__);
    Diag_Inc(DIAG_SPI1_CS_ERR);
    return eRet;
  }

  /*! Receive data over SPI */
  // Data coming from SPI Slave to PLC Digital Input-----------+
  //                                                              |
  //                                                              V
  while(DRV_OK != (eRet = DRV_SPI_Receive(&s_tSPI_InHandle, spiRxdata, HOSTAL_DEVICE_RX_BYTE_SIZE)))
  {
    if (DRV_LOCKED == eRet)
    {
      Diag_Inc(DIAG_SPI1_LOCKED);
      continue;
    }
    else if (DRV_BUSY == eRet)
    {
      Diag_Inc(DIAG_SPI1_BUSY);
      continue;
    }
    else if (DRV_ERROR_BUFFER == eRet)
    {
      Diag_Inc(DIAG_SPI1_BUFFER);
      
      DRV_SPI_Abort(&s_tSPI_InHandle);
      DRV_SPI_DeInit(&s_tSPI_InHandle);
      SPI_DiInit();

      continue;
    }
    else
    {
      Diag_Inc(DIAG_SPI1_ERROR);
    }
  }

  /* disable chip select */
  if (DRV_OK != (eRet = DRV_SPI_ChangeFss(&s_tSPI_InHandle, DRV_SPI_FSS_NONE)))  
  {
    //DIAG_Error("SPI Digital Input Chips Select disable failed!", DRV_SPI, eRet, __FILE__, __LINE__);
    Diag_Inc(DIAG_SPI1_CS_ERR);
    return eRet;
  }

  Diag_Inc(DIAG_SPI1_OK);

  for (i=0;i<HOSTAL_DEVICE_RX_BYTE_SIZE;i++)
  {
    *(pRxSpiData + (HOSTAL_DEVICE_RX_BYTE_SIZE-i-1)) = ((spiRxdata[i])^0xff);
  }

  return eRet;
}

/*****************************************************************************/
/*! Shell Read a single digital input                                        */
/* \return HOSTAL_OK if parameter is valid (1-64)                            */
/*****************************************************************************/
uint32_t Shell_ReadSingleDI(uint32_t argc, const char *argv[])
{
  uint8_t uDiData[HOSTAL_DEVICE_RX_BYTE_SIZE];

  if(2 != argc){
    printf("\r\n Usage : %s <ch> \r\n", argv[0]);
    return 0;
  }

  if( g_DiAppControl ){
    printf("\r\n Nope. APP has control!\r\n");
    return 0;
  }

  int channel = atoi( argv[1] );
  if( ( channel < 0) || (channel > (HOSTAL_DEVICE_INPUT_REGISTER_COUNT) ) ){
    printf("\r\n Invalid Digital Input: %d (Range: 1-64)", channel );
    return 0;
  }

  if (HOSTAL_OK != SPI_StartInputDataTransmit(&uDiData[0]))
  {
    printf("\r\n SPI Digital Input transfer failed");
    return HOSTAL_ERROR;
  }

  /* retrieve the Digital Input */
  channel -= 1; /* switch to bit number */
  uint8_t byte = channel/8;
  uint8_t bit = channel%8;

  printf("\r\n Digital Input Value %d. \r\n", (uDiData[byte]>>bit)&0x01);  

  return HOSTAL_OK;
}

/*****************************************************************************/
/*! Shell Read all digital inputs                                            */
/* \return HOSTAL_OK                                                         */
/*****************************************************************************/
uint32_t Shell_ReadAllDI(uint32_t argc, const char *argv[])
{
  uint8_t uDiData[HOSTAL_DEVICE_RX_BYTE_SIZE];
  uint8_t i;

  if( g_DiAppControl ){
    printf("\r\n Nope. APP has control!\r\n");
    return 0;
  }

  if (HOSTAL_OK != SPI_StartInputDataTransmit(&uDiData[0]))
  {
    printf("\r\n SPI Digital Input transfer failed");
    return HOSTAL_ERROR;
  }

  printf("\r\n Digital Inputs: ");  
  for (i = 0; i < HOSTAL_DEVICE_RX_BYTE_SIZE; i++)
  {
    printf("%02X ", uDiData[i]);
  }
  printf("\r\n");

  return HOSTAL_OK;
}

/***************************************************************************************************
*! \brief  Shell command to (re)assign control of DI update
*
*  \ details  1 argument :  0 = shell, 1 = app
**************************************************************************************************/
uint32_t Shell_DiAppControl( uint32_t argc, const char *argv[] )
{
  if( argc > 1 ){

     int argval = atoi(argv[1]);//0 as err is ok here

      if ( argval ){
         printf("\r\n Returning DI control to APP \r\n");
         g_DiAppControl = true;
      } else {
        printf("\r\n Acquired DI control for shell only! \r\n");
        g_DiAppControl = false;
      }
  } else {
    printf("\r\n %s is in control AI. \r\n", g_DiAppControl ? "APP" : "Shell");
  }

  return 0;
}
