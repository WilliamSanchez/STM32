#ifndef _MISC_H_
#define _MISC_H_

#include <stm32f30x.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "misc.h"
#include "modulo3G.h"

#define false 	0
#define true 	1

typedef struct{
  uint8_t dato;
  uint8_t hora;
  uint8_t minuto;
  uint8_t segundo;
  uint8_t millsec;
}dataTeclado;

typedef int bool;

void InitializeLEDs();
void delay_init(void);
void delay(__IO uint32_t nTime);
void configButton();

uint8_t parseData(const char *data1, uint8_t len1, const char *data2);

#endif  // _MISC_H_
