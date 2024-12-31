#ifndef __CONFIGSPI_H__
#define __CONFIGSPI_H__

#include "stm32f4xx.h"

void Init_Reset(void);
void reset(void);

void Init_SPI(void);
void SPI_writeReg(uint8_t reg, uint8_t regvalue);
uint8_t SPI_readReg(uint8_t reg);
void SPI_TxData(uint8_t value);
void SPI_writeData(uint8_t reg, uint8_t *Datavalue, uint8_t length);

#endif
