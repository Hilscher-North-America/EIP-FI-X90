#ifndef RTC_H_
#define RTC_H_

#include "time.h"
#include "hostAbstractionLayer.h"
#include "mks_types.h"

#define CLK_FIRST_YEAR           (1900L)
#define TM_YEAR(x)               (x-CLK_FIRST_YEAR)
#define TM_TO_YEAR(x)            (x+CLK_FIRST_YEAR)

#define TM_MAX_MONTH             12
#define TM_MAX_DAY               31
#define TM_MIN_DAY               1
#define TM_MAX_YEAR              2036
#define TM_MIN_YEAR              1970
#define TM_MAX_HOUR              23
#define TM_MAX_MINUTE            59
#define TM_MAX_SECOND            59

#define TM_UNIX_MIN_YEAR         1900
#define TM_UNIX_MONTH_OFS        1
#define TM_UNIX_YEAR_OFS         100 // (2000 - 1900)

#define RTC_SHELL_CMDS \
  {"time" , NULL , Shell_HandleTime, "[<month>] [<day>] [<year>] [<hour>] [<min>] [<sec>] Sets or displays current time"}, \

uint32_t Shell_HandleTime(uint32_t argc, const char *argv[]);

/*--------------------------------------------------------------------------*/
/* TIME STRUCTURE */
/*!
 * \brief A structure to represent time in seconds and milliseconds.
 */
typedef struct time_struct
{
   /*! \brief The number of seconds in the time. */
   time_t       SECONDS;

   /*! \brief The number of milliseconds in the time. */
   uint32_t     MILLISECONDS;

} TIME_STRUCT, * TIME_STRUCT_PTR;

MksReturnCodes RTC_I2C_Init();
int32_t RTC_HandleTimeCmd (int argc, char* argv[]);
HOSTAL_RESULT_E HOSTAL_RTC_GetTime (struct tm* pRTC);
HOSTAL_RESULT_E HOSTAL_RTC_SyncTime (bool bNetX);
HOSTAL_RESULT_E HOSTAL_Update_netXTime ();

void _time_get(TIME_STRUCT_PTR ptTime);
void _time_set(register TIME_STRUCT_PTR ts_ptr);

#endif /* RTC_H_ */
