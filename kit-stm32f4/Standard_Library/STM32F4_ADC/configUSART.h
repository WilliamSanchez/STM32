#ifndef _CONFIGUSART_h_
#define _CONFIGUSART_h_

#include <stm32f4xx.h>
#include "misc.h"

#define ADC_ADDRESS          ((uint32_t)0x4001224C)

void serialPC();

void config_ADC();
void config_ADC2();
uint32_t readData_ADC1();
uint32_t readData_ADC2();

#endif   // _MODULO3G_H_


