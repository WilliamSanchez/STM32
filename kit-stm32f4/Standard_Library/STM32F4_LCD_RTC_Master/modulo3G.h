#ifndef _MODULO3G_h_
#define _MODULO3G_h_

#include <stm32f4xx.h>
#include "misc_extras.h"

time timeData;

void configUSART_1();
void configUSART_2();

void Config_RTC();
void Config_Alarm(void);

void set_time(time *time_ptr, uint8_t hora, uint8_t minutos, uint8_t segundos);
void Init_RTC(uint8_t hora, uint8_t minutos, uint8_t segundos);
void begin_RTC(uint8_t hours, uint8_t minutes, uint8_t seconds);

//void get_Time(time *time_ptr);
uint8_t getSegundos();
uint8_t getMinutos();
uint8_t getHoras();
uint8_t getmillSegundos();


#endif   // _MODULO3G_H_


