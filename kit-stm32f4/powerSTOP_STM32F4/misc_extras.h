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

time timeData;

typedef int bool;

void InitializeLEDs();
void delay_init(void);
void delay(__IO uint32_t nTime);
void delay_us(u32 micro_sec);
void configButton();

uint8_t parseData(const char *data1, uint8_t len1, const char *data2);
void sendData(const char *data);

void Config_RTC();
void Config_Alarm(void);
void SysTick_Configuration(void);
void SYSCLKConfig_STOP(void);

void set_time(time *time_ptr, uint8_t hora, uint8_t minutos, uint8_t segundos);
void Init_RTC(uint8_t hora, uint8_t minutos, uint8_t segundos);
void begin_RTC(uint8_t hours, uint8_t minutes, uint8_t seconds);

//void get_Time(time *time_ptr);
uint8_t getSegundos();
uint8_t getMinutos();
uint8_t getHoras();
uint32_t getmillSegundos();

#endif  // _MISC_H_
