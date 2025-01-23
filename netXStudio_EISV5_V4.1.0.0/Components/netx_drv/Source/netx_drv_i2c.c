/*!************************************************************************//*!
 * \file     netx_drv_i2c.c
 * \brief    I2C peripheral module driver.
 * $Revision: 8341 $
 * $Date: 2020-11-06 17:49:28 +0100 (Fri, 06 Nov 2020) $
 * \copyright Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
 * \note Exclusion of Liability for this demo software:
 * The following software is intended for and must only be used for reference and in an
 * evaluation laboratory environment. It is provided without charge and is subject to
 * alterations. There is no warranty for the software, to the extent permitted by
 * applicable law. Except when otherwise stated in writing the copyright holders and/or
 * other parties provide the software "as is" without warranty of any kind, either
 * expressed or implied.
 * Please refer to the Agreement in README_DISCLAIMER.txt, provided together with this file!
 * By installing or otherwise using the software, you accept the terms of this Agreement.
 * If you do not agree to the terms of this Agreement, then do not install or use the
 * Software!
 ******************************************************************************/

#include "netx_drv.h"
#ifdef DRV_I2C_MODULE_ENABLED

/*! \addtogroup I2C I2C
 * \{
 * \brief The I2C driver, defined by DRV_I2C_HANDLE_T
 *
 * \details
 */

/*!
 * \brief Table of the device addresses.
 *
 * Used to identify the device addresses by the device id.
 */
static DRV_I2C_DEVICE_T * const s_apDeviceAddressTable[DRV_I2C_DEVICE_COUNT] = DRV_I2C_DEVICE_LIST;
/*!
 * \brief Table of the IRQ vector numbers.
 *
 * Used to identify the interrupt channels by the device id.
 */
static IRQn_Type const s_apHandleIRQnTable[DRV_I2C_DEVICE_COUNT] = DRV_I2C_DEVICE_IRQ_LIST;
/*!
 * \brief Used for mapping the handle to an interrupt.
 *
 * Threadsafe and reentrant because its is only written in normal context an used in interrupt context of the specific interrupt.
 */
static DRV_I2C_HANDLE_T * s_apHandleAddressTable[DRV_I2C_DEVICE_COUNT] = { 0 };

/*!
 * Define for checking the consistency of the handle or static representation of the driver.
 */
#ifndef DRV_HANDLE_CHECK_INACTIVE
#define DRV_HANDLE_CHECK(handle)\
  if((handle)==0){ \
    return DRV_ERROR_PARAM; \
  }\
  if((handle)->ptDevice==0){ \
    return DRV_ERROR_PARAM; \
  }
#else
#define DRV_HANDLE_CHECK(handle)
#endif

/*!
 * This method takes an i2c context object of the type DRV_I2C_HANDLE_T and uses the configuration given within to initialize the
 * device. This driver context may be used further on in the i2c api functions to interact with the given i2c device. This context
 * object shall not be modified after initialization. It is only valid to modify it before initializing the device again. The
 * behavior between modification and the performed reinitialization is unknown.
 *
 * Given by the operating mode, the interrupts will be modified and the dma controllers will be initialized.
 *
 * \note The address of the own slave has to be specified even if the slave is not used actively.
 *
 * \public
 * \memberof DRV_I2C_HANDLE_T
 * \param[in,out] ptDriver The given handle of the drivers class
 * \return DRV_OK Everything is fine.
 *         DRV_ERROR_PARAM A given parameter is not correct.
 */
DRV_STATUS_E DRV_I2C_Init(DRV_I2C_HANDLE_T * const ptDriver)
{
  if(ptDriver == 0)
  {
    return DRV_ERROR_PARAM;
  }
  ptDriver->tLock = DRV_LOCK_INITIALIZER;
  DRV_LOCK(ptDriver);
  if(ptDriver->tConfiguration.eOperationMode == DRV_OPERATION_MODE_DMA)
  {
    return DRV_NSUPP;
  }
  if(ptDriver->tConfiguration.eDeviceID >= DRV_I2C_DEVICE_ID_MIN && ptDriver->tConfiguration.eDeviceID <= DRV_I2C_DEVICE_ID_MAX)
  {
    ptDriver->ptDevice = s_apDeviceAddressTable[(uint32_t) ptDriver->tConfiguration.eDeviceID - (uint32_t) DRV_I2C_DEVICE_ID_MIN];
  }
  else
  {
    return DRV_ERROR_PARAM;
  }
  if((uint32_t) ptDriver->tConfiguration.tSlaveAddress.eAddressingScheme == (uint32_t) DRV_I2C_ADDRESS_10_BIT)
  {
    ptDriver->ptDevice->i2c_scr_b.sid10 = 1;
    ptDriver->ptDevice->i2c_scr_b.sid = ptDriver->tConfiguration.tSlaveAddress.uDeviceAddress;
  }
  else if((uint32_t) ptDriver->tConfiguration.tSlaveAddress.eAddressingScheme == (uint32_t) DRV_I2C_ADDRESS_7_BIT)
  {
    ptDriver->ptDevice->i2c_scr_b.sid10 = 0;
    ptDriver->ptDevice->i2c_scr_b.sid = ptDriver->tConfiguration.tSlaveAddress.uDeviceAddress;
  }
  else
  {
    return DRV_ERROR_PARAM;
  }
  if(ptDriver->tConfiguration.eMstRxFifoWatermark == DRV_I2C_WATERMARK_UNINITIALIZED)
  {
    ptDriver->tConfiguration.eMstRxFifoWatermark = DRV_I2C_WATERMARK_DEFAULT;
  }
  if(ptDriver->tConfiguration.eMstTxFifoWatermark == DRV_I2C_WATERMARK_UNINITIALIZED)
  {
    ptDriver->tConfiguration.eMstTxFifoWatermark = DRV_I2C_WATERMARK_DEFAULT;
  }
  if(ptDriver->tConfiguration.eMstTxFifoRefillLevel == DRV_I2C_WATERMARK_UNINITIALIZED)
  {
    ptDriver->tConfiguration.eMstTxFifoRefillLevel = DRV_I2C_WATERMARK_MAX;
  }
  if(ptDriver->tConfiguration.eSlvRxFifoWatermark == DRV_I2C_WATERMARK_UNINITIALIZED)
  {
    ptDriver->tConfiguration.eSlvRxFifoWatermark = DRV_I2C_WATERMARK_DEFAULT;
  }
  if(ptDriver->tConfiguration.eSlvTxFifoWatermark == DRV_I2C_WATERMARK_UNINITIALIZED)
  {
    ptDriver->tConfiguration.eSlvTxFifoWatermark = DRV_I2C_WATERMARK_DEFAULT;
  }
  if(ptDriver->tConfiguration.eSlvTxFifoRefillLevel == DRV_I2C_WATERMARK_UNINITIALIZED)
  {
    ptDriver->tConfiguration.eSlvTxFifoRefillLevel = DRV_I2C_WATERMARK_MAX;
  }
  ptDriver->ptDevice->i2c_mcr = 0;
  ptDriver->ptDevice->i2c_mcr_b.en_i2c = 0;
  ptDriver->ptDevice->i2c_mcr_b.rst_i2c = 1;
  ptDriver->ptDevice->i2c_mcr_b.pio_mode = 0;
  ptDriver->ptDevice->i2c_mcr_b.sadr = 0;
  ptDriver->ptDevice->i2c_mcr_b.en_timeout = 1;
  ptDriver->ptDevice->i2c_sfifo_cr_b.sfifo_clr = 1;
  ptDriver->ptDevice->i2c_mfifo_cr_b.mfifo_clr = 1;
  ptDriver->ptDevice->i2c_irqsr = 0xFFFFFFFF;
  ptDriver->ptDevice->i2c_scr_b.autoreset_ac_start = 1;
  ptDriver->ptDevice->i2c_scr_b.ac_start = 0;
  ptDriver->ptDevice->i2c_scr_b.ac_srx = 0;
  ptDriver->ptDevice->i2c_scr_b.ac_gcall = 0;
  ptDriver->ptDevice->i2c_sfifo_cr_b.sfifo_wm = ((uint32_t) ptDriver->tConfiguration.eSlvRxFifoWatermark
    + (uint32_t) ptDriver->tConfiguration.eSlvTxFifoWatermark) / 2;
  s_apHandleAddressTable[(uint32_t) ptDriver->tConfiguration.eDeviceID - (uint32_t) DRV_I2C_DEVICE_ID_MIN] = ptDriver;
  ptDriver->eRwFlagMaster = DRV_I2C_RW_FLAG_UNKNOWN;
  ptDriver->MstBuffer = 0;
  ptDriver->MstBufferSize = 0;
  ptDriver->MstBufferCounter = 0;
  ptDriver->eRwFlagSlave = DRV_I2C_RW_FLAG_UNKNOWN;
  ptDriver->SlvBuffer = 0;
  ptDriver->SlvBufferSize = 0;
  ptDriver->SlvBufferCounter = 0;
  if(ptDriver->tConfiguration.ulDriverTimeout == 0)
  {
    ptDriver->tConfiguration.ulDriverTimeout = 0xFFFFFFFFul;
  }
  // Configure nvic (activate IRQ, define priority and so on)
  ptDriver->ptDevice->i2c_mcr_b.mode = ptDriver->tConfiguration.eSpeedMode;
  ptDriver->ptDevice->i2c_sfifo_cr_b.sfifo_clr = 0;
  ptDriver->ptDevice->i2c_mfifo_cr_b.mfifo_clr = 0;
  ptDriver->ptDevice->i2c_mcr_b.rst_i2c = 0;
  ptDriver->ptDevice->i2c_mcr_b.en_i2c = 1;
  if(ptDriver->tConfiguration.eOperationMode == DRV_OPERATION_MODE_IRQ)
  {
    DRV_NVIC_ClearPendingIRQ(s_apHandleIRQnTable[(uint32_t) ptDriver->tConfiguration.eDeviceID - (uint32_t) DRV_I2C_DEVICE_ID_MIN]);
    DRV_NVIC_EnableIRQ(s_apHandleIRQnTable[(uint32_t) ptDriver->tConfiguration.eDeviceID - (uint32_t) DRV_I2C_DEVICE_ID_MIN]);
  }
  DRV_UNLOCK(ptDriver);
  return DRV_OK;
}

/*!
 * It tries to get a lock on the handle so that it is able to be reset.
 * Then it will reset the nvic, the device states and the context given.
 *
 * \public
 * \memberof DRV_I2C_HANDLE_T
 * \param[in,out] ptDriver The handle of the driver
 * \return DRV_OK Everything is ok.
 *         DRV_LOCKED The api is locked and not accessible.
 *         DRV_ERROR_PARAM A given parameter is not correct.
 */
DRV_STATUS_E DRV_I2C_DeInit(DRV_I2C_HANDLE_T * const ptDriver)
{
  DRV_HANDLE_CHECK(ptDriver);
  DRV_LOCK(ptDriver);
  DRV_NVIC_DisableIRQ(s_apHandleIRQnTable[(uint32_t) ptDriver->tConfiguration.eDeviceID - (uint32_t) DRV_I2C_DEVICE_ID_MIN]);
  DRV_NVIC_ClearPendingIRQ(s_apHandleIRQnTable[(uint32_t) ptDriver->tConfiguration.eDeviceID - (uint32_t) DRV_I2C_DEVICE_ID_MIN]);
  ptDriver->ptDevice->i2c_irqsr = 0xFFFFFFFFul;
  ptDriver->ptDevice->i2c_mcr_b.rst_i2c = 1u;
  ptDriver->ptDevice->i2c_sfifo_cr_b.sfifo_clr = 1u;
  ptDriver->ptDevice->i2c_mfifo_cr_b.mfifo_clr = 1u;
  ptDriver->ptDevice->i2c_mcr_b.en_i2c = 0u;
  *ptDriver = (DRV_I2C_HANDLE_T ) { 0 };
  return DRV_OK;
}

/*!
 * \brief The flush function is performing the task of equalizing the hardware and software buffers.
 *
 * The flush function checks, if there is data to transmit or receive in the given fifos and
 * depending on the direction emptying the hardware buffer or filling it up. Because there is only one
 * interrupt source for master and slave, both buffers are serviced.
 *
 * \private
 * \memberof DRV_I2C_HANDLE_T
 * \param[in,out] ptDriver The given handle of the drivers class
 * \return void Because there is no margin for errors here.
 */
__INLINE static void DRV_I2C_Flush_Buffers(DRV_I2C_HANDLE_T * const ptDriver)
{
  size_t border;
  size_t i;
  if(ptDriver->ptDevice->i2c_sr_b.slave_access == 1u)
  {
    if(ptDriver->eRwFlagSlave == DRV_I2C_RW_FLAG_READ) // slave receive
    {
      border = ptDriver->ptDevice->i2c_sr_b.sfifo_level;
      if(ptDriver->SlvBufferCounter + border >= ptDriver->SlvBufferSize)
      {
        border = ptDriver->SlvBufferSize - ptDriver->SlvBufferCounter;
      }
      for(i = 0; i < border; i++)
      {
        ((uint8_t*) ptDriver->SlvBuffer)[ptDriver->SlvBufferCounter + i] = ptDriver->ptDevice->i2c_sdr;
      }
      ptDriver->SlvBufferCounter += border;
    }
    else if(ptDriver->eRwFlagSlave == DRV_I2C_RW_FLAG_WRITE) // slave transmit
    {
      border = ptDriver->tConfiguration.eSlvTxFifoRefillLevel - ptDriver->ptDevice->i2c_sr_b.sfifo_level;
      if(ptDriver->SlvBufferCounter + border >= ptDriver->SlvBufferSize)
      {
        border = ptDriver->SlvBufferSize - ptDriver->SlvBufferCounter;
      }
      for(i = 0; i < border; i++)
      {
        ptDriver->ptDevice->i2c_sdr = ((uint8_t*) ptDriver->SlvBuffer)[ptDriver->SlvBufferCounter + i];
      }
      ptDriver->SlvBufferCounter += border;
    }
  }
  if(ptDriver->MstBufferCounter != ptDriver->MstBufferSize)
  {
    if(ptDriver->eRwFlagMaster == DRV_I2C_RW_FLAG_WRITE)
    {
      border = ptDriver->tConfiguration.eMstTxFifoRefillLevel - ptDriver->ptDevice->i2c_sr_b.mfifo_level;
      if(ptDriver->MstBufferCounter + border >= ptDriver->MstBufferSize)
      {
        border = ptDriver->MstBufferSize - ptDriver->MstBufferCounter;
      }
      for(i = 0; i < border; i++)
      {
        ptDriver->ptDevice->i2c_mdr = ((uint8_t*) ptDriver->MstBuffer)[ptDriver->MstBufferCounter + i];
      }
      ptDriver->MstBufferCounter += border;
    }
    else if(ptDriver->eRwFlagMaster == DRV_I2C_RW_FLAG_READ)
    {
      border = ptDriver->ptDevice->i2c_sr_b.mfifo_level;
      if(ptDriver->MstBufferCounter + border >= ptDriver->MstBufferSize)
      {
        border = ptDriver->MstBufferSize - ptDriver->MstBufferCounter;
      }
      for(i = 0; i < border; i++)
      {
        ((uint8_t*) ptDriver->MstBuffer)[ptDriver->MstBufferCounter + i] = ptDriver->ptDevice->i2c_mdr;
      }
      ptDriver->MstBufferCounter += border;
    }
  }
}

/*!
 * \note The master code transmission function has to be used in front of every high speed arbitration context.
 *
 * Because of the i2c protocol, after a start in high speed mode, the master code has to be transmitted in
 * standard mode or fast mode. This function performs this master code transmission and switches the high speed
 * mode on. The driver is afterwards in the continuous mode and a transmission must be performed.
 *
 * \public
 * \memberof DRV_I2C_HANDLE_T
 * \param[in,out] ptDriver The given handle of the drivers class
 * \param[in] tTargetAddress as described in \ref i2c_addressing_scheme
 * \param[in] eSpeed The low or standard speed mode in which the master code will be transmitted
 * \return DRV_OK Everything is fine.
 *         DRV_LOCKED The api is locked and not accessible.
 *         DRV_ERROR_PARAM A given parameter is not correct.
 */
DRV_STATUS_E DRV_I2C_HS_Master_Code(DRV_I2C_HANDLE_T * const ptDriver, DRV_I2C_ADDRESS_T tTargetAddress, DRV_I2C_SPEED_MODE_E eSpeed)
{
  DRV_HANDLE_CHECK(ptDriver);
  DRV_LOCK(ptDriver);
  DRV_I2C_ADDRESS_U uTargetAddress;
  uTargetAddress.bf = tTargetAddress;
  if((uint32_t) tTargetAddress.eAddressingScheme != (uint32_t) DRV_I2C_ADDRESS_MASTER_CODE)
  {
    DRV_UNLOCK(ptDriver);
    return DRV_ERROR_PARAM;
  }
  if(ptDriver->tConfiguration.eSpeedMode < DRV_I2C_SPEED_MODE_HS_800k)
  {
    DRV_UNLOCK(ptDriver);
    return DRV_ERROR_PARAM;
  }
  DRV_STATUS_E ret = DRV_ERROR;
  if(ptDriver->MstBuffer != 0 || ptDriver->MstBufferSize != 0 || ptDriver->MstBufferCounter != 0)
  {
    ret = DRV_BUSY;
  }
  else
  {
    if(ptDriver->tConfiguration.eOperationMode == DRV_OPERATION_MODE_POLL || ptDriver->tConfiguration.eOperationMode == DRV_OPERATION_MODE_IRQ
      || ptDriver->tConfiguration.eOperationMode == DRV_OPERATION_MODE_DMA)
    {
      ptDriver->eCT = DRV_I2C_CONT_CONTINUOUS;
      uint32_t tCMD = 0;
      ptDriver->ptDevice->i2c_mcr_b.sadr = uTargetAddress.array[1];
      tCMD |= ((uTargetAddress.array[0] >> 7) << i2c_app_i2c_cmd_nwr_Pos) & i2c_app_i2c_cmd_nwr_Msk;
      tCMD |= (0 << i2c_app_i2c_cmd_tsize_Pos) & i2c_app_i2c_cmd_tsize_Msk;
      tCMD |= (DRV_I2C_COMMAND_S_AC_TC << i2c_app_i2c_cmd_cmd_Pos) & i2c_app_i2c_cmd_cmd_Msk;
      tCMD |= (0 << i2c_app_i2c_cmd_acpollmax_Pos) & i2c_app_i2c_cmd_acpollmax_Msk;
      ptDriver->ptDevice->i2c_mcr_b.mode = eSpeed;
      ptDriver->ptDevice->i2c_irqmsk_b.cmd_err = 0;
      ptDriver->ptDevice->i2c_cmd = tCMD;
      while(ptDriver->ptDevice->i2c_cmd_b.cmd != DRV_I2C_COMMAND_CT && ptDriver->ptDevice->i2c_cmd_b.cmd != DRV_I2C_COMMAND_CTC
        && ptDriver->ptDevice->i2c_cmd_b.cmd != DRV_I2C_COMMAND_IDLE && ptDriver->ptDevice->i2c_cmd_b.cmd != DRV_I2C_COMMAND_STOP)
      {
        if(ptDriver->ullFrameStartTick++ > ptDriver->tConfiguration.ulDriverTimeout)
        {
          ret = DRV_TOUT;
          DRV_UNLOCK(ptDriver);
          return ret;
        }
      }
      ptDriver->ptDevice->i2c_irqsr = i2c_app_i2c_irqsr_cmd_err_Msk;
      ptDriver->ptDevice->i2c_mcr_b.sadr = uTargetAddress.array[0];
      ptDriver->ptDevice->i2c_mcr_b.mode = ptDriver->tConfiguration.eSpeedMode;
      if(ptDriver->ptDevice->i2c_sr_b.bus_master == 0)
      {
        if(ptDriver->tConfiguration.fnArbitrationLostCallback != 0)
        {
          ptDriver->tConfiguration.fnArbitrationLostCallback(ptDriver, ptDriver->tConfiguration.pArbitrationLostCallbackHandle);
        }
        ret = DRV_ERROR;
      }
      else
      {
        if(ptDriver->tConfiguration.fnMasterCompleteCallback != 0)
        {
          ptDriver->tConfiguration.fnMasterCompleteCallback(ptDriver, ptDriver->tConfiguration.pMasterCompleteCallbackHandle);
        }
        ret = DRV_OK;
      }
      ptDriver->eRwFlagMaster = DRV_I2C_RW_FLAG_UNKNOWN;
      ptDriver->ptDevice->i2c_irqmsk_b.cmd_err = 1;
      ptDriver->MstBuffer = 0;
      ptDriver->MstBufferCounter = 0;
      ptDriver->MstBufferSize = 0;
    }
    else
    {
      ret = DRV_ERROR_PARAM;
    }
  }
  DRV_UNLOCK(ptDriver);
  return ret;
}

/*!
 * The clear function puts out 9 clock pulses on the I2C Bus in the defined speed.
 *
 * Because there is no hardware support for it in the netX 90, this call uses the pio mode
 * of the device.
 * It is blocking and should only be used directly after initialization.
 * This functionality is not conform to the original specification of I2C and contradicts
 * with a lot of the available implementations of I2C in the field.
 *
 * The duration of the high and low phase loops have to be provided as a parameter and may
 * differ widely between builds and hardware.
 *
 * Example calculation: 10ns * 100.000 = 1kHz / 100 = 100kHz with two phases and
 * 23 or 11 instructions result in 100.000 / 23 as loop frequency / 100 / 2 as loop
 * iteration counter.
 * In debug builds: 100000 / 23 / 100 / 2 approx. 100kHz.
 * In release builds: 100000 / 11 / 100 / 2 approx. 100kHz.
 *
 * \public
 * \memberof DRV_I2C_HANDLE_T
 * \param[in,out] ptDriver The given handle of the drivers class
 * \param[in] ulLoops Amount of iterations the IO of sdc is put low and high
 * \return DRV_OK if everything is fine
 *         DRV_LOCKED The api is locked and not accessible.
 *         DRV_ERROR_PARAM A given parameter is not correct.
 */
DRV_STATUS_E DRV_I2C_Clear(DRV_I2C_HANDLE_T * const ptDriver, const size_t ulLoops)
{
  DRV_HANDLE_CHECK(ptDriver);
  if(ulLoops == 0)
  {
    return DRV_ERROR_PARAM;
  }
  (void) DRV_IRQ_Disable();
  DRV_I2C_STATE_E tState;
  DRV_STATUS_E ret;
  if((ret = DRV_I2C_GetState(ptDriver, &tState)) != DRV_OK)
  {
    (void) DRV_IRQ_Enable();
    return ret;
  }
  ptDriver->ptDevice->i2c_mcr_b.pio_mode = 1;
  ptDriver->ptDevice->i2c_pio_b.scl_oe = 1;
  for(size_t i = 0; i < ulLoops; i++)
  {
    ptDriver->ptDevice->i2c_pio_b.scl_out = 0;
  }
  for(size_t j = 0; j < 9; j++)
  {
    for(size_t i = 0; i < ulLoops; i++)
    {
      ptDriver->ptDevice->i2c_pio_b.scl_out = 1;
    }
    for(size_t i = 0; i < ulLoops; i++)
    {
      ptDriver->ptDevice->i2c_pio_b.scl_out = 0;
    }
  }
  ptDriver->ptDevice->i2c_pio_b.scl_oe = 0;
  ptDriver->ptDevice->i2c_mcr_b.pio_mode = 0;
  (void) DRV_IRQ_Enable();
  return DRV_OK;
}

/*!
 * The master transmit function will initiate a transmit of the given data of the given size to the given slave address.
 *
 * If highspeed mode is active, the master code has to be transmitted first.
 *
 * If you like to continue or end the arbitration context the continuous flag has to be set to one or the other value.
 *
 * \memberof DRV_I2C_HANDLE_T
 * \param[in,out] ptDriver The given handle of the drivers class
 * \param[in] tTargetAddress as described in \ref i2c_addressing_scheme
 * \param[in] pbData Pointer to the data to transmit
 * \param[in] size Amount of chars to transmit
 * \return DRV_OK if everything is fine
 */
DRV_STATUS_E DRV_I2C_MasterTransmit(DRV_I2C_HANDLE_T * const ptDriver, DRV_I2C_ADDRESS_T tTargetAddress, uint8_t const * const pbData, size_t size,
  DRV_I2C_CONT_E eCont)
{
  DRV_HANDLE_CHECK(ptDriver);
  DRV_LOCK(ptDriver);
  if(pbData == 0 || size > 1024 || size == 0
    || ((uint32_t) tTargetAddress.eAddressingScheme != (uint32_t) DRV_I2C_ADDRESS_10_BIT
      && (uint32_t) tTargetAddress.eAddressingScheme != (uint32_t) DRV_I2C_ADDRESS_7_BIT
      && (uint32_t) tTargetAddress.eAddressingScheme != (uint32_t) DRV_I2C_ADDRESS_MASTER_CODE))
  {
    DRV_UNLOCK(ptDriver);
    return DRV_ERROR_PARAM;
  }
  if(ptDriver->tConfiguration.eSpeedMode >= DRV_I2C_SPEED_MODE_HS_800k && ptDriver->eCT == DRV_I2C_CONT_END)
  {
    DRV_UNLOCK(ptDriver);
    return DRV_ERROR;
  }
  DRV_STATUS_E ret = DRV_ERROR;
  ptDriver->ullFrameStartTick = 0;
  if(ptDriver->MstBuffer != 0 || ptDriver->MstBufferSize != 0 || ptDriver->MstBufferCounter != 0)
  {
    ret = DRV_BUSY;
  }
  else
  {
    if((size <= ptDriver->tConfiguration.eMstTxFifoWatermark) && (size > 0))
    {
      ptDriver->ptDevice->i2c_mfifo_cr_b.mfifo_wm = size;
    }
    else
    {
      ptDriver->ptDevice->i2c_mfifo_cr_b.mfifo_wm = ptDriver->tConfiguration.eMstTxFifoWatermark;
    }
    ptDriver->MstBuffer = (void*) pbData;
    ptDriver->MstBufferCounter = 0;
    ptDriver->MstBufferSize = size;
    ptDriver->eCT = eCont;
    ptDriver->eRwFlagMaster = DRV_I2C_RW_FLAG_WRITE;
    if(ptDriver->tConfiguration.eOperationMode == DRV_OPERATION_MODE_POLL)
    {
      uint32_t tCMD = 0;
      tCMD |= (DRV_I2C_RW_FLAG_WRITE << i2c_app_i2c_cmd_nwr_Pos) & i2c_app_i2c_cmd_nwr_Msk;
      if((uint8_t) tTargetAddress.eAddressingScheme == (uint8_t) DRV_I2C_ADDRESS_7_BIT
        || (uint8_t) tTargetAddress.eAddressingScheme == (uint8_t) DRV_I2C_ADDRESS_MASTER_CODE)
      {
        ptDriver->ptDevice->i2c_mcr_b.sadr = tTargetAddress.uDeviceAddress;
        tCMD |= ((size - 1) << i2c_app_i2c_cmd_tsize_Pos) & i2c_app_i2c_cmd_tsize_Msk;
      }
      else if((uint8_t) tTargetAddress.eAddressingScheme == (uint8_t) DRV_I2C_ADDRESS_10_BIT)
      {
        DRV_I2C_ADDRESS_U uTargetAddress;
        uTargetAddress.bf = tTargetAddress;
        ptDriver->ptDevice->i2c_mcr_b.sadr = (uint32_t) uTargetAddress.array[1];
        ptDriver->ptDevice->i2c_mdr_b.mdata = uTargetAddress.array[0];
        tCMD |= (size << i2c_app_i2c_cmd_tsize_Pos) & i2c_app_i2c_cmd_tsize_Msk;
      }
      else
      {
        DRV_UNLOCK(ptDriver);
        return DRV_ERROR;
      }
      if(eCont)
      {
        tCMD |= (DRV_I2C_COMMAND_S_AC_TC << i2c_app_i2c_cmd_cmd_Pos) & i2c_app_i2c_cmd_cmd_Msk;
      }
      else
      {
        tCMD |= (DRV_I2C_COMMAND_S_AC_T << i2c_app_i2c_cmd_cmd_Pos) & i2c_app_i2c_cmd_cmd_Msk;
      }
      tCMD |= (ptDriver->tConfiguration.sAckPollMaximum << i2c_app_i2c_cmd_acpollmax_Pos) & i2c_app_i2c_cmd_acpollmax_Msk;
      ptDriver->ptDevice->i2c_cmd = tCMD;
      while(ptDriver->ptDevice->i2c_cmd_b.cmd != DRV_I2C_COMMAND_CT && ptDriver->ptDevice->i2c_cmd_b.cmd != DRV_I2C_COMMAND_CTC)
      {
        if(ptDriver->ptDevice->i2c_irqsr_b.cmd_err)
        {
          break;
        }
        if(ptDriver->ullFrameStartTick++ > ptDriver->tConfiguration.ulDriverTimeout)
        {
          ret = DRV_TOUT;
          DRV_UNLOCK(ptDriver);
          return ret;
        }
      }
      while(ptDriver->MstBufferCounter != ptDriver->MstBufferSize)
      {
        if(ptDriver->ptDevice->i2c_irqsr_b.cmd_err)
        {
          break;
        }
        if(ptDriver->ullFrameStartTick++ > ptDriver->tConfiguration.ulDriverTimeout)
        {
          ret = DRV_TOUT;
          DRV_UNLOCK(ptDriver);
          return ret;
        }
        DRV_I2C_Flush_Buffers(ptDriver);
      }
      while(ptDriver->ptDevice->i2c_cmd_b.cmd != DRV_I2C_COMMAND_IDLE && ptDriver->ptDevice->i2c_cmd_b.cmd != DRV_I2C_COMMAND_CTC)
      {
        if(ptDriver->ptDevice->i2c_irqsr_b.cmd_err)
        {
          break;
        }
        if(ptDriver->ullFrameStartTick++ > ptDriver->tConfiguration.ulDriverTimeout)
        {
          ret = DRV_TOUT;
          DRV_UNLOCK(ptDriver);
          return ret;
        }
      }
      if(!eCont)
      {
        tCMD = ((tCMD & ~i2c_app_i2c_cmd_tsize_Msk) | ((0 << i2c_app_i2c_cmd_tsize_Pos) & i2c_app_i2c_cmd_tsize_Msk));
        tCMD = ((tCMD & ~i2c_app_i2c_cmd_cmd_Msk) | ((DRV_I2C_COMMAND_STOP << i2c_app_i2c_cmd_cmd_Pos) & i2c_app_i2c_cmd_cmd_Msk));
        ptDriver->ptDevice->i2c_cmd = tCMD;
        while(ptDriver->ptDevice->i2c_cmd_b.cmd != DRV_I2C_COMMAND_IDLE)
        {
          if(ptDriver->ptDevice->i2c_irqsr_b.cmd_err)
          {
            break;
          }
          if(ptDriver->ullFrameStartTick++ > ptDriver->tConfiguration.ulDriverTimeout)
          {
            ret = DRV_TOUT;
            DRV_UNLOCK(ptDriver);
            return ret;
          }
        }
      }
      if(ptDriver->ptDevice->i2c_irqsr_b.cmd_err == 1)
      {
        if(ptDriver->ptDevice->i2c_sr_b.bus_master == 0)
        {
          if(ptDriver->tConfiguration.fnArbitrationLostCallback != 0)
          {
            ptDriver->tConfiguration.fnArbitrationLostCallback(ptDriver, ptDriver->tConfiguration.pArbitrationLostCallbackHandle);
          }
        }
        else
        {
          if(ptDriver->tConfiguration.fnErrorCallback != 0)
          {
            ptDriver->tConfiguration.fnErrorCallback(ptDriver, ptDriver->tConfiguration.pErrorCallbackHandle);
          }
        }
        ptDriver->ptDevice->i2c_irqsr = i2c_app_i2c_irqsr_cmd_err_Msk;
        ret = DRV_ERROR;
      }
      else
      {
        if(ptDriver->tConfiguration.fnMasterCompleteCallback != 0 && !ptDriver->ptDevice->i2c_irqsr_b.cmd_err)
        {
          ptDriver->tConfiguration.fnMasterCompleteCallback(ptDriver, ptDriver->tConfiguration.pMasterCompleteCallbackHandle);
        }
        ret = DRV_OK;
      }
      ptDriver->eRwFlagMaster = DRV_I2C_RW_FLAG_UNKNOWN;
      ptDriver->MstBuffer = 0;
      ptDriver->MstBufferCounter = 0;
      ptDriver->MstBufferSize = 0;
    }
    else if(ptDriver->tConfiguration.eOperationMode == DRV_OPERATION_MODE_IRQ)
    {
      uint32_t tCMD = 0;
      tCMD |= (DRV_I2C_RW_FLAG_WRITE << i2c_app_i2c_cmd_nwr_Pos) & i2c_app_i2c_cmd_nwr_Msk;
      if((uint8_t) tTargetAddress.eAddressingScheme == (uint8_t) DRV_I2C_ADDRESS_7_BIT
        || (uint8_t) tTargetAddress.eAddressingScheme == (uint8_t) DRV_I2C_ADDRESS_MASTER_CODE)
      {
        ptDriver->ptDevice->i2c_mcr_b.sadr = tTargetAddress.uDeviceAddress;
        tCMD |= ((size - 1) << i2c_app_i2c_cmd_tsize_Pos) & i2c_app_i2c_cmd_tsize_Msk;
      }
      else if((uint8_t) tTargetAddress.eAddressingScheme == (uint8_t) DRV_I2C_ADDRESS_10_BIT)
      {
        DRV_I2C_ADDRESS_U uTargetAddress;
        uTargetAddress.bf = tTargetAddress;
        ptDriver->ptDevice->i2c_mcr_b.sadr = (uint32_t) uTargetAddress.array[1];
        ptDriver->ptDevice->i2c_mdr = (uint32_t) uTargetAddress.array[0];
        tCMD |= (size << i2c_app_i2c_cmd_tsize_Pos) & i2c_app_i2c_cmd_tsize_Msk;
      }
      else
      {
        DRV_UNLOCK(ptDriver);
        return DRV_ERROR;
      }
      if(eCont)
      {
        tCMD |= (DRV_I2C_COMMAND_S_AC_TC << i2c_app_i2c_cmd_cmd_Pos) & i2c_app_i2c_cmd_cmd_Msk;
      }
      else
      {
        tCMD |= (DRV_I2C_COMMAND_S_AC_TC << i2c_app_i2c_cmd_cmd_Pos) & i2c_app_i2c_cmd_cmd_Msk;
      }
      tCMD |= (ptDriver->tConfiguration.sAckPollMaximum << i2c_app_i2c_cmd_acpollmax_Pos) & i2c_app_i2c_cmd_acpollmax_Msk;
      (void) DRV_IRQ_Disable();
      ptDriver->ptDevice->i2c_cmd = tCMD;
      ptDriver->ptDevice->i2c_irqmsk_b.cmd_ok = 1;
      ptDriver->ptDevice->i2c_irqmsk_b.cmd_err = 1;
      ptDriver->ptDevice->i2c_irqmsk_b.fifo_err = 1;
      ptDriver->ptDevice->i2c_irqmsk_b.mfifo_req = 1;
      (void) DRV_IRQ_Enable();
      ret = DRV_OK;
    }
    else if(ptDriver->tConfiguration.eOperationMode == DRV_OPERATION_MODE_DMA)
    {
      ptDriver->eRwFlagMaster = DRV_I2C_RW_FLAG_UNKNOWN;
      ptDriver->MstBuffer = 0;
      ptDriver->MstBufferCounter = 0;
      ptDriver->MstBufferSize = 0;
      ret = DRV_NSUPP;
    }
    else
    {
      ptDriver->eRwFlagMaster = DRV_I2C_RW_FLAG_UNKNOWN;
      ptDriver->MstBuffer = 0;
      ptDriver->MstBufferCounter = 0;
      ptDriver->MstBufferSize = 0;
      ret = DRV_ERROR_PARAM;
    }
  }
  DRV_UNLOCK(ptDriver);
  return ret;
}

/*!
 * The master receive function will initiate a transmit of the given data of the given size to the given slave address.
 *
 * If highspeed mode is active, the master code has to be transmitted first.
 *
 * If you like to continue or end the arbitration context the continuous flag has to be set to one or the other value.
 * \memberof DRV_I2C_HANDLE_T
 * \param[in,out] ptDriver The given handle of the drivers class
 * \param[in] tTargetAddress as described in \ref i2c_addressing_scheme
 * \param[out] pbData Pointer to the data to transmit
 * \param[in] size Amount of chars to transmit
 * \return DRV_OK if everything is fine
 */
DRV_STATUS_E DRV_I2C_MasterReceive(DRV_I2C_HANDLE_T * const ptDriver, DRV_I2C_ADDRESS_T tTargetAddress, uint8_t * const pbData, size_t size,
  DRV_I2C_CONT_E eCont)
{
  DRV_HANDLE_CHECK(ptDriver);
  DRV_LOCK(ptDriver);
  if(pbData == 0 || size > 1024 || size == 0
    || ((uint32_t) tTargetAddress.eAddressingScheme != (uint32_t) DRV_I2C_ADDRESS_10_BIT
      && (uint32_t) tTargetAddress.eAddressingScheme != (uint32_t) DRV_I2C_ADDRESS_7_BIT
      && (uint32_t) tTargetAddress.eAddressingScheme != (uint32_t) DRV_I2C_ADDRESS_MASTER_CODE))
  {
    DRV_UNLOCK(ptDriver);
    return DRV_ERROR_PARAM;
  }
  DRV_STATUS_E ret = DRV_ERROR;
  ptDriver->ullFrameStartTick = 0;
  if(ptDriver->MstBuffer != 0 || ptDriver->MstBufferSize != 0 || ptDriver->MstBufferCounter != 0)
  {
    ret = DRV_BUSY;
  }
  else
  {
    if((size <= ptDriver->tConfiguration.eMstRxFifoWatermark) && (size > 0))
    {
      ptDriver->ptDevice->i2c_mfifo_cr_b.mfifo_wm = size;
    }
    else
    {
      ptDriver->ptDevice->i2c_mfifo_cr_b.mfifo_wm = ptDriver->tConfiguration.eMstRxFifoWatermark;
    }
    ptDriver->MstBuffer = (void*) pbData;
    ptDriver->MstBufferCounter = 0;
    ptDriver->MstBufferSize = size;
    ptDriver->eCT = eCont;
    ptDriver->eRwFlagMaster = DRV_I2C_RW_FLAG_READ;
    if(ptDriver->tConfiguration.eOperationMode == DRV_OPERATION_MODE_POLL)
    {
      uint32_t tCMD = 0;
      tCMD |= (DRV_I2C_RW_FLAG_READ << i2c_app_i2c_cmd_nwr_Pos) & i2c_app_i2c_cmd_nwr_Msk;
      if((uint8_t) tTargetAddress.eAddressingScheme == (uint8_t) DRV_I2C_ADDRESS_7_BIT
        || (uint8_t) tTargetAddress.eAddressingScheme == (uint8_t) DRV_I2C_ADDRESS_MASTER_CODE)
      {
        ptDriver->ptDevice->i2c_mcr_b.sadr = tTargetAddress.uDeviceAddress;
        tCMD |= ((size - 1) << i2c_app_i2c_cmd_tsize_Pos) & i2c_app_i2c_cmd_tsize_Msk;
      }
      else if((uint8_t) tTargetAddress.eAddressingScheme == (uint8_t) DRV_I2C_ADDRESS_10_BIT)
      {
        DRV_I2C_ADDRESS_U uTargetAddress;
        uTargetAddress.bf = tTargetAddress;
        ptDriver->ptDevice->i2c_mcr_b.sadr = (uint32_t) uTargetAddress.array[1];
        ptDriver->ptDevice->i2c_mdr_b.mdata = uTargetAddress.array[0];
        tCMD |= (size << i2c_app_i2c_cmd_tsize_Pos) & i2c_app_i2c_cmd_tsize_Msk;
      }
      else
      {
        DRV_UNLOCK(ptDriver);
        return DRV_ERROR;
      }
      if(eCont)
      {
        tCMD |= (DRV_I2C_COMMAND_S_AC_TC << i2c_app_i2c_cmd_cmd_Pos) & i2c_app_i2c_cmd_cmd_Msk;
      }
      else
      {
        tCMD |= (DRV_I2C_COMMAND_S_AC_T << i2c_app_i2c_cmd_cmd_Pos) & i2c_app_i2c_cmd_cmd_Msk;
      }
      tCMD |= (ptDriver->tConfiguration.sAckPollMaximum << i2c_app_i2c_cmd_acpollmax_Pos) & i2c_app_i2c_cmd_acpollmax_Msk;
      ptDriver->ptDevice->i2c_cmd = tCMD;
      while(ptDriver->ptDevice->i2c_cmd_b.cmd != DRV_I2C_COMMAND_CT && ptDriver->ptDevice->i2c_cmd_b.cmd != DRV_I2C_COMMAND_CTC)
      {
        if(ptDriver->ptDevice->i2c_irqsr_b.cmd_err)
        {
          break;
        }
        if(ptDriver->ullFrameStartTick++ > ptDriver->tConfiguration.ulDriverTimeout)
        {
          ret = DRV_TOUT;
          DRV_UNLOCK(ptDriver);
          return ret;
        }
      }
      while(ptDriver->MstBufferCounter != ptDriver->MstBufferSize)
      {
        if(ptDriver->ptDevice->i2c_irqsr_b.cmd_err)
        {
          break;
        }
        if(ptDriver->ullFrameStartTick++ > ptDriver->tConfiguration.ulDriverTimeout)
        {
          ret = DRV_TOUT;
          DRV_UNLOCK(ptDriver);
          return ret;
        }
        DRV_I2C_Flush_Buffers(ptDriver);
      }
      while(ptDriver->ptDevice->i2c_cmd_b.cmd != DRV_I2C_COMMAND_IDLE)
      {
        if(ptDriver->ptDevice->i2c_irqsr_b.cmd_err)
        {
          break;
        }
        if(ptDriver->ullFrameStartTick++ > ptDriver->tConfiguration.ulDriverTimeout)
        {
          ret = DRV_TOUT;
          DRV_UNLOCK(ptDriver);
          return ret;
        }
      }
      tCMD = ((tCMD & ~i2c_app_i2c_cmd_tsize_Msk) | ((0 << i2c_app_i2c_cmd_tsize_Pos) & i2c_app_i2c_cmd_tsize_Msk));
      tCMD = ((tCMD & ~i2c_app_i2c_cmd_cmd_Msk) | ((DRV_I2C_COMMAND_STOP << i2c_app_i2c_cmd_cmd_Pos) & i2c_app_i2c_cmd_cmd_Msk));
      ptDriver->ptDevice->i2c_cmd = tCMD;
      while(ptDriver->ptDevice->i2c_cmd_b.cmd != DRV_I2C_COMMAND_IDLE)
      {
        if(ptDriver->ptDevice->i2c_irqsr_b.cmd_err)
        {
          break;
        }
        if(ptDriver->ullFrameStartTick++ > ptDriver->tConfiguration.ulDriverTimeout)
        {
          ret = DRV_TOUT;
          DRV_UNLOCK(ptDriver);
          return ret;
        }
      }
      if(ptDriver->ptDevice->i2c_irqsr_b.cmd_err == 1)
      {
        if(ptDriver->ptDevice->i2c_sr_b.bus_master == 0)
        {
          if(ptDriver->tConfiguration.fnArbitrationLostCallback != 0)
          {
            ptDriver->tConfiguration.fnArbitrationLostCallback(ptDriver, ptDriver->tConfiguration.pArbitrationLostCallbackHandle);
          }
        }
        else
        {
          if(ptDriver->tConfiguration.fnErrorCallback != 0)
          {
            ptDriver->tConfiguration.fnErrorCallback(ptDriver, ptDriver->tConfiguration.pErrorCallbackHandle);
          }
        }
        ptDriver->ptDevice->i2c_irqsr = i2c_app_i2c_irqsr_cmd_err_Msk;
        ret = DRV_ERROR;
      }
      else
      {
        if(ptDriver->tConfiguration.fnMasterCompleteCallback != 0)
        {
          ptDriver->tConfiguration.fnMasterCompleteCallback(ptDriver, ptDriver->tConfiguration.pMasterCompleteCallbackHandle);
        }
        ret = DRV_OK;
      }
      ptDriver->eRwFlagMaster = DRV_I2C_RW_FLAG_UNKNOWN;
      ptDriver->MstBuffer = 0;
      ptDriver->MstBufferCounter = 0;
      ptDriver->MstBufferSize = 0;
    }
    else if(ptDriver->tConfiguration.eOperationMode == DRV_OPERATION_MODE_IRQ)
    {
      uint32_t tCMD = 0;
      tCMD |= (DRV_I2C_RW_FLAG_READ << i2c_app_i2c_cmd_nwr_Pos) & i2c_app_i2c_cmd_nwr_Msk;
      if((uint8_t) tTargetAddress.eAddressingScheme == (uint8_t) DRV_I2C_ADDRESS_7_BIT
        || (uint8_t) tTargetAddress.eAddressingScheme == (uint8_t) DRV_I2C_ADDRESS_MASTER_CODE)
      {
        ptDriver->ptDevice->i2c_mcr_b.sadr = tTargetAddress.uDeviceAddress;
        tCMD |= ((size - 1) << i2c_app_i2c_cmd_tsize_Pos) & i2c_app_i2c_cmd_tsize_Msk;
      }
      else if((uint8_t) tTargetAddress.eAddressingScheme == (uint8_t) DRV_I2C_ADDRESS_10_BIT)
      {
        DRV_I2C_ADDRESS_U uTargetAddress;
        uTargetAddress.bf = tTargetAddress;
        ptDriver->ptDevice->i2c_mcr_b.sadr = (uint32_t) uTargetAddress.array[1];
        ptDriver->ptDevice->i2c_mdr_b.mdata = uTargetAddress.array[0];
        tCMD |= (size << i2c_app_i2c_cmd_tsize_Pos) & i2c_app_i2c_cmd_tsize_Msk;
      }
      else
      {
        DRV_UNLOCK(ptDriver);
        return DRV_ERROR;
      }
      if(eCont)
      {
        tCMD |= (DRV_I2C_COMMAND_S_AC_TC << i2c_app_i2c_cmd_cmd_Pos) & i2c_app_i2c_cmd_cmd_Msk;
      }
      else
      {
        tCMD |= (DRV_I2C_COMMAND_S_AC_T << i2c_app_i2c_cmd_cmd_Pos) & i2c_app_i2c_cmd_cmd_Msk;
      }
      tCMD |= (ptDriver->tConfiguration.sAckPollMaximum << i2c_app_i2c_cmd_acpollmax_Pos) & i2c_app_i2c_cmd_acpollmax_Msk;
      (void) DRV_IRQ_Disable();
      ptDriver->ptDevice->i2c_cmd = tCMD;
      ptDriver->ptDevice->i2c_irqmsk_b.cmd_ok = 1;
      ptDriver->ptDevice->i2c_irqmsk_b.cmd_err = 1;
      ptDriver->ptDevice->i2c_irqmsk_b.fifo_err = 1;
      ptDriver->ptDevice->i2c_irqmsk_b.mfifo_req = 1;
      (void) DRV_IRQ_Enable();
      ret = DRV_OK;
    }
    else if(ptDriver->tConfiguration.eOperationMode == DRV_OPERATION_MODE_DMA)
    {
      ptDriver->eRwFlagMaster = DRV_I2C_RW_FLAG_UNKNOWN;
      ptDriver->MstBuffer = 0;
      ptDriver->MstBufferCounter = 0;
      ptDriver->MstBufferSize = 0;
      ret = DRV_NSUPP;
    }
    else
    {
      ptDriver->eRwFlagMaster = DRV_I2C_RW_FLAG_UNKNOWN;
      ptDriver->MstBuffer = 0;
      ptDriver->MstBufferCounter = 0;
      ptDriver->MstBufferSize = 0;
      ret = DRV_ERROR_PARAM;
    }
  }
  DRV_UNLOCK(ptDriver);
  return ret;
}

/*!
 * The slave transmit will prepare the data to be transmitted via the i2cs slave.
 *
 * A slave transaction has to be queried before the start on the bus is generated or an
 * fifo underrun will occur.
 *
 * \memberof DRV_I2C_HANDLE_T
 * \param[in,out] ptDriver The given handle of the drivers class
 * \param[in] pbData Pointer to the data to transmit
 * \param[in] size Amount of chars to transmit
 * \return DRV_OK if everything is fine
 */
DRV_STATUS_E DRV_I2C_SlaveTransmit(DRV_I2C_HANDLE_T * const ptDriver, uint8_t const * const pbData, size_t size)
{
  DRV_HANDLE_CHECK(ptDriver);
  DRV_LOCK(ptDriver);
  if(pbData == 0 || size > 1024 || size == 0)
  {
    DRV_UNLOCK(ptDriver);
    return DRV_ERROR_PARAM;
  }
  DRV_STATUS_E ret = DRV_ERROR;
  if(ptDriver->SlvBuffer != 0 || ptDriver->SlvBufferSize != 0 || ptDriver->SlvBufferCounter != 0)
  {
    DRV_UNLOCK(ptDriver);
    ret = DRV_BUSY;
  }
  else
  {
    if((size <= ptDriver->tConfiguration.eSlvTxFifoWatermark) && (size > 0))
    {
      ptDriver->ptDevice->i2c_sfifo_cr_b.sfifo_wm = size;
    }
    else
    {
      ptDriver->ptDevice->i2c_sfifo_cr_b.sfifo_wm = ptDriver->tConfiguration.eSlvTxFifoWatermark;
    }
    ptDriver->SlvBuffer = (void*) pbData;
    ptDriver->SlvBufferCounter = 0;
    ptDriver->SlvBufferSize = size;
    ptDriver->eRwFlagSlave = DRV_I2C_RW_FLAG_WRITE;
    if(ptDriver->tConfiguration.eOperationMode == DRV_OPERATION_MODE_POLL)
    {
      DRV_I2C_Flush_Buffers(ptDriver);
      /* The data should be in the buffer before the start of the read will be acknowledged,
       * because there will be an error if the data is not in the buffer after the acknowledge is generated.
       */
      ptDriver->ptDevice->i2c_scr_b.ac_start = 1;
      while(ptDriver->SlvBufferCounter != ptDriver->SlvBufferSize)
      {
        DRV_I2C_Flush_Buffers(ptDriver);
      }
      ptDriver->eRwFlagSlave = DRV_I2C_RW_FLAG_UNKNOWN;
      ptDriver->SlvBuffer = 0;
      ptDriver->SlvBufferCounter = 0;
      ptDriver->SlvBufferSize = 0;
      ret = DRV_OK;
    }
    else if(ptDriver->tConfiguration.eOperationMode == DRV_OPERATION_MODE_IRQ)
    {
      (void) DRV_IRQ_Disable();
      ptDriver->ptDevice->i2c_irqmsk_b.sfifo_req = 1;
      ptDriver->ptDevice->i2c_irqmsk_b.sreq = 1;
      DRV_I2C_Flush_Buffers(ptDriver);
      ptDriver->ptDevice->i2c_scr_b.ac_start = 1;
      (void) DRV_IRQ_Enable();
      ret = DRV_OK;
    }
    else if(ptDriver->tConfiguration.eOperationMode == DRV_OPERATION_MODE_DMA)
    {
      ptDriver->eRwFlagSlave = DRV_I2C_RW_FLAG_UNKNOWN;
      ptDriver->SlvBuffer = 0;
      ptDriver->SlvBufferCounter = 0;
      ptDriver->SlvBufferSize = 0;
      ret = DRV_NSUPP;
    }
    else
    {
      ptDriver->eRwFlagSlave = DRV_I2C_RW_FLAG_UNKNOWN;
      ptDriver->SlvBuffer = 0;
      ptDriver->SlvBufferCounter = 0;
      ptDriver->SlvBufferSize = 0;
      ret = DRV_ERROR_PARAM;
    }
  }
  DRV_UNLOCK(ptDriver);
  return ret;
}

/*!
 * The slave transmit will prepare the data to be transmitted via the i2cs slave.
 *
 * A slave transaction has to be queried before the start on the bus is generated
 * or the data might not be received correctly.
 *
 * \memberof DRV_I2C_HANDLE_T
 * \param[in,out] ptDriver The given handle of the drivers class
 * \param[out] pbData Pointer to the data to transmit
 * \param[in] size Amount of chars to transmit
 * \return DRV_OK if everything is fine
 */
DRV_STATUS_E DRV_I2C_SlaveReceive(DRV_I2C_HANDLE_T * const ptDriver, uint8_t * const pbData, size_t size)
{
  DRV_HANDLE_CHECK(ptDriver);
  DRV_LOCK(ptDriver);
  if(pbData == 0 || size > 1024 || size == 0)
  {
    DRV_UNLOCK(ptDriver);
    return DRV_ERROR_PARAM;
  }
  DRV_STATUS_E ret = DRV_ERROR;
  if(ptDriver->SlvBuffer != 0 || ptDriver->SlvBufferSize != 0 || ptDriver->SlvBufferCounter != 0)
  {
    ret = DRV_BUSY;
  }
  else
  {
    if((size <= ptDriver->tConfiguration.eSlvRxFifoWatermark) && (size > 0))
    {
      ptDriver->ptDevice->i2c_sfifo_cr_b.sfifo_wm = size;
    }
    else
    {
      ptDriver->ptDevice->i2c_sfifo_cr_b.sfifo_wm = ptDriver->tConfiguration.eSlvRxFifoWatermark;
    }
    ptDriver->SlvBuffer = (void*) pbData;
    ptDriver->SlvBufferCounter = 0;
    ptDriver->SlvBufferSize = size;
    ptDriver->ptDevice->i2c_scr_b.ac_start = 1;
    ptDriver->eRwFlagSlave = DRV_I2C_RW_FLAG_READ;
    if(ptDriver->tConfiguration.eOperationMode == DRV_OPERATION_MODE_POLL)
    {
      ptDriver->ptDevice->i2c_scr_b.ac_srx = 1;
      ptDriver->ptDevice->i2c_scr_b.ac_start = 1;
      while(ptDriver->SlvBufferCounter != ptDriver->SlvBufferSize)
      {
        DRV_I2C_Flush_Buffers(ptDriver);
      }
      ptDriver->eRwFlagSlave = DRV_I2C_RW_FLAG_UNKNOWN;
      ptDriver->SlvBuffer = 0;
      ptDriver->SlvBufferCounter = 0;
      ptDriver->SlvBufferSize = 0;
      ret = DRV_OK;
    }
    else if(ptDriver->tConfiguration.eOperationMode == DRV_OPERATION_MODE_IRQ)
    {
      (void) DRV_IRQ_Disable();
      ptDriver->ptDevice->i2c_irqmsk_b.sfifo_req = 1;
      ptDriver->ptDevice->i2c_irqmsk_b.sreq = 1;
      ptDriver->ptDevice->i2c_scr_b.ac_srx = 1;
      ptDriver->ptDevice->i2c_scr_b.ac_start = 1;
      (void) DRV_IRQ_Enable();
      ret = DRV_OK;
    }
    else if(ptDriver->tConfiguration.eOperationMode == DRV_OPERATION_MODE_DMA)
    {
      ptDriver->eRwFlagSlave = DRV_I2C_RW_FLAG_UNKNOWN;
      ptDriver->SlvBuffer = 0;
      ptDriver->SlvBufferCounter = 0;
      ptDriver->SlvBufferSize = 0;
      ret = DRV_NSUPP;
    }
    else
    {
      ptDriver->eRwFlagSlave = DRV_I2C_RW_FLAG_UNKNOWN;
      ptDriver->SlvBuffer = 0;
      ptDriver->SlvBufferCounter = 0;
      ptDriver->SlvBufferSize = 0;
      ret = DRV_ERROR_PARAM;
    }
  }
  DRV_UNLOCK(ptDriver);
  return ret;
}

/*!
 * This funciton shall abort any i2c data io.
 * An reinitialize afterwards might be necessary to attach back on the bus in a valid state.
 *
 * \memberof DRV_I2C_HANDLE_T
 * \param[in,out] ptDriver The given handle of the drivers class
 * \return DRV_OK if everything is fine
 */
DRV_STATUS_E DRV_I2C_Abort(DRV_I2C_HANDLE_T * const ptDriver)
{
  DRV_HANDLE_CHECK(ptDriver);
  DRV_LOCK(ptDriver);
  DRV_STATUS_E ret = DRV_ERROR;
  if(ptDriver->tConfiguration.eOperationMode == DRV_OPERATION_MODE_POLL)
  {
    ret = DRV_NSUPP;
  }
  else if(ptDriver->tConfiguration.eOperationMode == DRV_OPERATION_MODE_IRQ)
  {
    DRV_NVIC_DisableIRQ(s_apHandleIRQnTable[(uint32_t) ptDriver->tConfiguration.eDeviceID - (uint32_t) DRV_UART_DEVICE_ID_MIN]);
    DRV_NVIC_ClearPendingIRQ(s_apHandleIRQnTable[(uint32_t) ptDriver->tConfiguration.eDeviceID - (uint32_t) DRV_UART_DEVICE_ID_MIN]);
    ptDriver->ptDevice->i2c_mcr_b.en_i2c = 0;
    ptDriver->MstBuffer = 0;
    ptDriver->MstBufferSize = 0;
    ptDriver->MstBufferCounter = 0;
    ptDriver->SlvBuffer = 0;
    ptDriver->SlvBufferSize = 0;
    ptDriver->SlvBufferCounter = 0;
    ptDriver->ptDevice->i2c_irqsr = (uint32_t) -1;
    ptDriver->ptDevice->i2c_mcr_b.en_i2c = 1;
    DRV_NVIC_EnableIRQ(s_apHandleIRQnTable[(uint32_t) ptDriver->tConfiguration.eDeviceID - (uint32_t) DRV_UART_DEVICE_ID_MIN]);
  }
  else if(ptDriver->tConfiguration.eOperationMode == DRV_OPERATION_MODE_DMA)
  {
    ret = DRV_NSUPP;
  }
  else
  {
    ret = DRV_ERROR_PARAM;
  }
  DRV_UNLOCK(ptDriver);
  return ret;
}

/*!
 * The get state function shall modify the given value to the devices internal states and
 * return the states of the driver and indicate an error.
 *
 * \memberof DRV_I2C_HANDLE_T
 * \param[in,out] ptDriver The given handle of the drivers class
 * \return DRV_OK if everything is fine
 */
DRV_STATUS_E DRV_I2C_GetState(DRV_I2C_HANDLE_T * const ptDriver, DRV_I2C_STATE_E * const ptState)
{
  DRV_HANDLE_CHECK(ptDriver);
  if(ptState == NULL)
  {
    return DRV_ERROR_PARAM;
  }
  *ptState = (DRV_I2C_STATE_E) 0ull;
  *ptState = (DRV_I2C_STATE_E) ptDriver->ptDevice->i2c_sr;
  if(ptDriver->ptDevice->i2c_cmd_b.nwr)
  {
    *ptState = (DRV_I2C_STATE_E) (((uint64_t) *ptState) | (uint64_t) DRV_I2C_STATE_MASTER_CMD_READ);
  }
  switch (ptDriver->ptDevice->i2c_cmd_b.cmd)
  {
  case DRV_I2C_COMMAND_START:
    *ptState = (DRV_I2C_STATE_E) (((uint64_t) *ptState) | (uint64_t) DRV_I2C_STATE_MASTER_CMD_START);
    break;
  case DRV_I2C_COMMAND_S_AC:
    *ptState = (DRV_I2C_STATE_E) (((uint64_t) *ptState) | (uint64_t) DRV_I2C_STATE_MASTER_CMD_S_AC);
    break;
  case DRV_I2C_COMMAND_S_AC_T:
    *ptState = (DRV_I2C_STATE_E) (((uint64_t) *ptState) | (uint64_t) DRV_I2C_STATE_MASTER_CMD_S_AC_T);
    break;
  case DRV_I2C_COMMAND_S_AC_TC:
    *ptState = (DRV_I2C_STATE_E) (((uint64_t) *ptState) | (uint64_t) DRV_I2C_STATE_MASTER_CMD_S_AC_TC);
    break;
  case DRV_I2C_COMMAND_CT:
    *ptState = (DRV_I2C_STATE_E) (((uint64_t) *ptState) | (uint64_t) DRV_I2C_STATE_MASTER_CMD_CT);
    break;
  case DRV_I2C_COMMAND_CTC:
    *ptState = (DRV_I2C_STATE_E) (((uint64_t) *ptState) | (uint64_t) DRV_I2C_STATE_MASTER_CMD_CTC);
    break;
  case DRV_I2C_COMMAND_STOP:
    *ptState = (DRV_I2C_STATE_E) (((uint64_t) *ptState) | (uint64_t) DRV_I2C_STATE_MASTER_CMD_STOP);
    break;
  case DRV_I2C_COMMAND_IDLE:
    *ptState = (DRV_I2C_STATE_E) (((uint64_t) *ptState) | (uint64_t) DRV_I2C_STATE_MASTER_CMD_IDLE);
    break;
  default:
    break;
  }
  uint64_t fill = ptDriver->ptDevice->i2c_cmd_b.tsize;
  *ptState = (DRV_I2C_STATE_E) (((uint64_t) *ptState) | (fill << 48));

  DRV_STATUS_E ret = DRV_OK;
  if((!((uint64_t) *ptState & (uint64_t) DRV_I2C_STATE_MASTER_CMD_IDLE) || ((uint64_t) *ptState & (uint64_t) DRV_I2C_STATE_MASTER_CMD_CTC))
    || ptDriver->SlvBuffer != 0 || ptDriver->SlvBufferCounter > 0 || ptDriver->MstBuffer != 0
    || ptDriver->MstBufferCounter != ptDriver->MstBufferSize)
  {
    ret = DRV_BUSY;
  }
  if(ptDriver->ptDevice->i2c_irqsr_b.cmd_err)
  {
    ret = DRV_ERROR;
  }
  return ret;
}

/*!
 * This is the inline handler of the i2c driver. It is called by the generated isrs.
 *
 * \memberof DRV_I2C_HANDLE_T
 * \param[in,out] eDeviceID The given handle of the drivers class
 */
__INLINE static void DRV_I2C_IRQ_Inline_Handler(DRV_I2C_DEVICE_ID_E const eDeviceID)
{
  uint32_t ulDeviceOffset = (uint32_t) eDeviceID - (uint32_t) DRV_I2C_DEVICE_ID_MIN;
  DRV_I2C_HANDLE_T* ptDriver = s_apHandleAddressTable[ulDeviceOffset];
#ifndef NDEBUG
  if(ptDriver == 0 || ptDriver->ptDevice != s_apDeviceAddressTable[ulDeviceOffset])
  {
    ((DRV_I2C_DEVICE_T * const ) s_apDeviceAddressTable[ulDeviceOffset])->i2c_mcr = 0;
    DRV_NVIC_DisableIRQ(s_apHandleIRQnTable[ulDeviceOffset]);
    return;
  }
#endif
  DRV_I2C_Flush_Buffers(ptDriver);
  if(ptDriver->MstBuffer != 0 && ptDriver->MstBufferCounter == ptDriver->MstBufferSize)
  {
    if(ptDriver->ptDevice->i2c_irqsr_b.cmd_ok
      && (ptDriver->ptDevice->i2c_cmd_b.cmd == DRV_I2C_COMMAND_CT || ptDriver->ptDevice->i2c_cmd_b.cmd == DRV_I2C_COMMAND_CTC
        || ptDriver->ptDevice->i2c_cmd_b.cmd == DRV_I2C_COMMAND_IDLE))
    {
      if(ptDriver->eCT == DRV_I2C_CONT_END)
      {
        ptDriver->ptDevice->i2c_cmd_b.cmd = DRV_I2C_COMMAND_STOP;
      }
      if(ptDriver->tConfiguration.fnMasterCompleteCallback != 0)
      {
        ptDriver->tConfiguration.fnMasterCompleteCallback(ptDriver, ptDriver->tConfiguration.pMasterCompleteCallbackHandle);
      }
      ptDriver->eRwFlagMaster = DRV_I2C_RW_FLAG_UNKNOWN;
      ptDriver->ptDevice->i2c_irqmsk_b.cmd_ok = 0;
      ptDriver->ptDevice->i2c_irqsr = i2c_app_i2c_irqsr_cmd_ok_Msk;
      ptDriver->MstBuffer = 0;
      ptDriver->MstBufferCounter = 0;
      ptDriver->MstBufferSize = 0;
    }
    ptDriver->ptDevice->i2c_irqmsk_b.mfifo_req = 0;
    ptDriver->ptDevice->i2c_irqsr = i2c_app_i2c_irqsr_mfifo_req_Msk;
  }
  if(ptDriver->MstBuffer == 0 && ptDriver->ptDevice->i2c_irqmsk_b.cmd_ok == 1)
  {
    ptDriver->ptDevice->i2c_irqmsk_b.cmd_ok = 0;
  }
  if(ptDriver->ptDevice->i2c_irqsr_b.cmd_err)
  {
    if(ptDriver->tConfiguration.fnArbitrationLostCallback != 0 && ptDriver->ptDevice->i2c_sr_b.bus_master == 0)
    {
      ptDriver->tConfiguration.fnArbitrationLostCallback(ptDriver, ptDriver->tConfiguration.pArbitrationLostCallbackHandle);
    }
    else if(ptDriver->tConfiguration.fnErrorCallback != 0)
    {
      ptDriver->tConfiguration.fnErrorCallback(ptDriver, ptDriver->tConfiguration.pErrorCallbackHandle);
    }
    ptDriver->ptDevice->i2c_irqmsk_b.cmd_err = 0;
    ptDriver->ptDevice->i2c_irqmsk_b.mfifo_req = 0;
    ptDriver->ptDevice->i2c_irqsr = i2c_app_i2c_irqsr_cmd_err_Msk;
  }
  if(ptDriver->ptDevice->i2c_irqsr_b.fifo_err)
  {
    if(ptDriver->tConfiguration.fnErrorCallback != 0)
    {
      ptDriver->tConfiguration.fnErrorCallback(ptDriver, ptDriver->tConfiguration.pErrorCallbackHandle);
    }
    ptDriver->ptDevice->i2c_irqmsk_b.fifo_err = 0;
    ptDriver->ptDevice->i2c_irqsr = i2c_app_i2c_irqsr_fifo_err_Msk;
  }
  if(ptDriver->SlvBuffer != 0 && ptDriver->SlvBufferCounter == ptDriver->SlvBufferSize)
  {
    if(ptDriver->tConfiguration.fnSlaveCompleteCallback != 0)
    {
      ptDriver->tConfiguration.fnSlaveCompleteCallback(ptDriver, ptDriver->tConfiguration.pSlaveCompleteCallbackHandle);
    }
    ptDriver->ptDevice->i2c_irqmsk_b.sfifo_req = 0;
    ptDriver->ptDevice->i2c_irqsr = i2c_app_i2c_irqsr_sfifo_req_Msk;
    ptDriver->ptDevice->i2c_irqmsk_b.sreq = 0;
    ptDriver->ptDevice->i2c_irqsr = i2c_app_i2c_irqsr_sreq_Msk;
    ptDriver->eRwFlagSlave = DRV_I2C_RW_FLAG_UNKNOWN;
    ptDriver->SlvBuffer = 0;
    ptDriver->SlvBufferCounter = 0;
    ptDriver->SlvBufferSize = 0;
  }
  if(ptDriver->SlvBuffer == 0 && (ptDriver->ptDevice->i2c_irqsr_b.sfifo_req == 1 || ptDriver->ptDevice->i2c_irqsr_b.sreq == 1))
  {
    if(ptDriver->tConfiguration.fnErrorCallback != 0)
    {
      ptDriver->tConfiguration.fnErrorCallback(ptDriver, ptDriver->tConfiguration.pErrorCallbackHandle);
    }
    ptDriver->ptDevice->i2c_irqmsk_b.sfifo_req = 0;
    ptDriver->ptDevice->i2c_irqsr = i2c_app_i2c_irqsr_sfifo_req_Msk;
    ptDriver->ptDevice->i2c_irqmsk_b.sreq = 0;
    ptDriver->ptDevice->i2c_irqsr = i2c_app_i2c_irqsr_sreq_Msk;
  }
  size_t ulDiff = ptDriver->SlvBufferSize - ptDriver->SlvBufferCounter;
  if((ulDiff <= ptDriver->tConfiguration.eSlvRxFifoWatermark) && (ulDiff > 0))
  {
    ptDriver->ptDevice->i2c_sfifo_cr_b.sfifo_wm = ulDiff;
  }
  ulDiff = ptDriver->MstBufferSize - ptDriver->MstBufferCounter;
  if((ulDiff <= ptDriver->tConfiguration.eMstRxFifoWatermark) && (ulDiff > 0))
  {
    ptDriver->ptDevice->i2c_mfifo_cr_b.mfifo_wm = ulDiff;
  }
  ptDriver->ptDevice->i2c_irqsr = 0xFFFFFFFFu;
}

/*!
 * The generator define for generating irq handler source code.
 */
#define  DRV_I2C_IRQHandler_Generator(id, _) DRV_Default_IRQHandler_Function_Generator(DRV_I2C_IRQ_HANDLER ## id ,DRV_I2C_IRQ_Inline_Handler,DRV_I2C_DEVICE_ID_I2C ## id)

/*!
 * The define to where the irq handler code shall be generated.
 */
/*lint -save -e123 */
DRV_DEF_REPEAT_EVAL(DRV_I2C_DEVICE_COUNT, DRV_I2C_IRQHandler_Generator, ~)
/*lint -restore */

/*! \} *//* End of group I2C */

#endif /* DRV_DIO_MODULE_DISABLED */
