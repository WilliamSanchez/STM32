#include <stdint.h>
#include "MLX90640_I2C_Driver.h"
#include "stm32f4xx_hal.h"
#include "config_i2c.h"
#include "config_usart.h"

#include <string.h>
#include <stdio.h>

extern char debug[2024];
void MLX90640_I2CInit(void)
{
    I2C_config();
}
int MLX90640_I2CGeneralReset(void)
{
    return 0;
}
int MLX90640_I2CRead(uint8_t slaveAddr,uint16_t startAddress, uint16_t nMemAddressRead, uint16_t *data)
{
    uint8_t reg[2];     
    uint16_t cont = 0;
    //read 16 data maximum type uint16_t
    uint8_t lenReadData = 32/2;

    while(nMemAddressRead > 0)
    {        
        // read until 16 data type uint16_t
        if(nMemAddressRead < 32/2 )
            lenReadData = nMemAddressRead;

        uint8_t buffer[2*lenReadData];

        reg[0] = startAddress >> 8;
        reg[1] = startAddress;
         
        if(i2c_received(slaveAddr, reg, 2, buffer, 2*lenReadData) < 0)
            return 1;

        for(int i=0; i < lenReadData;  i++ ){
            data[cont*lenReadData + i] = (buffer[2*i]<< 8) | buffer[2*i+1];
            //sprintf(debug,"%d> %x|%x\n\r",cont*lenReadData + i,buffer[2*i],buffer[2*i+1]);
            //memset(debug,0x00,2024);
            //sprintf(debug,"%d> %x Addr: %x|%x, numAddress %d, lenReadData %d\n\r",cont*lenReadData + i,data[cont*lenReadData + i], reg[0], reg[1] ,nMemAddressRead,lenReadData);
            //sprintf(debug,"%d> Addr: %x|%x, numAddress %d, lenReadData %d\n\r",cont*lenReadData + i, reg[0], reg[1] ,nMemAddressRead,lenReadData);
            //usart_send((uint8_t*)debug,strlen(debug));
        }

        startAddress += lenReadData;
        nMemAddressRead -= lenReadData;
        cont++;
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
