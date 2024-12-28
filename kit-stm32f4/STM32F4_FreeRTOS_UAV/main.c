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

#include <math.h>
#include "misc_extras.h"
#include "configUSART.h"
#include "lcdOLED.h"

uint8_t trasmitData[255];
uint8_t contthrottle = 20;
uint16_t conelevator = 0;
uint16_t conaileron = 0;
uint16_t conrudder = 0;
uint8_t pinread = 0;

volatile int i = 0;

void vTask1(void *pvParameters);
void vTask2(void *pvParameters);


int main(void){

    InitializeLEDs();
    serialPC();
    delay_init();
    Init_I2C1();
    configButton_ThM();
    configButton_ThP();
    configButton_Rudder();
    config_ADC();
    ADC_SoftwareStartConv(ADC1);    
    delay(1000);display();
    delay(1000);clearDisplay();
    
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
   	clearDisplay();
        drawLetter();
        display();
        GPIO_ResetBits(GPIOC,GPIO_Pin_13);
        vTaskDelay(50);
   }
}

void vTask2(void *pvParameters){
 
   while(1){
        readData_ADC();
    	sprintf((char*)trasmitData,"%d %d %d %d",conaileron, conelevator, conrudder, contthrottle);
        sendData((char*)trasmitData);
        GPIO_SetBits(GPIOC,GPIO_Pin_13);
        vTaskDelay(10);
   }
}

