/*
	USART_1 PC		USART_2 BLE

	Tx ==> PA9		Tx ==> PA2
	Rx ==> PA10		Rx ==> PA3
        baudrate => 115200	
*/

#include <stm32f30x.h>
#include <string.h>
#include <stdio.h>

#include "modulo3G.h"
#include "misc.h"

char trasmitData[64];

volatile int i = 0;

int main(void){

    InitializeLEDs();
    delay_init();
    configUSART_1();
    configButton();
//    configUSART_2();  
//    configUSART_3();
    delay(1000);
    while(1)
    {  	
	GPIO_SetBits(GPIOE,GPIO_Pin_9);
        GPIO_SetBits(GPIOE,GPIO_Pin_10);
	GPIO_SetBits(GPIOE,GPIO_Pin_11);
        for(i=0;i<5000000;i++);
        sprintf(trasmitData,"10:45:32:54 10:45:33:45 -70.128453 -23.198545 20 23 20 19\n\r");
        USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
        while ((USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET));
        GPIO_ResetBits(GPIOE,GPIO_Pin_9);
        for(i=0;i<5000000;i++);
    }

  return 0;
}

