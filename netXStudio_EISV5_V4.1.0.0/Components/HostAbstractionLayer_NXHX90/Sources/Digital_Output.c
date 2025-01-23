#include <stdio.h>
#include <stdlib.h>

#include "app_application.h"
#include "hostAbstractionLayer.h"
#include "netx_drv.h"
#include "hal.h"
#include "mks_syscounter.h"
#include "diag.h"

// TODO start with true
bool g_DoAppControl = true;

/*****************************************************************************/
/*! Private variables                                                        */
/*****************************************************************************/
static DRV_SPI_HANDLE_T s_tSPI_OutHandle = { { 0 } };  /*! Initializing the SPI Output */

/*****************************************************************************/
/*! Initializes SPI 1 interface                                              */
/* \return kMksOk if initialization succeeds, error code otherwise.          */
/*****************************************************************************/
HOSTAL_RESULT_E SPI_DoInit(void)
{
  DRV_STATUS_E eRet=0;
  uint8_t nCount = 0;
  uint8_t i;

  DIAG_Error("SPI Digital Output Init starts", DRV_SPI, eRet, __FILE__, __LINE__);

  /* SPI for outputs */
  do
  {
    if (DRV_SPI_DEVICE_ID_SPI1 == s_tSPI_OutHandle.tConfiguration.eSPIDeviceID )
    {
      /*! Disables the DRV_SPI_HANDLE_T */
      DRV_SPI_DeInit(&s_tSPI_OutHandle);
    }

    /** configure the SPI peripheral */
    s_tSPI_OutHandle.tConfiguration.eSPIDeviceID            = DRV_SPI_DEVICE_ID_SPI1;                        /** spi device ID */
    s_tSPI_OutHandle.tConfiguration.eMISO                   = DRV_SPI_MISO_INACTIVE;                          /** miso state */
    s_tSPI_OutHandle.tConfiguration.eOperationMode          = DRV_OPERATION_MODE_POLL;                      /** Poll/blocking mode */
    s_tSPI_OutHandle.tConfiguration.eParallelism            = DRV_SPI_PARALLELISM_1BIT;                     /** parallelism mode */
    s_tSPI_OutHandle.tConfiguration.eDuplex                 = DRV_SPI_DUPLEX_FULL;                          /** duplex mode */
    s_tSPI_OutHandle.tConfiguration.eFSS                    = DRV_SPI_FSS_NONE;                             /** slave select (FSS) signal pin mask control manually the CS pin */
    s_tSPI_OutHandle.tConfiguration.eFSSStatic              = DRV_SPI_FSS_STATIC_CALLER;                    /** FSS is driver controlled */
    s_tSPI_OutHandle.tConfiguration.eFrequency              = DRV_SPI_FREQUENCY_10MHz;                      /** spi frequencies */
    s_tSPI_OutHandle.tConfiguration.uMode.value             = DRV_SPI_MODE_3;                               /** sampling mode */
    s_tSPI_OutHandle.tConfiguration.eBehaviour              = DRV_SPI_BEHAVIOUR_MASTER;                     /** master/slave mode */
    s_tSPI_OutHandle.tConfiguration.eSlaveSigEarly          = DRV_SPI_SLV_SIG_EARLY;                        /** early response bit generation */
    s_tSPI_OutHandle.tConfiguration.eFILTER                 = DRV_SPI_FILTER_INACTIVE;                        /** filter state */
    s_tSPI_OutHandle.tConfiguration.eEndianess              = DRV_SPI_ENDIANESS_LITTLE;                     /** little/big Endianess */
    s_tSPI_OutHandle.tConfiguration.eRxFiFoWm               = DRV_SPI_FIFO_WM_DEFAULT;                      /** Receive FiFo watermark */
    s_tSPI_OutHandle.tConfiguration.eTxFiFoWm               = DRV_SPI_FIFO_WM_DEFAULT;                      /** Transmit FiFo watermark */
    s_tSPI_OutHandle.tConfiguration.eDataSize               = DRV_SPI_DATA_SIZE_SELECT_8b;                  /** data size of 8bit transmitted */
    
  } while ( DRV_OK != (eRet = DRV_SPI_Init(&s_tSPI_OutHandle)) && nCount++ < 3); /* driver could be locked */

  //check for errors
  if(DRV_OK != eRet) {
    DIAG_Error("SPI Digital Output Init failed!", DRV_SPI, eRet, __FILE__, __LINE__);
    return HOSTAL_ERROR;
  }
  //dummy SPI transaction for IC02120 - tEN=3 Cycles (U76)
  //Link for Datasheet: https://www.renesas.com/us/en/document/dst/5pb11xx-family-datasheet
  // Need to toggle CS 3 times before the buffer will propagate the outputs
  for (i=0;i<3;i++)
  {
    //Disable cs spi for DOUT
    if (DRV_OK != (eRet = DRV_SPI_ChangeFss(&s_tSPI_OutHandle, DRV_SPI_FSS_NONE)))
    {
      DIAG_Error("SPI Digital Output Chips Select enable failed!", DRV_SPI, eRet, __FILE__, __LINE__);
      return eRet;
    }
      
    SysCounterWaitMicroSeconds(2);
    //Enable cs spi for DOUT
    if (DRV_OK != (eRet = DRV_SPI_ChangeFss(&s_tSPI_OutHandle, DRV_SPI_FSS_0)))
    {
      DIAG_Error("SPI Digital Output Chips Select disable failed!", DRV_SPI, eRet, __FILE__, __LINE__);
      return eRet;
    }
    SysCounterWaitMicroSeconds(2);
  }
  //Disable cs spi for DOUT  
  return DRV_SPI_ChangeFss(&s_tSPI_OutHandle, DRV_SPI_FSS_NONE);
}

/**************************************************************************************/
/*! Transmit data from PLC -> SPI Slave
 *
 *  \param  pTxSpiData            SPI Master Output is SPI Slave Input
 *
 *  \return HOSTAL_OK on success                                                      */
/**************************************************************************************/
HOSTAL_RESULT_E SPI_StartOutputDataTransmit(uint8_t* pOutData)
{
  DRV_STATUS_E eRet = DRV_OK;
  uint8_t spiTxdata[HOSTAL_DEVICE_TX_BYTE_SIZE]={0};

  /* Digital output bits populated, ignore DO 64-49, 25-21 */
  /* clear bits 63-48, 24-20 */
  spiTxdata[0]= *(pOutData + 5);
  spiTxdata[1]= *(pOutData + 4);
  spiTxdata[2]= *(pOutData + 2);
  spiTxdata[3]= *(pOutData + 1);
  spiTxdata[4]= *(pOutData + 0);


  /* enable chip select DO MMIO15 */
  if (DRV_OK != (eRet = DRV_SPI_ChangeFss(&s_tSPI_OutHandle, DRV_SPI_FSS_0)))
  {
    //DIAG_Error("SPI Digital Output Chips Select enable failed!", DRV_SPI, eRet, __FILE__, __LINE__);
    Diag_Inc(DIAG_SPI2_CS_ERR);
    return eRet;
  }

  /*! Send/Receive data over SPI */
  // Data going to SPI Slave from the PLC-------------------------+
  //                                                              |
  //                                                              V
  while(DRV_OK != (eRet = DRV_SPI_Transmit(&s_tSPI_OutHandle, spiTxdata, HOSTAL_DEVICE_TX_BYTE_SIZE)))
  {
    if (DRV_LOCKED == eRet)
    {
      Diag_Inc(DIAG_SPI2_LOCKED);
      continue;
    }
    else if (DRV_BUSY == eRet)
    {
      Diag_Inc(DIAG_SPI2_BUSY);
      continue;
    }
    else if (DRV_ERROR_BUFFER == eRet)
    {
      Diag_Inc(DIAG_SPI2_BUFFER);

      if (DRV_ERROR == DRV_SPI_Abort(&s_tSPI_OutHandle))
        return HOSTAL_ERROR;
      if (HOSTAL_OK != SPI_DoInit())
        return HOSTAL_ERROR;

      continue;
    }
    else
    {
      Diag_Inc(DIAG_SPI2_ERROR);
    }
  }

/* disable chip select */
  if (DRV_OK != (eRet = DRV_SPI_ChangeFss(&s_tSPI_OutHandle, DRV_SPI_FSS_NONE)))  
  {
    //DIAG_Error("SPI Digital Output Chips Select disable failed!", DRV_SPI, eRet, __FILE__, __LINE__);
    Diag_Inc(DIAG_SPI2_CS_ERR);
    return eRet;
  }

  Diag_Inc(DIAG_SPI2_OK);

  return eRet;
}

/*****************************************************************************/
/*! Shell Write a single digital output                                      */
/* \return HOSTAL_OK if parameter is valid (1-64)                            */
/*****************************************************************************/
uint32_t Shell_WriteSingleDO(uint32_t argc, const char *argv[])
{
  uint8_t uDoData[HOSTAL_DEVICE_OUT_BYTE_SIZE] = {0};

  printf("\r\n Write Single DO Shell command");
  if(2 != argc){
    printf("\r\n Usage : %s <ch> \r\n", argv[0]);
    return 0;
  }

  if( g_DoAppControl ){
    printf("\r\n Nope. APP has control!\r\n");
    return 0;
  }

  int channel = atoi( argv[1] );
  if( ( channel < 0) || (channel > (HOSTAL_DEVICE_COILS_COUNT) ) ){
    printf("\r\n Invalid Digital Output: %d (Range: 1-64)", channel );
    return 0;
  }

  /* set the Digital Output */
  channel -= 1; /* switch to bit number */
  uint8_t byte = channel/8;
  uint8_t bit = channel%8;

  uDoData[byte] = 1<<bit;
  
  if (HOSTAL_OK != SPI_StartOutputDataTransmit(&uDoData[0]))
  {
    printf("\r\n SPI Digital Output transfer failed");
    return HOSTAL_ERROR;
  }

  printf("\r\n Digital Output Value %d. \r\n", (uDoData[byte]>>bit)&0x01);  

  return HOSTAL_OK;
}

/*****************************************************************************/
/*! Shell Write all digital outputs                                          */
/* \return HOSTAL_OK                                                         */
/*****************************************************************************/
uint32_t Shell_WriteAllDO(uint32_t argc, const char *argv[])
{
  uint8_t uDoData[HOSTAL_DEVICE_OUT_BYTE_SIZE];
  uint8_t i;

  if( g_DoAppControl ){
    printf("\r\n Nope. APP has control!\r\n");
    return 0;
  }

  if (HOSTAL_OK != SPI_StartOutputDataTransmit(&uDoData[0]))
  {
    printf("\r\n SPI Digital Output transfer failed");
    return HOSTAL_ERROR;
  }

  printf("\r\n Digital Outputs: ");  
  for (i = 0; i < HOSTAL_DEVICE_RX_BYTE_SIZE; i++)
  {
    printf("%02X ", uDoData[i]);
  }
  printf("\r\n");

  return HOSTAL_OK;
}

/***************************************************************************************************
*! \brief  Shell command to (re)assign control of DI update
*
*  \ details  1 argument :  0 = shell, 1 = app
**************************************************************************************************/
uint32_t Shell_DoAppControl( uint32_t argc, const char *argv[] )
{
  if( argc > 1 ){

     int argval = atoi(argv[1]);//0 as err is ok here

      if ( argval ){
         printf("\r\n Returning DO control to APP \r\n");
         g_DoAppControl = true;
      } else {
        printf("\r\n Acquired DO control for shell only! \r\n");
        g_DoAppControl = false;
      }
  } else {
    printf("\r\n %s is in control DO. \r\n", g_DoAppControl ? "APP" : "Shell");
  }

  return 0;
}
