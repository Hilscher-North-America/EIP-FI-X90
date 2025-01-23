/**
  * \copyright (c) 2022  MKS Instruments, Inc. All rights reserved.
  * The information herein is confidential, and includes
  * trade secrets and other proprietary information.
  *
  * \file mks_mcu.c
  *
  * Types and enumerations shared by the MKS code.
  *
  * MODIFICATION HISTORY
  *   | Date     | Initials | Notes                                                                  |
  *   | -------- | -------- | -----------------------------------------------------------------------|
  *   | 01/24/21 | CC       | Created this file                                                      |
  *   | 02/09/21 | CC       | Added SysCounterIsInit()                                               |
  *   | 10/06/22 | JR       | Ported to Hilscher Netx90                                              |
  */

//#include "mks_mcu.h"
#include "mks_syscounter.h"
#include "netx90_app.h"
#include "core_cm4.h"
//#include "mks_mcu_watchdog.h"

/*! \brief Initialize the System Counter
 *
 *  \return MksBoolType
 *  * MKS_TRUE  if system counter is running
 *  * MKS_FALSE if system counter is not running
 *
 *  \par Algorithm
 *  1. Check that the cycle counter is increasing
 *  2. Return error if cycle counter is same with consecutive reads to the register
 *
 *  Related requirements:
 *    none specified
 */
MksBoolType SysCounterIsInit(void){

  if (GetSysCounter() == GetSysCounter()) {
    return MKS_FALSE;
  }
  else {
    return MKS_TRUE;
  }
}

/*! \brief Initialize the System Counter
 *
 *  \return MksReturnCodes
 *  * kMksOk if no error;
 *  * kMksSystemCounterNotRuning
 *
 *  \par Algorithm
 *
 *  1. Enable the Trace and Debug blocks of the CPU (TRCENA in DEMCR)
 *  2. Enable the Cycle Counter in the DWT
 *  3. Check that the cycle counter is increasing
 *  4. Return error if cycle counter is same with consecutive reads to the register
 *
 *  Related requirements:
 *    none specified
 */
MksReturnCodes SysCounterInitialize(void){

 /*Enable the CYCCNT to use for SysCounter.  */
  OR_EQUALS_REG(CoreDebug->DEMCR, CoreDebug_DEMCR_TRCENA_Msk);
  WRITE_REG(DWT->CYCCNT, 0);
  OR_EQUALS_REG(DWT->CTRL, DWT_CTRL_CYCCNTENA_Msk);

  if( !(SysCounterIsInit()) ) {
    return kMksSystemCounterNotRuning;
  }

  return kMksOk;
}

/*! \brief Get Current System Counter

 *  \returns Value of System Counter.
 *
 *  Related requirements:
 *  * none specified
 */
MksUint32Type GetSysCounter(void)
{
    return (MksUint32Type) READ_REG(DWT->CYCCNT);
}


/*! \brief Use System Counter to see if something has timed out in tick counts
 *
 *  \param start_system_count [in] Last count we want to compare to current count for timeout
 *  \param timeout_in_counts  [in] Time to elapse to make a timeout, in tick counts
 *  \returns MKS_TRUE if timeout has occurred.  MKS_FALSE if timeout has not occurred.
 *
 *  \par Algorithm
 *  1. Get current count
 *  2. Check if time out time has been reached.
 *
 *  Related requirements:
 *   * SysCounterInitialize
 */
MksBoolType SysCounterTimeoutCounts(MksUint32Type start_system_count, MksUint32Type timeout_in_counts)
{
  MksBoolType return_bool = MKS_FALSE;
  MksUint32Type current_count = GetSysCounter();

  if ((current_count - start_system_count) > timeout_in_counts){
    return_bool = MKS_TRUE;
  }

  return return_bool;
}

/*! \brief Busy wait for specified number of milliseconds, servicing the watchdog every millisecond.
 *
 *  This method should maintain accuracy in ticks over the range of inputs.
 *
 *  \param wait_in_msec  [in] Time in milliseconds to wait
 *
 *  \par Algorithm
 *  1. Get current count as start count
 *  2. Get SysCount ticks needed for 1 ms wait
 *  3. Wait 1ms by comparing SysCount difference and ticks needed for wait.
 *  4. Service the watchdog.
 *  5. Update the start count by adding expected difference
 *  6. Repeat Steps 3 to 5 for each ms
 *
 *  Related requirements:
 *   * SysCounterInitialize
 */
void SysCounterWaitMilliSeconds(MksUint16Type wait_in_msec)
{
  MksUint32Type start_system_count = GetSysCounter();
  // time_delta is 1 ms, which is 1000 us.
  MksUint32Type time_delta = MILLI_SECONDS_TO_SYSTEM_TICK_COUNTS(1);

  while (wait_in_msec--){
    while (GetSysCounter() - start_system_count < time_delta) {};
    //todo implement for WD: WatchdogService();
    start_system_count += time_delta;
  }
}

/*! \brief Use System Counter to blind wait
 *
 *  \param wait_in_usec  [in] Time in microseconds to wait
 */
void SysCounterWaitMicroSeconds(MksUint16Type wait_in_usec)
{
  MksUint32Type start_system_count = GetSysCounter();
  MksUint32Type time_delta = MICRO_SECONDS_TO_SYSTEM_TICK_COUNTS(wait_in_usec);

  while (GetSysCounter() - start_system_count < time_delta) {};
}


