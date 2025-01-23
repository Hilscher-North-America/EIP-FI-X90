#include "app_application.h"
#include "Digital_Output.h"
#include "netx_drv.h"

/*****************************************************************************/
/*! Initializes shared SPI clock                                             */
/*  \return HOSTAL_OK on success                                             */
/*****************************************************************************/
HOSTAL_RESULT_E SPI_ClockInit ()
{
  DRV_SPI_HANDLE_T tSPI_Handle = { { 0 } };  /*! Initializing the SPI Output */
  DRV_STATUS_E eRet=0;
  uint8_t nCount = 0;
  uint8_t sendBuf = 0;
  
  /* SPI for outputs */
  do
  {
    if (DRV_SPI_DEVICE_ID_SPI1 == tSPI_Handle.tConfiguration.eSPIDeviceID )
    {
      /*! Disables the DRV_SPI_HANDLE_T */
      DRV_SPI_DeInit(&tSPI_Handle);
    }

    /** configure the SPI peripheral */
    tSPI_Handle.tConfiguration.eSPIDeviceID            = DRV_SPI_DEVICE_ID_SPI1;                        /** spi device ID */
    tSPI_Handle.tConfiguration.eMISO                   = DRV_SPI_MISO_INACTIVE;                          /** miso state */
    tSPI_Handle.tConfiguration.eOperationMode          = DRV_OPERATION_MODE_POLL;                      /** Poll/blocking mode */
    tSPI_Handle.tConfiguration.eParallelism            = DRV_SPI_PARALLELISM_1BIT;                     /** parallelism mode */
    tSPI_Handle.tConfiguration.eDuplex                 = DRV_SPI_DUPLEX_FULL;                          /** duplex mode */
    tSPI_Handle.tConfiguration.eFSS                    = DRV_SPI_FSS_NONE;                             /** slave select (FSS) signal pin mask control manually the CS pin */
    tSPI_Handle.tConfiguration.eFSSStatic              = DRV_SPI_FSS_STATIC_CALLER;                    /** FSS is driver controlled */
    tSPI_Handle.tConfiguration.eFrequency              = DRV_SPI_FREQUENCY_10MHz;                      /** spi frequencies */
    tSPI_Handle.tConfiguration.uMode.value             = DRV_SPI_MODE_3;                               /** sampling mode */
    tSPI_Handle.tConfiguration.eBehaviour              = DRV_SPI_BEHAVIOUR_MASTER;                     /** master/slave mode */
    tSPI_Handle.tConfiguration.eSlaveSigEarly          = DRV_SPI_SLV_SIG_EARLY;                        /** early response bit generation */
    tSPI_Handle.tConfiguration.eFILTER                 = DRV_SPI_FILTER_INACTIVE;                        /** filter state */
    tSPI_Handle.tConfiguration.eEndianess              = DRV_SPI_ENDIANESS_LITTLE;                     /** little/big Endianess */
    tSPI_Handle.tConfiguration.eRxFiFoWm               = DRV_SPI_FIFO_WM_DEFAULT;                      /** Receive FiFo watermark */
    tSPI_Handle.tConfiguration.eTxFiFoWm               = DRV_SPI_FIFO_WM_DEFAULT;                      /** Transmit FiFo watermark */
    tSPI_Handle.tConfiguration.eDataSize               = DRV_SPI_DATA_SIZE_SELECT_8b;                  /** data size of 8bit transmitted */
    
  } while ( DRV_OK != (eRet = DRV_SPI_Init(&tSPI_Handle)) && nCount++ < 3); /* driver could be locked */

  //check for errors
  if(DRV_OK != eRet) {
    return HOSTAL_ERROR;
  }  
  
  // initialize SPI clock buffer (U25), shared between Digital Input and Output, initializing it here is sufficient for both DI and DO
  nCount = 0;
  while(DRV_OK != (eRet = DRV_SPI_Transmit(&tSPI_Handle, &sendBuf, 1)) && nCount++ < 3)
  {
    // nothing here, just retry
  }

  return DRV_SPI_DeInit(&tSPI_Handle);
}
