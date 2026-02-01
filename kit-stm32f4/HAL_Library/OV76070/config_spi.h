#ifndef _CONFIG_SPI_H_
#define _CONFIG_SPI_H_

#include "stm32f4xx_hal.h"
#include <stdlib.h>
#include <string.h>

#define SPI                         SPI1

void nss_config();
void  SPI_config(void);
int8_t SPI_writeReg(uint8_t reg, uint8_t regvalue);
uint8_t SPI_readReg(uint8_t reg);
int8_t SPI_TxData(uint8_t value);
int8_t SPI_writeData(uint8_t reg, uint8_t *Datavalue, uint32_t length);
int8_t SPI_writePixel(uint16_t *Datavalue, uint32_t length);

#endif