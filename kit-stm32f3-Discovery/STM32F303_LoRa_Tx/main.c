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

char trasmitData[64];

volatile int i = 0;
int numero= 0;
uint8_t dataSD = 0xFF;

char data[50];
uint8_t flag = 0;

SX1278_Device Sx1278_LoRa;

int main(void){

    InitializeLEDs();
    delay_init();
    configUSART_1();
    Init_SPI();
    configButton();
    if (SX1278_Begin(&Sx1278_LoRa,SX1278_915MHZ, SX1278_POWER_17DBM, 
		SX1278_LORA_SF_12,SX1278_LORA_BW_62_5KHZ, 0) == 1){while(1);}
    delay(1000);
    while(1)
    {  	
	GPIO_SetBits(GPIOE,GPIO_Pin_9);
        GPIO_SetBits(GPIOE,GPIO_Pin_10);
	GPIO_SetBits(GPIOE,GPIO_Pin_11);
        GPIO_SetBits(GPIOE,GPIO_Pin_12);
        GPIO_SetBits(GPIOE,GPIO_Pin_13);
        
	delay(5000);

    		//sprintf(data,"\n\r Time current is %02d:%02d:%02d ", get_hours(), get_minutes(), get_seconds());
                sprintf(data,"\n\rAqui o transmisor que esta enviando informacao para o transsmisor");
    		uint8_t size = strlen(data);  
    		if (SX1278_beginTx(&Sx1278_LoRa, (uint8_t *)data, size) != 0x34 ){GPIO_ResetBits(GPIOE,GPIO_Pin_10);};
	        GPIO_ResetBits(GPIOE,GPIO_Pin_12);
    		if (SX1278_sendData(&Sx1278_LoRa) != 0x34 ){GPIO_ResetBits(GPIOE,GPIO_Pin_13);};
	        GPIO_SetBits(GPIOE,GPIO_Pin_12);
    		SX1278_Sleep(&Sx1278_LoRa);
		flag = 0;

        dataSD = SPI_readReg(0x39);
        sprintf(trasmitData,"AT [%d] numero => %d\r",dataSD,numero++);
        USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
        while ((USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET));
        GPIO_ResetBits(GPIOE,GPIO_Pin_9);
        delay(5000);
    }

  return 0;
}







