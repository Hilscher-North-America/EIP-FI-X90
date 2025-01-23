/*!************************************************************************//*!
 * \file    netx_drv_i2c.h
 * \brief   Header file of I2C DRV module.
 * $Revision: 8097 $
 * $Date: 2020-08-24 14:56:17 +0200 (Mon, 24 Aug 2020) $
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

/* Define to prevent recursive inclusion -------------------------------------*/
#include "netx_drv.h"
#ifndef NETX_DRV_I2C_H_
#define NETX_DRV_I2C_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*! \addtogroup netX_Driver
 * \{
 */

/*!
 * \addtogroup I2C
 * \{
 */

/*!***************************************************************************/

/*!
 * \brief This definition encodes the addressing scheme for 10bit i2c addresses.
 */
#define DRV_I2C_ADDRESSING_10_BIT 0xF800u

/*!
 * \brief This definition encodes the mask for the addressing scheme for 10bit i2c addresses.
 */
#define DRV_I2C_ADDRESSING_10_BIT_MSK 0x03FFu

/*!
 * \brief This definition encodes the addressing scheme for 7bit i2c addresses.
 */
#define DRV_I2C_ADDRESSING_7_BIT  0x8000u

/*!
 * \brief This definition encodes the mask for the addressing scheme for 7bit i2c addresses.
 */
#define DRV_I2C_ADDRESSING_7_BIT_MSK 0x007Fu

/*!
 * \brief This definition encodes the addressing scheme for Master accesses for highspeed i2c modes.
 */
#define DRV_I2C_ADDRESSING_MASTER  0x8400u

/*!
 * \brief This definition encodes the mask for the addressing part for Master accesses for highspeed i2c modes.
 */
#define DRV_I2C_ADDRESSING_MASTER_CODE_ADDRESS_MSK 0x007Fu

/*!
 * \brief This definition encodes the mask for the master code for Master accesses for highspeed i2c modes.
 */
#define DRV_I2C_ADDRESSING_MASTER_CODE_ID_MSK 0x0380u


/*!
 * \brief The I2C speed modes.
 *
 * They are used to identify the i2c devices in the driver context.
 */
typedef enum DRV_I2C_SPEED_MODE_Etag
{
  DRV_I2C_SPEED_MODE_FS_50k = 0x00ul,/*!< The I2C drivers default speed with 50kbit/s.*/
  DRV_I2C_SPEED_MODE_DEFAULT = DRV_I2C_SPEED_MODE_FS_50k,/*!< The I2C drivers default speed.*/
  DRV_I2C_SPEED_MODE_FS_100k = 0x01ul,/*!< The I2C drivers standard speed with 100kbit/s.*/
  DRV_I2C_SPEED_MODE_FS_STANDARD = DRV_I2C_SPEED_MODE_FS_100k,/*!< The I2C drivers standard mode speed.*/
  DRV_I2C_SPEED_MODE_FS_200k = 0x02ul,/*!< The I2C driver speed is set to 200kbit/s.*/
  DRV_I2C_SPEED_MODE_FS_400k = 0x03ul,/*!< The I2C driver speed is set to 400kbit/s.*/
  DRV_I2C_SPEED_MODE_FS_FAST = DRV_I2C_SPEED_MODE_FS_400k,/*!< The I2C drivers fast mode speed.*/
  DRV_I2C_SPEED_MODE_HS_800k = 0x04ul,/*!< The I2C driver speed is set to 800kbit/s.*/
  DRV_I2C_SPEED_MODE_HS_1_2M = 0x05ul,/*!< The I2C driver speed is set to 1.2Mbit/s.*/
  DRV_I2C_SPEED_MODE_HS_1_7M = 0x06ul,/*!< The I2C driver speed is set to 1.7Mbit/s.*/
  DRV_I2C_SPEED_MODE_HS_3_4M = 0x07ul,/*!< The I2C driver speed is set to 3.4Mbit/s.*/
  DRV_I2C_SPEED_MODE_HS_HIGH_SPEED = DRV_I2C_SPEED_MODE_HS_3_4M,/*!< The I2C drivers standard speed.*/
  DRV_I2C_SPEED_MODE_MIN = DRV_I2C_SPEED_MODE_FS_50k, /*!< The I2C drivers last device ID.*/
  DRV_I2C_SPEED_MODE_MAX = DRV_I2C_SPEED_MODE_HS_3_4M /*!< The I2C drivers last device ID.*/
} DRV_I2C_SPEED_MODE_E;

/*!
 * \brief The I2C speed modes.
 *
 * They are used to identify the i2c devices in the driver context.
 */
typedef enum DRV_I2C_COMMAND_Etag
{
  DRV_I2C_COMMAND_START = 0x00ul,/*!< Generate (r)START-condition.*/
  DRV_I2C_COMMAND_S_AC = 0x01ul,/*!< Acknowledge-polling: generate up to acpollmax+1 START-sequences (until acknowledged by slave).*/
  DRV_I2C_COMMAND_S_AC_T = 0x02ul,/*!< Run S_AC, then transfer tsize+1 bytes from/to master FIFO. Not to be continued.*/
  DRV_I2C_COMMAND_S_AC_TC = 0x03ul,/*!< Run S_AC, then transfer tsize+1 bytes from/to master FIFO. To be continued.*/
  DRV_I2C_COMMAND_CT = 0x04ul,/*!< Continued transfer not to be continued.*/
  DRV_I2C_COMMAND_CTC = 0x05ul,/*!< Continued transfer to be continued.*/
  DRV_I2C_COMMAND_STOP = 0x06ul,/*!< Generate STOP-condition.*/
  DRV_I2C_COMMAND_IDLE = 0x07ul, /*!< Nothing to do, last command finished, break current command.*/
} DRV_I2C_COMMAND_E;

/*!
 * \brief The watermark levels of the i2c.
 *
 * An interrupt is generated if the fill level is larger as the value.
 * With rvalue = lvalue+1 for reset differentiation.
 */
typedef enum DRV_I2C_WATERMARK_Etag
{
  DRV_I2C_WATERMARK_UNINITIALIZED = 0x00ul, /*!< Defines the reset value.*/
  DRV_I2C_WATERMARK_0 = 0x00ul, /*!< Watermark level of 0. */
  DRV_I2C_WATERMARK_1 = 0x01ul, /*!< Watermark level of 1. */
  DRV_I2C_WATERMARK_2 = 0x02ul, /*!< Watermark level of 2. */
  DRV_I2C_WATERMARK_3 = 0x03ul, /*!< Watermark level of 3. */
  DRV_I2C_WATERMARK_4 = 0x04ul, /*!< Watermark level of 4. */
  DRV_I2C_WATERMARK_5 = 0x05ul, /*!< Watermark level of 5. */
  DRV_I2C_WATERMARK_6 = 0x06ul, /*!< Watermark level of 6. */
  DRV_I2C_WATERMARK_7 = 0x07ul, /*!< Watermark level of 7. */
  DRV_I2C_WATERMARK_8 = 0x08ul, /*!< Watermark level of 8. */
  DRV_I2C_WATERMARK_DEFAULT = DRV_I2C_WATERMARK_8, /*!< Watermark level of 8 is the default value used. */
  DRV_I2C_WATERMARK_9 = 0x09ul, /*!< Watermark level of 8. */
  DRV_I2C_WATERMARK_10 = 0x0aul, /*!< Watermark level of 10. */
  DRV_I2C_WATERMARK_11 = 0x0bul, /*!< Watermark level of 11. */
  DRV_I2C_WATERMARK_12 = 0x0cul, /*!< Watermark level of 12. */
  DRV_I2C_WATERMARK_13 = 0x0dul, /*!< Watermark level of 13. */
  DRV_I2C_WATERMARK_14 = 0x0eul, /*!< Watermark level of 14. */
  DRV_I2C_WATERMARK_15 = 0x0ful, /*!< Watermark level of 15. */
  DRV_I2C_WATERMARK_MAX = DRV_I2C_WATERMARK_15 /*!< Maximum value of the enumeration used for boundary checks. */
} DRV_I2C_WATERMARK_E;

/*!
 * \brief Enumeration of i2c device states.
 */
typedef enum DRV_I2C_STATE_Etag
{
  DRV_I2C_STATE_MASTER_FIFO_LEVEL_1B  = 0x00000001ull, /*!< Bit 0. */
  DRV_I2C_STATE_MASTER_FIFO_LEVEL_2B  = 0x00000002ull, /*!< Bit 1. */
  DRV_I2C_STATE_MASTER_FIFO_LEVEL_4B  = 0x00000004ull, /*!< Bit 2. */
  DRV_I2C_STATE_MASTER_FIFO_LEVEL_8B  = 0x00000008ull, /*!< Bit 3. */
  DRV_I2C_STATE_MASTER_FIFO_LEVEL_16B = 0x00000010ull, /*!< Bit 4. */
  DRV_I2C_STATE_RESERVED0             = 0x00000020ull, /*!< Bit 5. */
  DRV_I2C_STATE_MASTER_FIFO_EMPTY     = 0x00000040ull, /*!< Bit 6. */
  DRV_I2C_STATE_MASTER_FIFO_FULL      = 0x00000080ull, /*!< Bit 7. */
  DRV_I2C_STATE_MASTER_FIFO_OVERFLOW  = 0x00000100ull, /*!< Bit 8. */
  DRV_I2C_STATE_MASTER_FIFO_UNDERRUN  = 0x00000200ull, /*!< Bit 9. */

  DRV_I2C_STATE_SLAVE_FIFO_LEVEL_1B   = 0x00000400ull, /*!< Bit 10. */
  DRV_I2C_STATE_SLAVE_FIFO_LEVEL_2B   = 0x00000800ull, /*!< Bit 11. */
  DRV_I2C_STATE_SLAVE_FIFO_LEVEL_4B   = 0x00001000ull, /*!< Bit 12. */
  DRV_I2C_STATE_SLAVE_FIFO_LEVEL_8B   = 0x00002000ull, /*!< Bit 13. */
  DRV_I2C_STATE_SLAVE_FIFO_LEVEL_16B  = 0x00004000ull, /*!< Bit 14. */
  DRV_I2C_STATE_RESERVED1             = 0x00008000ull, /*!< Bit 15. */
  DRV_I2C_STATE_SLAVE_FIFO_EMPTY      = 0x00010000ull, /*!< Bit 16. */
  DRV_I2C_STATE_SLAVE_FIFO_FULL       = 0x00020000ull, /*!< Bit 17. */
  DRV_I2C_STATE_SLAVE_FIFO_OVERFLOW   = 0x00040000ull, /*!< Bit 18. */
  DRV_I2C_STATE_SLAVE_FIFO_UNDERRUN   = 0x00080000ull, /*!< Bit 19. */

  DRV_I2C_STATE_BUS_MASTER            = 0x00100000ull, /*!< Bit 20. */
  DRV_I2C_STATE_READ_TRANSFER         = 0x00200000ull, /*!< Bit 21. */
  DRV_I2C_STATE_STARTED               = 0x00400000ull, /*!< Bit 22. */
  DRV_I2C_STATE_LOCAL_SLAVE_ACCESSED  = 0x00800000ull, /*!< Bit 23. */
  DRV_I2C_STATE_LAST_BYTE_ACKED       = 0x01000000ull, /*!< Bit 24. */
  DRV_I2C_STATE_READ_TRANSFER_ACKED   = 0x02000000ull, /*!< Bit 25. */
  DRV_I2C_STATE_GENERAL_CALL_ACKED    = 0x04000000ull, /*!< Bit 26. */
  DRV_I2C_STATE_10BIT_ADD_ACKED       = 0x08000000ull, /*!< Bit 27. */
  DRV_I2C_STATE_TIMEOUT_OCCURRED      = 0x10000000ull, /*!< Bit 28. */
  DRV_I2C_STATE_RESERVED2             = 0x20000000ull, /*!< Bit 29. */
  DRV_I2C_STATE_SDA_HIGH              = 0x40000000ull, /*!< Bit 30. */
  DRV_I2C_STATE_SCL_HIGH              = 0x80000000ull,  /*!< Bit 31. */

  DRV_I2C_STATE_MASTER_CMD_READ       = 0x00000001ull<<32,  /*!< The master is performing the READ command. */
  DRV_I2C_STATE_MASTER_CMD_START      = 0x00000002ull<<32,  /*!< The master is performing the START command. */
  DRV_I2C_STATE_MASTER_CMD_S_AC       = 0x00000004ull<<32,  /*!< The master is performing the START and ACK polling command. */
  DRV_I2C_STATE_MASTER_CMD_S_AC_T     = 0x00000008ull<<32,  /*!< The master is performing the START and ACK polling with termination command. */
  DRV_I2C_STATE_MASTER_CMD_S_AC_TC    = 0x00000010ull<<32,  /*!< The master is performing the START and ACK polling with continue command. */
  DRV_I2C_STATE_MASTER_CMD_CT         = 0x00000020ull<<32,  /*!< The master is performing the termination command. */
  DRV_I2C_STATE_MASTER_CMD_CTC        = 0x00000040ull<<32,  /*!< The master is performing the continue command. */
  DRV_I2C_STATE_MASTER_CMD_STOP       = 0x00000080ull<<32,  /*!< The master is performing the STOP command. */
  DRV_I2C_STATE_MASTER_CMD_IDLE       = 0x00000100ull<<32,  /*!< The master is in IDLE. */
  DRV_I2C_STATE_RESERVED3             = 0x00000200ull<<32,  /*!< Reserved value for future usage. */
  DRV_I2C_STATE_RESERVED4             = 0x00000400ull<<32,  /*!< Reserved value for future usage. */
  DRV_I2C_STATE_RESERVED5             = 0x00000800ull<<32,  /*!< Reserved value for future usage. */
  DRV_I2C_STATE_RESERVED6             = 0x00001000ull<<32,  /*!< Reserved value for future usage. */
  DRV_I2C_STATE_RESERVED7             = 0x00002000ull<<32,  /*!< Reserved value for future usage. */
  DRV_I2C_STATE_RESERVED8             = 0x00004000ull<<32,  /*!< Reserved value for future usage. */
  DRV_I2C_STATE_RESERVED9             = 0x00008000ull<<32,  /*!< Reserved value for future usage. */

  DRV_I2C_STATE_MASTER_TRANSFER_SIZE_1B   = 0x00010000ull<<32,  /*!< Transfer size bit mask. */
  DRV_I2C_STATE_MASTER_TRANSFER_SIZE_2B   = 0x00020000ull<<32,  /*!< Transfer size bit mask. */
  DRV_I2C_STATE_MASTER_TRANSFER_SIZE_4B   = 0x00040000ull<<32,  /*!< Transfer size bit mask. */
  DRV_I2C_STATE_MASTER_TRANSFER_SIZE_8B   = 0x00080000ull<<32,  /*!< Transfer size bit mask. */
  DRV_I2C_STATE_MASTER_TRANSFER_SIZE_16B  = 0x00100000ull<<32,  /*!< Transfer size bit mask. */
  DRV_I2C_STATE_MASTER_TRANSFER_SIZE_32B  = 0x00200000ull<<32,  /*!< Transfer size bit mask. */
  DRV_I2C_STATE_MASTER_TRANSFER_SIZE_64B  = 0x00400000ull<<32,  /*!< Transfer size bit mask. */
  DRV_I2C_STATE_MASTER_TRANSFER_SIZE_128B = 0x00800000ull<<32,  /*!< Transfer size bit mask. */
  DRV_I2C_STATE_MASTER_TRANSFER_SIZE_256B = 0x01000000ull<<32,  /*!< Transfer size bit mask. */
  DRV_I2C_STATE_MASTER_TRANSFER_SIZE_512B = 0x02000000ull<<32,  /*!< Transfer size bit mask. */
  DRV_I2C_STATE_RESERVED10                = 0x04000000ull<<32,  /*!< Reserved value for future usage. */
  DRV_I2C_STATE_RESERVED11                = 0x08000000ull<<32,  /*!< Reserved value for future usage. */
  DRV_I2C_STATE_RESERVED12                = 0x10000000ull<<32,  /*!< Reserved value for future usage. */
  DRV_I2C_STATE_RESERVED13                = 0x20000000ull<<32,  /*!< Reserved value for future usage. */
  DRV_I2C_STATE_RESERVED14                = 0x40000000ull<<32,  /*!< Reserved value for future usage. */
  DRV_I2C_STATE_RESERVED15                = 0x80000000ull<<32,  /*!< Reserved value for future usage. */
} DRV_I2C_STATE_E;

/*!
 * \brief Timeout detection
 */
typedef enum DRV_I2C_TIMEOUT_DETECTION_Etag
{
  DRV_I2C_TIMEOUT_DETECTION_ENABLED = 0x00u, /*!< The Timeout detection is enabled. */
  DRV_I2C_TIMEOUT_DETECTION_DISABLED = 0x01u /*!< The Timeout detection is disabled. */
} DRV_I2C_TIMEOUT_DETECTION_E;

/*!
 * \brief Read/Write Flag
 */
typedef enum DRV_I2C_RW_FLAG_Etag
{
  DRV_I2C_RW_FLAG_WRITE   = 0x00u, /*!< Write mode. */
  DRV_I2C_RW_FLAG_READ    = 0x01u, /*!< Read mode. */
  DRV_I2C_RW_FLAG_UNKNOWN = 0x02u
} DRV_I2C_RW_FLAG_E;

/*!
 * \brief Addressing Scheme enumeration for the DRV_I2C_ADDRESS_T.
 */
typedef enum DRV_I2C_ADDRESS_SCHEME_Ttag
{
  DRV_I2C_ADDRESS_10_BIT=0x3Eu, /*!< 10 bit addressing id. */
  DRV_I2C_ADDRESS_MASTER_CODE=0x21u, /*!< Master code id. */
  DRV_I2C_ADDRESS_7_BIT=0x20u /*!< 7 bit addressing id. */
} DRV_I2C_ADDRESS_SCHEME_E;

/*!
 * \brief Type definiton of a i2c address.
 *
 * \section i2c_addressing_scheme I2C Addressing Scheme
 * Target addresses are handled as followed:
 * \verbatim 0b111110AAAAAAAAAAus \endverbatim is treated as 10-bit addressing.
 * \verbatim 0b100001MMMAAAAAAAus \endverbatim is treated as master code with 7-bit addessing.
 * \verbatim 0b100000NNNAAAAAAAus \endverbatim is treated as 7-bit addessing.
 * \verbatim 0b0NNNNNNNNNNNNNNNus \endverbatim Indicates an invalid address.
 * To make use of it, there is the macros \verbatim DRV_I2C_ADDRESSING_10_BIT = 0b0111100000000000us \endverbatim
 * and \verbatim DRV_I2C_ADDRESSING_7_BIT = 0b1000000000000000us \endverbatim or one might use the
 * structure defined.
 * The Master Code ID has to be put into the upper three bits of the uDeviceAddress.
 *
 * Symbol | Description
 * -------|------------
 * 0b     | Start of binary sequence
 * N      | Ignore value
 * M      | Master code id
 * A      | Address bit
 * ~      | Bitwise NOT
 * us     | Unsigned short (uint16_t)
 */
typedef struct DRV_I2C_ADDRESS_Ttag
{
  uint16_t uDeviceAddress:10;/*!< The device address in 10 bit. In case of Master code the 7bit with master code. */
  DRV_I2C_ADDRESS_SCHEME_E eAddressingScheme:6;/*!< The addressing scheme used. */
}DRV_I2C_ADDRESS_T;

/*!
 * \brief This enumeration is used to define if the arbitration context will end after the command or continue.
 */
typedef enum DRV_I2C_CONT_Etag
{
    DRV_I2C_CONT_CONTINUOUS=0x1u,/*!< If the arbitration context shall be kept up for the next transmission. */
    DRV_I2C_CONT_END=0x0u,/*!< If the arbitration context shall end with this transmission. */
  }DRV_I2C_CONT_E;

  /*!
   * \brief This define is used to set the continuous flag.
   */
#define DRV_I2C_CONTINUOUS_FLAG (DRV_I2C_ADDRESS_T) {0x8000u}

/*!
 * \brief The matching union to DRV_I2C_ADDRESS_T.
 *
 * See \ref i2c_addressing_scheme for further information.
 */
typedef union DRV_I2C_ADDRESS_Utag
{
  uint32_t val;/*!< The value representation. */
  uint8_t array[4];/*!< The array representation. */
  DRV_I2C_ADDRESS_T bf;/*!< The bitfield representation. */
}DRV_I2C_ADDRESS_U;

/*!
 * \brief The configuration of the driver.
 *
 * This structure shall be modified to change configuration parameters.
 * It is evaluated during the init routine and used during the handles lifetime.
 * The structure shall not be modified after initialization of the device.
 */
typedef struct Drv_I2C_CONFIGURATION_Ttag
{
  DRV_I2C_DEVICE_ID_E eDeviceID; /*!< The device to be used*/
  DRV_OPERATION_MODE_E eOperationMode; /*!< Which programming method (DMA/IRQ/POLL) is used.*/
  DRV_I2C_SPEED_MODE_E eSpeedMode; /*!< The speed operation mode.*/
  uint16_t sAckPollMaximum; /*!< Value that stores the count of how often the device will try to get an acknowledge from the addressed slave(s).*/
  DRV_I2C_ADDRESS_T tSlaveAddress; /*!< Slave address format including \ref i2c_addressing_scheme.*/
  DRV_I2C_WATERMARK_E eMstRxFifoWatermark; /*!< Fifo size of the master rx.*/
  DRV_I2C_WATERMARK_E eMstTxFifoWatermark; /*!< Fifo size of the master tx.*/
  DRV_I2C_WATERMARK_E eMstTxFifoRefillLevel; /*!< Fifo size of the master tx.*/
  DRV_I2C_WATERMARK_E eSlvRxFifoWatermark; /*!< Fifo size of the slave rx.*/
  DRV_I2C_WATERMARK_E eSlvTxFifoWatermark; /*!< Fifo size of the slave tx.*/
  DRV_I2C_WATERMARK_E eSlvTxFifoRefillLevel; /*!< Fifo size of the slave tx.*/
  DRV_CALLBACK_F fnMasterCompleteCallback; /*!< The callback used if master transaction is completed.*/
  void* pMasterCompleteCallbackHandle; /*!< The handle associated with the master complete callback.*/
  DRV_CALLBACK_F fnSlaveCompleteCallback; /*!< The callback used if slave transaction is completed.*/
  void* pSlaveCompleteCallbackHandle; /*!< The handle associated with the slave complete callback.*/
  DRV_CALLBACK_F fnArbitrationLostCallback; /*!< The callback used if the arbitration got lost.*/
  void* pArbitrationLostCallbackHandle; /*!< The handle associated with the arbitration callback.*/
  DRV_CALLBACK_F fnErrorCallback; /*!< The callback used if an error occurred.*/
  void* pErrorCallbackHandle; /*!< The handle associated with the error callback.*/
  uint32_t ulDriverTimeout; /*!< Timeout used in the drivers Polling Mode. */
} DRV_I2C_CONFIGURATION_T;

/*!
 * \brief The handle of the driver.
 *
 * The configuration SHALL be changed before initializing the device and shall not be changed
 * afterwards.
 * The rest of it SHALL not be modified outside of the driver, even if it appears to be possible.
 */
typedef struct DRV_I2C_HANDLE_Ttag
{
  DRV_I2C_DEVICE_T* ptDevice; /*!< \private The GPIO device register as bitfield and value unions.*/
  DRV_I2C_CONFIGURATION_T tConfiguration; /*!< The configuration of the I2C device, shall be edited before the init. */
  DRV_I2C_CONT_E eCT; /*!< \private The flag if the transaction/arbitration context will be continued or not*/
  DRV_I2C_RW_FLAG_E eRwFlagSlave; /*!< \private The flag defining the next io direction.*/
  DRV_I2C_RW_FLAG_E eRwFlagMaster; /*!< \private The flag defining the next io direction.*/
  uint64_t ullFrameStartTick;/*!< \private Used for timeout detection. */
  void * volatile MstBuffer;/*!< \private Master RxTx buffer */
  size_t volatile MstBufferSize;/*!< \private Master RxTx size */
  size_t volatile MstBufferCounter;/*!< \private Master RxTx counter */
  void * volatile SlvBuffer;/*!< \private Slave RxTx buffer */
  size_t volatile SlvBufferSize;/*!< \private Slave RxTx size */
  size_t volatile SlvBufferCounter;/*!< \private Slave RxTx counter */
  DRV_LOCK_T tLock; /*!< \private The drivers locking variable used as internal mutex*/
} DRV_I2C_HANDLE_T;

/*!
 * \brief Initializes the i2c device and handle by the given configuration.
 * \memberof DRV_I2C_HANDLE_T
 */
DRV_STATUS_E DRV_I2C_Init(DRV_I2C_HANDLE_T * const ptDriver);
/*!
 * \brief Deinitializes the i2c device and handle.
 * \memberof DRV_I2C_HANDLE_T
 */
DRV_STATUS_E DRV_I2C_DeInit(DRV_I2C_HANDLE_T * const ptDriver);
/*!
 * \brief Transmits the given ID in the given speed as master code on the bus.
 * \memberof DRV_I2C_HANDLE_T
 */
DRV_STATUS_E DRV_I2C_HS_Master_Code(DRV_I2C_HANDLE_T * const ptDriver, DRV_I2C_ADDRESS_T tTargetAddress, DRV_I2C_SPEED_MODE_E eSpeed);
/*!
 * \brief Performs a transmission of the given size of the data array via the given i2c.
 * \memberof DRV_I2C_HANDLE_T
 */
DRV_STATUS_E DRV_I2C_MasterTransmit(DRV_I2C_HANDLE_T * const ptDriver, DRV_I2C_ADDRESS_T ulTargetAddress, uint8_t const * const data, size_t size, DRV_I2C_CONT_E eCont);
/*!
 * \brief Receives the given size and stores it into data via the given i2c.
 * \memberof DRV_I2C_HANDLE_T
 */
DRV_STATUS_E DRV_I2C_MasterReceive(DRV_I2C_HANDLE_T * const ptDriver, DRV_I2C_ADDRESS_T ulTargetAddress, uint8_t * const data, size_t size, DRV_I2C_CONT_E eCont);
/*!
 * \brief Performs a transmission of the given size of the data array via the given i2c.
 * \memberof DRV_I2C_HANDLE_T
 */
DRV_STATUS_E DRV_I2C_SlaveTransmit(DRV_I2C_HANDLE_T * const ptDriver, uint8_t const * const data, size_t size);
/*!
 * \brief Receives the given size and stores it into data via the given i2c.
 * \memberof DRV_I2C_HANDLE_T
 */
DRV_STATUS_E DRV_I2C_SlaveReceive(DRV_I2C_HANDLE_T * const ptDriver, uint8_t * const data, size_t size);
/*!
 * \brief Aborts any transmissions on the given i2c.
 * \memberof DRV_I2C_HANDLE_T
 */
DRV_STATUS_E DRV_I2C_Abort(DRV_I2C_HANDLE_T * const ptDriver);
/*!
 * \brief Clears the bus by sending nine clock pulses.
 * \memberof DRV_I2C_HANDLE_T
 */
DRV_STATUS_E DRV_I2C_Clear(DRV_I2C_HANDLE_T * const ptDriver, const size_t loops);
/*!
 * \brief The driver and device state are returned by this function.
 * \memberof DRV_I2C_HANDLE_T
 */
DRV_STATUS_E DRV_I2C_GetState(DRV_I2C_HANDLE_T * const ptDriver, DRV_I2C_STATE_E * const ptState);

/*! \} *//* End of group UART */

/*! \} *//* End of group netx_Driver */

#ifdef __cplusplus
}
#endif

#endif /* NETX_DRV_I2C_H_ */
