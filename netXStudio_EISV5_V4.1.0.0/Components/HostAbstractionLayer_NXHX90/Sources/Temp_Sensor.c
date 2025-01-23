#include <stdio.h>

#include "system_netx.h"
#include "sys.h"
#include "hostAbstractionLayer.h"

/*****************************************************************************/
/*! Global Variables                                                         */
/*****************************************************************************/
uint32_t s_ulADCTempValue   = 0;

/*****************************************************************************/
/*! Static Variables                                                         */
/*****************************************************************************/
static uint16_t adc_queue[ADC_AVG_COUNT];

/*****************************************************************************/
/*! returns system temperature value from ADC
 *   \return temperature                                                     */
/*****************************************************************************/
int Sensor_GetTemp()
{
  uint8_t adc_idx;
  uint16_t temp_avg=0;
  int temp;

  for (adc_idx = 0; adc_idx < ADC_AVG_COUNT; adc_idx++)
    temp_avg += adc_queue[adc_idx];

  SystemTemperatureByADCValue(temp_avg, &temp, 100); /* Temperature needs to be scaled by * 100. */
  return temp;
}

/*****************************************************************************/
/*! updates local temperature in the ADC queue
 *   \return None                                                            */
/*****************************************************************************/
void Sensor_UpdateLocalTemp()
{
  static uint8_t  adc_idx = 0;

  // 8 sampling
  adc_queue[adc_idx] = s_ulADCTempValue / 8;

  if (adc_idx == (ADC_AVG_COUNT - 1)) adc_idx = 0;
  else adc_idx++;
}
