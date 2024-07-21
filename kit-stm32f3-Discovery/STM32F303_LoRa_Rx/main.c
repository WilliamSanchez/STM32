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
#include "configSX1278.h"
#include "misc.h"


#define CMD0        0
#define CMD0_ARG    0x00000000
#define CMD0_CRC    0x94

char trasmitData[255];

volatile int i = 0;
int numero= 0;
uint8_t dataSD = 0xFF;

char data[50];
uint8_t flag = 0;

SX1278_Device Sx1278_LoRa;


char bufferTX[25];
char buffer[255];

int main(void){

    InitializeLEDs();
    delay_init();
    configUSART_1();
    Init_SPI();
    configButton();
    if (SX1278_Begin(&Sx1278_LoRa,SX1278_915MHZ, SX1278_POWER_17DBM, 
		SX1278_LORA_SF_12,SX1278_LORA_BW_125KHZ, 0) == 1){while(1);}
    delay(1000);
    while(1)
    {  	

        if(SX1278_readData(&Sx1278_LoRa, (uint8_t *)buffer) != 0){GPIO_ResetBits(GPIOE,GPIO_Pin_12);}
        //sprintf(trasmitData,"\n\r Received packet %s:", buffer);

        //GPIO_SetBits(GPIOE,GPIO_Pin_13);
        delay(500);
	GPIO_SetBits(GPIOE,GPIO_Pin_12);
        GPIO_ResetBits(GPIOE,GPIO_Pin_13);
        GPIO_SetBits(GPIOE,GPIO_Pin_9);
        delay(500);

        sprintf(trasmitData,"\n\rData [%s] numero bytes [%d]  => %d\r",buffer,Sx1278_LoRa.readBytes,numero++);
        USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
        while ((USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET));
        GPIO_ResetBits(GPIOE,GPIO_Pin_9);
        memset(trasmitData,0x00,strlen(trasmitData));

    }

  return 0;
}







