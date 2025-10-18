/*
	USART_1 PC		SPI_2   SD Card 

	Tx ==> PA9		MISO 	==> PA6         Optional
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



#define COL_ADDR_SET      0x2A
#define ROW_ADDR_SET      0X2B
#define MEM_WRT           0X2C



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

    
    InitializeLEDs();
    configUSART_1();
    Init_SPI();
    delay_init();
    Init_Reset();

    sendData("Init program\n\t");

    //uint8_t reg0[4], reg1[4], reg2[4];

    uint8_t reg[] = {0x80, 0x02, 0x3B};
    uint8_t reg0[] = {0x00, 0x00, 0x00, 0x45};
    uint8_t reg1[] = {0x00, 0x00, 0x00, 0x45};
    uint8_t reg2[2*200*200];
    memset(reg2,0xF0,2*69*69);

    GPIO_ResetBits(GPIOA, GPIO_Pin_4); // ---> nss 

    GPIO_ResetBits(GPIOB,GPIO_Pin_10); // -->> Write comnad
		SPI_TxData(0x01);
    delay(150);

    SPI_writeReg(0xF0, 0xC3);
    SPI_writeReg(0xF0, 0x96);
    SPI_writeReg(0xC5, 0x1C);
    SPI_writeReg(0x36, 0x48);
    SPI_writeReg(0x3A, 0x55);
    SPI_writeReg(0xB0, 0x80);
    SPI_writeReg(0xB4, 0x00);
    SPI_writeData(0xB6, reg, 3);
    SPI_writeReg(0xB7, 0xC6);
    SPI_writeReg(0xF0, 0x69);
    SPI_writeReg(0xF0, 0x3C);
    SPI_writeData(COL_ADDR_SET, reg0, 4);
    SPI_writeData(ROW_ADDR_SET, reg1, 4);
    GPIO_ResetBits(GPIOB,GPIO_Pin_10); // -->> Write comnad
    SPI_TxData(0x11);
    delay(150);
		SPI_TxData(0x29); 
    delay(150);
    GPIO_SetBits(GPIOA, GPIO_Pin_4); // ---> nss 

    while(1)
    {
      GPIO_SetBits(GPIOC,GPIO_Pin_13);
      delay(5000);
      
      memset(reg2,0xF0,2*69*69);
      GPIO_ResetBits(GPIOA, GPIO_Pin_4); // ---> nss 
      SPI_writeData(MEM_WRT, reg2, 2*69*69);
      GPIO_SetBits(GPIOA, GPIO_Pin_4); // ---> nss 
      //sprintf(buffer,"Register x04 is %x|%x|%x\n\r",reg0,reg1,reg2);
      //sendData((char*)buffer);
      GPIO_ResetBits(GPIOC,GPIO_Pin_13);
      delay(5000);

      GPIO_SetBits(GPIOC,GPIO_Pin_13);
      
      memset(reg2,0x0F,2*69*69);
      GPIO_ResetBits(GPIOA, GPIO_Pin_4); // ---> nss 
      SPI_writeData(MEM_WRT, reg2, 2*69*69);
      GPIO_SetBits(GPIOA, GPIO_Pin_4); // ---> nss 
      //sprintf(buffer,"Register x04 is %x|%x|%x\n\r",reg0,reg1,reg2);
      //sendData((char*)buffer);
      GPIO_ResetBits(GPIOC,GPIO_Pin_13);
      delay(5000);
    }
  return 0;
}
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////




