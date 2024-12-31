/*
	USART_1 PC		SPI_2   SD Card 

	Tx ==> PA9		MISO 	==> PA6
	Rx ==> PA10		MOSI 	==> PA7
				SCK  	==> PA5
				NSS  	==> PA4
				NReset	==> PB10
*/

#include <stm32f4xx.h>
#include <string.h>
#include <stdio.h>

#include "misc_extras.h"
#include "configSPI.h"
#include "configUSART.h"
#include "configSX1278.h"

SX1278_Device sx1278_Acosta;

char bufferTX[512];
char buffer[255];

char txBuffer_1[255]="";

void sendData(const char *data);


int main(void){

    Init_SPI();
    InitializeLEDs();
    configUSART_1();
    delay_init();

    if (SX1278_Begin(&sx1278_Acosta,SX1278_915MHZ, SX1278_POWER_17DBM, 
		SX1278_LORA_SF_12,SX1278_LORA_BW_125KHZ, 0) == 1){ GPIO_ResetBits(GPIOC,GPIO_Pin_13); while(1);}
     
    delay(1000);
    while(1)
    {
      GPIO_SetBits(GPIOC,GPIO_Pin_13);
      delay(10);
        if(SX1278_readData(&sx1278_Acosta, (uint8_t *)buffer) != 0){
        sprintf(bufferTX,"\n\rReceived packet ( %s ):", buffer);
        sendData(bufferTX);
        memset(bufferTX,0x00,strlen(bufferTX));
	memset(buffer,0x00,strlen(buffer));
      }
      GPIO_ResetBits(GPIOC,GPIO_Pin_13);  
      delay(10);	

    }
  return 0;
}




///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

void sendData(const char *data){

     sprintf(txBuffer_1,"%s",data);
     USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
     while ((USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET));
  
}

