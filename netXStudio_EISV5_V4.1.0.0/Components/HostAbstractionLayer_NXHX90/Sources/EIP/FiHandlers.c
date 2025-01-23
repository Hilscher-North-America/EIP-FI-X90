
#include <stdint.h>
#include <string.h> /* memcpy */

#include "App_Application.h"
#include "sys.h"

#include "Server.h"
#include "EIP.h"
#include "FI.h"
#include "FiHandlers.h"

#include "rtc.h"
#include "switch.h"

#include "cip_objects.h"
#include "Temp_Sensor.h"

//#define DEBUG_MANUF 

/*######################################################################*/
/* public functions */

int time_handler(
		op_type_t	opcode,
		data_type_t	data_type,
		addr_t		offset,			/* address of first unit */
		unsigned			qty,		/* number of data units */
		void *				data,
		void *				arg     //todo: is this needed?
	)
	{
    uint16_t * pOut, *pIn ;
    TIME_STRUCT time_netX;
    uint32_t    inverted[2];
    uint32_t    *pTime = (uint32_t*)&time_netX;

    if (opcode != IO_GET)
      return -EXC_ILL_ADDR;

    if( data_type != IND_IN )
      return (-EXC_ILL_ADDR);

    if (qty == 0)
      return (int)qty;
		if (qty > 4)
      qty = 4;

    if ( (qty + offset) > 4)
		{
      return (-EXC_ILL_ADDR);
    }

    _time_get (&time_netX);    

    time_netX.MILLISECONDS *= 1000UL;  //milliseconds from OS should never be more than 999.

    // Maintain same format as old FI    
    inverted[0] = SWAPLONG(*pTime);		
    pTime++;
    inverted[1] = SWAPLONG(*pTime);		
    
    pIn = (void *) &inverted;
    pOut = data ;

    /* apply offset; offset in 16-bit words */
    pIn += offset;
    pOut += offset;

    memcpy( pOut, pIn, qty * 2 );

    return (qty * 2 );
  }

  int temperature_handler (
	op_type_t	opcode,
	data_type_t	data_type,
	addr_t		offset,			/* address of first unit */
	unsigned			qty,			/* number of data units */
	void *				data,
	void *				arg
	)
	{
	int16_t *pOut = data;
	
	//old FI does not kick the dog for this register
	//kickWatchdog();
	
	if (opcode != IO_GET) 
		return -EXC_ILL_ADDR;

	if( data_type != IND_IN )
		return (-EXC_ILL_ADDR);

	if (qty == 0)
		return (int)qty;
	if (qty > 1)
	  qty = 1;
	
	if ( (qty + offset) > 1)
	{
		return (-EXC_ILL_ADDR);
	}

	*pOut = Sensor_GetTemp();
	
	return (qty * 2);
}

//////////////////////////////////// Input Registers ///////////////////////////////////////////////

////start addr 10000
int Device_Info_Handler (
		op_type_t	opcode,
		data_type_t	data_type,
		addr_t		offset,			/* address of first unit */
		unsigned			qty,			/* number of data units */
		void *				data,
		void *				arg
)
{
	//_uFloatConv uTmp;
	unsigned short *pui16Data;		
	_sDevInfo sDevInfo;
	
	//old FI does not kick the dog for this register
	//kickWatchdog();

	if( data_type != IND_IN )
		return -EXC_ILL_ADDR;

	if (qty == 0)
		return (int)qty;
	if (qty > sizeof(sDevInfo)/2)
	  qty = sizeof(sDevInfo)/2;

	if((offset+qty) > sizeof(sDevInfo)/2)
	{
		//printf("\n !!!! %u::%u:%u !!!!\n",offset,qty,sizeof(sDevInfo)/2);
		return -EXC_ILL_ADDR;
	}

	if(opcode == IO_GET)
	{
		_sIoDriverStats tmp;
		uint32_t ui32TimeStamp=0, ui32ProcessTime=0, ui32MsTimeDiff=0;

		//sDevInfo.ui16NvRamValid = (uint16_t)NvValid();

		sDevInfo.ui32SerialNumber = SWAPLONG(SysSerialNumber);
		sDevInfo.i16FwUpdateStatus = (int16_t)GetFwUpdateStatus();

		memcpy(sDevInfo.i8HwVersionStr,SysHwVersionNumber, 6);//Map document v1.002 lists the version string as 6 bytes (3 words)
		memcpy(sDevInfo.ui8EthernetMacAddr, SysMacAddress, 6);//ipcfg_get_mac(0, sDevInfo.ui8EthernetMacAddr);	

		tmp = sDevInfo.ioDriverStats;

		sDevInfo.ioDriverStats.numAiErrors = SWAPLONG(tmp.numAiErrors);
		sDevInfo.ioDriverStats.numAoErrors = SWAPLONG(tmp.numAoErrors);
		sDevInfo.ioDriverStats.numDiErrors = SWAPLONG(tmp.numDiErrors);
		sDevInfo.ioDriverStats.numDoErrors = SWAPLONG(tmp.numDoErrors);

		sDevInfo.ui32TimeStamp = SWAPLONG(ui32TimeStamp);
		sDevInfo.ui32ProcessTime =  SWAPLONG(ui32ProcessTime);
		sDevInfo.ui32MsTimeDiff = SWAPLONG(ui32MsTimeDiff);
		
#ifdef USE_LITTLE_ENDIAN
		{
			unsigned short * temp_SysHwVer;
			unsigned short * temp_SysMacAddr;
			unsigned short temp_val_s;
			int i=0;

			temp_SysHwVer = (unsigned short *)sDevInfo.i8HwVersionStr;
			temp_SysMacAddr = (unsigned short *)sDevInfo.ui8EthernetMacAddr;
			for(i=0; i<3; i++)
			{
				temp_val_s = *temp_SysHwVer;
				*temp_SysHwVer = mqx_ntohs(&temp_val_s);

				temp_val_s = *temp_SysMacAddr;
				*temp_SysMacAddr = mqx_ntohs(&temp_val_s);

				temp_SysHwVer ++;
				temp_SysMacAddr ++;
			}
		}
#endif		
///////////////////////////////////////	
		pui16Data = (unsigned short*) &sDevInfo;
		memcpy((unsigned short*)data, pui16Data+offset, qty*2);

		return (int)qty*2;
	}	

	return (-EXC_ILL_ADDR);
}

//start addr 60000
int Mfg_Key_Handler (
	op_type_t	opcode,
	data_type_t	data_type,
	addr_t		offset,			/* address of first unit */
	unsigned			qty,			/* number of data units */
	void *				data,
	void *				arg
	)
{
	
	uint16_t ui16Key;
	uint16_t *pOut;
	uint16_t tmpkey=0;
	
	if( data_type != IND_OUT )
			return -EXC_ILL_ADDR;
	
	if (qty == 0)
			return (int)qty;
	
	if (qty > 1)
	    qty = 1;

	if ((opcode != IO_GET) && (opcode != IO_SET))
			return -EXC_ILL_ADDR;
	
	if ( (qty + offset) > 1)
	{
		return (-EXC_ILL_ADDR);
	}
	
	//Read the current key value
	ui16Key = 		(uint16_t) MfgGetKey();
	pOut = data;
	
	if(IO_GET == opcode){
		*pOut = ui16Key;
	}else { // IO_SET

		//////////////////////////////////
		//Read in and save the key if it was valid
		if(offset == 0)
		{
			memcpy(&tmpkey, (unsigned short*)data, 2);
			if(MfgSetKey(tmpkey) > 0)
			{}
		}

		switch(MfgGetKey())
		{		
		case RESETKEY:	
		case FLASHAPP:
		case FLASHBOOT:  
		case OBRKEY:
		case NVSAVE:
			// Mfg task handles these.
			return (int)qty*2;//may not get processed   
			
		default:
			break;
		}			
	}

	return (qty*2);
}

//start addr 60001
int Manufacturing_Handler (
		op_type_t	opcode,
		data_type_t	data_type,
		addr_t		offset,			/* address of first unit */
		unsigned			qty,			/* number of data units */
		void *				data,
		void *				arg
)
{
	unsigned short *pui16Data, *pui16CmpData; // *pData;
	_sManuf sManuf, sTmpManuf;

	if (!(g_FI_unit_stats.mode & FI_MFG_CFG_MODE))
	{
		/* allow to read only the switches register */
		if (!(offset == 14 && opcode == IO_GET && qty == 1))
		{
			return -EXC_ILL_ADDR;
		}
	}

	if( data_type != IND_OUT )
		return -EXC_ILL_ADDR;

	if (qty == 0)
		return (int)qty;

	if (qty > sizeof(sManuf)/2)
	  qty = sizeof(sManuf)/2;

	if ((opcode != IO_GET) && (opcode != IO_SET))
		return -EXC_ILL_ADDR;

	if((offset+qty) > sizeof(sManuf)/2)
	{
		return -EXC_ILL_ADDR;
	}

	//Pull date from RAM
	sManuf.ui32SerialNumber = SysSerialNumber;

	memcpy(sManuf.i8ProductCode, SysProductCode, 12);
	memcpy(sManuf.i8HwVersionStr,SysHwVersionNumber, 6);//Map document v1.002 lists the version string as 6 bytes (3 words)
	memcpy(sManuf.ui8EthernetMacAddr, SysMacAddress, 6);//ipcfg_get_mac(0, sDevInfo.ui8EthernetMacAddr);	

	SWITCH_update();
	sManuf.ui16Switches = SWITCH_get_value();
	sManuf.ui32IpAddress	 = g_tInterfaceConfig.ulIpAddr;
	sManuf.ui32NetMask  = g_tInterfaceConfig.ulSubnetMask;
	sManuf.ui32Gateway  = g_tInterfaceConfig.ulGatewayAddr;
  sManuf.ui32IPAddrSwitchBase = g_FI_unit_stats.base_ip;
	/* Maximum Temperature Threshold does not use any scaling, it is express in degrees in Celsius */
  sManuf.ui16TempThreshold = g_FI_unit_stats.user_max_temperature_in_counts;
	
	pui16Data = (unsigned short*) &sManuf;	

	if(IO_GET == opcode){			
		//Send read values out
		memcpy((unsigned short*)data, pui16Data+offset, qty*2);
		return (int)qty*2;
	}else { // IO_SET

		pui16CmpData = (unsigned short*) &sTmpManuf;
		sTmpManuf = sManuf;

		memcpy(pui16CmpData+offset, (unsigned short*)data, qty*2);

		if (memcmp(&sManuf.ui32SerialNumber, &sTmpManuf.ui32SerialNumber, sizeof(sTmpManuf.ui32SerialNumber)) != 0){
			memcpy(&SysSerialNumber, &sTmpManuf.ui32SerialNumber, sizeof(sTmpManuf.ui32SerialNumber));					
#ifdef DEBUG_MANUF
			printf("\r\nSerial Number Change!\r\n");
#endif
		}

		if (memcmp(sManuf.i8ProductCode, sTmpManuf.i8ProductCode, sizeof(sTmpManuf.i8ProductCode)) != 0){

			//Option 1
			memcpy(SysProductCode, sTmpManuf.i8ProductCode, sizeof(sTmpManuf.i8ProductCode));
			SysProductCode[sizeof(SysProductCode)-1] = 0; // Ensure NULL termination!

#ifdef DEBUG_MANUF
			printf("\r\nProduct Code Change!\r\n");
#endif

		}

		/* verify valid MAC address */
		if (sTmpManuf.ui8EthernetMacAddr[0] & 0x01)
		{
			return -EXC_ILL_VAL;
		}
		if (memcmp(sManuf.ui8EthernetMacAddr, sTmpManuf.ui8EthernetMacAddr, sizeof(sTmpManuf.ui8EthernetMacAddr)) != 0){
			memcpy(SysMacAddress, sTmpManuf.ui8EthernetMacAddr, sizeof(sTmpManuf.ui8EthernetMacAddr));

#ifdef DEBUG_MANUF
			printf("\r\nMAC Change!\r\n");
#endif			
		} 

		if (memcmp(sManuf.i8HwVersionStr, sTmpManuf.i8HwVersionStr, sizeof(sTmpManuf.i8HwVersionStr)) != 0){
			memcpy(SysHwVersionNumber, sTmpManuf.i8HwVersionStr, sizeof(sTmpManuf.i8HwVersionStr));
#ifdef DEBUG_MANUF
			printf("\r\nHW Version string Change!\r\n");
#endif			

		}

		if (memcmp(&sManuf.ui32IpAddress, &sTmpManuf.ui32IpAddress, sizeof(sTmpManuf.ui32IpAddress)) != 0){
			memcpy(&g_tInterfaceConfig.ulIpAddr, &sTmpManuf.ui32IpAddress, sizeof(sTmpManuf.ui32IpAddress));
#ifdef DEBUG_MANUF
			printf("\r\nIP Address Change!\r\n");
#endif
		}

		if (memcmp(&sManuf.ui32NetMask, &sTmpManuf.ui32NetMask, sizeof(sTmpManuf.ui32NetMask)) != 0){
			memcpy(&g_tInterfaceConfig.ulSubnetMask, &sTmpManuf.ui32NetMask, sizeof(sTmpManuf.ui32NetMask));
#ifdef DEBUG_MANUF
			printf("\r\nIP Netmask Change!\r\n");
#endif
		}

		if (memcmp(&sManuf.ui32Gateway, &sTmpManuf.ui32Gateway, sizeof(sTmpManuf.ui32Gateway)) != 0){
			memcpy(&g_tInterfaceConfig.ulGatewayAddr, &sTmpManuf.ui32Gateway, sizeof(sTmpManuf.ui32Gateway));
#ifdef DEBUG_MANUF
			printf("\r\nIP Gateway Change!\r\n");
#endif
		}
		
		if (memcmp(&sManuf.ui32IPAddrSwitchBase, &sTmpManuf.ui32IPAddrSwitchBase, sizeof(sTmpManuf.ui32IPAddrSwitchBase)) != 0){
			memcpy(&g_FI_unit_stats.base_ip, &sTmpManuf.ui32IPAddrSwitchBase, sizeof(sTmpManuf.ui32IPAddrSwitchBase));					
		#ifdef DEBUG_MANUF
					printf("\r\nIP Address Base Change!\r\n");
		#endif
				}
		
		if (memcmp(&sManuf.ui16TempThreshold, &sTmpManuf.ui16TempThreshold, sizeof(sTmpManuf.ui16TempThreshold)) != 0){
				memcpy(&g_FI_unit_stats.user_max_temperature_in_counts, &sTmpManuf.ui16TempThreshold, sizeof(sTmpManuf.ui16TempThreshold));					
			#ifdef DEBUG_MANUF
						printf("\r\nTemperature Threshold Change!\r\n");
			#endif
					}
		
		return (int)qty*2;
	}
}

//start addr 60025-60030
int RTC_Time_Mfg_Handler (
		op_type_t	opcode,
		data_type_t	data_type,
		addr_t		offset,			/* address of first unit */
		unsigned			qty,			/* number of data units */
		void *				data,
		void *				arg
)
{
	unsigned short *pui16Data, *pui16CmpData; // *pData;
	_sManRTC sManRTC, sTmpManRTC;
	TIME_STRUCT time_netX;
	struct tm time_rtc;
	
	if (!(g_FI_unit_stats.mode & FI_MFG_CFG_MODE))
		return -EXC_ILL_ADDR;

	if( data_type != IND_OUT )
		return -EXC_ILL_ADDR;

	if (qty == 0)
		return (int)qty;

	if (qty > sizeof(sManRTC)/2)
	  qty = sizeof(sManRTC)/2;

	if ((opcode != IO_GET) && (opcode != IO_SET))
		return -EXC_ILL_ADDR;

	if((offset+qty) > sizeof(sManRTC)/2)
	{
		return -EXC_ILL_ADDR;
	}
	
	HOSTAL_RTC_GetTime (&time_rtc);
	
	// update date format
	sManRTC.ui16Year = time_rtc.tm_year + TM_UNIX_MIN_YEAR;
	sManRTC.ui16Month = time_rtc.tm_mon + TM_UNIX_MONTH_OFS;
	sManRTC.ui16Day = time_rtc.tm_mday;
	sManRTC.ui16Hour = time_rtc.tm_hour;
	sManRTC.ui16Minute = time_rtc.tm_min;
	sManRTC.ui16Second = time_rtc.tm_sec;
	
	pui16Data = (unsigned short*) &sManRTC;	

	if(IO_GET == opcode){			
		//Send read values out
		memcpy((unsigned short*)data, pui16Data+offset, qty*2);
		return (int)qty*2;
	}else { // IO_SET
		
		pui16CmpData = (unsigned short*) &sTmpManRTC;
		sTmpManRTC = sManRTC;

	  memcpy(pui16CmpData+offset, (unsigned short*)data, qty*2);
	    
		if (memcmp(&sManRTC.ui16Year, &sTmpManRTC.ui16Year, sizeof(sTmpManRTC.ui16Year)) != 0) 
			time_rtc.tm_year = sTmpManRTC.ui16Year - TM_UNIX_MIN_YEAR;
		
		if (memcmp(&sManRTC.ui16Month, &sTmpManRTC.ui16Month, sizeof(sTmpManRTC.ui16Month)) != 0)
			time_rtc.tm_mon = sTmpManRTC.ui16Month - TM_UNIX_MONTH_OFS;
		
		if (memcmp(&sManRTC.ui16Day, &sTmpManRTC.ui16Day, sizeof(sTmpManRTC.ui16Day)) != 0)
			time_rtc.tm_mday = sTmpManRTC.ui16Day;
		
		if (memcmp(&sManRTC.ui16Hour, &sTmpManRTC.ui16Hour, sizeof(sTmpManRTC.ui16Hour)) != 0)
			time_rtc.tm_hour = sTmpManRTC.ui16Hour;
		
		if (memcmp(&sManRTC.ui16Minute, &sTmpManRTC.ui16Minute, sizeof(sTmpManRTC.ui16Minute)) != 0)
			time_rtc.tm_min = sTmpManRTC.ui16Minute;
		
		if (memcmp(&sManRTC.ui16Second, &sTmpManRTC.ui16Second, sizeof(sTmpManRTC.ui16Second)) != 0)
			time_rtc.tm_sec = sTmpManRTC.ui16Second;
				
		// Now set the time based on new values.
		time_netX.SECONDS = mktime(&time_rtc);
		 if(-1 != time_netX.SECONDS)
			{
				_time_set( &time_netX);
				if( HOSTAL_RTC_SyncTime(false) != HOSTAL_OK )
					return -EXC_DEV_FAIL;
			}
		 else
			 return  -EXC_DEV_FAIL;
	}
	
	return (int) qty*2;
}

// TODO remove analog IO
//start addr 60031
int Manufacturing_Handler_aio (
		op_type_t	opcode,
		data_type_t	data_type,
		addr_t		offset,			/* address of first unit */
		unsigned			qty,			/* number of data units */
		void *				data,
		void *				arg
)
{
	uint16_t aio_disabled = g_FI_unit_stats.aio_disabled;

	//old FI does not kick the dog for this register
	//kickWatchdog();

	if (!(g_FI_unit_stats.mode & FI_MFG_CFG_MODE))
		return -EXC_ILL_ADDR;

	if( data_type != IND_OUT )
		return -EXC_ILL_ADDR;

	if (qty == 0)
		return (int)qty;

	if (qty > 1)
	  qty = 1;

	if ((opcode != IO_GET) && (opcode != IO_SET))
		return -EXC_ILL_ADDR;

	if((offset+qty) > 1)
	{
		return -EXC_ILL_ADDR;
	}

	if(IO_GET == opcode){			
		//Send read values out
		memcpy((unsigned short*)data, &aio_disabled, qty*2);
		return (int)qty*2;
	}else { // IO_SET

		memcpy(&aio_disabled, (unsigned short*)data, qty*2);
		
		// Only allow a value of 0 or 1 for the aio disabled variable
		if(aio_disabled > 1)
			return -EXC_ILL_VAL;
		
		g_FI_unit_stats.aio_disabled = aio_disabled;

#ifdef DEBUG_MANUF
		printf("\r\nAnalog IO Flag %d!\r\n", aio_disabled);
#endif
		
		return (int)qty*2;
	}
}


