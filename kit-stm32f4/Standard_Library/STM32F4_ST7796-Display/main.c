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

float _BandWidth[] = {7.8, 10.4, 15.6, 20.8, 31.25, 41.7, 62.5, 125, 250, 500};

char bufferTX[512];
char buffer[255];

volatile int numero = 0;
uint8_t trasmitData[255];
uint8_t trasmitDataS[255];

volatile uint8_t hora;
volatile uint8_t minuto;
volatile uint8_t segundo;
volatile uint32_t miliseg;

int main(void){

    Init_SPI();
    InitializeLEDs();
    configUSART_1();
    delay_init();
    Init_I2C1();
    //begin_RTC(00, 00, 00);
    //LCD_Init();

    sendData("Init program\n\t");

 
    while(1)
    {
      GPIO_SetBits(GPIOC,GPIO_Pin_13);
      delay(5000);
      GPIO_ResetBits(GPIOB,GPIO_Pin_10);
      GPIO_ResetBits(GPIOA, GPIO_Pin_4); // ---> nss 
      SPI_readReg(0x04);
      GPIO_SetBits(GPIOB,GPIO_Pin_10);
      uint8_t reg0 = SPI_readReg(0x00);
      uint8_t reg1 = SPI_readReg(0x00);
      uint8_t reg2 = SPI_readReg(0x00);
      GPIO_SetBits(GPIOA, GPIO_Pin_4); // ---> nss 
      sprintf(buffer,"Register x04 is %x|%x|%x\n\r",reg0,reg1,reg2);
      sendData((char*)buffer);
      GPIO_ResetBits(GPIOC,GPIO_Pin_13);
      delay(5000);
    }
  return 0;
}
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////




