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
#include "configSX1278.h"
#include "lcdOLED.h"

#define max(x,y)    (x>y) ? x:y

SX1278_Device sx1278_Acosta;

float _BandWidth[] = {7.8, 10.4, 15.6, 20.8, 31.25, 41.7, 62.5, 125, 250, 500};

char bufferTX[512];
char buffer[255];
char dataMode[225] = "XXXX";

float timeOnAir(uint16_t payloadLength);
uint16_t getBW();
uint8_t getSF();
uint16_t getPreambule();
uint8_t getHeader();

volatile int numero = 0;
uint8_t trasmitData[255];
uint8_t trasmitDataS[255];
uint8_t dataTime[255];
uint8_t mode =0;
uint8_t changeMode[16];
uint8_t changeDelay[16];
uint8_t contDelay = 1;

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
    begin_RTC(00, 00, 00);
    LCD_Init();
    configButton();
    configButton_mode();
    configButton_delay();
/*
       long frequency = 915+x

 	MODE	|	 BW	|	CR	|	SF  
	  1	|	125     |	4/5	|	12		Max range, slow date rate
	  2	|	250     |	4/5	|	12  
	  3	|	125     |	4/5	|	10
	 10	|	500	|	4/5	|	7		Min range, fast date rate, minimum battey impact

*/
     
    delay(1000);display();
    delay(1000);clearDisplay();
    uint8_t contret = 0;
    while(1)
    {
      contret = 10*contDelay;
      while (contret > 0 ){
        contret--;
        GPIO_SetBits(GPIOC,GPIO_Pin_13);
        delay(2500); clearDisplay();
        sprintf((char*)changeDelay,"%d>   [ %d]s",10*contDelay,contret);
        drawLetter((uint8_t*)("END DEVICE"), (uint8_t*)(" "),changeDelay,changeMode, (uint8_t*)("Lat: ---"), (uint8_t*)("Lon: ---"), (uint8_t*)("time: "),(uint8_t*)("dif time: "));
        display();  //2
        GPIO_ResetBits(GPIOC,GPIO_Pin_13);
        delay(2500);
      }
    }
  return 0;
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////


uint16_t getBW(){

    uint16_t _BW = 0;
    uint8_t reg = 0;
    reg = SPI_readReg(0x1D);
    reg &= 0xF0; 
    _BW = _BandWidth[(reg>>4)];
   
   return _BW;

}

uint8_t getSF(){

    uint8_t _SF = 0;
    uint8_t reg = 0;
    reg = SPI_readReg(0x1E);
    reg &= 0xF0; 
    _SF = (reg>>4);
   
   return _SF;

}


uint16_t getPreambule(){

    uint16_t _nPreambule = 0x0000;
    uint8_t reg1 = 0;
    uint8_t reg2 = 0;
    reg1 = SPI_readReg(0x20);
    reg2 = SPI_readReg(0x21);
    _nPreambule  = (reg1<<4) | reg2;     

   return _nPreambule;

}

uint8_t getHeader(){

    uint16_t _Header = 0x0000;
    uint8_t reg1 = 0;
    reg1 = SPI_readReg(0x1D);     
    _Header = 0x01 & reg1;

   return _Header;

}



float timeOnAir(uint16_t payloadLength){

  float _time = 0.0;
  
  float BW = getBW();
  uint16_t nPreambuleLength= getPreambule();
  float DE = 0;
  float SF = 12.0;//(float)getSF();
  float PL = (float)payloadLength;
  float H = getHeader();
  float CR = 1.0;

  if(SF>10) DE = 1;

  float Tsym = pow(2,SF)/BW; //ms
  float Tpreambule = ((float)nPreambuleLength+4.25)*Tsym; //ms
  float value0 = (8.0*PL-4.0*SF+28.0+16.0-20.0*H)/(4.0*(SF-2.0*DE));
  float value1 = ceil(value0)*(CR+4.0);
  float value2 = 0;
  float payloadSymbNb = 8 + max(value1,value2); 
  float Tpayload = payloadSymbNb*Tsym; 
  _time = Tpreambule + Tpayload;


  return _time;
   
}

