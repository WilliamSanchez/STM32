#include "configSPI.h"
#include<misc_extras.h>


void Init_Reset(void){

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  GPIO_InitTypeDef  GPIO_InitStruct;

  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/////////////

void reset(void){

    GPIO_ResetBits(GPIOB,GPIO_Pin_10);
    delay_us(10000);
    GPIO_SetBits(GPIOB,GPIO_Pin_10);
    delay_us(10000);

}

void Init_SPI(void){

   GPIO_InitTypeDef  GPIO_InitStructure;
   SPI_InitTypeDef   SPI_InitStructure;

   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);


   // Configure sEE pins: SCK /
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;   // --> SPI2_SCK
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
   GPIO_Init(GPIOA, &GPIO_InitStructure);

   // Connect PXx to sEE_SCK /
   GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);

   // Connect PXx to sEE_MISO /
   GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1); 

   // Connect PXx to sEE_MOSI //
   GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);

   // Configure sEE_CS_PIN pin: sEE Card CS pin //
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;  // --> SPI2_CSs 
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOA, &GPIO_InitStructure); 

  // SPI configuration -------------------------------------------------------//

  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI1, &SPI_InitStructure);

  SPI_Cmd(SPI1, ENABLE);

   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;  // --> SPI2_CSs 
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOA, &GPIO_InitStructure); 

  GPIO_SetBits(GPIOA, GPIO_Pin_4); 



}




/////////  SPI Read Regster

uint8_t SPI_readReg(uint8_t reg){

   uint8_t regvalue = 10;
   GPIO_ResetBits(GPIOA, GPIO_Pin_4); // ---> nss 
   while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
   SPI_SendData(SPI1, reg);
   while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
   SPI_ReceiveData(SPI1);
   while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
   SPI_SendData(SPI1, 0x00);
   while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
   regvalue = SPI_ReceiveData(SPI1);
   GPIO_SetBits(GPIOA, GPIO_Pin_4);  // ---> nss   
   return regvalue;
}


/////////  SPI Write Regster

void SPI_writeReg(uint8_t reg, uint8_t regvalue){

   GPIO_ResetBits(GPIOA, GPIO_Pin_4); // ---> nss 
   while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
   SPI_SendData(SPI1, reg);
   while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
   SPI_ReceiveData(SPI1);
   while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
   SPI_SendData(SPI1, regvalue);
   while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
   SPI_ReceiveData(SPI1);
   GPIO_SetBits(GPIOA, GPIO_Pin_4);  // ---> nss 

}



/////////  SPI Write one byte

void SPI_TxData(uint8_t value){

   while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
   SPI_SendData(SPI1, value);
   while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
   SPI_ReceiveData(SPI1);

}

/////////  SPI Write Burst data

void SPI_writeData(uint8_t reg, uint8_t *Datavalue, uint8_t length){

	if (length <= 1){
	  return;
	} else{
		GPIO_ResetBits(GPIOA, GPIO_Pin_11);
		SPI_TxData(reg);
		for (int i=0; i<length; i++){
		   SPI_TxData(*(Datavalue+i));		
		}
		GPIO_SetBits(GPIOB, GPIO_Pin_11); 
	}

}


