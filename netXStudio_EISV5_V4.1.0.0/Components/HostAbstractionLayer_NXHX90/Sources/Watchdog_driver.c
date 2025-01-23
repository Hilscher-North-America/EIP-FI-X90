/*
 * Watchdog_driver.c
 *
 *  Created on: Feb 11, 2016
 *      Author: DyessGre
 */

#include <stdio.h>

#include "netx_drv.h"
#include "core_cm4.h"
#include "mks_mcu.h"
#include "mks_syscounter.h"
#include "Watchdog_Driver.h"

#ifndef UNIT_TEST  // Mocking the h/w macros that are used here.
//#include "WDI.h"		// Watchdog Kick GPIO
//#include "IO_nCLR.h"	// Watchdog Clear GPIO
#else
#include "Watchdog_Mocks.h"
#endif

#define HW_WATCHDOG_KICK_TIME 100
#define FI_WATCHDOG_KICK_PIN DRV_DIO_ID_GPIO_6
#define FI_WATCHDOG_RESET_PIN DRV_DIO_ID_PIO_27

static bool wdstarted = true;

/*****************************************************************************/
/*! kick hardware watchdog periodically, must be kicked within 1600ms
    WDI configured as MMIO5
    Rising of falling edge will kick the watchdog
 *   \return None                                                            */
/*****************************************************************************/
void WatchdogKick()
{
	static uint8_t toggle = 1;
	
	if (wdstarted == false)
		return;

	/* check if enough time passed since last kick */
	//TODO verify if there is min Kick time requirement
  /*if (SysCounterTimeoutCounts(tmp_period_ms_cntr, ((uint32_t)MILLI_SECONDS_TO_SYSTEM_TICK_COUNTS(HW_WATCHDOG_KICK_TIME))) == MKS_FALSE)
  {
		return;    
  }

	tmp_period_ms_cntr = GetSysCounter();*/
		
	if (toggle == 1)
	{
		/* falling edge */
	  DRV_DIO_ChannelOutSet(FI_WATCHDOG_KICK_PIN);
    toggle = 0;
	}
	else
	{
		/* rising edge */
		DRV_DIO_ChannelOutReset(FI_WATCHDOG_KICK_PIN);
	  toggle = 1;
	}
}

/*****************************************************************************/
/*! reset hardware watchdog line for 25 us
 *   \return None                                                            */
/*****************************************************************************/
void Watchdog_IO_Rst()
{	
	// Force hardware watchdog to bite
	DRV_DIO_ChannelOutReset(FI_WATCHDOG_RESET_PIN);	// Negative logic.  0 = force WD to clear I/O

#ifndef UNIT_TEST
	/* TODO verify watchdog chip timing */
	// delay for 25us to satisfy LM3710 watchdog timing	
	SysCounterWaitMicroSeconds(25);	
#endif // UNIT_TEST
	
	// Release watchdog
	DRV_DIO_ChannelOutSet(FI_WATCHDOG_RESET_PIN);	// Negative logic.  1 = force WD to disable
}

/*****************************************************************************/
/*! reset hardware watchdog line
 *   \return None                                                            */
/*****************************************************************************/
void Watchdog_Bite()
{	
	// Force hardware watchdog to bite
	DRV_DIO_ChannelOutReset(FI_WATCHDOG_RESET_PIN);	// Negative logic.  0 = force WD to clear I/O
}

/*****************************************************************************/
/*! release hardware watchdog line
 *   \return None                                                            */
/*****************************************************************************/
void Watchdog_Release()
{	
	// Release watchdog
	DRV_DIO_ChannelOutSet(FI_WATCHDOG_RESET_PIN);	// Negative logic.  1 = force WD to disable
}

uint32_t Shell_WdStartIo(uint32_t argc, const char *argv[])
{
	wdstarted = true;
	printf("\r\n Hardware Watchdog is started. \r\n");  

	return 0;
}

uint32_t Shell_WdStopIo(uint32_t argc, const char *argv[])
{
	wdstarted = false;
	printf("\r\n Hardware Watchdog is stopped. \r\n");  

	return 0;	
}
