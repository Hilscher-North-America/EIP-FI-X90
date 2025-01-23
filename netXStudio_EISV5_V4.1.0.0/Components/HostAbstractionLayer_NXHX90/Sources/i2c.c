#include <string.h>

#include "i2c.H"
#include "hal.h"
#include "mks_syscounter.h"

unsigned long  (*g_pCounterFunc)() = NULL; //keep declaration same as dma_tmr_get_cntrX() calls;

/*****************************************************************************/
/*! send to i2c peripheral request
 *   \return bool                                                            */
/*****************************************************************************/
bool I2C_Send(DRV_I2C_HANDLE_T* const pi2cHandle, DRV_I2C_ADDRESS_T i2cAddress, uint8_t const * const pData, size_t size)
{
  uint32_t startTime;
  uint32_t waitTime= MILLI_SECONDS_TO_SYSTEM_TICK_COUNTS(10);
  DRV_STATUS_E      FceRet;
  DRV_I2C_STATE_E   eState;

  startTime = GetSysCounter();
  while(DRV_OK != (FceRet = DRV_I2C_MasterTransmit(pi2cHandle, i2cAddress, pData, size, DRV_I2C_CONT_END))){
    if( DRV_BUSY != FceRet ){
      printf("\r\n %10s: Sending I2C command failed with error: %d ", HAL_ERROR_STR, FceRet);
      return false;
    }

    if (MKS_TRUE == SysCounterTimeoutCounts(startTime, waitTime)) {
      printf("\r\n %10s: Sending I2C command timed out", HAL_ERROR_STR);
      return false;
      }
  }

  /* Verify state of the i2c send request */
  startTime = GetSysCounter();
  while(DRV_OK != (FceRet = DRV_I2C_GetState(pi2cHandle, &eState)))
  {
    if((uint64_t) eState & ((uint64_t) DRV_I2C_STATE_SLAVE_FIFO_OVERFLOW | (uint64_t) DRV_I2C_STATE_SLAVE_FIFO_UNDERRUN | (uint64_t) DRV_I2C_STATE_MASTER_FIFO_OVERFLOW | (uint64_t) DRV_I2C_STATE_MASTER_FIFO_UNDERRUN)) {
      printf("\r\n %10s: I2C send state verification failed %d ", HAL_ERROR_STR, FceRet);
      return false;  
    }
    if (MKS_TRUE == SysCounterTimeoutCounts(startTime, waitTime)) {
      printf("\r\n %10s: I2C send state verification timed out", HAL_ERROR_STR);
      return false;
    }
  }

  return true;
}

/*****************************************************************************/
/*! receive from i2c peripheral request
 *   \return bool                                                            */
/*****************************************************************************/
bool I2C_Receive(DRV_I2C_HANDLE_T* const pi2cHandle, DRV_I2C_ADDRESS_T i2cAddress, uint8_t * const pData, size_t size)
{
  uint32_t startTime;
  uint32_t waitTime= MILLI_SECONDS_TO_SYSTEM_TICK_COUNTS(10);
  DRV_STATUS_E      FceRet;
  DRV_I2C_STATE_E   eState;

  startTime = GetSysCounter();
  while(DRV_OK != (FceRet = DRV_I2C_MasterReceive(pi2cHandle, i2cAddress, pData, size, DRV_I2C_CONT_END))){
    if( DRV_BUSY != FceRet ){
      printf("\r\n %10s: Receiving I2C command failed with error: %d ", HAL_ERROR_STR, FceRet);
      return false;
    }

    if (MKS_TRUE == SysCounterTimeoutCounts(startTime, waitTime)) {
      printf("\r\n %10s: Receiving I2C command timed out", HAL_ERROR_STR);
      return false;
      }
  }

  /* Verify state of the i2c send request */
  startTime = GetSysCounter();
  while(DRV_OK != (FceRet = DRV_I2C_GetState(pi2cHandle, &eState)))
  {
    if((uint64_t) eState & ((uint64_t) DRV_I2C_STATE_SLAVE_FIFO_OVERFLOW | (uint64_t) DRV_I2C_STATE_SLAVE_FIFO_UNDERRUN | (uint64_t) DRV_I2C_STATE_MASTER_FIFO_OVERFLOW | (uint64_t) DRV_I2C_STATE_MASTER_FIFO_UNDERRUN)) {
      printf("\r\n %10s: I2C receive state verification failed %d ", HAL_ERROR_STR, FceRet);
      return false;  
    }
    if (MKS_TRUE == SysCounterTimeoutCounts(startTime, waitTime)) {
      printf("\r\n %10s: I2C receive state verification timed out", HAL_ERROR_STR);
      return false;
    }
  }

  return true;
}
