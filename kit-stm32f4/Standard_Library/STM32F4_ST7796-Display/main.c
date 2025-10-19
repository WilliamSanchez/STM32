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

#define max(x,y)    (x>y) ? x:y

const uint16_t HORIZONTAL =       320;
const uint16_t VERTICAL   =       125;//480;//
const uint32_t numPixelbits = 2*HORIZONTAL*VERTICAL;

#define COL_ADDR_SET      0x2A
#define ROW_ADDR_SET      0X2B
#define MEM_WRT           0X2C

const uint16_t Horizontal_line = 320; //320
const uint16_t Vertical_line = 480;//480;

char bufferTX[512];
char buffer[255];

volatile int numero = 0;
uint8_t trasmitData[255];
uint8_t trasmitDataS[255];

int main(void){

    
    InitializeLEDs();
    configUSART_1();
    Init_SPI();
    delay_init();
    Init_Reset();

    sendData("Init program\n\t");

    /* 
      RGB 320H x 480V x 18 bit, dot 262k  
      16-bit/pixel: RGB=(565) 65K
      18-bit/pixel: RGB=(666) 262K
    */

    uint8_t YMLBs = (Vertical_line >> 8) & 0xFF;
    uint8_t YLSBs = Vertical_line & 0xFF;
    uint8_t XMLBs = (Horizontal_line >> 8) & 0xFF;
    uint8_t XLSBs = Horizontal_line & 0xFF;
 
    uint8_t reg[] = {0x80, 0x02, 0x3B};
    uint8_t col[] = {0x00, 0x00, XMLBs, XLSBs};
    uint8_t row[] = {0x00, 0x00, YMLBs, YLSBs};
    uint8_t size_display[numPixelbits];
    memset(size_display,0xF0,numPixelbits);

    GPIO_ResetBits(GPIOA, GPIO_Pin_4); // ---> nss 

    GPIO_ResetBits(GPIOB,GPIO_Pin_10); // -->> Write comnad
		SPI_TxData(0x01);
    delay(150);
    /*
      F0h Comand Set Control pag 239
      
      C3h: Enable command part I
      96h: Enable command part II

      3Ch: Disable command part I
      69h Disable command part II
    */
    SPI_writeReg(0xF0, 0xC3);
    SPI_writeReg(0xF0, 0x96);
    /*
      C5h VCOM Control pag 227.


    */
    SPI_writeReg(0xC5, 0x1C);
    /*
      36h MADCTL Memory Data Access Control pag 183

      MY|MX|MV: These 3bits controls MCU to memory write/read direction.
      ML:       Vertical refresh order "0" Top to Bottom, "1" Bottom to Top
      RGB:      Color selector switch "0" RGB, "1" BGR.
      MH:       Horizontal refresh order '0' Left to Right, '1' Right to Left
    */
    SPI_writeReg(0x36, 0x48);
    /*
      3Ah Interface pixel format pag 190
      
      D7-D4: RGB Interface color format '0101' 16bit/pixel, '0110' 18bit/pixel
      D3-D0: Control interface color format '0101' 16bit/pixel, '0110' 18bit/pixel, '0111' 24 bit/pixel.
    */
    SPI_writeReg(0x3A, 0x55);  
    /*
      B0h Interface moed Control pag 212

      SPI_EN: 3/4 wire wire serial interface selection, '0' DIN and DOUT pins are used for 3/4 wire serial interface, 
                                                        '1' DIN/SDA pin is used for 3/4 wire serial interface and DOUT pin is not used

      DEP: polarity '0' High enable for RGB Interface, '1' Low enbale for RGB interface
      PKP: PCLK polarity '0' rising time, '1' falling time.
      HSCP: HSYNC Polarity '0' low level sync clock, '1' High level sync clock.
      VSCP: VSYNC polarity '0' low level sync clock, '1' Higj level sync clock.
    */
    SPI_writeReg(0xB0, 0x80);
    /*
      B4h Dislay inversion control pag 216

      '00' Column inversion, '01' 1-dot inversion, '10' 2-dot inversion
    */
    SPI_writeReg(0xB4, 0x00);
    /*
      B6h Display fnction control pag 218
    */
    SPI_writeData(0xB6, reg, 3);
    /*
      B7h Entry mode set pag 222
    */
    SPI_writeReg(0xB7, 0xC6);
    SPI_writeReg(0xF0, 0x69);
    SPI_writeReg(0xF0, 0x3C);
    /*
      2Ah Column address set pag 170.

      Xs[15:0]: Start
      Xe[15:0]: End
    */
    SPI_writeData(COL_ADDR_SET,col, 4);
      /*
      2Bh Row address set pag 172.

      ys[15:0]: Start
      ye[15:0]: End
    */
    SPI_writeData(ROW_ADDR_SET, row, 4);
    GPIO_ResetBits(GPIOB,GPIO_Pin_10); // -->> Write comnad
    /*
      11h Sleep OUT ag 160.

      Turn off sleep out.
    */
    SPI_TxData(0x11);
    delay(150);
    /*
      29h Display ON pag 169
      This command is used to recover from display off mode 28h
    */
		SPI_TxData(0x29); 
    delay(150);
    GPIO_SetBits(GPIOA, GPIO_Pin_4); // ---> nss 
    uint8_t cont = 0;

    while(1)
    {
      GPIO_SetBits(GPIOC,GPIO_Pin_13);
      delay(500);
      
      memset(size_display,cont++,numPixelbits);
      GPIO_ResetBits(GPIOA, GPIO_Pin_4); // ---> nss 
      /*
        2Ch Data memory write pag 173
      */
      SPI_writePixel(size_display, 50*320);
      memset(size_display,cont++,numPixelbits);
      SPI_writePixel(size_display, 50*320);
      GPIO_SetBits(GPIOA, GPIO_Pin_4); // ---> nss 
      GPIO_ResetBits(GPIOC,GPIO_Pin_13);
      delay(500);

      GPIO_SetBits(GPIOC,GPIO_Pin_13);
      
      memset(size_display,cont++,numPixelbits);
      GPIO_ResetBits(GPIOA, GPIO_Pin_4); // ---> nss 
      /*
        2Ch Data memory write pag 173
      */
      SPI_writePixel(size_display, 50*320);
      memset(size_display,cont++,numPixelbits);
      SPI_writePixel(size_display, 50*320);
      GPIO_SetBits(GPIOA, GPIO_Pin_4); // ---> nss 
      GPIO_ResetBits(GPIOC,GPIO_Pin_13);
      delay(500);
    }
  return 0;
}
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////




