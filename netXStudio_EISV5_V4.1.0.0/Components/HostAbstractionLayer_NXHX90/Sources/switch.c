/**
 * @copyright (c) 2022  MKS Instruments, Inc. All rights reserved.
 * The information herein is confidential, and includes
 * trade secrets and other proprietary information.
 *
 * @file switch.c
 * @brief rotary switch
 */

/** @defgroup Rotary_Switches Rotary Switches
  * @ingroup Rotary_Switches
  *
  * @brief Rotary Switch API
  *
  * @details
  *
  * @par Modification History
  *    Date     | Initials | Notes
  *    -------- | -------- | -----------------------------------------------------------------------
  *    09/28/22 |   RQ     | Initial
  *    10/17/22 |   JR     | cleanup
  *
  * @{
 */

/*####################################################################################
 *  ____   ___ _____  _    ______   __  ______        _____ _____ ____ _   _
 * |  _ \ / _ \_   _|/ \  |  _ \ \ / / / ___\ \      / /_ _|_   _/ ___| | | |
 * | |_) | | | || | / _ \ | |_) \ V /  \___ \\ \ /\ / / | |  | || |   | |_| |
 * |  _ <| |_| || |/ ___ \|  _ < | |    ___) |\ V  V /  | |  | || |___|  _  |
 * |_| \_\\___/ |_/_/   \_\_| \_\|_|   |____/  \_/\_/  |___| |_| \____|_| |_|
 *
 #####################################################################################*/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "netx_drv.h"
#include "app_application.h"
#include "hostAbstractionLayer.h"
#include "HAL.h"
#include "i2c.h"
#include "mks_syscounter.h"
#include "sys.h"
#include "shell.h"
#include "fi.h"

//We will be reading Port 0 and Port 1 in one i2c transaction
#define FI_ROTARY_SWITCH_DATA_SIZE    ( 2 )

#define FI_ROTARY_SWITCH_UNIT 0
#define FI_ROTARY_SWITCH_TEN 1
#define FI_ROTARY_SWITCH_HUNDRED 2
#define FI_ROTARY_SWITCH_MAX 3

/* I2C slave address definitions */
#define SLAVE_SWITCH_ADDRESS  0x20

// Switch is active low.
#define SWITCH_VAL(x) ((x^0xf)&0xf)
#define SWITCH_ERROR 0xFFFF

// i2c slave address
static DRV_I2C_ADDRESS_T g_RotarySlaveAddress = {SLAVE_SWITCH_ADDRESS, DRV_I2C_ADDRESS_7_BIT};
typedef enum {
  kPCA9555_RD_INPUTPORT_0,
  kPCA9555_RD_INPUTPORT_1,
  kPCA9555_WR_OUTPUTPORT_0,
  kPCA9555_WR_OUTPUTPORT_1,
  kPCA9555_POL_INV_PORT_0,
  kPCA9555_POL_INV_PORT_1,
  kPCA9555_CFG_PORT_0,
  kPCA9555_CFG_PORT_1
} kPCA9555_Cmdbytes;

/*****************************************************************************/
/*! I2C Variables                                                            */
/*****************************************************************************/
static DRV_I2C_HANDLE_T  s_tI2C_SwitchHandle = {0};   /** Rotary Switch I2C handler declaration */

//heap container for data read by i2c
static uint8_t g_RotaryValues[FI_ROTARY_SWITCH_DATA_SIZE]={0};

//track if we have initialized the PCA or not...
static bool g_RotaryInitComplete = false;


/*****************************************************************************/
/*! configure the I2C peripheral 
 *   \return None                                                            */
/*****************************************************************************/
MksReturnCodes Rotary_I2C_Init()
{
  uint8_t cmd[2];
  
  if (g_RotaryInitComplete == false)
  {
    /** configure the I2C Rotary Switch peripheral */
    s_tI2C_SwitchHandle.tConfiguration.sAckPollMaximum = 10;
    s_tI2C_SwitchHandle.tConfiguration.eDeviceID = DRV_I2C_DEVICE_ID_I2C0;
    s_tI2C_SwitchHandle.tConfiguration.eOperationMode = DRV_OPERATION_MODE_POLL;
    s_tI2C_SwitchHandle.tConfiguration.eSpeedMode = DRV_I2C_SPEED_MODE_FS_100k;
    s_tI2C_SwitchHandle.tConfiguration.tSlaveAddress.eAddressingScheme = DRV_I2C_ADDRESS_7_BIT;
    s_tI2C_SwitchHandle.tConfiguration.eMstTxFifoWatermark = DRV_I2C_WATERMARK_8;
    s_tI2C_SwitchHandle.tConfiguration.eMstRxFifoWatermark = DRV_I2C_WATERMARK_8;
    s_tI2C_SwitchHandle.tConfiguration.eSlvTxFifoWatermark = DRV_I2C_WATERMARK_8;
    s_tI2C_SwitchHandle.tConfiguration.eSlvRxFifoWatermark = DRV_I2C_WATERMARK_8;    
    
    /** Init the I2c device */
    if(DRV_OK != DRV_I2C_Init(&s_tI2C_SwitchHandle))
    {
      return kMksOperationFailed;
    }
  }

  // Invert all bits on Port 0
  cmd[0] = kPCA9555_POL_INV_PORT_0 ;
  cmd[1] = 0xFF;
  
  if (false == I2C_Send (&s_tI2C_SwitchHandle, g_RotarySlaveAddress, cmd, 2)) return kMksOperationFailed;
  
  // Invert all bits on Port 1
  cmd[0] = kPCA9555_POL_INV_PORT_1 ;
  cmd[1] = 0xFF;
  
  if (false == I2C_Send (&s_tI2C_SwitchHandle, g_RotarySlaveAddress, cmd, 2)) return kMksOperationFailed;
  
  // Start read from Input port 0
  cmd[0] = kPCA9555_RD_INPUTPORT_0;

  if (false == I2C_Send (&s_tI2C_SwitchHandle, g_RotarySlaveAddress, cmd, 1)) return kMksOperationFailed;
  
  g_RotaryInitComplete = true;

  g_FI_unit_stats.base_ip = FI_DEFAULT_IP_ADDR;
  
  return kMksOk;
}

/***************************************************************************************************
*! \brief  Update / Read the phisical Rotary Switches.
*
*  \details  This function Reads Rotary Switch 0, 1 and 2 to gather the switch and 3 upper octets of the IP address
*
*  \return true = success , false = error
*****************************************************************************************M*********/
bool SWITCH_update(void)
{
  uint8_t           FcContent[FI_ROTARY_SWITCH_DATA_SIZE] = {0};
  
  if( !g_RotaryInitComplete ){
    //printf("\r\n IO Expander update Failed. Init not completed! ");
    return false;
  }

  if (false == I2C_Receive (&s_tI2C_SwitchHandle, g_RotarySlaveAddress, FcContent, FI_ROTARY_SWITCH_DATA_SIZE)) return false;   
  
  DRV_IRQ_Disable();
  // copy to local memory
  memcpy( g_RotaryValues, FcContent, FI_ROTARY_SWITCH_DATA_SIZE );
  printf("Rotary Switch: %d%d%d\r\n", g_RotaryValues[1] & 0x0F, g_RotaryValues[0] >> 4, g_RotaryValues[0] & 0x0F);
  DRV_IRQ_Enable();

  return true;
}

/***************************************************************************************************
*! \brief  Retrieve the i2c raw rotary switch value from memory.
*
*  \details
*  This function returns the raw switch data value
*
*   \param inRotaryIndex   [in]  - rotary index
*   \param outDataPt [out]  - pointer to data to return value
*
*   \return true = success , false = error
**************************************************************************************************/
static bool get_rotary_value(uint8_t inRotaryIndex, uint8_t *outDataPt)
{
  uint8_t unRotaryDataIndex;
  uint8_t unNibbleShift;

  if ( !g_RotaryInitComplete || inRotaryIndex >= FI_ROTARY_SWITCH_MAX)
    return false;
  
  /* convert nibble into switch offset */
  unRotaryDataIndex = inRotaryIndex / 2;
  unNibbleShift = (inRotaryIndex % 2) ? 4 : 0;
    
  *outDataPt = (g_RotaryValues[unRotaryDataIndex] >> unNibbleShift) & 0x0F;

  return true;
}

/***************************************************************************************************
*! \brief  Get all switch values from memory.
*
*  \details
*  This function get the rotary switch value stored in the local memory
* 
*  \return uint16_t switch value or SWITCH_ERROR 
**************************************************************************************************/
uint16_t SWITCH_get_value(void)
{
  uint16_t switchval = 0;
  uint8_t switch_digit = 0;

  // get units value
  if (false == get_rotary_value(FI_ROTARY_SWITCH_UNIT, &switch_digit)) return SWITCH_ERROR;
  // convert physical value to actual value
  switchval += switch_digit;  
  // get tens value
  if (false == get_rotary_value(FI_ROTARY_SWITCH_TEN, &switch_digit)) return SWITCH_ERROR;
  // convert physical value to actual value, combine tens
  switchval += switch_digit * 10;  
  // get hundreds value
  if (false == get_rotary_value(FI_ROTARY_SWITCH_HUNDRED, &switch_digit)) return SWITCH_ERROR;
  // convert physical value to actual value, combine hundreds
  switchval += switch_digit * 100;  
  
  return switchval;
}

/***************************************************************************************************
*! \brief  Is switch set for DHCP config
*
*  \details  
*  This functions determines the mode of the device based on the rotary switch settings
*  \param usSwitchValue [in] - rotary switch value
*  \param pusMode [out] - returns mode of the device
*  
*  \return bool success/failure
**************************************************************************************************/
bool SWITCH_set_mode(uint16_t usSwitchValue)
{  
  bool bRet = false;

  g_FI_unit_stats.mode = 0;
  
  // DHCP mode
  if ((usSwitchValue == 0) || 
      (usSwitchValue >= 255 && usSwitchValue <= 300) ||
      (usSwitchValue >= 555 && usSwitchValue <= 600) ||
      (usSwitchValue >= 855 && usSwitchValue <= 900))
  {
    g_FI_unit_stats.mode = FI_DHCP_MODE;
    // check WD state
    if (usSwitchValue <= 299)
    {
      g_FI_unit_stats.mode |= FI_WD_ENABLED;
    }
    bRet = true;
    PRINTF("DHCP mode - switch %d\r\n", usSwitchValue);
  }
  // Static IP
  else if ((usSwitchValue >= 1 && usSwitchValue <= 254) ||
           (usSwitchValue >= 301 && usSwitchValue <= 554) ||
           (usSwitchValue >= 601 && usSwitchValue <= 854))
  {
    if (usSwitchValue <= 254)
    {
      g_FI_unit_stats.mode = FI_STATIC_IP_MODE | FI_WD_ENABLED;
    }
    else
    {
       g_FI_unit_stats.mode = FI_STATIC_IP_MODE; 
    }    
    bRet = true;
    PRINTF("STATIC IP mode - switch %d\r\n", usSwitchValue);
  }
  // System Config Mode
  else if (((usSwitchValue >= 901) && (usSwitchValue <= 977)) ||
           ((usSwitchValue >= 979) && (usSwitchValue <= 999)))
  {
    g_FI_unit_stats.mode = FI_SYS_CFG_MODE;
    bRet = true;
    PRINTF("System Config mode - switch %d\r\n", usSwitchValue);
  }
  // must be 978, Manufacturing Config
  else if (usSwitchValue == FI_MFG_SWITCH_VALUE)
  {
    g_FI_unit_stats.mode = FI_MFG_CFG_MODE;
    bRet = true;
    PRINTF("Manufacturing Config mode - switch %d\r\n", usSwitchValue);
  }

  return bRet;
}

/***************************************************************************************************
*! \brief  Shell read memory switch values
*
*  \details  no argument
**************************************************************************************************/
uint32_t Shell_ReadSwitchValues( uint32_t argc, const char *argv[] )
{
  uint8_t ip;
  uint8_t i;
  bool	print_usage, shorthelp = false;
  
  print_usage = Shell_check_help_request(argc, argv, &shorthelp );

  if (print_usage)
  {
		if (shorthelp)  {
			printf("%s  Read memory switch values\r\n", argv[0]);
		} else  {
			printf("Usage: \'%s\' reads memory switch values.\r\n", argv[0]);
		}
  }
  else
  {
  
    for (i = FI_ROTARY_SWITCH_UNIT; i < FI_ROTARY_SWITCH_MAX; i++)
    {
      if (!get_rotary_value(i, &ip) )
      {
        printf("\r\n Failed to retrieve IP octet %d switch value!", i + 1);
      }
      else
      {
        printf("\r\n IP octet %d switch value : %u \r\n", i + 1, ip);      
      }
    }
  }

  return 0;
}

/***************************************************************************************************
*! \brief  Shell read hardware switch values
*
*  \details  no argument
**************************************************************************************************/
uint32_t Shell_Rotary_update_switches( uint32_t argc, const char *argv[] ){

  bool	print_usage, shorthelp = false;
  
  print_usage = Shell_check_help_request(argc, argv, &shorthelp );

  if (print_usage)
  {
		if (shorthelp)  {
			printf("%s  Read rotary switch values\r\n", argv[0]);
		} else  {
			printf("Usage: \'%s\' reads rotary switch values.\r\n", argv[0]);
		}
  }
  else
  {
    SWITCH_update();    
    Shell_ReadSwitchValues(0, NULL);
  }

  return 0;
}


/** @}*/
