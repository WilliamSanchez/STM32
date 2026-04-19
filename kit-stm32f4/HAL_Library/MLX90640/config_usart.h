#ifndef _CONFIG_USART_H_
#define _CONFIG_USART_H_

#include "stm32f4xx_hal.h"

#define USART                         USART1

void SystemClock_Config(void);
void BSP_LED_Init();
void USART_config(void);
int8_t usart_send(uint8_t *buffer, int len);

#endif