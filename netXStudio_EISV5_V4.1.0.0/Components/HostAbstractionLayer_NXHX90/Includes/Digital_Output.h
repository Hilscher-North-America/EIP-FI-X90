#ifndef DIGITAL_OUTPUT_H_
#define DIGITAL_OUTPUT_H_

#define   HOSTAL_DEVICE_COILS_COUNT           64
#define   HOSTAL_DEVICE_PHISICAL_COILS_COUNT  40
#define   HOSTAL_DEVICE_TX_BYTE_SIZE          HOSTAL_DEVICE_PHISICAL_COILS_COUNT/8           /* SPI data size sent */
#define   HOSTAL_DEVICE_OUT_BYTE_SIZE         HOSTAL_DEVICE_COILS_COUNT/8   /* data size received from PLC */


#include "hostAbstractionLayer.h"

extern bool g_DoAppControl;

#define DIGITAL_OUT_SHELL_CMDS \
  {"write_do" , NULL , Shell_WriteSingleDO, "<channel> Write a single Digital Output"}, \
  {"write_do_all" , NULL , Shell_WriteAllDO, "Write all Digital Outputs"}, \
  {"do_ctrl"     , NULL , Shell_DoAppControl  , "<0> Shell <1> APP (re)assign ownership of DO driver SPI events"}, \


uint32_t Shell_WriteSingleDO(uint32_t argc, const char *argv[]);
uint32_t Shell_WriteAllDO(uint32_t argc, const char *argv[]);
uint32_t Shell_DoAppControl( uint32_t argc, const char *argv[] );

HOSTAL_RESULT_E SPI_DoInit(void);
HOSTAL_RESULT_E SPI_StartOutputDataTransmit(uint8_t* pOutData);

#endif /* DIGITAL_OUTPUT_H_ */