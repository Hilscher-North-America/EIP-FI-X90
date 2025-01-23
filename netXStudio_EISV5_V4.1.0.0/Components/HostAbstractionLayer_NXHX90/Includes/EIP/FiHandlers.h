#ifndef CHICAGOCFGHANDLERS_H_
#define CHICAGOCFGHANDLERS_H_

#include "Server.h"

extern DECLARE_HANDLER(Manufacturing_Handler);
extern DECLARE_HANDLER(Manufacturing_Handler_aio);
extern DECLARE_HANDLER(RTC_Time_Mfg_Handler);
extern DECLARE_HANDLER(Mfg_Key_Handler);
extern DECLARE_HANDLER(Device_Info_Handler);
extern DECLARE_HANDLER(temperature_handler);
extern DECLARE_HANDLER(time_handler);

#pragma pack(push)	
#pragma pack(1)
/*! Driver access statistics*/
typedef struct {  
	uint32_t numAiErrors; //!< number of Ai errors detected  //todo remove Ai and Ao
	uint32_t numAoErrors; //!< number of Ao errors detected
	uint32_t numDiErrors; //!< number of Di errors detected
	uint32_t numDoErrors; //!< number of Do errors detected
} _sIoDriverStats;

typedef struct {
	//uint16_t ui16NvRamValid;
	int16_t  i16FwUpdateStatus;
	
	uint32_t ui32TimeStamp;
	uint32_t ui32ProcessTime;
	uint32_t ui32MsTimeDiff;

	uint32_t ui32SerialNumber;
	char 	i8HwVersionStr[6];		////Map document v1.002 lists the version string as 6 bytes (3 words)
	uint8_t 	ui8EthernetMacAddr[6];	//3 words
	_sIoDriverStats ioDriverStats;
} _sDevInfo;
#pragma pack(pop)	

#pragma pack(push)
#pragma pack(1)
	typedef struct {
		uint32_t ui32SerialNumber;			//0,
		char 	i8ProductCode[12];			//2,
		char 	i8HwVersionStr[6];			//8,	--> ////Map document v1.002 lists the version string as 6 bytes (3 words)
		uint8_t 	ui8EthernetMacAddr[6];	//11,	--> //3 words
		uint16_t	ui16Switches;			//14
		uint32_t ui32IpAddress;				//15
		uint32_t ui32NetMask;				//17
		uint32_t ui32Gateway;				//19
		uint32_t ui32IPAddrSwitchBase;		//21  
		uint16_t ui16TempThreshold;			//23
	} _sManuf;
#pragma pack(pop)	

#pragma pack(push)
#pragma pack(1)
	typedef struct
	{
	   /*! \brief Range from 1970 to 2099. */
	   uint16_t ui16Year;

	   /*! \brief Range from 1 to 12. */
	   uint16_t ui16Month;

	   /*! \brief Range from 1 to 31 (depending on month). */
	   uint16_t ui16Day;

	   /*! \brief Range from 0 to 23. */
	   uint16_t ui16Hour;

	   /*! \brief Range from 0 to 59. */
	   uint16_t ui16Minute;

	   /*! \brief Range from 0 to 59. */
	   uint16_t ui16Second;
	} _sManRTC;
#pragma pack(pop)


#endif /* CHICAGOCFGHANDLERS_H_ */
