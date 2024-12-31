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
#include "lcdOLED.h"

#define max(x,y)    (x>y) ? x:y

char dataMode[225] = "XXXX";

volatile int numero = 0;
uint8_t trasmitData[255];
uint8_t contthrottle = 20;
uint16_t conelevator = 0;
uint16_t conaileron = 0;
uint16_t conrudder = 0;
uint8_t pinread = 0;

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
    while(1)
    {
        GPIO_SetBits(GPIOC,GPIO_Pin_13);
        delay(50); clearDisplay();
        drawLetter();
        display();  //2
        // 	ELEVATOR 	\ 	AILERON 	\ 	RUDDER 	\ 	THROTTLE
        sprintf((char*)trasmitData," %d %d %d %d",conaileron, conelevator, conrudder, contthrottle);
        sendData((char*)trasmitData);
        GPIO_ResetBits(GPIOC,GPIO_Pin_13);
        delay(50);
    }
  return 0;
}

