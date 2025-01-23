#ifndef CHICAGOWATCHDOG_H_
#define CHICAGOWATCHDOG_H_

#include "Server.h"

typedef enum {
	WD_UNINITIALIZED = 0,
	WD_DISABLED,
	WD_FAILURE,
	WD_OK,
	WD_UNKNOWN,
	WD_FORCED_TRANSITION_STATE
} _eWatchdogState;

typedef struct {
  int   tv_sec;
  int   tv_usec;
} _stimeVal;


extern DECLARE_HANDLER(WatchdogInterval_Handler);
extern DECLARE_HANDLER(WatchdogCnt_Handler);

//as defined in old FI
#define MIN_WATCHDOG_INTERVAL	100U
#define MAX_WATCHDOG_INTERVAL	65535U

unsigned short getWatchdogInterval ();
void updateWatchdogCount ();
unsigned short getWatchdogCount ();
int Watchdog_Update ();

#endif /* CHICAGOWATCHDOG_H_ */
