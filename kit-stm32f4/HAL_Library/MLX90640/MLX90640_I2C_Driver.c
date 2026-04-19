#include <stdint.h>
#include "MLX90640_I2C_Driver.h"
#include "stm32f4xx_hal.h"

I2C_HandleTypeDef I2CHandle;

void MLX90640_I2CInit(void)
{
    I2CHandle.Instance              = I2C;
    I2CHandle.Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
    I2CHandle.Init.ClockSpeed       = 400000;
    I2CHandle.Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
    I2CHandle.Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;

    while(HAL_I2C_Init(&I2CHandle) != HAL_OK);
}
int MLX90640_I2CGeneralReset(void)
{
    return 0;
}
int MLX90640_I2CRead(uint8_t slaveAddr,uint16_t startAddress, uint16_t nMemAddressRead, uint16_t *data)
{
    uint8_t addressW = slaveAddr << 1;
    uint8_t reg[2]; 
    uint8_t buffer[nMemAddressRead*2];

    int len = nMemAddressRead*2;

    reg[0] = startAddress >> 8;
    reg[1] = startAddress & 0x00FF;
    if(HAL_I2C_Master_Transmit(&I2CHandle, addressW, (uint8_t*)reg, 2, 1000) != HAL_OK)
        return -1;
    addressW = (0x01 | (slaveAddr << 1));
    if(HAL_I2C_Master_Receive(&I2CHandle, addressW, buffer, len, 10000) != HAL_OK)
        return -1;

    for(int i=0; i < nMemAddressRead;  i++ ){
        data[i] = (buffer[2*i]<< 8) | (buffer[2*i+1] & 0x00FF);
    }
    
    return 0;
}
int MLX90640_I2CWrite(uint8_t slaveAddr,uint16_t writeAddress, uint16_t data)
{
    return 0;
}
void MLX90640_I2CFreqSet(int freq)
{

}
