#ifndef COMPONENTS_HAL_INCLUDE_I2C_H_
#define COMPONENTS_HAL_INCLUDE_I2C_H_

#include <stdio.h>
#include <stdbool.h>

#include "netx_drv.h"

/* public function prototypes */
bool I2C_Send(DRV_I2C_HANDLE_T* const pi2cHandle, DRV_I2C_ADDRESS_T i2cAddress, uint8_t const * const pData, size_t size);
bool I2C_Receive(DRV_I2C_HANDLE_T* const pi2cHandle, DRV_I2C_ADDRESS_T i2cAddress, uint8_t * const pData, size_t size);

#endif // COMPONENTS_HAL_INCLUDE_I2C_H_
