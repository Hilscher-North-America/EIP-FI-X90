#ifndef DIGITAL_INPUT_H_
#define DIGITAL_INPUT_H_


#define   HOSTAL_DEVICE_INPUT_REGISTER_COUNT  64
#define   HOSTAL_DEVICE_RX_BYTE_SIZE          HOSTAL_DEVICE_INPUT_REGISTER_COUNT/8  /* data size sent to network */

#include "hostAbstractionLayer.h"

#define DIGITAL_IN_SHELL_CMDS \
  {"read_di"     , NULL , Shell_ReadSingleDI, "<channel> Read a single Digital Input"}, \
  {"read_di_all" , NULL , Shell_ReadAllDI, "Read all Digital Inputs"}, \
  {"di_ctrl"     , NULL , Shell_DiAppControl  , "<0> Shell <1> APP (re)assign ownership of DI driver SPI events"}, \


extern bool g_DiAppControl;

uint32_t Shell_ReadSingleDI(uint32_t argc, const char *argv[]);
uint32_t Shell_ReadAllDI(uint32_t argc, const char *argv[]);
uint32_t Shell_DiAppControl( uint32_t argc, const char *argv[] );

HOSTAL_RESULT_E SPI_DiInit(void);
HOSTAL_RESULT_E SPI_StartInputDataTransmit(uint8_t* pRxSpiData);

#endif /* DIGITAL_INPUT_H_ */
