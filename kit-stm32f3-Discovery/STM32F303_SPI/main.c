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

char trasmitData[64];

volatile int i = 0;
int numero= 0;
uint8_t dataSD = 0xFF;
void sendCommand_SDCard(uint8_t cmd, uint32_t arg, uint8_t crc);

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
        //sendCommand_SDCard(CMD0,CMD0_ARG,CMD0_CRC);
        //int j=0;
        //while(dataSD == 0xFF){dataSD = SPI_RxData();}uint8_t SPI_readReg(uint8_t reg);
        dataSD = SPI_readReg(0x42);
        sprintf(trasmitData,"AT [%d] numero => %d\r",dataSD,numero++);
        USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
        while ((USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET));
        GPIO_ResetBits(GPIOE,GPIO_Pin_9);
        delay(1000);
    }

  return 0;
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





