/*
 * RTC.c
 *
 *  Created on: May 6, 2014
 *      Author: RobertsJ
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "netx_drv.h"
#include "app_application.h"
#include "mks_types.h"
#include "rtc.h"
#include "mks_syscounter.h"
#include "shell.h"

#if (defined UNIT_TEST && !defined printf)
#define printf	_io_printf
#endif

#define RTC_OSCILATOR_REGISTER 0x25
#define SLAVE_RTC_ADDRESS     0x51
static DRV_I2C_ADDRESS_T g_RtcAddress = {SLAVE_RTC_ADDRESS, DRV_I2C_ADDRESS_7_BIT};
#define RTC_OSCILATOR_12pF5    0x03

#define STOP_ENABLE_RTCCLK_REG_ADR (0x2E)
#define STOP_ENABLE_RTCCLK_REG_CLKSTOPPED_VAL (1)
#define STOP_ENABLE__RTCCLK_REG_CLKRUNS_VAL (0)
#define CLEAR_PRESCALAR (0xA4) //To set the time for RTC mode accurately, the clear prescaler instruction is needed.

#define START_WATCH_CMD_BSIZE (2)
#define STOP_WATCH_AND_SET_TIME_CMD_BSIZE (11)

#define RTC_OFFSET_MASK           0x07
#define FLAG_MASK_SEC_MIN         0x7f
#define RTC_MAX_ARG_COUNT         7

typedef enum {
  RTC_100th_SECONDS=0,
  RTC_SECONDS,
  RTC_MINUTES,
  RTC_HOURS,
  RTC_DAYS,
  RTC_WEEKDAYS,
  RTC_MONTHS,
  RTC_YEARS,
  RTC_TIMEDATE_REG_COUNT
}Rtc_Registers;

/*****************************************************************************/
/*! Time tracking Variables                                                  */
/*****************************************************************************/
static uint32_t s_ulTimeSec = 0;          /* Time register seconds */
static uint32_t s_ulTimeMilliSec = 0;     /* Time register milliseconds */
static uint32_t s_ulRTCSeconds = 0;       /* RTC seconds */
static uint32_t s_ulSysLastMilliSec = 0;  /* system last time in ms */

HOSTAL_RESULT_E HOSTAL_RTC_SetTime(struct tm* ptime);

/*****************************************************************************/
/*! I2C Variables                                                            */
/*****************************************************************************/
static DRV_I2C_HANDLE_T  s_tI2C_RTCHandle = {0};      /** Real Time Clock I2C handler declaration */

//track if we have initialized the PCA or not...
static bool g_RTCInitComplete = false;

/*****************************************************************************/
/*! write data to I2C RTC peripheral 
 *   \return MksReturnCodes                                                  */
/*****************************************************************************/
static MksReturnCodes RTC_write (const MksUint8Type *p_data, MksUint16Type num_bytes)
{
  DRV_STATUS_E      FceRet;
  DRV_I2C_STATE_E   eState;
  uint32_t startTime;
  uint32_t waitTime= MILLI_SECONDS_TO_SYSTEM_TICK_COUNTS(10);

  startTime = GetSysCounter();
  while(DRV_OK != (FceRet = DRV_I2C_MasterTransmit(&s_tI2C_RTCHandle, g_RtcAddress, p_data, num_bytes, DRV_I2C_CONT_END))){
    if( DRV_BUSY != FceRet ){ 
       printf("Write RTC failed to send command: %d\r\n", FceRet);
       return kMksOperationFailed;
    }

    if (MKS_TRUE == SysCounterTimeoutCounts(startTime, waitTime)) {
      printf("\r\n Write RTC command timed out");
      return kMksTimeout;
    }
  }

  while(DRV_OK != (FceRet = DRV_I2C_GetState(&s_tI2C_RTCHandle, &eState))){
    if((uint64_t) eState & ((uint64_t) DRV_I2C_STATE_SLAVE_FIFO_OVERFLOW | (uint64_t) DRV_I2C_STATE_SLAVE_FIFO_UNDERRUN | (uint64_t) DRV_I2C_STATE_MASTER_FIFO_OVERFLOW | (uint64_t) DRV_I2C_STATE_MASTER_FIFO_UNDERRUN)) {
       return false;
    }
    if (MKS_TRUE == SysCounterTimeoutCounts(startTime, waitTime)) {
       return false;
    }
  }

  return kMksOk;
}

/*****************************************************************************/
/*! read data from I2C RTC peripheral 
 *   \return MksReturnCodes                                                  */
/*****************************************************************************/
static MksReturnCodes RTC_read (const MksUint8Type *w_data, MksUint16Type num_bytes, MksUint8Type *r_data)
{
  DRV_STATUS_E      FceRet;
  DRV_I2C_STATE_E   eState;
  uint32_t startTime;
  uint32_t waitTime= MILLI_SECONDS_TO_SYSTEM_TICK_COUNTS(10);

  startTime = GetSysCounter();
  while(DRV_OK != (FceRet = DRV_I2C_MasterTransmit(&s_tI2C_RTCHandle, g_RtcAddress, w_data, 1, DRV_I2C_CONT_CONTINUOUS))){
    if( DRV_BUSY != FceRet ){
       printf("Read RTC failed to send command: %d\r\n", FceRet);
       return kMksOperationFailed;
    }

    if (MKS_TRUE == SysCounterTimeoutCounts(startTime, waitTime)) {
      printf("Read RTC send command timed out\r\n");
      return kMksTimeout;
    }
  }

  while(DRV_OK != (FceRet = DRV_I2C_GetState(&s_tI2C_RTCHandle, &eState))){
    if((uint64_t) eState & ((uint64_t) DRV_I2C_STATE_SLAVE_FIFO_OVERFLOW | (uint64_t) DRV_I2C_STATE_SLAVE_FIFO_UNDERRUN | (uint64_t) DRV_I2C_STATE_MASTER_FIFO_OVERFLOW | (uint64_t) DRV_I2C_STATE_MASTER_FIFO_UNDERRUN)) {
      return kMksOperationFailed;
    }
    if (MKS_TRUE == SysCounterTimeoutCounts(startTime, waitTime)) {
      return kMksTimeout;
    }
  }

  startTime = GetSysCounter();
    while(DRV_OK != (FceRet = DRV_I2C_MasterReceive(&s_tI2C_RTCHandle, g_RtcAddress, r_data, num_bytes, DRV_I2C_CONT_END))){
      if( DRV_BUSY != FceRet ){
        printf("Read RTC failed to receive command: %d\r\n", FceRet);
        return kMksOperationFailed;
      }

      if (MKS_TRUE == SysCounterTimeoutCounts(startTime, waitTime)) {
        printf("Read RTC receive command timed out\r\n");
        return kMksTimeout;
      }
    }

    while(DRV_OK != (FceRet = DRV_I2C_GetState(&s_tI2C_RTCHandle, &eState))){
      if((uint64_t) eState & ((uint64_t) DRV_I2C_STATE_SLAVE_FIFO_OVERFLOW | (uint64_t) DRV_I2C_STATE_SLAVE_FIFO_UNDERRUN | (uint64_t) DRV_I2C_STATE_MASTER_FIFO_OVERFLOW | (uint64_t) DRV_I2C_STATE_MASTER_FIFO_UNDERRUN)) {
        return kMksOperationFailed;
      }
      if (MKS_TRUE == SysCounterTimeoutCounts(startTime, waitTime)) {
        return kMksTimeout;
      }
    }

  return kMksOk;
}

/*****************************************************************************/
/*! initialize I2C RTC peripheral 
 *   \return MksReturnCodes                                                  */
/*****************************************************************************/
MksReturnCodes RTC_I2C_Init()
{
  uint8_t cmd[2]={RTC_OSCILATOR_REGISTER,RTC_OSCILATOR_12pF5};
  if (g_RTCInitComplete == false)
  {
    /** configure the I2C RTC peripheral */
    s_tI2C_RTCHandle.tConfiguration.sAckPollMaximum = 10;
    s_tI2C_RTCHandle.tConfiguration.eDeviceID = DRV_I2C_DEVICE_ID_I2C0;
    s_tI2C_RTCHandle.tConfiguration.eOperationMode = DRV_OPERATION_MODE_POLL;
    s_tI2C_RTCHandle.tConfiguration.eSpeedMode = DRV_I2C_SPEED_MODE_FS_100k;
    s_tI2C_RTCHandle.tConfiguration.tSlaveAddress.eAddressingScheme = DRV_I2C_ADDRESS_7_BIT;
    s_tI2C_RTCHandle.tConfiguration.eMstTxFifoWatermark = DRV_I2C_WATERMARK_8;
    s_tI2C_RTCHandle.tConfiguration.eMstRxFifoWatermark = DRV_I2C_WATERMARK_8;
    s_tI2C_RTCHandle.tConfiguration.eSlvTxFifoWatermark = DRV_I2C_WATERMARK_8;
    s_tI2C_RTCHandle.tConfiguration.eSlvRxFifoWatermark = DRV_I2C_WATERMARK_8;    

    /** Init the I2c device */
    if(DRV_OK != DRV_I2C_Init(&s_tI2C_RTCHandle))
    {
      return kMksOperationFailed;
    }
  }
  SysCounterWaitMilliSeconds (100);
  
  /* initial oscillator capacitance value, register 0x25 value 0x03 */
  return (RTC_write(cmd,2));
}

/*****************************************************************************/
/*! display APP internal time 
 *   \return None                                                            */
/*****************************************************************************/
static void print_netX_time
(
  struct tm*          time_rtc,
  TIME_STRUCT_PTR     time_netX
)
{
	printf("NETX : %d s, %d ms (%02d/%02d/%4d %02d:%02d:%02d (mm/dd/yyyy))\r\n", (int) time_netX->SECONDS, (int) time_netX->MILLISECONDS, (int) (time_rtc->tm_mon + 1), time_rtc->tm_mday, (int) TM_TO_YEAR(time_rtc->tm_year), time_rtc->tm_hour, time_rtc->tm_min, time_rtc->tm_sec);
}

/*****************************************************************************/
/*! display RTC time 
 *   \return None                                                            */
/*****************************************************************************/
static void print_rtc_time
(
  struct tm*          time_rtc,
  TIME_STRUCT_PTR     time_netX
)
{
   printf("RTC  : %02d/%02d/%4d %02d:%02d:%02d (mm/dd/yyyy)(%02d s, %03d ms)\r\n", (int) (time_rtc->tm_mon + 1), time_rtc->tm_mday, (int) TM_TO_YEAR(time_rtc->tm_year), time_rtc->tm_hour, time_rtc->tm_min, time_rtc->tm_sec, (int) time_netX->SECONDS, (int) time_netX->MILLISECONDS);
}

/*****************************************************************************/
/*! display current RTC internal time 
 *   \return None                                                            */
/*****************************************************************************/
static void print_current_time(void)
{
  struct tm     time_rtc;
  TIME_STRUCT   time_netX;

  HOSTAL_RTC_GetTime (&time_rtc);
  time_netX.SECONDS = mktime (&time_rtc);
  time_netX.MILLISECONDS = 0;
    
  if(-1 != time_netX.SECONDS)
  	print_rtc_time(&time_rtc, &time_netX);
  else
  	printf("Failed to retrieve time information.\r\n");
}

/*****************************************************************************/
/*! display current RTC and APP internal time               
 *   \return None                                                            */
/*****************************************************************************/
static void _time_print(void)
{
  struct tm*      ptime_rtc;
  TIME_STRUCT     time_netX;

  _time_get (&time_netX);

  /* convert time to date */
  ptime_rtc = localtime(&time_netX.SECONDS);

  print_netX_time(ptime_rtc, &time_netX);
  print_current_time();
}

/*****************************************************************************/
/*! get time register, updates Time Register, called periodically               
 *   \return None                                                            */
/*****************************************************************************/
void _time_get(TIME_STRUCT_PTR ptTime)
{
  uint32_t ulMilliSec = SYSTEM_TICK_COUNTS_TO_MILLI_SECONDS(GetSysCounter());
  uint32_t ulElapsedMilliSec;

  if (ulMilliSec < s_ulSysLastMilliSec)
  {
    // sync netX clock with RTC every 32bit uint overflows
    HOSTAL_RTC_SyncTime(true);
    s_ulSysLastMilliSec = ulMilliSec;
    return;    
  }    
  else
  {
    ulElapsedMilliSec = ulMilliSec - s_ulSysLastMilliSec;
  }

  /* update last time */
  s_ulSysLastMilliSec = ulMilliSec;

  /* update time */
  s_ulTimeMilliSec += ulElapsedMilliSec;  
  s_ulTimeSec += s_ulTimeMilliSec / 1000;
  s_ulTimeMilliSec %= 1000;  
  
  if (NULL != ptTime)
  {    
    ptTime->MILLISECONDS = s_ulTimeMilliSec;
    ptTime->SECONDS = s_ulTimeSec;
  }
}

/*****************************************************************************/
/*! set time register
 *   \return None                                                            */
/*****************************************************************************/
void _time_set(TIME_STRUCT_PTR ptTime)
{
  /* offset RTC time by netX time elapsed */
  s_ulRTCSeconds = ptTime->SECONDS;
  s_ulTimeSec = ptTime->SECONDS;
  s_ulTimeMilliSec = ptTime->MILLISECONDS;
}

/*****************************************************************************/
/*! set RTC time                
 *   \return bool true - success                                             */
/*****************************************************************************/
bool RTC_SetTime (struct tm* ptime_rtc)
{    
  TIME_STRUCT time_netX;
  bool rc = false;
    
  time_netX.SECONDS = mktime(ptime_rtc);
  if(-1 != time_netX.SECONDS)
	{
		_time_set(&time_netX);
    /* update RTC */
		if( HOSTAL_RTC_SyncTime(false) != HOSTAL_OK )
		{
			printf("\r\nError synchronizing time!\r\n");
		}
		else
		{
			rc = true;
		}
	}

  return rc;
}

/*****************************************************************************/
/*! handle shell TIME command               
 *   \return None                                                            */
/*****************************************************************************/
uint32_t Shell_HandleTime (uint32_t argc, const char* argv[])
{
  HOSTAL_RESULT_E eRet = HOSTAL_OK;
  struct tm desired_time;
  uint32_t dummy;
 
  if (argc != 1 && argc != 7)
  {
    printf("ERROR, TIME invalid number of arguments\r\n");
    printf("Use as \"TIME [<month> <day> <year> <hours> <minutes> <seconds>]\"\r\n");
    return HOSTAL_ERROR;
  }
  
  if (argc == 1)
  {
    _time_print();
  }
  else if (argc == RTC_MAX_ARG_COUNT)
  {
    /* validate month */
    if(Shell_check_hex_scan_str_to_u32(argv[1],&dummy)!=0)
      eRet = HOSTAL_INVALID_TYPE;
    else if(TM_MAX_MONTH < dummy)
      eRet = HOSTAL_INVALID_VALUE;
    else
      desired_time.tm_mon = dummy - 1;

    /* validate day */
    if(Shell_check_hex_scan_str_to_u32(argv[2],&dummy)!=0)
      eRet = HOSTAL_INVALID_TYPE;
    else if(dummy < TM_MIN_DAY || dummy > TM_MAX_DAY)
      eRet = HOSTAL_INVALID_VALUE;
    else
      desired_time.tm_mday = dummy;

    /* validate year */
    if(Shell_check_hex_scan_str_to_u32(argv[3],&dummy)!=0)
      eRet = HOSTAL_INVALID_TYPE;
    else if(dummy > TM_MAX_YEAR || dummy < TM_MIN_YEAR)
      eRet = HOSTAL_INVALID_VALUE;
    else
      desired_time.tm_year = TM_YEAR(dummy);

    /* validate hour */
    if(Shell_check_hex_scan_str_to_u32(argv[4],&dummy)!=0)
      eRet = HOSTAL_INVALID_TYPE;
    else if (dummy > TM_MAX_HOUR)
      eRet = HOSTAL_INVALID_VALUE;
    else
     desired_time.tm_hour = dummy;

    /* validate minute */
    if(Shell_check_hex_scan_str_to_u32(argv[5],&dummy)!=0)
      eRet = HOSTAL_INVALID_TYPE;
    else if (dummy > TM_MAX_MINUTE)
      eRet = HOSTAL_INVALID_VALUE;
    else
      desired_time.tm_min = dummy;

    /* validate seconds */
    if(Shell_check_hex_scan_str_to_u32(argv[6],&dummy)!=0)
      eRet = HOSTAL_INVALID_TYPE;
    else if (dummy > TM_MAX_SECOND)
      eRet = HOSTAL_INVALID_VALUE;
    else
      desired_time.tm_sec = dummy;
        
    switch (eRet)
    {
      case  HOSTAL_OK:
        if(!RTC_SetTime(&desired_time))
        {
            printf("Failed to set time.\r\n");
            eRet = HOSTAL_ERROR;
        }
        else
        {
            _time_print();
        }
        break;
      case HOSTAL_INVALID_TYPE:
        printf("No numeric argument type received.\r\n\r\n");
        break;
      case HOSTAL_INVALID_VALUE:
        printf("Invalid argument value received.\r\n\r\n");
        break;
      default:
        printf("Unknown error.\r\n\r\n");
        break;
    }
  }
  else
  {
    eRet = HOSTAL_ERROR;
  }

  return eRet;
}

// todo temp for testing
struct tm RTC = {30/*sec*/,1/*min*/,12/*hour*/,1/*mday*/,0/*mon*/,70/*year*/,0/*wday*/,1/*yday*/,0/*DTS*/};

/*****************************************************************************/
/*! returns real time clock i2c value
 *   \return RTC in struct tm                                                */
/*****************************************************************************/
HOSTAL_RESULT_E HOSTAL_RTC_GetTime (struct tm* pRTC)
{
  HOSTAL_RESULT_E eRet = HOSTAL_OK;
  uint8_t rtc[RTC_TIMEDATE_REG_COUNT];
  uint8_t cmd=0x1;//start read data from Register 0x1- Seconds.
  uint8_t val;
  if (NULL == pRTC)
  {
    return HOSTAL_ERROR;
  }

  if(g_App_Control > 0)
  {
    if (kMksOk == RTC_read(&cmd, RTC_MAX_ARG_COUNT, &(rtc[RTC_SECONDS])))
    {
      /* convert RTC BCD data to tm structure */
      val = rtc[RTC_SECONDS]&FLAG_MASK_SEC_MIN;
      pRTC->tm_sec = val - 6 * (val >> 4);
      val = rtc[RTC_MINUTES]&FLAG_MASK_SEC_MIN;
      pRTC->tm_min = val - 6 * (val >> 4);
      val = rtc[RTC_HOURS];
      pRTC->tm_hour = val - 6 * (val >> 4);
      val = rtc[RTC_DAYS];
      pRTC->tm_mday = val - 6 * (val >> 4);
      pRTC->tm_wday = rtc[RTC_WEEKDAYS];
      val = rtc[RTC_MONTHS];
      pRTC->tm_mon = val - 6 * (val >> 4) - TM_UNIX_MONTH_OFS;
      val = rtc[RTC_YEARS];
      pRTC->tm_year = val - 6 * (val >> 4)  + TM_UNIX_YEAR_OFS; // UNIX year start from 1900, RTC 0 is 2000;
      //printf("%d %d %d w%d %d:%d:%d" NEWLINE, pRTC->tm_year, pRTC->tm_mon, pRTC->tm_mday, pRTC->tm_wday, pRTC->tm_hour, pRTC->tm_min, pRTC->tm_sec);
      //printf("RTC %02x %02x" NEWLINE, rtc[RTC_YEARS], rtc[RTC_MONTHS]);
    }
    else
      eRet=HOSTAL_ERROR;
  }
  else
    memcpy(pRTC, &RTC, sizeof (RTC));

  return eRet;
}

/*****************************************************************************/
/*! returns real time clock i2c event status values
 *   \return status bit OS-Oscillator stop, EMON-event monitor               */
/*****************************************************************************/
HOSTAL_RESULT_E HOSTAL_RTC_GetStatusBit (uint8_t* os, uint8_t* emon)
{
  HOSTAL_RESULT_E eRet = HOSTAL_OK;
  uint8_t status_bit[2];
  uint8_t cmd=0x1;//start read data from Register 0x1- Seconds.

    if (kMksOk == RTC_read(&cmd, 2, status_bit))
    {
      /* convert RTC data to tm structure */
      *os   =  ((status_bit[0]&(FLAG_MASK_SEC_MIN^0xff))>>7);  //masking the OS bit
      *emon =  ((status_bit[1]&(FLAG_MASK_SEC_MIN^0xff))>>7);  //masking the EMON bit

    }
    else
      eRet=HOSTAL_ERROR;


  return eRet;
}

/*****************************************************************************/
/*! sync netX time with RTC
 *   \return HOSTAL_OK                                                       */
/*****************************************************************************/
HOSTAL_RESULT_E HOSTAL_RTC_SyncTime (bool bNetX)
{
  struct tm time_rtc = {0};
  time_t seconds = 0;

  //todo check for scheduled update time

  // sync netX
  if (true == bNetX)
  {
    HOSTAL_RTC_GetTime(&time_rtc);
    seconds = mktime(&time_rtc);

    /* update System Time */
    s_ulSysLastMilliSec = SYSTEM_TICK_COUNTS_TO_MILLI_SECONDS(GetSysCounter());
    s_ulRTCSeconds = seconds;
    s_ulTimeSec = seconds;
  }
  // sync RTC
  else
  {
    struct tm* ptime_rtc = &time_rtc;

    ptime_rtc = localtime((time_t*) &s_ulRTCSeconds);
    HOSTAL_RTC_SetTime(ptime_rtc);
  }
  return HOSTAL_OK;
}

/*****************************************************************************/
/*! update netX time
 *   \return HOSTAL_OK                                                       */
/*****************************************************************************/
HOSTAL_RESULT_E HOSTAL_Update_netXTime ()
{
  static uint16_t sCount = 0;
  if (sCount++ % 20000 == 0)
    _time_get (NULL);
  return HOSTAL_OK;
}

/*****************************************************************************/
/*! sets real time clock i2c value
 *   \return HOSTAL_OK on success, HOSTAL_ERROR on error                     */
/*****************************************************************************/
HOSTAL_RESULT_E HOSTAL_RTC_SetTime(struct tm* ptime)
{
  uint8_t rtc[RTC_TIMEDATE_REG_COUNT];
  uint8_t startwatch[START_WATCH_CMD_BSIZE]={STOP_ENABLE_RTCCLK_REG_ADR, STOP_ENABLE__RTCCLK_REG_CLKRUNS_VAL};  

  if (NULL == ptime)
  {
    return HOSTAL_ERROR;
  }

  if(g_App_Control > 0)
  {
    /* convert tm structure to RTC data  */
    rtc[RTC_100th_SECONDS]=         0;    
    rtc[RTC_SECONDS] = 16 * (ptime->tm_sec / 10) + ptime->tm_sec % 10;    
    rtc[RTC_MINUTES] = 16 * (ptime->tm_min / 10) + ptime->tm_min % 10;
    rtc[RTC_HOURS]   = 16 * (ptime->tm_hour / 10) + ptime->tm_hour % 10;
    rtc[RTC_DAYS]    = 16 * (ptime->tm_mday / 10) + ptime->tm_mday % 10;
    rtc[RTC_WEEKDAYS]= ptime->tm_wday;
    rtc[RTC_MONTHS]  = 16 * ((ptime->tm_mon + TM_UNIX_MONTH_OFS) / 10) + (ptime->tm_mon + TM_UNIX_MONTH_OFS) % 10;
    // UNIX year start from 1900, RTC 0 is 2000
    rtc[RTC_YEARS]   = 16 * ((ptime->tm_year - TM_UNIX_YEAR_OFS) / 10) + (ptime->tm_year - TM_UNIX_YEAR_OFS) % 10;

    //printf("Set %d %d %d w%d %d:%d:%d" NEWLINE, ptime->tm_year, ptime->tm_mon, ptime->tm_mday, ptime->tm_wday, ptime->tm_hour, ptime->tm_min, ptime->tm_sec);
    //printf("Set RTC %02x %02x" NEWLINE, rtc[RTC_YEARS], rtc[RTC_MONTHS]);

    uint8_t currenttime[STOP_WATCH_AND_SET_TIME_CMD_BSIZE]=
                              {STOP_ENABLE_RTCCLK_REG_ADR,\
                              STOP_ENABLE_RTCCLK_REG_CLKSTOPPED_VAL,\
                              CLEAR_PRESCALAR,\
                              rtc[RTC_100th_SECONDS] /*100th_seconds*/,\
                              rtc[RTC_SECONDS]/*sec 0->59*/,\
                              rtc[RTC_MINUTES]/*min  0->59*/,\
                              rtc[RTC_HOURS]/*hour*/,\
                              rtc[RTC_DAYS]/*Day 1->31*/,\
                              rtc[RTC_WEEKDAYS],/*weekday 0->6*/\
                              rtc[RTC_MONTHS],/*month  1->12*/\
                              rtc[RTC_YEARS]}/*year   0->99*/;
    /* set current time */
    if (kMksOk!=RTC_write(currenttime,STOP_WATCH_AND_SET_TIME_CMD_BSIZE))
      return HOSTAL_ERROR;
    /* start RTC */
    if (kMksOk!=RTC_write(startwatch,START_WATCH_CMD_BSIZE))
      return HOSTAL_ERROR;
  }

  return HOSTAL_OK;
}
