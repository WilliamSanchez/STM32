/*
	USART_1 PC		USART_2 BLE

	Tx ==> PA9		Tx ==> PA2
	Rx ==> PA10		Rx ==> PA3
*/

#include <stm32f4xx.h>
#include <string.h>
#include <stdio.h>

#include <FreeRTOSConfig.h>
#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>
#include <semphr.h>
#include <queue.h>

#include <misc_extras.h>

char trasmitData[64];

volatile int i = 0;

void vTask1(void *pvParameters);
void vTask2(void *pvParameters);


int main(void){

    InitializeLEDs();
    delay_init();
    GPIO_SetBits(GPIOC,GPIO_Pin_13);
    delay(1000);
    xTaskCreate(vTask1,"Task_1",128,NULL,1,NULL);
    xTaskCreate(vTask2,"Task_2",128,NULL,2,NULL);
    
    vTaskStartScheduler();
    
    while(1)
    {  	

    }
    
  return 0;
}


void vTask1(void *pvParameters){
 
   while(1){
   
        GPIO_ResetBits(GPIOC,GPIO_Pin_13);
        vTaskDelay(500);
   }
}

void vTask2(void *pvParameters){
 
   while(1){
   
        GPIO_SetBits(GPIOC,GPIO_Pin_13);
        vTaskDelay(100);
   }
}

