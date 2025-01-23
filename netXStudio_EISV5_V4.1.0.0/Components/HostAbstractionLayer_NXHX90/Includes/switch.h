/*
 * @copyright (c) 2022  MKS Instruments, Inc. All rights reserved.
 * The information herein is confidential, and includes
 * trade secrets and other proprietary information.
 *
 * @file switch.h
 *
 */

#ifndef _SWITCH_H_
#define _SWITCH_H_

#include "mks_types.h"

typedef enum {
  kIP1_switch,
  kIP2_switch,
  kIP3_switch,
  kMAX_NUM_switches,
} kSwitch;

#define FI_DHCP_MODE        0x0001
#define FI_STATIC_IP_MODE   0x0002
#define FI_WD_ENABLED       0x0004
#define FI_SYS_CFG_MODE     0x0008
#define FI_MFG_CFG_MODE     0x0010

#define FI_MFG_SWITCH_VALUE 978

// Public functions
MksReturnCodes Rotary_I2C_Init();
uint16_t SWITCH_get_value(void);
bool SWITCH_set_mode(uint16_t usSwitchValue);
bool SWITCH_update(void);

///////////////////////////////////////////////////////////////////////////

#define SWITCH_SHELL_CMDS \
  {"----- HAL commands \x01---------------------------", NULL, NULL, "" }, \
  {"switches" , NULL , Shell_ReadSwitchValues , "Read memory switch values"}, \
  {"read_rs" , NULL , Shell_Rotary_update_switches , "Read physical switch values"}, \

uint32_t Shell_ReadSwitchValues( uint32_t argc, const char *argv[] );
uint32_t Shell_Rotary_update_switches( uint32_t argc, const char *argv[] );
#endif /* _SWITCH_H_ */

