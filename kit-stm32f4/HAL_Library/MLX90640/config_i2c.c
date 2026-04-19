#include "config_i2c.h"
#include "stm32f4xx_hal.h"


I2C_HandleTypeDef I2CHandle;

void I2C_config(void)
{

    I2CHandle.Instance              = I2C;
    I2CHandle.Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
    I2CHandle.Init.ClockSpeed       = 400000;
    I2CHandle.Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
    I2CHandle.Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;

    while(HAL_I2C_Init(&I2CHandle) != HAL_OK);

}

int8_t i2c_send(uint8_t address, uint8_t *buffer, int len){
    /* Timeout is set to 10S */ 
    if(HAL_I2C_Master_Transmit(&I2CHandle, address, buffer, len, 1000)!= HAL_OK)
        return -1;
    return 0;
}

int8_t i2c_received(uint8_t address, uint8_t *reg, int Txlen, uint8_t *buffer, int len)
{
    
      /* Timeout is set to 10S */ 
      uint8_t addressW = address << 1;
     if(HAL_I2C_Master_Transmit(&I2CHandle, addressW, reg, Txlen, 1000) != HAL_OK)
        return -1;
    addressW = (0x01 | (address << 1));
    if(HAL_I2C_Master_Receive(&I2CHandle, addressW, buffer, len, 1000) != HAL_OK)
        return -1;
    return 0;

}
