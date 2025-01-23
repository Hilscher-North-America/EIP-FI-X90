/*
 * led.c
 *
 *  Created on: Sep 28, 2022
 *      Author: Vedprakash.Pande
 */

#include "netx_drv.h"
#include "mks_mcu.h"
#include "mks_syscounter.h"
#include "led.h"
#include "stdio.h"
#include "string.h"

/*
 * This is a LED driver for Bi-color LEDs.
 * Current implementation is for 2 LEDs, NET & MOD LEDs. This is configurable by macros below.
 * A 'Led_Configuration_St' structure entry must be populated for every led added. This structure
 * describes the DIO to use, LED identifier & blink timings.
 *
 * LED_Init() should be called in initialize section of main.
 * LED_StateMachine(kLED_EXEC_PERIODIC) is called from main loop.
 * LED_Set(LED_ID_XXX, LED_ST_XXX) is called from application code to set the led to a state (off, on, flashing).
 *
*/

extern uint32_t s_fLink;

/*
 * Time in milliseconds to call LED state machine handling, effect changes in LED states, also implement blinking
 */
#define LED_STATE_MACHINE_PERIOD_MS        (10)
#define LED_STATE_MACHINE_PERIOD_SYSCNTS   ((uint32_t)MILLI_SECONDS_TO_SYSTEM_TICK_COUNTS(LED_STATE_MACHINE_PERIOD_MS))

/*
 * Time in milliseconds to blink LED ON, same time for maintaining LED OFF.
 * MIN & MAX values, when blink timing is set by API.
 */
#define LED_BLINK_TIME_MS       (250)
#define LED_BLINK_TIME_MIN_MS   (LED_STATE_MACHINE_PERIOD_MS)
#define LED_BLINK_TIME_MAX_MS   (5000)

/*
 * This driver needs following functions from netx DIO driver.
 * The following netx DIO driver functions should work for MMIOs as well as PIOs.
 */
#define LED_INIT      DRV_DIO_Init
#define LED_OFF(x)    ((led_conf_st[i].pled_off != NULL) ? led_conf_st[i].pled_off(x) : DRV_ERROR)
#define LED_ON(x)     ((led_conf_st[i].pled_on != NULL) ? led_conf_st[i].pled_on(x) : DRV_ERROR)
#define LED_TOGGLE    DRV_DIO_ChannelOutToggle


/*
 * Mask to verify if a LED state is one of flashing states.
*/
#define LED_ST_FLASHING_MASK  (kLED_ST_FLASHING_RED | kLED_ST_FLASHING_GREEN | kLED_ST_FLASHING_AMBER)


/*
 * LED configuration structure.
 * This structure is assigned values, as per LEDs assigned in hardware & control requirements.
 */
typedef struct
{
  /* netx driver DIO to use to activate Red LED, for this Bi-color LED. */
  DRV_DIO_ID_T led_red_id;

  /* netx driver DIO to use to activate Green LED, for this Bi-color LED. */
  DRV_DIO_ID_T led_green_id;

  /* Blink timings for this LED. */
  uint32_t led_blink_ms;

  /* Driver APIs to use for turning LED ON / OFF.
   * DIO output polarity may need inversion for LED ON / OFF operation. */
  DRV_STATUS_E (*pled_on)(DRV_DIO_ID_T);
  DRV_STATUS_E (*pled_off)(DRV_DIO_ID_T);

}Led_Configuration_St;


/*
 * LED control structure.
 * API sets values for control of LEDs. Also has run-time variables for each LED.
 */
typedef struct
{
  /* LED state written by calling API. */
  Led_States led_state_cntrl;

  /* LED state status, after execution of previous API. */
  Led_States led_state_status;

  /* Run-time variable for blink time - in syscounts. */
  uint32_t led_blink_cntr_syscnts;

  /* Run-time variable for blink period - in syscounts. */
  uint32_t led_blink_period_syscnts;

}Led_Control_St;

#define LED_DRV_FIFX90_TARGET_HW

/*
 * netx driver DIO pins assigned for LED control, as per hardware design.
 */
#ifdef LED_DRV_NRPEB_H90_BOARD
  #define LED_DIO_NET_GREEN     DRV_DIO_ID_MMIO_8
  #define LED_DIO_NET_RED       DRV_DIO_ID_MMIO_9  
  #define LED_DIO_HEALTH_RED    DRV_DIO_ID_MMIO_10
  #define LED_DIO_HEALTH_GREEN  DRV_DIO_ID_MMIO_11
#endif
#ifdef LED_DRV_FIFX90_TARGET_HW
  #define LED_DIO_NET_GREEN     DRV_DIO_ID_GPIO_3
  #define LED_DIO_NET_RED       DRV_DIO_ID_GPIO_2  
  #define LED_DIO_HEALTH_RED    DRV_DIO_ID_GPIO_1
  #define LED_DIO_HEALTH_GREEN  DRV_DIO_ID_GPIO_0
#endif


/*
 * An array of Led_Configuration_St structure has elements in the same order as Enum Led_Status_Indicators.
 */
Led_Configuration_St led_conf_st[kNO_OF_LED_ID] = {
  {
    //kLED_ID_NET
    LED_DIO_NET_RED,
    LED_DIO_NET_GREEN,
    LED_BLINK_TIME_MS,
    &DRV_DIO_ChannelOutSet,//on
    &DRV_DIO_ChannelOutReset//off
  },
  {
    //kLED_ID_HLT
    LED_DIO_HEALTH_RED,
    LED_DIO_HEALTH_GREEN,
    LED_BLINK_TIME_MS,
    &DRV_DIO_ChannelOutSet,//on
    &DRV_DIO_ChannelOutReset//off
  }
};

/*
 * An array of Led_Control_St structure has elements in the same order as Enum Led_Status_Indicators.
 */
Led_Control_St led_cntrl_st[kNO_OF_LED_ID];

/* Time counting to execute the LED state machine.  */
uint32_t led_sm_period_cntr_syscnts = 0;

uint16_t s_usHealthStatus   = 0;
uint16_t s_usNetworkStatus   = 0;

/*
 * Couple the LED driver with application or serial console testing.
 */
typedef enum {
  ktestmode_false = 0,
  ktestmode_true = 1,
}Led_testmode_t;

Led_testmode_t g_led_testmode;


/***********************************
 *  Variables for testing LED driver from serial console */

#ifdef LED_DRV_INCLUDE_SERIAL_CONSOLE

/*
 * Constants required for parsing command line arguments from serial console.
 */
#define SERIAL_CMD_LEN_BYTES  (15)  /* Maximum command length in bytes */
#define MAX_SERIAL_CMD_ARGS   (5)   /* Maximum number of arguments to parse */
#define SERIAL_ARG_LEN_BYTES  (15)  /* Maximum argument length in bytes */

/*
 * Structure to store output of parsing function - command & arguments
 */
typedef struct
{
  /* Command received from serial console */
  char cmd[SERIAL_CMD_LEN_BYTES];

  /* Arguments received from serial console */
  char args[MAX_SERIAL_CMD_ARGS][SERIAL_ARG_LEN_BYTES];

  /* Number of arguments received from serial console */
  uint8_t no_args;

}Serial_Cmd_St_t;

/*
 * For console_led_testmode command:
 * Possible values of argument 0 enumerators, corresponding to the argument 0 string
 */
typedef enum {
  kfalse = 0,
  ktrue = 1,
  kno_of_testmode=2
}Led_testmode_arg0_t;

/*
 * For console_led_testmode command:
 * Possible values of argument 0 string
 */
char str_led_testmode[kno_of_testmode][SERIAL_ARG_LEN_BYTES] =
{
  "false",
  "true",
};


/*
 * For console_led_set command:
 * Possible values of argument 0 enumerators, corresponding to the argument 0 string
 */
typedef enum {
  kled_id_net = 0,
  kled_id_hlt,
  kno_of_led_ids
}led_set_led_id_enum_t;

/*
 * For console_led_set command:
 * Possible values of argument 0 string
 */
char str_led_id[kno_of_led_ids][SERIAL_ARG_LEN_BYTES] =
{
  "net",
  "hlt"
};

/*
 * For console_led_set command:
 * Possible values of argument 1 enumerators, corresponding to the argument 1 string
 */
typedef enum {
  kled_st_off = 0,
  kled_st_solid_red,
  kled_st_solid_green,
  kled_st_solid_amber,                   /* Both Red & Green LEDs are on. */
  kled_st_flashing_red,
  kled_st_flashing_green,
  kled_st_flashing_amber,                /* Both Red & Green LEDs are flashing. */
  kno_of_led_states
}led_set_led_states_enum_t;

/*
 * For console_led_set command:
 * Possible values of argument 1 string
 */
char str_led_state[kno_of_led_states][SERIAL_ARG_LEN_BYTES] =
{
  "off",
  "red",
  "green",
  "amber",
  "flashing_red",
  "flashing_green",
  "flashing_amber"
};

#endif

/***********************************
 *  Private Functions */

/*! \brief
 *    API to set LED state variable for a LED indicator.
 *    Calls LED_StateMachine(kLED_EXEC_IMMEDIATE) for immediate output update.
 *
 *  \param Led_Status_Indicators led_id    [in] Identify LED to control by this driver.
 *  \param Led_States led_state    [in] One of valid LED states that can be assigned to a LED.
 *
 *  \return kMksOk if pass, else if invalid parameter, or if DIO driver returns error.
 *
 *  \par Errors
 *
 *  \par Related requirements
 *    None Specified
 */
MksReturnCodes LED_Set_Internal(Led_Status_Indicators led_id, Led_States led_state)
{
  DRV_STATUS_E eRet=0;

  if ((led_id < kLED_ID_NET) || (led_id > kLED_ID_HLT))
    return kMksOperationFailed;

  if ((led_state < kLED_ST_OFF) || (led_state > kLED_ST_FLASHING_AMBER))
    return kMksOperationFailed;

  led_cntrl_st[led_id].led_state_cntrl = led_state;
  eRet = LED_StateMachine(kLED_EXEC_IMMEDIATE);

  if (eRet == DRV_OK)
  {
    return kMksOk;
  }
  else
  {
    return kMksOperationFailed;
  }

}//LED_Set_Internal()

#ifdef LED_DRV_INCLUDE_SERIAL_CONSOLE
/*! \brief
 *    API to set LED state variable for a LED indicator.
 *    Called from SERIAL CONSOLE TESTING.
 *    Calls LED_Set_Internal() to set LED.
 *    This is done to provide decoupling of LED driver from application and serial console testing mode.
 *
 *  \param Led_Status_Indicators led_id    [in] Identify LED to control by this driver.
 *  \param Led_States led_state    [in] One of valid LED states that can be assigned to a LED.
 *
 *  \return kMksOk if pass, else error.
 *
 *  \par Errors
 *
 *  \par Related requirements
 *    None Specified
 */
MksReturnCodes LED_Set_Testmode(Led_Status_Indicators led_id, Led_States led_state)
{
  DRV_STATUS_E eRet=0;

  if (g_led_testmode == ktestmode_false)
    return kMksOperationFailed;

  eRet = LED_Set_Internal(led_id, led_state);

  if (eRet == DRV_OK)
  {
    return kMksOk;
  }
  else
  {
    return kMksOperationFailed;
  }
}//LED_Set_Testmode()

/*! \brief
 *    API for parsing command line arguments from serial console.
 *
 *  \param char *strCmdBuffer    [in] Command line buffer passed by shell.
 *  \param Serial_Cmd_St_t p_Serial_Cmd    [in] Structure to store output of parsing function - command & arguments.
 *
 *  \return kMksOk if pass, else if invalid parameter.
 *
 *  \par Errors
 *
 *  \par Related requirements
 *    None Specified
 */
MksReturnCodes Parse_CmdBuffer(char *strCmdBuffer, Serial_Cmd_St_t *p_Serial_Cmd)
{
  char *token;
  uint8_t i = 0;

  if ((strCmdBuffer == NULL) || (p_Serial_Cmd == NULL))
  {
    return kMksOperationFailed;
  }

  //Initialize return cmd, args buffer structure
  p_Serial_Cmd->cmd[0] = '\0';
  for (i=0; i< MAX_SERIAL_CMD_ARGS; i++)
  {
    p_Serial_Cmd->args[i][0] = '\0';
  }
  p_Serial_Cmd->no_args = 0;

  //Parse command
  token = strtok(strCmdBuffer, " ");
  if (token == NULL)
  {
    return kMksOperationFailed;
  }
  strncat(p_Serial_Cmd->cmd, token, SERIAL_CMD_LEN_BYTES-1);

  //Parse arguments
  token = strtok(NULL, " ");
  while ((token != NULL) && (p_Serial_Cmd->no_args < MAX_SERIAL_CMD_ARGS))
  {
      strncat(p_Serial_Cmd->args[p_Serial_Cmd->no_args], token, SERIAL_ARG_LEN_BYTES - 1);
      token = strtok(NULL, " ");
      p_Serial_Cmd->no_args++;
  }

  return kMksOk;

}//Parse_CmdBuffer
#endif

/***********************************
 *  API Functions */


/*! \brief Initialize netx DIO driver, also DIOs required for LED operation.
 *          Also initialize runtime variables for LED state machine handling at periodic interval.
 *
 *  \return kMksOk if initialization succeeds, error code otherwise.
 *
 *  \par Errors
 *
 *  \par Related requirements
 *    None Specified
 */
MksReturnCodes LED_Init()
{
  DRV_STATUS_E eRet=0;
  uint8_t i = 0;

  //Initialize the DIO device
  DRV_DIO_Init();

  //Initialize variables & turn LEDs off
  for (i=0; i< kNO_OF_LED_ID; i++)
  {
    led_cntrl_st[i].led_state_cntrl = kLED_ST_OFF;
    led_cntrl_st[i].led_state_status = kLED_ST_OFF;
    led_cntrl_st[i].led_blink_cntr_syscnts = 0;
    led_cntrl_st[i].led_blink_period_syscnts =
      MILLI_SECONDS_TO_SYSTEM_TICK_COUNTS(led_conf_st[i].led_blink_ms);

    eRet |= LED_OFF(led_conf_st[i].led_red_id);
    eRet |= LED_OFF(led_conf_st[i].led_green_id);

    if (led_conf_st[i].pled_off == NULL)
      eRet |= DRV_ERROR;
    if (led_conf_st[i].pled_on == NULL)
      eRet |= DRV_ERROR;
  }

  led_sm_period_cntr_syscnts = GetSysCounter();

  g_led_testmode = ktestmode_false;

  if (eRet == DRV_OK)
  {
    return kMksOk;
  }
  else
  {
    return kMksOperationFailed;
  }

}//LED_Init()


/*! \brief
 *    Function called from main loop, executes every LED_STATE_MACHINE_PERIOD_MS.
 *    Function also called from LED_Set() API for immediate execution.
 *
 *    Check if LED state variables are updated, make changes to LED outputs.
 *    If using blinking state, implement blinking logic here every LED_BLINK_ON_TIME_MS.
 *
 *  \param Led_StateMachine_Execution led_exec    [in] Execute immediately or after period elapsed.
 *
 *  \return kMksOk if state machine succeeds, error code otherwise.
 *
 *  \par Errors
 *
 *  \par Related requirements
 *    None Specified
 */
MksReturnCodes LED_StateMachine(Led_StateMachine_Execution led_exec)
{
  DRV_STATUS_E eRet=0;
  uint8_t i = 0;
  uint32_t tmp_sys_cntr = 0;

  if ((led_exec < kLED_EXEC_IMMEDIATE) || (led_exec > kLED_EXEC_PERIODIC))
    return kMksOperationFailed;

  //Once recorded value of GetSysCounter() is used to update period as well as blink counter
  //This is because GetSysCounter() will return different values each time & offset shall get added to blink counter
  tmp_sys_cntr = GetSysCounter();

//
//  Removed for now - can be enabled based on testing to slow down call to the state machine.
//
//  if (led_exec == kLED_EXEC_PERIODIC)
//  {
//    if (SysCounterTimeoutCounts(led_sm_period_cntr_syscnts, LED_STATE_MACHINE_PERIOD_SYSCNTS))
//    {
//      led_sm_period_cntr_syscnts = tmp_sys_cntr;
//    }
//    else
//      return kMksOk;
//
//  }//periodic

  if (led_exec == kLED_EXEC_IMMEDIATE)
  {
    for (i=0; i< kNO_OF_LED_ID; i++)
    {
      if (led_cntrl_st[i].led_state_cntrl != led_cntrl_st[i].led_state_status)
      {
          led_cntrl_st[i].led_state_status = led_cntrl_st[i].led_state_cntrl;
          if (led_cntrl_st[i].led_state_cntrl & LED_ST_FLASHING_MASK)
          {
            //Start from a known state of OFF, since using toggle function for blinking
            //(both red & green must blink for amber)
            eRet |= LED_OFF(led_conf_st[i].led_red_id);
            eRet |= LED_OFF(led_conf_st[i].led_green_id);
            led_cntrl_st[i].led_blink_cntr_syscnts = tmp_sys_cntr;

            //State machine period is synchronized with start of blinking
            //This is to avoid adding up to 1 period in every blink
            //Blink time is multiple of period
            led_sm_period_cntr_syscnts = tmp_sys_cntr;
          }
      }
    }//for
  }//immediate

  //check if change in state & implement it
  //implement blinking
  for (i=0; i< kNO_OF_LED_ID; i++)
  {
    if (led_cntrl_st[i].led_state_cntrl == kLED_ST_OFF)
    {
      eRet |= LED_OFF(led_conf_st[i].led_red_id);
      eRet |= LED_OFF(led_conf_st[i].led_green_id);
    }
    else if (led_cntrl_st[i].led_state_cntrl == kLED_ST_SOLID_RED)
    {
      eRet |= LED_ON(led_conf_st[i].led_red_id);
      eRet |= LED_OFF(led_conf_st[i].led_green_id);
    }
    else if (led_cntrl_st[i].led_state_cntrl == kLED_ST_SOLID_GREEN)
    {
      eRet |= LED_OFF(led_conf_st[i].led_red_id);
      eRet |= LED_ON(led_conf_st[i].led_green_id);
    }
    else if (led_cntrl_st[i].led_state_cntrl == kLED_ST_SOLID_AMBER)
    {
      eRet |= LED_ON(led_conf_st[i].led_red_id);
      eRet |= LED_ON(led_conf_st[i].led_green_id);
    }
    else if (led_cntrl_st[i].led_state_cntrl & LED_ST_FLASHING_MASK)
    {
      if (SysCounterTimeoutCounts(led_cntrl_st[i].led_blink_cntr_syscnts,
        led_cntrl_st[i].led_blink_period_syscnts))
      {
        led_cntrl_st[i].led_blink_cntr_syscnts = tmp_sys_cntr;
        if (led_cntrl_st[i].led_state_cntrl == kLED_ST_FLASHING_RED)
        {
            eRet |= LED_OFF(led_conf_st[i].led_green_id);
            eRet |= LED_TOGGLE(led_conf_st[i].led_red_id);
        }
        else if (led_cntrl_st[i].led_state_cntrl == kLED_ST_FLASHING_GREEN)
        {
            eRet |= LED_OFF(led_conf_st[i].led_red_id);
            eRet |= LED_TOGGLE(led_conf_st[i].led_green_id);
        }
        else if (led_cntrl_st[i].led_state_cntrl == kLED_ST_FLASHING_AMBER)
        {
            eRet |= LED_TOGGLE(led_conf_st[i].led_red_id);
            eRet |= LED_TOGGLE(led_conf_st[i].led_green_id);
        }
      }
    }
  }//for

  if (eRet == DRV_OK)
  {
    return kMksOk;
  }
  else
  {
    return kMksOperationFailed;
  }

}//LED_StateMachine()

/*! \brief
 *    API to set LED state variable for a LED indicator.
 *    Called from application.
 *    Calls LED_Set_Internal() to set LED.
 *    This is done to provide decoupling of LED driver from application and serial console testing mode.
 *
 *  \param Led_Status_Indicators led_id    [in] Identify LED to control by this driver.
 *  \param Led_States led_state    [in] One of valid LED states that can be assigned to a LED.
 *
 *  \return kMksOk if pass, else error.
 *
 *  \par Errors
 *
 *  \par Related requirements
 *    None Specified
 */
MksReturnCodes LED_Set(Led_Status_Indicators led_id, Led_States led_state)
{
  DRV_STATUS_E eRet=0;

  if (g_led_testmode == ktestmode_true)
    return kMksOperationFailed;

  eRet = LED_Set_Internal(led_id, led_state);

  if (eRet == DRV_OK)
  {
    return kMksOk;
  }
  else
  {
    return kMksOperationFailed;
  }
}//LED_Set()


/*! \brief
 *    API to set LED blink time for a LED indicator.
 *
 *  \param Led_Status_Indicators led_id [in]  Identify LED to configure by this API.
 *  \param uint32_t tmp_blink_time_ms [in]    Blink time in mS, for this LED
 *    Applies to LED ON time, equally to LED OFF time.
 *
 *  \return kMksOk if pass, else if invalid parameter.
 *
 *  \par Errors
 *
 *  \par Related requirements
 *    None Specified
 */
MksReturnCodes LED_SetBlinkTime(Led_Status_Indicators led_id, uint32_t tmp_blink_time_ms)
{
  DRV_STATUS_E eRet=0;

  if ((led_id < kLED_ID_NET) || (led_id > kLED_ID_HLT))
    return kMksOperationFailed;

  if ((tmp_blink_time_ms < LED_BLINK_TIME_MIN_MS) || (tmp_blink_time_ms > LED_BLINK_TIME_MAX_MS))
    return kMksOperationFailed;

  led_conf_st[led_id].led_blink_ms = tmp_blink_time_ms;
  led_cntrl_st[led_id].led_blink_period_syscnts =
    MILLI_SECONDS_TO_SYSTEM_TICK_COUNTS(led_conf_st[led_id].led_blink_ms);

  if (eRet == DRV_OK)
  {
    return kMksOk;
  }
  else
  {
    return kMksOperationFailed;
  }

}//LED_SetBlinkTime()



#ifdef LED_DRV_INCLUDE_SERIAL_CONSOLE

/*! \brief
 *    API to set LED driver to testing mode through serial console.
 *    Couples / decouples LED driver with application.
 *    Called from SERIAL CONSOLE.
 *
 *    Usage: led_testmode true | false
 *
 *  \param char *strCmdBuffer    [in] Command line buffer passed by shell.
 *
 *  \return 0 if success, -1 if error.
 *
 *  \par Errors
 *
 *  \par Related requirements
 *    None Specified
 */
uint32_t console_led_testmode( char *strCmdBuffer )
{
  MksReturnCodes eRet = kMksOk;
  uint8_t i = 0;
  MksBoolType found_flag = MKS_FALSE;

  Led_testmode_arg0_t arg_0_testmode = kfalse;

  Serial_Cmd_St_t cmd_arg_st;
  eRet = Parse_CmdBuffer(strCmdBuffer, &cmd_arg_st);
  if ((eRet != kMksOk) ||
      (cmd_arg_st.no_args != 1))
  {
    eRet = kMksOperationFailed;
  }

  //argument 0
  if (eRet == kMksOk)
  {
    found_flag = MKS_FALSE;
    for (i=0; i<kno_of_testmode; i++)
    {
      if (strcmp(cmd_arg_st.args[0], str_led_testmode[i]) == 0)
      {
        arg_0_testmode = i;
        found_flag = MKS_TRUE;
        break;
      }
    }
    if (found_flag == MKS_FALSE)
      eRet = kMksOperationFailed;
  }

  if (eRet != kMksOk)
  {
    printf("led_testmode: Invalid usage! \r\n");
    printf("usage is: led_testmode test_mode \r\n");
    printf("test_mode has following options: 'true', 'false' \r\n");
    printf("Example: led_testmode true \r\n");
    printf("Example: led_testmode false \r\n");
    return -1;
  }

  if (arg_0_testmode == ktrue)
  {
    printf("led_testmode: TRUE: LED driver set to testing mode through serial console. \r\n");
    g_led_testmode = ktestmode_true;
  }
  else
  {
    printf("led_testmode: FALSE: LED driver set to normal functions mode through application. \r\n");
    g_led_testmode = ktestmode_false;
  }

  return 0;
}

/*! \brief
 *    API to set a LED to one of possible states.
 *    Called from SERIAL CONSOLE.
 *
 *    Usage: led_set led_id led_state
 *    where:
 *      led_id: net, hlt
 *      led_state: off, red, green, amber, flashing_red, flashing_green, flashing_amber
 *
 *  \param char *strCmdBuffer    [in] Command line buffer passed by shell.
 *
 *  \return 0 if success, -1 if error.
 *
 *  \par Errors
 *
 *  \par Related requirements
 *    None Specified
 */
uint32_t console_led_set( char *strCmdBuffer )
{
  MksReturnCodes eRet = kMksOk;
  uint8_t i = 0;
  MksBoolType found_flag = MKS_FALSE;
  Serial_Cmd_St_t cmd_arg_st;
  led_set_led_id_enum_t arg_0_led_id;
  led_set_led_states_enum_t arg_1_led_state;

  Led_Status_Indicators led_id;
  Led_States led_state;

  if (g_led_testmode == ktestmode_false)
  {
    printf("led_set: Invalid usage! \r\n");
    printf("LED test mode must be set before test, & disabled after test. \r\n");

    printf("Example is as follows: \r\n");
    printf("led_testmode true \r\n");
    printf("led_set net red \r\n");
    printf("led_testmode false \r\n");
    return -1;
  }

  eRet = Parse_CmdBuffer(strCmdBuffer, &cmd_arg_st);

  if ((eRet != kMksOk) ||
      (cmd_arg_st.no_args != 2))
  {
    eRet = kMksOperationFailed;
  }

  //argument 0
  if (eRet == kMksOk)
  {
    found_flag = MKS_FALSE;
    for (i=0; i<kno_of_led_ids; i++)
    {
      if (strcmp(cmd_arg_st.args[0], str_led_id[i]) == 0)
      {
        arg_0_led_id = i;
        found_flag = MKS_TRUE;
        break;
      }
    }
    if (found_flag == MKS_FALSE)
      eRet = kMksOperationFailed;
  }

  //argument 1
  if (eRet == kMksOk)
  {
    found_flag = MKS_FALSE;
    for (i=0; i<kno_of_led_states; i++)
    {
      if (strcmp(cmd_arg_st.args[1], str_led_state[i]) == 0)
      {
        arg_1_led_state = i;
        found_flag = MKS_TRUE;
        break;
      }
    }
    if (found_flag == MKS_FALSE)
      eRet = kMksOperationFailed;
  }

  if (eRet != kMksOk)
  {
    printf("led_set: Invalid usage! \r\n");
    printf("usage is: led_set led_id led_state \r\n");
    printf("led_id has following options: 'net', 'mod' \r\n");
    printf("led_state has following options: 'off', 'red', 'green', 'amber', 'flashing_red', 'flasing_green', 'flashing_amber' \r\n");
    printf("Example: led_set net red \r\n");
    printf("Example: led_set health flashing_green \r\n");
    return -1;
  }

  switch (arg_0_led_id)
  {
  case kled_id_net:
    printf("led_set: NET LED selected. \r\n");
    led_id = kLED_ID_NET;
    break;
  case kled_id_hlt:
    printf("led_set: HLT LED selected. \r\n");
    led_id = kLED_ID_HLT;
    break;
  default:
    printf("led_set: INVALID LED selected. \r\n");
    eRet = kMksOperationFailed;
    break;
  }

  switch (arg_1_led_state)
  {
  case kled_st_off:
    printf("led_set: OFF state selected. \r\n");
    led_state = kLED_ST_OFF;
    break;
  case kled_st_solid_red:
    printf("led_set: RED state selected. \r\n");
    led_state = kLED_ST_SOLID_RED;
    break;
  case kled_st_solid_green:
    printf("led_set: GREEN state selected. \r\n");
    led_state = kLED_ST_SOLID_GREEN;
    break;
  case kled_st_solid_amber:
    printf("led_set: AMBER state selected. \r\n");
    led_state = kLED_ST_SOLID_AMBER;
    break;
  case kled_st_flashing_red:
    printf("led_set: FLASHING RED state selected. \r\n");
    led_state = kLED_ST_FLASHING_RED;
    break;
  case kled_st_flashing_green:
    printf("led_set: FLASHING GREEN state selected. \r\n");
    led_state = kLED_ST_FLASHING_GREEN;
    break;
  case kled_st_flashing_amber:
    printf("led_set: FLASHING AMBER state selected. \r\n");
    led_state = kLED_ST_FLASHING_AMBER;
    break;
  default:
    printf("led_set: INVALID state selected. \r\n");
    eRet = kMksOperationFailed;
    break;

  }//switch

  if (eRet == kMksOk)
  {
    LED_Set_Testmode(led_id, led_state);
  }
  else
  {
    return -1;
  }

  return 0;
}

/*! \brief
 *    API to start a LED self test routine.
 *    Called from SERIAL CONSOLE.
 *
 *    Usage: led_selftest
 *
 *  \param char *strCmdBuffer    [in] Command line buffer passed by shell.
 *
 *  \return 0 if success, -1 if error.
 *
 *  \par Errors
 *
 *  \par Related requirements
 *    None Specified
 */
uint32_t console_led_selftest( char *strCmdBuffer ){

  uint32_t tmp_period_ms_cntr = 0;

  if (g_led_testmode == ktestmode_false)
  {
    printf("LED_Test: Invalid usage! \r\n");
    printf("LED test mode must be set before test, & disabled after test. \r\n");

    printf("Example is as follows: \r\n");
    printf("led_testmode true \r\n");
    printf("LED_Test \r\n");
    printf("led_testmode false \r\n");
    return -1;
  }

  printf("\r\n LED Self Test - STARTED \r\n");

  //Both OFF
  printf("NET OFF, HLT OFF \r\n");
  LED_Set_Testmode(kLED_ID_NET, kLED_ST_OFF);
  LED_Set_Testmode(kLED_ID_HLT, kLED_ST_OFF);
  SysCounterWaitMilliSeconds(2000);

  //NET LED
  printf("NET RED \r\n");
  LED_Set_Testmode(kLED_ID_NET, kLED_ST_SOLID_RED);
  SysCounterWaitMilliSeconds(2000);

  printf("NET GREEN \r\n");
  LED_Set_Testmode(kLED_ID_NET, kLED_ST_SOLID_GREEN);
  SysCounterWaitMilliSeconds(2000);

  printf("NET AMBER \r\n");
  LED_Set_Testmode(kLED_ID_NET, kLED_ST_SOLID_AMBER);
  SysCounterWaitMilliSeconds(2000);

  printf("NET OFF \r\n");
  LED_Set_Testmode(kLED_ID_NET, kLED_ST_OFF);
  SysCounterWaitMilliSeconds(2000);

  //HLT LED
  printf("HLT RED \r\n");
  LED_Set_Testmode(kLED_ID_HLT, kLED_ST_SOLID_RED);
  SysCounterWaitMilliSeconds(2000);

  printf("HLT GREEN \r\n");
  LED_Set_Testmode(kLED_ID_HLT, kLED_ST_SOLID_GREEN);
  SysCounterWaitMilliSeconds(2000);

  printf("HLT AMBER \r\n");
  LED_Set_Testmode(kLED_ID_HLT, kLED_ST_SOLID_AMBER);
  SysCounterWaitMilliSeconds(2000);

  printf("HLT OFF \r\n");
  LED_Set_Testmode(kLED_ID_HLT, kLED_ST_OFF);
  SysCounterWaitMilliSeconds(2000);

  //Flashing RED
  printf("NET FLASHING RED \r\n");
  LED_Set_Testmode(kLED_ID_NET, kLED_ST_FLASHING_RED);
  LED_SetBlinkTime(kLED_ID_NET, 500);
  SysCounterWaitMilliSeconds(20);
  //Flashing GREEN
  printf("HLT FLASHING GREEN \r\n");
  LED_Set_Testmode(kLED_ID_HLT, kLED_ST_FLASHING_GREEN);
  LED_SetBlinkTime(kLED_ID_HLT, 500);
  SysCounterWaitMilliSeconds(20);

  tmp_period_ms_cntr = GetSysCounter();
  while (SysCounterTimeoutCounts(tmp_period_ms_cntr, ((uint32_t)MILLI_SECONDS_TO_SYSTEM_TICK_COUNTS(10000))) == MKS_FALSE)
  {
    LED_StateMachine(kLED_EXEC_PERIODIC);
    SysCounterWaitMilliSeconds(20);
  }

  printf("NET OFF, HLT OFF \r\n");
  LED_Set_Testmode(kLED_ID_NET, kLED_ST_OFF);
  LED_Set_Testmode(kLED_ID_HLT, kLED_ST_OFF);
  SysCounterWaitMilliSeconds(2000);

  printf("\r\n LED Self Test - COMPLETED \r\n");

  return 0;
}

#define LED_ALTERNATE_DELAY_MS 125
/***************************************************************************************************
* Led_Alternate()
*
* Handle switching LED colors
*
*   \return none
**************************************************************************************************/
static void Led_Alternate(bool* pstart_flag, MksUint32Type* pstart_count, Led_States* pcurrent_color, Led_States color1, Led_States color2)
{
  if (false == *pstart_flag)
  {
    *pstart_count = GetSysCounter();
    *pstart_flag = true;
    
    LED_Set( kLED_ID_HLT , *pcurrent_color );
  }
  
  if (true == SysCounterTimeoutCounts(*pstart_count, MILLI_SECONDS_TO_SYSTEM_TICK_COUNTS(LED_ALTERNATE_DELAY_MS)))
  {
    *pstart_flag = false;

    /* switch LED color */
    if (*pcurrent_color == color1)
    {
      *pcurrent_color = color2;
    }
    else
    {
      *pcurrent_color = color1;
    }
  }
}

/***************************************************************************************************
* HOSTAL_Health_Led_Handle()
*
* Health LED Handle
*
*   \return none
**************************************************************************************************/
void HOSTAL_Health_Led_Handle()
{
  static MksUint32Type start_count;
  static bool start_flag = false;
  static Led_States current_color = kLED_ST_SOLID_RED;

  switch(s_usHealthStatus)
  {
    case HIL_MB_HEALTH_STATUS_NO_POWER:
      LED_Set( kLED_ID_HLT , kLED_ST_OFF );
      break;

    case HIL_MB_HEALTH_STATUS_FW_FAULT:
      LED_Set( kLED_ID_HLT , kLED_ST_SOLID_RED );
      break;

    case HIL_MB_HEALTH_STATUS_FW_INITIALIZING:    
      LED_Set( kLED_ID_HLT , kLED_ST_SOLID_AMBER );
      break;

    case HIL_MB_HEALTH_STATUS_DIAG_MODE:    
      LED_Set( kLED_ID_HLT , kLED_ST_FLASHING_AMBER );
      break;

    case HIL_MB_HEALTH_STATUS_CONNECTED:    
      LED_Set( kLED_ID_HLT , kLED_ST_FLASHING_GREEN );
      break;

    case HIL_MB_HEALTH_STATUS_WD_FAULT:
      /* alternating RED, GREEN */
      Led_Alternate(&start_flag, &start_count, &current_color, kLED_ST_SOLID_GREEN, kLED_ST_SOLID_RED);
      break;

    case HIL_MB_HEALTH_STATUS_HOT:
      /* alternating GREEN, YELLOW */
      Led_Alternate(&start_flag, &start_count, &current_color, kLED_ST_SOLID_GREEN, kLED_ST_SOLID_AMBER);
      break;

    default:
      LED_Set( kLED_ID_HLT , kLED_ST_OFF );
      break;
  }
}

/***************************************************************************************************
* HOSTAL_Network_Led_Handle()
*
* Network LED Handle
*
*   \return none
**************************************************************************************************/
void HOSTAL_Network_Led_Handle()
{
  static MksUint32Type start_count;
  static bool start_flag = false;
  static Led_States current_color = kLED_ST_SOLID_RED;

  switch(s_usNetworkStatus)
  {
    case HIL_MB_NETWORK_STATUS_NO_POWER:
      LED_Set( kLED_ID_NET , kLED_ST_OFF );
      break;

    case HIL_MB_NETWORK_STATUS_NO_CONNECTION:    
      LED_Set( kLED_ID_NET , kLED_ST_SOLID_RED );
      break;

    case HIL_MB_NETWORK_STATUS_INITIALIZING:    
      LED_Set( kLED_ID_NET , kLED_ST_OFF );
      break;

    case HIL_MB_NETWORK_STATUS_STANDBY:    
      if (s_fLink == 0)
        LED_Set( kLED_ID_NET , kLED_ST_SOLID_RED );
      else
        LED_Set( kLED_ID_NET , kLED_ST_FLASHING_AMBER );
      break;

    case HIL_MB_NETWORK_STATUS_CONNECTED:
      if (s_fLink == 0)
        LED_Set( kLED_ID_NET , kLED_ST_SOLID_RED );
      else
        LED_Set( kLED_ID_NET , kLED_ST_FLASHING_GREEN );
      break;

    case HIL_MB_NETWORK_STATUS_CONFIG_ERROR:
      /* alternating RED, GREEN */
      Led_Alternate(&start_flag, &start_count, &current_color, kLED_ST_SOLID_GREEN, kLED_ST_SOLID_RED);
      break;

    case HIL_MB_NETWORK_STATUS_WAIT_FOR_LEASE:
      LED_Set( kLED_ID_NET , kLED_ST_SOLID_AMBER );
      break;

    default:
      LED_Set( kLED_ID_NET , kLED_ST_OFF );
      break;
  }
}


#endif

