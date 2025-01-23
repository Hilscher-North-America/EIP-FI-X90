#include "sys.h"
#include "fi.h"
#include "mks_mcu.h"

//TODO review this
static int32_t g_FwUpdateStatus=0;

int32_t GetFwUpdateStatus(void) {
	return g_FwUpdateStatus;
}

void UtilDoMfg(void)
{
  switch(MfgGetKey())
  {		
  case RESETKEY:	
    mks_netX90_system_restart();
    break;
  case OBRKEY:
    // save to flash out of box params
    MfgSaveParams(1);
    mks_netX90_system_restart();
    break;
  case NVSAVE:
    // save to flash all params
    MfgSaveParams(0);
    /* clear manufacturer key */
    MfgSetKey(CLEARKEY);
    break;
  default:
    // No action on these values
    break;
  }  
}
