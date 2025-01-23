/* Includes */
#include <stdio.h>

#include "sys.h"

#include "Server.h"
#include "FiHandlers.h"
#include "Watchdog.h"

#define DATA_TYPE_TO_REGS(datatype,num_inst) (((sizeof(datatype) +1)/sizeof(uint16_t))*num_inst)

/*######################################################################*/
/* private definitions */


/*######################################################################*/
/* public functions */

int RegisterHandlers(void)
{	
		/////////////////////////////////////////// Input Registers ///////////////////////////////////////////////////////////
	  if (acyclic_reg_handler(IND_IN, 	65000, 4,	time_handler,	NULL, "Time 32 bit sec, 32 bit us BB") < 0)
    	return -1;

		if (acyclic_reg_handler(IND_IN, 	65100, 1,	temperature_handler,	NULL, "Temperature, 1/100 �C; i16") < 0)
  		return -1;

		//MKS Device Information
    if(acyclic_reg_handler(IND_IN, 	10000U, DATA_TYPE_TO_REGS(_sDevInfo,1), Device_Info_Handler, NULL, "Device Info" ) < 0)
    	return -1;    

		/////////////////////////////////////////// Settable Registers //////////////////////////////////////////////////////////
		if(acyclic_reg_handler(IND_OUT, 60000U, 1, Mfg_Key_Handler, NULL, "Mfg Key" ) < 0)
			return -1;		

		//todo: test simulate manufacturing mode
		//g_FI_unit_stats.mfg_enabled = true; //todo remove this line

		// 60001 to 60024
   	if (g_FI_unit_stats.mfg_enabled) {
			if(acyclic_reg_handler(IND_OUT, 60001U, (sizeof(_sManuf)/2), Manufacturing_Handler, NULL, "Manufacturing" ) < 0)
				return -1;	
   	} 

		// 60025 to 60030
 		if (g_FI_unit_stats.mfg_enabled) {
			if(acyclic_reg_handler(IND_OUT, 60025U, (sizeof(_sManRTC)/2), RTC_Time_Mfg_Handler, NULL, "RTC Time" ) < 0)
				return -1;	
   	} 

		// 60031
   	if (g_FI_unit_stats.mfg_enabled) {
			if(acyclic_reg_handler(IND_OUT, 60031U, 1, Manufacturing_Handler_aio, NULL, "Manufacturing AIO Config" ) < 0)
				return -1;	
   	}

		if(acyclic_reg_handler(IND_OUT, 65100U, 1U, WatchdogInterval_Handler, NULL, "Watchdog Timeout, millsec; ui16" ) < 0)
			return -1;	

		if (acyclic_reg_handler(IND_OUT, 65101U, 1U, WatchdogCnt_Handler,	NULL, "Watchdog Count, counts; ui16" ) < 0)
   		return -1;   	
       
    return 0;
}
