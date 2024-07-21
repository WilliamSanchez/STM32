#ifndef _MISC_EXTRAS_H_
#define _MISC_EXTRAS_H_

#include <stm32f4xx.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>



#define false 	0
#define true 	1


typedef struct {

  uint8_t hours;
  uint8_t minutes;
  uint8_t seconds;
}time;



typedef int bool;

void InitializeLEDs();
void delay_init(void);
void delay(__IO uint32_t nTime);
void delay_us(u32 micro_sec);


void configButton();

#endif  // _MISC_H_
