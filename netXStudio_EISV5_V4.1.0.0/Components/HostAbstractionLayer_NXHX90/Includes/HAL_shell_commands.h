/**
 * @copyright (c) 2022  MKS Instruments, Inc. All rights reserved.
 * The information herein is confidential, and includes
 * trade secrets and other proprietary information.
 *
 * HAL_shell_commands.h
 *
 *  Created on: Oct 11, 2022
 *      Author: RobertsJ
 */

#ifndef _HAL_SHELL_COMMANDS_H_
#define _HAL_SHELL_COMMANDS_H_

#include "switch.h"
#include "shell.h"
#include "watchdog_driver.h"
#include "rtc.h"
#include "led.h"
#include "digital_input.h"
#include "digital_output.h"

//Add HAL commands here.
#define HAL_SHELL_CMDS  \
  APP_SHELL_CMDS \
  WATCHDOG_SHELL_CMDS \
  SHELL_LINK_CMDS \
  SWITCH_SHELL_CMDS \
  RTC_SHELL_CMDS \
  DIGITAL_IN_SHELL_CMDS \
  DIGITAL_OUT_SHELL_CMDS \
  LED_SHELL_CMDS \
  SHELL_TEMP_CMDS \
  SHELL_NVRAM_CMDS \
  DIAG_CMDS \

#endif /* _HAL_SHELL_COMMANDS_H_ */
