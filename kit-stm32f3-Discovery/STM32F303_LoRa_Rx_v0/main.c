/*
	USART_1 PC		SPI_2   SD Card 

	Tx ==> PA9		MISO 	==> PB14
	Rx ==> PA10		MOSI 	==> PB15
				SCK  	==> PB13
				NSS 	==> PB11
				Nreset 	==> PB10
*/

#include <stm32f30x.h>
#include <string.h>
#include <stdio.h>

#include "modulo3G.h"
#include "configSPI.h"
#include "configSX1278.h"
#include "misc.h"


#define CMD0        0
#define CMD0_ARG    0x00000000
#define CMD0_CRC    0x94

#define MAX_PKT_LENGTH		255

__IO uint32_t TimeOut = 0x0;

int _packetIndex;
int _implicitHeaderMode;


char trasmitData[255];
char bufferRX[25];

volatile int i = 0;
int numero= 0;
uint8_t dataSD = 0xFF;

char data[50];
uint8_t flag = 0;

SX1278_Device Sx1278_LoRa;

uint8_t Init_LoRa(long frequency);
void SetSpreadingfactor(int sf);
void SetSignalBandwith(long sbw);
void SetCodingRate4(int denominator);

int begin_RxTx(int implicitHeader);
size_t sendData_LoRa(const uint8_t *buffer, size_t size);
size_t WriteData_LoRa(char buffer[]);
uint8_t end_RxTx();

int ParseData_LoRa(int size);
int Available_LoRa();
int read_LoRa();

char bufferTX[25];
char buffer[255];

int main(void){

    InitializeLEDs();
    delay_init();
    configUSART_1();
    Init_SPI();
    reset(); 
    uint8_t dataLORA = 0x0A; 
    while(Init_LoRa(915E6) != 0x12){};
    SetSpreadingfactor(12);
    SetSignalBandwith(62.5E3);
    SetCodingRate4(8);
    delay(1000);
    while(1)
    {  	
      int i =0;
      int packetSize = ParseData_LoRa(0);
      GPIO_SetBits(GPIOE,GPIO_Pin_10);
      if (packetSize) {
      // received a packet
        sprintf(trasmitData,"\n\rReceived packet %d:", packetSize);
        USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
        while ((USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET));

      // read packet
      while (Available_LoRa()) {
      // Serial.print((char)read_LoRa());
      bufferRX[i]=((uint8_t)read_LoRa());
      i++;
      GPIO_ResetBits(GPIOE,GPIO_Pin_10);
     }

        sprintf(trasmitData,"\tData ( %s )", bufferRX);
        USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
        while ((USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET));

     delay(50);
    }
 }

/*
	GPIO_ResetBits(GPIOE,GPIO_Pin_12);
        GPIO_SetBits(GPIOE,GPIO_Pin_13);
        GPIO_ResetBits(GPIOE,GPIO_Pin_9);
        delay(500);
	GPIO_SetBits(GPIOE,GPIO_Pin_12);
        GPIO_ResetBits(GPIOE,GPIO_Pin_13);
        GPIO_SetBits(GPIOE,GPIO_Pin_9);
        delay(500);
*/


  return 0;
}



uint8_t Init_LoRa(long frequency)
{


  if(SPI_readReg(0x42) != 18){
	return 1;
   }
  /// Mode Sleep
  SPI_writeReg(writeReg | 0x01, MODE_LoRa | MODE_SLEEP);

  // Set frequency
  uint64_t frf = ((uint64_t)frequency << 19) / 32000000;

  SPI_writeReg(writeReg | 0x06, (uint8_t)(frf >> 16));
  SPI_writeReg(writeReg | 0x07, (uint8_t)(frf >> 8));
  SPI_writeReg(writeReg | 0x08, (uint8_t)(frf >> 0));

  // set base addresses
  SPI_writeReg(writeReg | 0x0E, 0);
  SPI_writeReg(writeReg | 0x0F, 0);

   // set LNA boost
  SPI_writeReg(writeReg | 0x0C, SPI_readReg(0x0C) | 0x03);

    // set auto AGC
  SPI_writeReg(writeReg | 0x26, 0x04);

  // set output power to 17 dBm
  SPI_writeReg(writeReg | 0x09, 0x70 | 14);

  /// Mode IDLE
  SPI_writeReg(writeReg | 0x01, MODE_LoRa | MODE_STDBY);

  delay(50);

  return SPI_readReg(0x42);
}

///// SetSpreadingfactor 

void SetSpreadingfactor(int sf){

  if (sf < 6) {
    sf = 6;
  } else if (sf > 12) {
    sf = 12;
  }

  if (sf == 6) {
    SPI_writeReg(writeReg | 0x31, 0xc5);
    SPI_writeReg(writeReg | 0x37, 0x0c);
  } else {
    SPI_writeReg(writeReg | 0x31, 0xc3);
    SPI_writeReg(writeReg | 0x37, 0x0a);
  }

  SPI_writeReg(writeReg |0x1E, (SPI_readReg(0x1E) & 0x0f) | ((sf << 4) & 0xf0));

}

///// SetSignalBandwith 

void SetSignalBandwith(long sbw){

   int bw;

  if (sbw <= 7.8E3) {
    bw = 0;
  } else if (sbw <= 10.4E3) {
    bw = 1;
  } else if (sbw <= 15.6E3) {
    bw = 2;
  } else if (sbw <= 20.8E3) {
    bw = 3;
  } else if (sbw <= 31.25E3) {
    bw = 4;
  } else if (sbw <= 41.7E3) {
    bw = 5;
  } else if (sbw <= 62.5E3) {
    bw = 6;
  } else if (sbw <= 125E3) {
    bw = 7;
  } else if (sbw <= 250E3) {
    bw = 8;
  } else /*if (sbw <= 250E3)*/ {
    bw = 9;
  }

  SPI_writeReg(writeReg | 0x1D, (SPI_readReg(0x1D) & 0x0f) | (bw << 4));


}

///// SetCodingRate

void SetCodingRate4(int denominator){

   if (denominator < 5) {
    denominator = 5;
  } else if (denominator > 8) {
    denominator = 8;
  }

 int cr = denominator - 4;

  SPI_writeReg(writeReg | 0x1D, (SPI_readReg(0x1D) & 0xf1) | (cr << 1));


}

//////  begin Rx/Tx Data

int begin_RxTx(int implicitHeader)
{
  // put in standby mode
  SPI_writeReg(writeReg | 0x01, MODE_LoRa | MODE_STDBY);

  if (implicitHeader) {
    SPI_writeReg(writeReg | 0x1D, SPI_readReg(0x1D) | 0x01);
  } else {
    SPI_writeReg(writeReg | 0x1D, SPI_readReg(0x1D) & 0xfe);
  }

  // reset FIFO address and paload length
  SPI_writeReg(writeReg | 0x0D, 0);
  SPI_writeReg(writeReg | 0x22, 0);

  return 1;
}

/////////  Send Data LoRa

size_t sendData_LoRa(const uint8_t *buffer, size_t size)
{
  int currentLength = SPI_readReg(0x22);

  // check size
  if ((currentLength + size) > MAX_PKT_LENGTH) {
    size = MAX_PKT_LENGTH - currentLength;
  }

  // write data
  for (size_t i = 0; i < size; i++) {
    SPI_writeReg(writeReg | 0x00, currentLength + buffer[i]);
  }

  // update length
  SPI_writeReg(writeReg | 0x22, currentLength + size);


  return size;
}

size_t WriteData_LoRa(char buffer[])
{int mlen=strlen(buffer); return sendData_LoRa((uint8_t*)buffer,mlen);}


/////////  parse Data LoRa

int ParseData_LoRa(int size){
  
  int packetLength = 0;
  int irqFlags = (SPI_readReg(0x12));
 
  if (size > 0) {
    SPI_writeReg(writeReg | 0x1D, SPI_readReg(0x1D) | 0x01);
    SPI_writeReg(writeReg | 0x22, size & 0xff);
    _implicitHeaderMode = 1;
  } else {
    SPI_writeReg(writeReg | 0x1D, SPI_readReg(0x1D) & 0xfe);
    _implicitHeaderMode = 0;
  }

    // clear IRQ's
  SPI_writeReg(writeReg | 0x12, irqFlags);
 
  if ((irqFlags & 0x40) && (irqFlags & 0x20) == 0){

       _packetIndex = 0;

    // read packet length
    if (_implicitHeaderMode) {
      packetLength = SPI_readReg(0x22);
    } else {
      packetLength = SPI_readReg(0x13);
    }
   
    // set FIFO address to current RX address
    SPI_writeReg(writeReg | 0x0D, SPI_readReg(0x10));

    // put in standby mode
    SPI_writeReg(writeReg | 0x01, MODE_LoRa | MODE_STDBY);

  }else if(SPI_readReg(0x01) != (0x80 | 0x06)){
    // not currently in RX mode
      
    // reset FIFO address
    SPI_writeReg(writeReg | 0x0D, 0);

    // put in single RX mode
    SPI_writeReg(writeReg | 0x01, 0x80 | 0x06);

}

  return packetLength;
}

//// LoRa available

int Available_LoRa(){

  return (SPI_readReg(0x13) - _packetIndex);
}

//// End receiver/transmition of data

uint8_t end_RxTx(){
  
   // put in TX mode
  SPI_writeReg(writeReg | 0x01, MODE_LoRa | MODE_TX);

  // wait for TX done
  //while ((SPI_readReg(0x12) & 0x08) == 0){};
  while ((SPI_readReg(0x12)) == 0);
  // clear IRQ's
  SPI_writeReg(0x12, 0x08);

  return 1;
}

int read_LoRa()
{
  if (!Available_LoRa()) {
    return -1;
  }

  _packetIndex++;

  return SPI_readReg(0x00);
}

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////






