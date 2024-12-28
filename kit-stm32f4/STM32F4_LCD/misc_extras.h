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
void configButton_Rudder();
void configButton_ThM();
void configButton_ThP();

uint8_t parseData(const char *data1, uint8_t len1, const char *data2);
void sendData(const char *data);



#endif  // _MISC_H_
