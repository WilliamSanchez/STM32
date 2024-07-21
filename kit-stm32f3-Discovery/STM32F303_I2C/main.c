/*
	USART_1 PC		I2C_1 BMP280 

	Tx ==> PA9		SCL ==> PB8
	Rx ==> PA10		SDA ==> PB9
*/



#include <stm32f30x.h>
#include <string.h>
#include <stdio.h>

#include "modulo3G.h"
#include "configI2C.h"
#include "misc.h"



char trasmitData[64];

volatile int i = 0;



int main(void){

    InitializeLEDs();
    delay_init();
    Init_I2C1();
    configUSART_1();
    configButton();

    delay(1000);
    while(1)
    {  	
	GPIO_SetBits(GPIOE,GPIO_Pin_9);
        GPIO_SetBits(GPIOE,GPIO_Pin_10);
	GPIO_SetBits(GPIOE,GPIO_Pin_11);
        delay(1000);
        I2C_WriteReg(0xD0);
        uint8_t dataCommand = I2C1_ReadData();
        sprintf(trasmitData,"AT [%x] ==> (%d)\r",dataCommand,i++);
	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
        while ((USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET));
        GPIO_ResetBits(GPIOE,GPIO_Pin_9);
        delay(1000);
    }

  return 0;
}

