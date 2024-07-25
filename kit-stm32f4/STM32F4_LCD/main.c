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
#include "configSPI.h"
#include "configUSART.h"
#include "lcdOLED.h"

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
    Init_I2C1();
    serialPC();
    delay_init();
    LCD_Init();
    configButton();
    configButton_mode();
    configButton_delay();
    
    delay(1000);display();
    delay(1000);clearDisplay();
    uint8_t contret = 0;
    while(1)
    {
      contret = 10*contDelay;
      while (contret > 0 ){
        contret--;
        GPIO_SetBits(GPIOC,GPIO_Pin_13);
        delay(50); clearDisplay();
        drawLetter(&contDelay);
        display();  //2
        sprintf((char*)trasmitData,"CONT %d\r\n", contDelay);
        sendData((char*)trasmitData);
        GPIO_ResetBits(GPIOC,GPIO_Pin_13);
        delay(50);
      }
    }
  return 0;
}

