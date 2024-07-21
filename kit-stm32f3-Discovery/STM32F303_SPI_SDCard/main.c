/*
	USART_1 PC		SPI_2   SD Card 

	Tx ==> PA9		MISO ==> PB14
	Rx ==> PA10		MOSI ==> PB15
				SCK  ==> PB13
				NSS  ==> PB11
*/

#include <stm32f30x.h>
#include <string.h>
#include <stdio.h>

#include "modulo3G.h"
#include "configSPI.h"
#include "misc.h"


#define CMD0        0
#define CMD0_ARG    0x00000000
#define CMD0_CRC    0x94

#define CMD8        8
#define CMD8_ARG    0x000001AA
#define CMD8_CRC    0x86

char trasmitData[64];

volatile int i = 0;
int numero= 0;
uint8_t dataSD = 0xFF;
uint8_t dataRes7[5];

void sendCommand_SDCard(uint8_t cmd, uint32_t arg, uint8_t crc);
uint8_t SD_readRes1();
void SD_readRes7(uint8_t *res);

int main(void){

    InitializeLEDs();
    delay_init();
    configUSART_1();
    Init_SPI();
    configButton();
    delay(1000);
    while(1)
    {  	
	GPIO_SetBits(GPIOE,GPIO_Pin_9);
        GPIO_SetBits(GPIOE,GPIO_Pin_10);
	GPIO_SetBits(GPIOE,GPIO_Pin_11);
        delay(1000);
        sendCommand_SDCard(CMD8, CMD8_ARG, CMD8_CRC);
	SD_readRes7(dataRes7);
        sprintf(trasmitData,"AT [%x%x%x%X%x] numero => %d\r",dataRes7[0],dataRes7[1],dataRes7[2],dataRes7[3],dataRes7[4],numero++);
        USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
        while ((USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET));
        GPIO_ResetBits(GPIOE,GPIO_Pin_9);
        delay(1000);
    }

  return 0;
}


uint8_t SD_readRes1()
{
    uint8_t i = 0, res1;

    // keep polling until actual data received
    while((res1 = SPI_readReg(0x00)) == 0xFF)
    {
        i++;

        // if no data received for 8 bytes, break
        if(i > 8) break;
    }

    return res1;
}

void sendCommand_SDCard(uint8_t cmd, uint32_t arg, uint8_t crc){

    SPI_TxData(cmd|0x40);

    // transmit argument
    SPI_TxData((uint8_t)(arg >> 24));
    SPI_TxData((uint8_t)(arg >> 16));
    SPI_TxData((uint8_t)(arg >> 8));
    SPI_TxData((uint8_t)(arg));

    // transmit crc
    SPI_TxData(crc|0x01);

}

void SD_readRes7(uint8_t *res)
{
       res[0] = SPI_readReg(0xFF);
       res[1] = SPI_readReg(0xFF);
       res[2] = SPI_readReg(0xFF);
       res[3] = SPI_readReg(0xFF);
       res[4] = SPI_readReg(0xFF);

}





