#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

#include "shell.h"
#include "device_id.h"
#include "sys.h"
#include "fi.h"
#include "App_FileSystemPackets.h"
#include "App_SystemPackets.h"
#include "App_SockIfFunctions.h"
#include "system_netx.h"
#include "HostAbstractionLayer.h"
#include "switch.h"
#include "cip_objects.h"

extern APP_DATA_T g_tAppData;

bool g_App_Control = true;

/***************************************************************************************************
* PRIVATE functions
**************************************************************************************************/

/***************************************************************************************************
*! \brief  checks hex format of the string
*
*  \details  verifies that the string is in 0x,0X format
*  \param value - string for hex value
*
*  \return : 0 - failure, 1 - success
**************************************************************************************************/
static uint8_t shell_check_hex(const char * value)
{
  if(value[0] == '0' && (value[1] == 'x' || value[1] == 'X'))
  {
    return 1;
  }
  else
    return 0;
}

/***************************************************************************************************
*! \brief  parses parameter as an IP Address
*
*  \details  verifies that the string passed is a valid IP address and stores it as uint32_t
*  \param argv[in] - list of arguments
*  \param ipaddr_ptr[out] - returns IP address as an uint32_t
*
*  \return : 0 - failure, 1 - success
**************************************************************************************************/
static bool shell_parse_ip_address( const char *arg, uint32_t *ipaddr_ptr)
{
  uint32_t      num[4] =  { 0 };
  uint32_t      i, index = 0;
  uint32_t      temp = 0;
   
  if (ipaddr_ptr == NULL) return false;

  for (i=0; arg[i] && (i<16) && (index<4); i++)  
  {
    if (isdigit((int)arg[i]))  
		{
      num[index] = num[index]*10 + arg[i]-'0';
    } 
		else if (arg[i] == '.') 
		{
      index++; 
    } 
		else  
		{
      return false;
    }
  }
   
  if ((arg[i] == '\0') && (index==3))  
	{
    for (i=0;i<4;i++)  
		{
      if (num[i] > 255)  
	 	  {
        return false;
      }
			else
			{
       	temp = (temp << 8) | num[i];
      }
    } 
  }
	else
	{
		return false;
	} 

  *ipaddr_ptr = temp;
  return true;
}

/***************************************************************************************************
*! \brief  Shell command to change base IP address
*
*  \ details  1 argument :  0 = shell, 1 = app
**************************************************************************************************/
uint32_t shell_baseipsave (uint32_t argc, const char * argv[])
{   
	bool	print_usage, shorthelp = false;
	uint32_t temp;
	
	print_usage = Shell_check_help_request(argc, argv, &shorthelp );
	
	temp = 0;  
	
	if (!print_usage)  
	{	
		if (argc > 1)
		{
			if (!shell_parse_ip_address (argv[1], &temp))
			{
				printf ("Error invalid ip address!\r\n");
				return SHELL_EXIT_ERROR;
			}
		}
		else
		{
			printf ("Error  missing ip address parameter!\r\n");
			return SHELL_EXIT_ERROR;
		}
		
		g_FI_unit_stats.base_ip = temp;
		printf ("BASE IP:  %d.%d.%d.%d\r\n", IPBYTES(g_FI_unit_stats.base_ip));
		
		printf("Use 'nvsave' to save Base Ip Address Information to NVRAM.\r\n");
		return SHELL_EXIT_SUCCESS;
	}
		
	if (print_usage)  {
		if (shorthelp)  {
			printf("%s  Change Base Ip Address\r\n", argv[0]);
		} else  {
			printf("Usage: \'%s  <ip>\' to change the base ip address.\r\n", argv[0]);
		}
	}
	return SHELL_EXIT_SUCCESS;
}

/***************************************************************************************************
*! \brief  Display NVRAM paramters
*
**************************************************************************************************/
void shell_DisplayNvRamParams()
{
/* dispay nvram parameters */
  printf("NVRAM parameters\r\n--------------------------------------------------------------\r\n");
  printf("HW Version Number: %s\r\n", SysHwVersionNumber);
  printf("Serial Number:     %lu\r\n", SysSerialNumber);
  printf("MAC Address:       %02X:%02X:%02X:%02X:%02X:%02X\r\n", SysMacAddress[0],SysMacAddress[1],SysMacAddress[2],SysMacAddress[3],SysMacAddress[4],SysMacAddress[5]);	
  printf("Product Code:      %s\r\n", SysProductCode);
  printf("IP Address:        %d.%d.%d.%d\r\n", IPBYTES(g_tInterfaceConfig.ulIpAddr));
  printf("Mask:              %d.%d.%d.%d\r\n", IPBYTES(g_tInterfaceConfig.ulSubnetMask));
  printf("Gateway:           %d.%d.%d.%d\r\n", IPBYTES(g_tInterfaceConfig.ulGatewayAddr));
	printf("BASE IP Address:   %d.%d.%d.%d\r\n", IPBYTES(g_FI_unit_stats.base_ip));
  printf("Max Temperature:   %u.%uC\r\n", g_FI_unit_stats.user_max_temperature_in_counts / 100, g_FI_unit_stats.user_max_temperature_in_counts % 100);
	printf("Analog IO Disabled:%u", g_FI_unit_stats.aio_disabled);
  printf("\r\n");
}

/***************************************************************************************************
* Public functions
**************************************************************************************************/

/***************************************************************************************************
*! \brief  checks for valid hex string and converts to uint
*
*  \details  verifies that the string passed is in hex format anc converts it to uint
*  \param chp_cmdline[in] - string hex value
*  \param u32ptr_outbuf[out] - returns hex value of the stirng passed
*
*  \return : -1 - failure, 0 - success
**************************************************************************************************/
int Shell_check_hex_scan_str_to_u32(const char * chp_cmdline, uint32_t* u32ptr_outbuf)
{
	if (shell_check_hex(chp_cmdline)==1) 
	{
		if (sscanf(chp_cmdline, "0x%x", (unsigned int*) u32ptr_outbuf) != 1)
			//string entered can't be scanned as an unsigned value
			return -1;
	}
	else 
	{
		if (sscanf(chp_cmdline, "%u", (unsigned int*) u32ptr_outbuf) != 1)
			//string entered can't be scanned as an unsigned value
			return -1;
	}

	//str successfully scanned to unsigned value
	return 0;
}

/***************************************************************************************************
*! \brief  checks for shell help command
*
*  \details  verifies that the string passed is "help" command
*  \param argc[in] - num arguments
*  \param argv[in] - list of arguments
*  \param short_ptr[out] - returns if "short" parameter passed
*
*  \return : 0 - failure, 1 - success
**************************************************************************************************/
bool Shell_check_help_request(uint32_t argc, const char *argv[], bool  *short_ptr )
{
  if ((argc >= 2) && (strcmp(argv[1], "help") == 0))  
	{
		if (argc == 3)
     	*short_ptr = (strcmp(argv[2], "short")==0)?true:false;
    return true;
  }
  return false;
}

/***************************************************************************************************
*! \brief  Shell command to format netX90 file system
*
*  \ details  1 argument :  0 = shell, 1 = app
**************************************************************************************************/
uint32_t Shell_FileSystemInit( uint32_t argc, const char *argv[] )
{
  /* init flash file system with full format flag */
  App_SysPkt_FileSystemInit(0, true);

  return 0;
}

/***************************************************************************************************
*! \brief  Shell command to display file list
*
*  \ details  1 argument :  0 = shell, 1 = app
**************************************************************************************************/
uint32_t Shell_FileSystemList( uint32_t argc, const char *argv[] )
{
  App_SysPkt_FileSystemList(0);

  return 0;
}

/***************************************************************************************************
*! \brief  Shell command to display nvram registers
*
*  \ details  1 argument :  0 = shell, 1 = app
**************************************************************************************************/
uint32_t Shell_FileSystemShow( uint32_t argc, const char *argv[] )
{
  MfgInitParams();
  
  shell_DisplayNvRamParams ();

  return 0;
}

/***************************************************************************************************
*! \brief  Shell command to save registers to nvram
*
*  \ details  1 argument :  0 = shell, 1 = app
**************************************************************************************************/
uint32_t Shell_FileSystemSave( uint32_t argc, const char *argv[] )
{
  MfgSaveParams(0);

  shell_DisplayNvRamParams ();

  return 0;
}

/***************************************************************************************************
*! \brief  Shell command to delete registers to nvram
*
*  \ details  1 argument :  0 = shell, 1 = app
**************************************************************************************************/
uint32_t Shell_FileSystemDel( uint32_t argc, const char *argv[] )
{
  MfgDelParams();
	
  return 0;
}

/***************************************************************************************************
*! \brief  Shell command to display current netX temperature
*
*  \ details  1 argument :  0 = shell, 1 = app
**************************************************************************************************/
uint32_t Shell_Temp( uint32_t argc, const char *argv[] )
{
	uint16_t usTemp;

	HOSTAL_Sensor_GetData(1, &usTemp);
	printf("Temp %d.%02d C\r\n", usTemp / 100, usTemp % 100);
	
  return 0;
}

/***************************************************************************************************
*! \brief  Shell command to save or show temperature intercept parameter
*
*  \ details  1 argument :  0 = shell, 1 = app
**************************************************************************************************/
uint32_t Shell_TempIntercept( uint32_t argc, const char *argv[] )
{
	uint32_t ulValue;

	//int g_TemperatureIntercept = (int) (ADC_INTERCEPT * ADC_COUNT_RESOLUTION_MULTIPLIER * TEMP_MULTIPLIER / ADC_REF_2V6);
	ulValue = g_TemperatureIntercept * ADC_REF_2V6 * ADC_MULTIPLIER / ADC_COUNT_RESOLUTION_MULTIPLIER;

	/* show intercept value */
	if( argc == 1 )
	{		
		//printf("Temp Intercept int value %lu\r\n", g_TemperatureIntercept); // dbg
		printf("Temp Intercept value: %lu.%06lu\r\n", ulValue / TEMP_MULTIPLIER_SHELL, ulValue % TEMP_MULTIPLIER_SHELL);
		printf("Default Value: %f\r\n", ADC_INTERCEPT);
	}
	/* set intercept value */
	else if (argc == 2)
	{
		float fValue;
		if (sscanf(argv[1], "%f", &fValue) != 1)
		{
			printf("Error: Invalid format of the intercept paremeter %s x.xxxxxx (%f).\r\n", argv[1], ADC_INTERCEPT);
			printf("Usage: \'%s  <param>\' to change or show the temperature intercept parameter.\r\n", argv[0]);
			return 1;
		}
		
		/* save new intercept value */
		g_TemperatureIntercept = (int) (fValue * ADC_COUNT_RESOLUTION_MULTIPLIER * TEMP_MULTIPLIER / ADC_REF_2V6);
		printf("Temp Intercept value: %f\r\n", fValue);
		printf("Default Value: %f\r\n", ADC_INTERCEPT);
	}
	/* too many parameters */
	else
	{
		printf("Usage: \'%s  <param>\' to change or show the temperature intercept parameter.\r\n", argv[0]);
	}  

  return 0;
}

/***************************************************************************************************
*! \brief  Shell command to save or show temperature slope parameter
*
*  \ details  1 argument :  0 = shell, 1 = app
**************************************************************************************************/
uint32_t Shell_TempSlope( uint32_t argc, const char *argv[] )
{
	uint32_t ulSlope;
		
	ulSlope = g_TemperatureGradient * ADC_REF_2V6 / ADC_COUNT_RESOLUTION_MULTIPLIER;

 	/* show slope value */
	if( argc == 1 )
	{		
		//printf("Temp Slope int value %lu\r\n", g_TemperatureGradient); // dbg
		printf("Temp Slope value: %lu.%06lu\r\n", ulSlope / (TEMP_MULTIPLIER_SHELL), ulSlope % (TEMP_MULTIPLIER_SHELL));
		printf("Default Value: %f\r\n", ADC_SLOPE);
	}
	/* set slope value */
	else if (argc == 2)
	{
		float fValue;
		if (sscanf(argv[1], "%f", &fValue) != 1)
		{
			printf("Error: Invalid format of the slope paremeter %s x.xxxxxx (%f)).\r\n", argv[1], ADC_SLOPE);
			printf("Usage: \'%s  <param>\' to change or show the temperature intercept parameter.\r\n", argv[0]);
			return 1;
		}

		/* save new slope value */
		g_TemperatureGradient = (int) (fValue * ADC_COUNT_RESOLUTION_MULTIPLIER * ADC_MULTIPLIER * TEMP_MULTIPLIER / ADC_REF_2V6);
		printf("Temp Slope value: %f\r\n", fValue);
		printf("Default Value: %f\r\n", ADC_SLOPE);
	}
	/* too many parameters */
	else
	{
		printf("Usage: \'%s  <param>\' to change or show the temperature slope parameter.\r\n", argv[0]);
	}

  return 0;
}

/***************************************************************************************************
*! \brief  Shell command to (re)assign control of IO expandar update
*
*  \ details  1 argument :  0 = shell, 1 = app
**************************************************************************************************/
uint32_t Shell_AppControl( uint32_t argc, const char *argv[] )
{
  if( argc > 0 ){

     int argval = atoi(argv[1]);//0 as err is ok here

      if ( argval ){
         printf("\r\n Returning control to APP \r\n");
         g_App_Control = true;
      } else {
        printf("\r\n Acquired control for shell only! \r\n");
         g_App_Control = false;
      }
  } else {
    printf("\r\n %s is in control. \r\n", g_App_Control ? "APP" : "Shell");
  }

  return 0;
}

/***************************************************************************************************
*! \brief  Shell command to set IP address for manufacturing and system configuration
*
*  \ details  1 argument :  0 = shell, 1 = app
**************************************************************************************************/
uint32_t ipsave (uint32_t argc, const char * argv[])
{
   uint32_t					index = 0;
    
	bool	print_usage, shorthelp = false;
	EIP_TCP_INTERFACE_CONFIG_T temp = {0};
	
	print_usage = Shell_check_help_request(argc, argv, &shorthelp );

	if (!print_usage)  
	{	
		if (argc > ++index)
		{
			if (!shell_parse_ip_address (argv[index], &temp.ulIpAddr))
			{
				printf ("Error in ipsave command, invalid ip address!\r\n");
				return SHELL_EXIT_ERROR;
			}
		}
		else
		{
			printf ("Error in ipsave command, missing ip address parameter!\r\n");
			return SHELL_EXIT_ERROR;
		}
	
		if (temp.ulIpAddr != 0){
			if (argc > ++index)
			{
				if (!shell_parse_ip_address (argv[index], &temp.ulSubnetMask))
				{
					printf ("Error in ipsave command, invalid mask!\r\n");
					return SHELL_EXIT_ERROR;
				}
			}
			else
			{
				printf ("Error in ipsave command, missing mask parameter!\r\n");
				return SHELL_EXIT_ERROR;
			}
		
			if (argc > ++index)
			{
				if (!shell_parse_ip_address (argv[index], &temp.ulGatewayAddr))
				{
					printf ("Error in ipsave command, invalid gateway!\r\n");
					return SHELL_EXIT_ERROR;
				}
			}
			else
			{				
			  temp.ulGatewayAddr = (temp.ulIpAddr & 0xFFFFFF00) | 1;
				//ip_data.gateway = 0;
			}
		}else{
		  temp.ulSubnetMask = 0;
		  temp.ulGatewayAddr = 0;
		}
	
		
		//_int_disable();
    g_tInterfaceConfig = temp;
   // InitEcatMfgIp();
		//_int_enable();
		
		if (g_tInterfaceConfig.ulIpAddr == 0){
			printf("IP: DHCP (0.0.0.0)\r\n");
		}else{
			printf ("IP:  %d.%d.%d.%d\r\n", IPBYTES(g_tInterfaceConfig.ulIpAddr));
			printf ("MASK: %d.%d.%d.%d\r\n", IPBYTES(g_tInterfaceConfig.ulSubnetMask));
			printf ("GATE: %d.%d.%d.%d\r\n", IPBYTES(g_tInterfaceConfig.ulGatewayAddr));
		}
		
		printf("Use 'nvsave' to save Unit Ip Address Information to NVRAM.\r\n");
		return SHELL_EXIT_SUCCESS;
	}
		
	if (print_usage)  {
		if (shorthelp)  {
			printf("%s  <ip> <mask> <gateway>\r\n", argv[0]);
		} else  {
			printf("Usage: \'%s  <ip> <mask> <gateway>\' sets network parameters.\r\n", argv[0]);
		}
	}
	return SHELL_EXIT_SUCCESS;
}

uint32_t link (uint32_t argc, const char * argv[])
{
	/* get Ethernet info for DHCP mode */
  //if (g_FI_unit_stats.mode & FI_DHCP_MODE)
    Sock_GetIfAddrs(&g_tAppData);

	return SHELL_EXIT_SUCCESS;
}

/***************************************************************************************************
*! Shell handler to dump revision info of the product
*
*  \details  this MUST occur before AppDNS_SetConfiguration(), as the CONFIG object
*            will define the assembly sizes
*
*   \param argc[in] - num arguments
*   \param argv[in] - list of arguments
*
*   \return
**************************************************************************************************/
uint32_t Term_VersionInfo(uint32_t argc, const char *argv[]){

  TERM_PRINTF("\r\n-------------------------------------------------------");
  TERM_PRINTF("\r\n%s - revision %s\r\n", DEVICE_MODEL_NAME, DEVICE_REVISION);
  TERM_PRINTF("Built: %s\r\n", __DATE__);
  TERM_PRINTF("Copyright (c) MKS Instruments, Inc.\r\n");
  TERM_PRINTF("-------------------------------------------------------\r\n");

#ifdef INCLUDE_BUILD_AUTHOR
  TERM_PRINTF("\r\nBuild Author Info:\r\n\r\n%s\r\n" , build_author );
#endif

  if ((g_tAppData.aptChannels[0] == NULL) ||
      (g_tAppData.aptChannels[0]->hChannel == NULL))
  {
    TERM_PRINTF("ERROR, VERSION channel not used by FI project" NEWLINE);
    return CIFX_INVALID_CHANNEL;
  }

  App_ReadChannelInfo(g_tAppData.aptChannels[0]->hChannel,
                     &g_tAppData.aptChannels[0]->tChannelInfo);      

  return CIFX_NO_ERROR;
}
