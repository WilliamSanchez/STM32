/*
	USART_1 PC		SPI_2   SD Card 

	Tx ==> PA9		MISO 	==> PB14
	Rx ==> PA10		MOSI 	==> PB15
				SCK  	==> PB13
				NSS  	==> PB11
				NReset	==> PB10
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

#define writeReg			0x80
#define readReg				0x7F

char trasmitData[64];

volatile int i = 0;
int numero= 0;
uint8_t dataSD = 0xFF;

void Init_Reset(void);
void reset(void);

int main(void){

    InitializeLEDs();
    delay_init();
    configUSART_1();
    Init_SPI();
    configButton();
    Init_Reset();
    delay(1000);
    while(1)
    {  	
	GPIO_SetBits(GPIOE,GPIO_Pin_9);
        GPIO_SetBits(GPIOE,GPIO_Pin_10);
	GPIO_SetBits(GPIOE,GPIO_Pin_11);
        delay(1000);
        dataSD = SPI_readReg(0x42);
        sprintf(trasmitData,"AT [%x] versao => %d\n\r",dataSD,numero);
        USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
        while ((USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET));

        dataSD = SPI_readReg(0x39);
        sprintf(trasmitData,"AT [%x] versao other => %d\n\r",dataSD,numero);
        USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
        while ((USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET));
 
        SPI_writeReg(writeReg | 0x01, 0x80);
        dataSD = SPI_readReg(0x01);
        sprintf(trasmitData,"AT [%x] modo => %d\n\r",dataSD,numero);
        USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
        while ((USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET));

	delay(10);

        SPI_writeReg(writeReg | 0x01, dataSD | 0x01);
        dataSD = SPI_readReg(0x01);
        sprintf(trasmitData,"AT [%x] status => %d\n\r",dataSD,numero);
        USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
        while ((USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET));

//  SX1278_writeReg(writeReg | 0x40, 0x01);  //  DIO0 --> TxDone

        SPI_writeReg(writeReg | 0x40, 0x15);
        dataSD = SPI_readReg(0x40);
        sprintf(trasmitData,"AT [%x] TxDone => %d\n\r",dataSD,numero);
        USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
        while ((USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET));

        SPI_writeReg(writeReg | 0x40, 0x13);
        dataSD = SPI_readReg(0x40);
        sprintf(trasmitData,"AT [%x] RX_Done => %d\n\r",dataSD,numero++);
        USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
        while ((USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET));

        GPIO_ResetBits(GPIOE,GPIO_Pin_9);
        //reset();
        delay(1000);
    }

  return 0;
}


void Init_Reset(void){

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  GPIO_InitTypeDef  GPIO_InitStruct;

  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/////////////

void reset(void){

    GPIO_ResetBits(GPIOB,GPIO_Pin_10);
    delay(10);
    GPIO_SetBits(GPIOB,GPIO_Pin_10);
    delay(10);

}

