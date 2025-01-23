#include "App_Application.h"
#include "App_PacketCommunication.h"

#include "sys.h"
#include "fi.h"
#include "device_id.h"
#include "switch.h"
#include "cifxerrors.h"
#include "EipAps_Public.h"
#include "cip_objects.h"
#include "watchdog.h"

#define NET_WD "EtherNet/IP Server::< Watchdog >"

/*****************************************************************************/
/*! Global Variables                                                         */
/*****************************************************************************/
sFactoryInterfaceUnitStats g_FI_unit_stats;

/*****************************************************************************/
/*! Private functions                                                        */
/*****************************************************************************/

/***************************************************************************************************
*! Function to display device info.
*   \param ptOmbConfigIdent   pointer to APP_DATA_T structure
***************************************************************************************************/
static void sys_DisplayDeviceInfo ()
{
  PRINTF("EtherNet/IP Device Info Block:" NEWLINE);
  PRINTF("Vendor Name      : %s" NEWLINE, DEVICE_VENDOR_NAME);
  PRINTF("Product Code     : %s" NEWLINE, SysProductCode);
  PRINTF("FW Revision      : %s" NEWLINE, SysHwVersionNumber);
  PRINTF("Vendor URL       : %s" NEWLINE, DEVICE_VENDOR_URL);
  PRINTF("Product Name     : %s" NEWLINE, DEVICE_PRODUCT_NAME);
  PRINTF("Model Name       : %s" NEWLINE, DEVICE_MODEL_NAME);
  PRINTF("Application Name : %s" NEWLINE, DEVICE_USE_APP_NAME);

  PRINTF("%s default value [%u ms Timeout Period]" NEWLINE, NET_WD, (unsigned int) getWatchdogInterval ());
  
  if (g_FI_unit_stats.mode & FI_WD_ENABLED)
  {
    PRINTF("%s Enabled" NEWLINE, NET_WD);
  }  
  else
  {
    PRINTF("%s Disabled" NEWLINE, NET_WD);
  }

  /* display Ethernet info for static mode */
  if (!(g_FI_unit_stats.mode & FI_DHCP_MODE))
  {
    // IP Address
    PRINTF("IP Address:        %d.%d.%d.%d\r\n", IPBYTES(g_tInterfaceConfig.ulIpAddr));
    // Network Mask
    PRINTF("Mask:              %d.%d.%d.%d\r\n", IPBYTES(g_tInterfaceConfig.ulSubnetMask));
    // Gateway
    PRINTF("Gateway:           %d.%d.%d.%d\r\n", IPBYTES(g_tInterfaceConfig.ulGatewayAddr));
  }
}

/***************************************************************************************************
*! Configure CIP objects
***************************************************************************************************/
static uint32_t sys_ConfigureCipObjects(uint16_t usSwitchValue)
{
  uint32_t ulRet = CIFX_NO_ERROR;

  /* configure TCP control register */
  if (g_FI_unit_stats.mode & FI_DHCP_MODE)
  {
    /* static IP address */
    g_ulConfigControl = EIP_TCP_CNTRL_DHCP;    
  }
  else if (g_FI_unit_stats.mode & FI_STATIC_IP_MODE)
  {    
    g_ulConfigControl = EIP_TCP_CNTRL_INTERN;

    g_tInterfaceConfig.ulIpAddr = g_FI_unit_stats.base_ip & 0xFFFFFF00;;
    g_tInterfaceConfig.ulIpAddr |= usSwitchValue % 300;
    g_tInterfaceConfig.ulSubnetMask = g_tInterfaceConfig.ulSubnetMask;
    g_tInterfaceConfig.ulGatewayAddr = g_tInterfaceConfig.ulGatewayAddr;
  }
  // System or Manufacturing Config modes
  else if (g_FI_unit_stats.mode & (FI_SYS_CFG_MODE | FI_MFG_CFG_MODE))
  {    
    // configure default IP address
    g_tInterfaceConfig.ulIpAddr = g_tInterfaceConfig.ulIpAddr;
    g_tInterfaceConfig.ulSubnetMask = g_tInterfaceConfig.ulSubnetMask;
    g_tInterfaceConfig.ulGatewayAddr = g_tInterfaceConfig.ulGatewayAddr;

    HOSTAL_Device_SetStatus(kLED_ID_HLT, HIL_MB_HEALTH_STATUS_DIAG_MODE);
  }
  // invalid mode
  else 
  {
    HOSTAL_Device_SetStatus(kLED_ID_HLT, HIL_MB_HEALTH_STATUS_FW_FAULT);
    /* turn it on */
    HOSTAL_Health_Led_Handle();
    return CIFX_INVALID_PARAMETER;
  }

  return ulRet;
}

/*****************************************************************************/
/*! Public functions                                                         */
/*****************************************************************************/
uint32_t SYS_InitConfig ()
{
  uint32_t ulRet = CIFX_NO_ERROR;

  /* set mode based on Rotary switches */
  SWITCH_update();
  
  uint16_t usSwitchValue = SWITCH_get_value ();  
	
  if (false == SWITCH_set_mode (usSwitchValue))
  {
    PRINTF("Invalid Switch Value %d\r\n", usSwitchValue);
    return CIFX_INVALID_PARAMETER;	
  }
	
  /* init max_temp */
  g_FI_unit_stats.user_max_temperature_in_counts = DEFAULT_MAX_TEMP;
  MfgInitParams();

  /* configure CIP objects */
  ulRet = sys_ConfigureCipObjects(usSwitchValue);

  /* display device info */
  sys_DisplayDeviceInfo();

  return ulRet;
}
