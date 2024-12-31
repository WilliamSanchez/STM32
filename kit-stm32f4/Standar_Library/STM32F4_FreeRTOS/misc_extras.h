#ifndef _MISC_EXTRAS_H_
#define _MISC_EXTRAS_H_

#include <stm32f4xx.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void InitializeLEDs();
void delay_init(void);
void delay(__IO uint32_t nTime);

#endif  // _MISC_H_
