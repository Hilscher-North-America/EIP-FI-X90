/**
 * \copyright (c) 2022  MKS Instruments, Inc. All rights reserved.
 * The information herein is confidential, and includes
 * trade secrets and other proprietary information.
 */

/**
 * \file mks_mcu.h
 *
 * \par Modification History
 *   | Date     | Initials | Notes |
 *   | -------- | -------- | -----------------------------------------------------------------------|
 *   | 10/06/22 | JR       | Initial file, ported from NGMODIO |
 */

#ifndef _INCLUDE_MKS_MCU_H_
#define _INCLUDE_MKS_MCU_H_

#define MILLISECONDS_PER_SECOND ((uint32_t) 1000)
#define NANOSECONDS_PER_MICROSECOND ((uint32_t) 1000)
#define KHZ ((uint32_t) 1000)
#define MHZ ((uint32_t) 1000000)

#define PLL_CLK_HZ           ((uint32_t) 100*MHZ) /*system clock,10ns*/

#define CPU_BUS_CLK_HZ       (PLL_CLK_HZ)
#define CM4_CORE_CLK_HZ      (CPU_BUS_CLK_HZ)
#define CPU_CORE_CLK_HZ      (CM4_CORE_CLK_HZ)

#define MICRO_SECONDS_TO_SYSTEM_TICK_COUNTS(x) (uint32_t)((CPU_CORE_CLK_HZ / MHZ) * ((uint32_t)x))
#define MILLI_SECONDS_TO_SYSTEM_TICK_COUNTS(x) (uint32_t)((CPU_CORE_CLK_HZ / KHZ) * ((uint32_t)x))
#define SYSTEM_TICK_COUNTS_TO_MILLI_SECONDS(x) (uint32_t)( ((uint32_t)x) / (CPU_CORE_CLK_HZ / KHZ))
#define SYSTEM_TICK_COUNTS_TO_MICRO_SECONDS(x) (uint32_t)( ((uint32_t)x) / (CPU_CORE_CLK_HZ / MHZ))
#define SYSTEM_TICK_COUNTS_TO_NANO_SECONDS(x) SYSTEM_TICK_COUNTS_TO_MICRO_SECONDS( ((uint32_t)x) * NANOSECONDS_PER_MICROSECOND)

#define AND_EQUALS_REG(reg, value) reg &= value
#define OR_EQUALS_REG(reg, value)  reg |= value
#define READ_REG(reg)              (reg)
#define WRITE_REG(reg, value)      reg = value
#define XOR_EQUALS_REG(reg, value) reg ^= value

void mks_netX90_system_restart(void);

#endif /* _INCLUDE_MKS_MCU_H_ */
