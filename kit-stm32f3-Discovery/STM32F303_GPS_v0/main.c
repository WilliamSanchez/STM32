/*
	USART_1 PC		USART_2 BLE

	Tx ==> PA9		Tx ==> PA2
	Rx ==> PA10		Rx ==> PA3
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
    configUSART_2();  
    configButton();


    delay(1000);
    while(1)
    {  	
	GPIO_SetBits(GPIOE,GPIO_Pin_9);
        GPIO_SetBits(GPIOE,GPIO_Pin_10);
	GPIO_SetBits(GPIOE,GPIO_Pin_11);
        delay(1000);
        GPIO_ResetBits(GPIOE,GPIO_Pin_9);
        delay(1000);
    }

  return 0;
}

