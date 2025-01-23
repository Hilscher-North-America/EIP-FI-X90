#ifndef SYS_H_
#define SYS_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define DEFAULT_MAX_TEMP 55 * 100 // default KFI max temperature
#define IPBYTES(a)            ((int)((a)>>24)&0xFF),(int)(((a)>>16)&0xFF),(int)(((a)>> 8)&0xFF),(int)((a)&0xFF)

/*
** IP address definition
*/
typedef uint32_t _ip_address;

typedef enum
{
	sysEvent_grp		= 0,
	sysBusEvent_grp,
}eEventGrps;

//Events group (sysEvent_grp)
typedef enum
{
	evADC 			= 0x00000001,
	evPID 			= 0x00000002,
	evMfg 			= 0x00000004,
	evNvSave 		= 0x00000008,
	evEcat			= 0x00000010,
	evLed			= 0x00000020, //ry002: CHIC-54
	evSbStatus		= 0x00000040, //phoen-28
	evSbUpIostream 	= 0x00000080, //phoen-28
	evSbSync		= 0x00000100, //phoen-28
	evSbTimeout		= 0x00000200, //phoen-28
	evSbUpCfgstream = 0x00000300,
	evSbInitDone	= 0x00000400,
	evReset			= 0x00000800,
	evSysInitDone	= 0x00001000,
	evFlashBoot 	= 0x00002000,
	evFlashApp 		= 0x00004000,
	evEraseDll		= 0x00008000,
	evUndefined 	= 0xfffffc00,
	evAll			= 0xffffffff
}eSysEvents;

typedef enum {
	knetOK = 0,
	knetConfigErr,
	knetDHCPWaiting,
} knetStatus;

typedef struct 
{
	uint32_t ip;
	uint32_t mask;
	uint32_t gateway;
} IP_ADDRESS_DATA;


typedef struct 
{
	uint32_t 			base_ip;						//<! base ip address to be used to derive final ip address
	
	uint8_t 			mode;							//<! 	
	uint32_t 			offset;							//<! derived final 

	IP_ADDRESS_DATA		derived_ip;						//<! final ip address derived from switches
	uint8_t 			mfg_enabled;					//<! switches set to 978 = mfg mode enabled

	// TODO remove Analog IO
	uint8_t 			aio_disabled;					//<! MFG switch to disable AIO support
	
	int16_t				temperature_in_counts;			//<! old FI temperature (local) stored as counts (1 degC / count)
	int16_t				user_max_temperature_in_counts; //<! nv user over temperature setting in Celsius 1/100s
	knetStatus		net_status;  					//<! OK, Config Error, DHCP Waiting keeps track of config init
	uint8_t				link_status;					//<! link up/down 
} sFactoryInterfaceUnitStats;

/************************************************/
/* external global system variables */
extern uint32_t SysSerialNumber;
extern char SysHwVersionNumber[12];
extern uint8_t SysMacAddress[6];
extern uint8_t FDLMacAddress[6];
extern char SysProductCode[12];
extern char    abProductName[];

extern void Default_gradient();
extern void Default_intercept();

uint32_t SYS_InitConfig ();

extern sFactoryInterfaceUnitStats g_FI_unit_stats;

#endif /* SYS_H_ */
