/**
 * \copyright (c) 2021  MKS Instruments, Inc. All rights reserved.
 * The information herein is confidential, and includes
 * trade secrets and other proprietary information.
 */

/**
 * \file sys_counter.h
 *
 * \par Modification History
 *   | Date     | Initials | Notes                                                        |
 *   | -------- | -------- | -------------------------------------------------------------|
 *   | 02/01/21 | CC       | Initial file                                                 |
 *   | 10/06/22 | JR       | Ported from NGMODIO
 */

#ifndef _MKS_SYSCOUNTER_H_
#define _MKS_SYSCOUNTER_H_

#include "mks_types.h"
#include "mks_mcu.h"

MksBoolType SysCounterIsInit(void);
MksReturnCodes SysCounterInitialize(void);
MksUint32Type GetSysCounter(void);
MksBoolType SysCounterTimeoutCounts(MksUint32Type start_system_count, MksUint32Type timeout_in_counts);
void SysCounterWaitMilliSeconds(MksUint16Type wait_in_msec);
void SysCounterWaitMicroSeconds(MksUint16Type wait_in_usec);

#endif /* _MKS_SYSCOUNTER_H_ */
