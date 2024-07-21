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
#include <math.h>

#include "misc_extras.h"
#include "configUSART.h"

#define max(x,y)    (x>y) ? x:y

char dataMode[225] = "XXXX";

volatile int numero = 0;
uint8_t trasmitData[255];
uint8_t trasmitDataS[255];
uint8_t dataTime[255];
uint8_t mode =0;
uint8_t changeMode[16];
uint8_t changeDelay[16];
uint8_t contDelay = 20;


int main(void){

    InitializeLEDs();
    serialPC();
    delay_init();
    config_ADC();
    ADC_SoftwareStartConv(ADC1);
    //ADC_SoftwareStartConv(ADC2);
    while(1)
    {
        GPIO_SetBits(GPIOC,GPIO_Pin_13);
        delay(50);
        sprintf((char*)trasmitData,"The date is %lu %lu\r\n",readData_ADC1(), readData_ADC2());
        sendData((char*)trasmitData);
        GPIO_ResetBits(GPIOC,GPIO_Pin_13);
        delay(50);
    }
  return 0;
}

