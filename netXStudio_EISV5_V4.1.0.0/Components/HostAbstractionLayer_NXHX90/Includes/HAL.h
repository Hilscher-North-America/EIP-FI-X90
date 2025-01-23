/**
  * @copyright (c) 2022  MKS Instruments, Inc. All rights reserved.
  * The information herein is confidential, and includes
  * trade secrets and other proprietary information.
  *
  * @file HAL.h
  */

#ifndef _HAL_H_
#define _HAL_H_

void HOST_HAL_Init(void);

#include "HAL_shell_commands.h"

//Add your HAL driver includes here.
#include "rtc.h"
#include "switch.h"
#include "led.h"
#include "Digital_Input.h"
#include "Digital_Output.h"


#define HAL_ERROR_STR   "HAL ERROR"
#endif /* _HAL_H_ */
