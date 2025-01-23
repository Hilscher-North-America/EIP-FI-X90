/*
 * netx90_reset_handler.c
 *
 *  Created on: Dec 15, 2022
 *      Author: RobertsJ
 */

#include "mks_syscounter.h"
#include "netx90_app.h"
#include "core_cm4.h"

#define MAX_WAIT_COMM_RESET_TIME_MSECS  ( 1000  ) /* 1 sec */

#define HIL_SYS_CONTROL_RESET_MODE_COLDSTART                0
#define HIL_SYS_RESET_COOKIE                                0x55AA55AA  /* System Reset cookie */
#define HSF_RESET                         0x0001                      /*!< Reset command bitmask */

typedef struct __attribute__((__packed__)) HIL_DPM_SYSTEM_CONTROL_BLOCK_Ttag
{
  uint32_t  ulSystemCommandCOS;                                  /*!< 0x00 System channel change of state command */
  uint32_t  ulSystemControl;                                     /*!< 0x04 System channel control (only for eCos on netX90/netX4000) */
} HIL_DPM_SYSTEM_CONTROL_BLOCK_T;

/***************************************************************************************************
*!  \brief Communication Processor Reset
*
*   \details Invoke graceful shutdown of Comm and App to avoid potential corruption of flash by Comm
*
*   \sa netXSystem_Restart
***************************************************************************************************/
__STATIC_FORCEINLINE void COMM_ReStart(void){
  volatile HIL_DPM_SYSTEM_CONTROL_BLOCK_T *pSysCtrl;
  volatile uint8_t *pSystemHostFlags;   // Note: instead of using 'typedef __HIL_PACKED_PRE union HIL_DPM_HANDSHAKE_CELL_Ttag'

  pSysCtrl        = (volatile HIL_DPM_SYSTEM_CONTROL_BLOCK_T *) (0xB0000000 + 0xB8); // 0x0B8:0x0BF System control block
  pSystemHostFlags= (volatile uint8_t *) (0xB0000200 + 0x03);
  pSysCtrl->ulSystemControl    = HIL_SYS_CONTROL_RESET_MODE_COLDSTART;
  pSysCtrl->ulSystemCommandCOS = HIL_SYS_RESET_COOKIE;
  *pSystemHostFlags            = HSF_RESET;
}

/***************************************************************************************************
*!  \brief System Reset
*
*   \details Invoke shutdown of system ( graceful if possible, forceful if not )
*
*   \sa COMM_ReStart
***************************************************************************************************/
void mks_netX90_system_restart(void){

  COMM_ReStart();

  //if COMM Restart is successful then it will also restart APP. So busy wait until reset...
  //SysCounterWaitMicroSeconds( MAX_WAIT_COMM_RESET_TIME_USECS );
  SysCounterWaitMilliSeconds( MAX_WAIT_COMM_RESET_TIME_MSECS );

  //Assume COMM failed to reset system, so force reset via AICR.
  //( apparently AICR is applicable to both APP and COMM processors )
  NVIC_SystemReset();
}
