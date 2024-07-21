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

float timeOnAir(uint16_t payloadLength);
uint16_t getBW();
uint8_t getSF();
uint16_t getPreambule();
uint8_t getHeader();

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
    begin_RTC(00, 00, 00);
    LCD_Init();
/*
       long frequency = 915+x

 	MODE	|	 BW	|	CR	|	SF  
	  1	|	125     |	4/5	|	12		Max range, slow date rate
	  2	|	250     |	4/5	|	12  
	  3	|	125     |	4/5	|	10
	 10	|	500	|	4/5	|	7		Min range, fast date rate, minimum battey impact

*/
    long frequenCentral = 915E6;

    if (SX1278_Begin(&sx1278_Acosta,frequenCentral, SX1278_POWER_20DBM, 
		SX1278_LORA_SF_12,SX1278_LORA_BW_125KHZ) == 1){ GPIO_ResetBits(GPIOC,GPIO_Pin_13); while(1);}
     
    delay(1000);display();
      sprintf((char*)trasmitData,"BW %f\n\rSF %d\n\rnPreambule %d\n\rHeader %s\n\r",(float)getBW(),getSF(),getPreambule(),getHeader ? "OFF":"ON");
      sendData((char*)trasmitData);
      memset(trasmitData,0x00,strlen(trasmitData));
    delay(1000);clearDisplay();
 
    while(1)
    {
      GPIO_SetBits(GPIOC,GPIO_Pin_13);
      delay(5000);delay(5000);delay(5000);delay(5000);delay(5000);
//      delay(5000);delay(5000);delay(5000);delay(5000);delay(5000);
//      delay(5000);delay(5000);delay(5000);delay(5000);delay(5000);
//      delay(5000);delay(5000);delay(5000);delay(5000);delay(5000);

      hora = getHoras(); minuto=getMinutos();segundo = getSegundos(); miliseg = getmillSegundos();
      sprintf((char*)trasmitData,"%02d:%02d:%02d:%03d ", hora, minuto, segundo, miliseg);
            float time_data = hora*3600+minuto*60+segundo+(float)miliseg/1000;      

 		// Send data to Gateway
//      sprintf(buffer,"%d %s %f %f",numero++,(char*)trasmitData,-23.213792,-45.877323);
//      sprintf(buffer,"[%s] END DEVICE SEND DATA [%d]",(char*)trasmitData,numero++);
//      sprintf(buffer,"[%s] If you want to master English, get involved and practise as much as possible. [%d]",(char*)trasmitData,numero++);
//      sprintf(buffer,"[%s] END DEVICE Walt Disney was well known as a man who made dreams come true, and you can, too. It just takes plenty of hard work. [%d]",(char*)trasmitData,numero++);
      sprintf(buffer,"{%d> %s: END DEVICE Learning a skill such as a new language can take a long time. If you feel like your progress is slow, bear in mind these wise words from Confucius. The important thing is to keep going and you will get there in the end. }",numero++,(char*)trasmitData);
      uint8_t size = strlen(buffer);  
      while (SX1278_beginTx(&sx1278_Acosta, (uint8_t *)buffer, size) != 0x12 ){};
      while (SX1278_sendData(&sx1278_Acosta) != 0x12){};
      SX1278_Sleep(&sx1278_Acosta);
      time_data = (getHoras()*3600+getMinutos()*60+getSegundos()+(float)getmillSegundos()/1000)-time_data;
      sprintf((char*)trasmitDataS,"%03f[s]",time_data);
      drawLetter((uint8_t*)"END DEVICE",(uint8_t*)" ",trasmitData,trasmitDataS);display();  //2
      sprintf((char*)trasmitData,"%d> time on air: %f ms time envio: %f numero de bytes [%d]\n\r",numero,timeOnAir((uint16_t)strlen(buffer)),time_data,strlen(buffer));
      sendData((char*)trasmitData);
      memset(buffer,0x00,strlen(buffer));
      memset(trasmitData,0x00,strlen(trasmitData));
      memset(trasmitDataS,0x00,strlen(trasmitDataS));
      GPIO_ResetBits(GPIOC,GPIO_Pin_13);
      delay(5000);delay(5000);delay(5000);delay(5000);delay(5000);
//      delay(5000);delay(5000);delay(5000);delay(5000);delay(5000);
//      delay(5000);delay(5000);delay(5000);delay(5000);delay(5000);
//      delay(5000);delay(5000);delay(5000);delay(5000);delay(5000);	

    }
  return 0;
}
/*
		sprintf(data,"Aqui o transmisor %d",numero++);
   		uint8_t size = strlen(data);  
    		while (SX1278_beginTx(&Sx1278_LoRa, (uint8_t *)data, size) != 0x12 ){};
    		while (SX1278_sendData(&Sx1278_LoRa) != 0x12){};
    		SX1278_Sleep(&Sx1278_LoRa);
*/



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

