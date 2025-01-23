#include <stdio.h>
#include <stdint.h>

#include "sys.h"
#include "Watchdog.h"
#include "Server.h"
#include "App_SystemPackets.h"
#include "switch.h"

#define DEFAULT_WATCHDOG_INTERVAL  255 /* using KFI default value */
////////////////////////////////////////////////////////////////////////////////////////

static unsigned char 	g_ui8_last_op_time_kick = 0;
static unsigned char 	g_ui8_first_op_time_kick = 0;
static _stimeVal 		  g_last_op_time = {0,0};
static _stimeVal      g_time = {0,0};
static unsigned long  g_ui32Prevtick;
static unsigned long  (*g_pCounterFunc)() = NULL; //keep declaration same as dma_tmr_get_cntrX() calls;

unsigned short 			  g_ui16WatchdogCounts = 0;
unsigned short        g_ui16WatchdogInterval = DEFAULT_WATCHDOG_INTERVAL;
_eWatchdogState 	    g_WatchdogState = WD_UNINITIALIZED;

////////////////////////////////////////////////////////////////////////////////////////

unsigned short getWatchdogInterval ()
{
	return g_ui16WatchdogInterval;
}

unsigned short getWatchdogCount ()
{
	return g_ui16WatchdogCounts;
}

void updateWatchdogCount ()
{
	//don't allow count roll over
	if (g_ui16WatchdogCounts != 0xFFFF)
	{
		g_ui16WatchdogCounts++;
	}
}

//NOTE::Assumes call intervals are under 1 sec!
void getTime(_stimeVal *tv)
{
  unsigned long ui32Tick;
  unsigned long ui32Elapsed;

  if (g_pCounterFunc == NULL)
    return;

  ui32Tick = g_pCounterFunc();

  //_int_disable();
    if ( ui32Tick >= g_ui32Prevtick)
      ui32Elapsed = ui32Tick - g_ui32Prevtick;
    else
      ui32Elapsed = (0xFFFFFFFF - g_ui32Prevtick) + ui32Tick + 1;

    ui32Elapsed += g_time.tv_usec;

    g_time.tv_sec += (int) (ui32Elapsed / 1000000UL);
    g_time.tv_usec = (int) (ui32Elapsed % 1000000UL);

    *tv = g_time;

    g_ui32Prevtick = ui32Tick;
  //_int_enable();
}

void kickWatchdog(void)
{
	g_ui8_first_op_time_kick = 1;
	g_ui8_last_op_time_kick = 1;
	getTime(&g_last_op_time);
}

int WatchdogInterval_Handler (
	op_type_t	opcode,
	data_type_t	data_type,
	addr_t		offset,			/* address of first unit */
	unsigned			qty,			/* number of data units */
	void *				data,
	void *				arg
)
{
	uint16_t ui16Data;
#ifndef DEBUG_MB_WATCHDOG	//if in debug, remove kick so calls to this handler will not impact watchdog
	//old FI does not kick the dog for this register
	//kickWatchdog();
#endif	
	
	if( data_type != IND_OUT )
		return -EXC_ILL_ADDR;
		
	if (qty == 0)
		return (int)qty;
	
	if (qty > 1)
	    qty = 1;

	if (offset != 0)
	  return -EXC_ILL_ADDR;

	if ((opcode != IO_GET) && (opcode != IO_SET))
		return -EXC_ILL_ADDR;
	
	if(opcode == IO_GET){			
		*(unsigned short*)data = g_ui16WatchdogInterval;
		return (int)qty*2;
	}else{
		ui16Data = *(unsigned short*)data;
		if ((ui16Data != 0) && ( (ui16Data < MIN_WATCHDOG_INTERVAL) || (ui16Data > MAX_WATCHDOG_INTERVAL) ) ){//limit watchdog intervals to >= 50ms
			return -(EXC_ILL_VAL);
		}

		kickWatchdog();	
		
		if(FI_WD_ENABLED & g_FI_unit_stats.mode)
		{
			if (ui16Data != g_ui16WatchdogInterval)
			{			
				g_WatchdogState = WD_FORCED_TRANSITION_STATE;
				g_ui16WatchdogInterval = ui16Data;				
			}
		}
		
		return (int)qty*2;
	}
}

int WatchdogCnt_Handler (
	op_type_t	opcode,
	data_type_t	data_type,
	addr_t		offset,			/* address of first unit */
	unsigned			qty,			/* number of data units */
	void *				data,
	void *				arg
)
{
	
#ifndef DEBUG_MB_WATCHDOG	//if in debug, remove kick so calls to this handler will not impact watchdog
	//old FI does not kick the dog for this register
	//kickWatchdog();
#endif	
		
	if( data_type != IND_OUT)
		return -EXC_ILL_ADDR;
		
	if (qty == 0)
		return (int)qty;
	
	if (qty > 1)
	  qty = 1;

	if (offset != 0)
		return -EXC_ILL_ADDR;
	
	if ((opcode != IO_GET) && (opcode != IO_SET))
		return -EXC_ILL_ADDR;
	
	if (opcode == IO_GET){
		*(unsigned short*)data = g_ui16WatchdogCounts;
	}else{
		if ( *(unsigned short*)data != 0){
			return (-EXC_ILL_VAL);
		}
			
		g_ui16WatchdogCounts = 0;
	}
	return (int)qty*2;
}

int Watchdog_Update ()
{	
	if (WD_FORCED_TRANSITION_STATE == g_WatchdogState)
	{
		g_WatchdogState = WD_OK;
		// update wd settings
		if (FI_WD_ENABLED & g_FI_unit_stats.mode)
		{
			if (0 != App_SysPkt_SetWd(g_ui16WatchdogInterval))
				return -EXC_GTW_PATH;
		}		
	}

	return 0;
}
			
