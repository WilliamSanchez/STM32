#ifndef __CONFIGI2C_H__
#define __CONFIGI2C_H__

#include "stm32f4xx.h"

#define Add_I2C	(0x3C<<1)


void Init_I2C1(void);
uint8_t I2C_WriteReg(uint8_t RegValue);
uint8_t I2C_WriteBuffer(uint8_t *buffer, uint8_t *length);
uint8_t I2C1_clearACK();
uint8_t I2C1_ReadReg(uint8_t reg);


#endif


