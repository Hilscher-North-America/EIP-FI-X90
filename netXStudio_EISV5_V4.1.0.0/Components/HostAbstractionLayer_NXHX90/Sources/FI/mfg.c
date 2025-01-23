#include "sys.h"
#include "FI.h"
#include "nvram.h"
#include "App_FileSystemPackets.h"
#include "App_SystemPackets.h"
#include "system_netx.h"
#include "HIL_Results.h"
#include "mks_mcu.h"
#include "cip_objects.h"

#define REGISTER_SIZE 2 /* 2 bytes */

struct {
	uint16_t u16Key;
	uint8_t	u8CalCommand;
	uint8_t	u8CalChannel;
	uint8_t	u8CalStatus;
	uint32_t  u32Data;  //CHIC-74
} slclManufacturingInformation;   

/* manufacturer system variables */
uint32_t SysSerialNumber = 0; 																	
char SysProductCode[6 * REGISTER_SIZE] = "AS01224G-01";	
char SysHwVersionNumber[6 * REGISTER_SIZE] = "1.0.0";		
uint8_t SysMacAddress[6] = {0x00,0x0B,0x17,0x10,0xFF,0xFF};

/*************************************************/
int MfgSetKey(uint16_t ui16Key)	{
	switch(ui16Key)
	{		
	case RESETKEY:	
	case FLASHAPP:
	case FLASHBOOT:  
	case OBRKEY:
	case NVSAVE:
	case CLEARKEY:
		// Valid keys to process
		break;

	default:
		return 0;
	}
	slclManufacturingInformation.u16Key = ui16Key;
	return 1;
}

/*************************************************/
void MfgInitParams()
{	
  uint32_t ulDataSize;
	int nRet;
	uint8_t enabled = 0;

	/* verify that file system is formatted */
	nRet = (int) App_SysPkt_IsFormatted(0);
	if (ERR_HIL_FS_NOT_AVAILABLE == nRet ||
		  ERR_HIL_NOT_AVAILABLE == nRet)
	{
		/* mfg mode - format the file system */
		nRet = (int) App_SysPkt_FileSystemInit(0, true);

		/* reset */
		if (CIFX_NO_ERROR == nRet)
		{
			mks_netX90_system_restart();
		}			
		else
		{
			printf("File system initialization failed! Error: %08X\r\n", nRet);
		}
	}

	/* read NVRAM */
	App_SysPkt_FileSystemRead(0, RAMCOPY, &ulDataSize);

	if (sizeof(RAMCOPY) != ulDataSize)
	{
	  printf ("NVRAM copy is not fully configured. NVRAM size %lu\r\n", ulDataSize);
	}

	for(int i = 0; i < ulDataSize; i++)
	{
		if (i % 16 == 0) printf("\r\n");
		printf ("%02X ", RAMCOPY[i]);
	}
	printf("\r\n");

	if (NV_OK != (nRet = NvReadRam(NV_HWREV, SysHwVersionNumber, sizeof(SysHwVersionNumber))))
		printf ("NVRAM HW Version Number not set! Err: %d\r\n", nRet);
  if (NV_OK != (nRet = NvReadRam(NV_SERNUMBER, &SysSerialNumber, NV_LONG)))
		printf ("NVRAM Serial Number not set! Err: %d\r\n", nRet);
  if (NV_OK != (nRet = NvReadRam(NV_MACADDRESS, SysMacAddress, NV_ETHMAC)))
		printf ("NVRAM MAC Address not set! Err: %d\r\n", nRet);
	if (NV_OK != (nRet = NvReadRam(NV_PRODUCT_CODE, SysProductCode, sizeof(SysProductCode))))
		printf ("NVRAM Product Code not set! Err: %d\r\n", nRet);
  if (NV_OK != (nRet = NvReadRam(NV_IPADDRESS, &g_tInterfaceConfig.ulIpAddr, NV_LONG)))
		printf ("NVRAM IP Address not set! Err: %d\r\n", nRet);
  if (NV_OK != (nRet = NvReadRam(NV_IPMASK, &g_tInterfaceConfig.ulSubnetMask, NV_LONG)))
		printf ("NVRAM IP Mask not set! Err: %d\r\n", nRet);
  if (NV_OK != (nRet = NvReadRam(NV_IPGATEWAY, &g_tInterfaceConfig.ulGatewayAddr, NV_LONG)))
		printf ("NVRAM IP Gateway not set! Err: %d\r\n", nRet);

  if (NV_OK != (nRet = NvReadRam(NV_BASE_IPADDRESS, &g_FI_unit_stats.base_ip, NV_LONG)))
		printf ("NVRAM Base IP Address not set! Err: %d\r\n", nRet);
  if (NV_OK != (nRet = NvReadRam(NV_HOT_CPU, &g_FI_unit_stats.user_max_temperature_in_counts, NV_SHORT)))
		printf ("NVRAM Max Temperature not set! Err: %d\r\n", nRet);
	if (NV_OK != (nRet = NvReadRam(NV_TEMP_INTERCEPT, &g_TemperatureIntercept, NV_LONG)))
		printf ("NVRAM Temperature Intercept Parameter not set! Err: %d\r\n", nRet);
	if (NV_OK != (nRet = NvReadRam(NV_TEMP_SLOPE, &g_TemperatureGradient, NV_LONG)))
		printf ("NVRAM Temperature Slope Parameter not set! Err: %d\r\n", nRet);	
		
	g_FI_unit_stats.aio_disabled = 1;
	if ( NV_OK == NvReadRam(NV_ANALOG_ENABLED, &enabled, NV_BYTE) ){
		if( enabled )
			g_FI_unit_stats.aio_disabled = 0;
	}
	else
		printf ("NVRAM Analog Enable Parameter not set! Err: %d\r\n", nRet);
}

/*************************************************/
void MfgSaveParams(uint8_t obr)
{	
	uint8_t enabled = 0;
	
	/* out of box reset */
	if(obr)
	{
		/* delete nvram */
		App_SysPkt_FileSystemDelete(0);

		g_tInterfaceConfig.ulIpAddr = FI_DEFAULT_IP_ADDR;
		g_tInterfaceConfig.ulSubnetMask = FI_DEFAULT_NETMASK;
		g_tInterfaceConfig.ulGatewayAddr = FI_DEFAULT_IP_ADDR;
				
		g_FI_unit_stats.base_ip = FI_DEFAULT_IP_ADDR;
		g_FI_unit_stats.user_max_temperature_in_counts = DEFAULT_MAX_TEMP;

		Default_intercept();
		Default_gradient();
	}

	NvWriteStart ();

	// These are MFG items and should persist even through OBR
  NvWriteRam(NV_HWREV, SysHwVersionNumber, sizeof(SysHwVersionNumber));
	NvWriteRam(NV_SERNUMBER, &SysSerialNumber, NV_LONG);
	NvWriteRam(NV_MACADDRESS, SysMacAddress, NV_ETHMAC);
	NvWriteRam(NV_PRODUCT_CODE, SysProductCode, sizeof(SysProductCode));
	
	NvWriteRam(NV_IPADDRESS, &g_tInterfaceConfig.ulIpAddr, NV_LONG);
	NvWriteRam(NV_IPMASK,	&g_tInterfaceConfig.ulSubnetMask, NV_LONG);
	NvWriteRam(NV_IPGATEWAY, &g_tInterfaceConfig.ulGatewayAddr,	NV_LONG);

	NvWriteRam(NV_BASE_IPADDRESS, &g_FI_unit_stats.base_ip, NV_LONG);
	NvWriteRam(NV_HOT_CPU, &g_FI_unit_stats.user_max_temperature_in_counts, NV_SHORT);

	NvWriteRam(NV_TEMP_INTERCEPT, &g_TemperatureIntercept, NV_LONG);
	NvWriteRam(NV_TEMP_SLOPE, &g_TemperatureGradient, NV_LONG);

	if (!g_FI_unit_stats.aio_disabled )
		enabled = 1;
	NvWriteRam(NV_ANALOG_ENABLED, &enabled, NV_BYTE);

	/* write NVRAM */
	App_SysPkt_FileSystemWrite(0, RAMCOPY, sizeof(RAMCOPY));

	for(int i = 0; i < NVRAM_MAX_SIZE; i++)
	{
		if (i % 16 == 0) printf("\r\n");
		printf ("%02X ", RAMCOPY[i]);			
	}
	printf("\r\n");
}

/*************************************************/
void MfgDelParams()
{
	App_SysPkt_FileSystemDelete(0);
}

/*************************************************/
#ifndef UNIT_TEST
uint16_t MfgGetKey(void)	{return slclManufacturingInformation.u16Key;}
#endif


