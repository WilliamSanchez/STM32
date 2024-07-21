#ifndef __CONFIGI2C_H__
#define __CONFIGI2C_H__

#include "stm32f30x.h"

#define Add_I2C	0x76

void Init_I2C1(void);
int I2C_WriteReg(uint8_t RegValue);
int I2C1_clearACK(void);
uint8_t I2C1_ReadData(void);


#endif


