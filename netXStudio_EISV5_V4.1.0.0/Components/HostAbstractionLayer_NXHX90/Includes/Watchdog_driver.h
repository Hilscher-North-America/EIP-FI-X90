/*
 * Watchdog_driver.h
 *
 *  Created on: Feb 11, 2016
 *      Author: DyessGre
 */

#ifndef WATCHDOG_DRIVER_H_
#define WATCHDOG_DRIVER_H_

#define WATCHDOG_SHELL_CMDS \
  {"start_io_wd" , NULL , Shell_WdStartIo, "Starts kicking WD for IO"}, \
  {"stop_io_wd" , NULL , Shell_WdStopIo, "Stops kicking WD for IO"}, \

uint32_t Shell_WdStartIo(uint32_t argc, const char *argv[]);
uint32_t Shell_WdStopIo(uint32_t argc, const char *argv[]);

extern void WatchdogKick();
extern void Watchdog_IO_Rst();
extern void Watchdog_Bite();
extern void Watchdog_Release();

#endif /* WATCHDOG_DRIVER_H_ */

