#ifndef _CONFIG_I2C_H_
#define _CONFIG_I2C_H_

#include "stm32f4xx_hal.h"

#define I2C                         I2C1

void I2C_config(void);
int8_t i2c_send(uint8_t address, uint8_t *buffer, int len);
int8_t i2c_received(uint8_t address, uint8_t *reg, int Txlen, uint8_t *buffer, int len);

#endif