#include "configSPI.h"


///////////////////////////////////////////////////////////////////
////////////////////     RESET          ///////////////////////////
///////////////////////////////////////////////////////////////////



void Init_Reset(void){

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
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

   RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

   /* Configure sEE pins: SCK */
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;   // --> SPI2_SCK
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
   GPIO_Init(GPIOB, &GPIO_InitStructure);

   /* Configure sEE pins: MISO */
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;   // --> SPI2_MISO
   GPIO_Init(GPIOB, &GPIO_InitStructure);

   /* Configure sEE pins: MOSI */
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;  // --> SPI2_MOSI
   GPIO_Init(GPIOB, &GPIO_InitStructure);

   /* Configure sEE_CS_PIN pin: sEE Card CS pin */
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;  // --> SPI2_CSs 
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOB, &GPIO_InitStructure);

   /* Connect PXx to sEE_SCK */
   GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_5);

   /* Connect PXx to sEE_MISO */
   GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_5); 

   /* Connect PXx to sEE_MOSI */
   GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_5); 

   /* sEE SPI Config */
   SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
   SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
   SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
   SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
   SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
   SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
   SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;

   SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
   SPI_InitStructure.SPI_CRCPolynomial = 7;
   SPI_Init(SPI2, &SPI_InitStructure);

   SPI_RxFIFOThresholdConfig(SPI2, SPI_RxFIFOThreshold_QF);

   /* sEE_SPI enable */
   SPI_Cmd(SPI2, ENABLE);

   GPIO_SetBits(GPIOB, GPIO_Pin_11); 


}




/////////  SPI Read Regster

uint8_t SPI_readReg(uint8_t reg){

   uint8_t regvalue = 10;
   GPIO_ResetBits(GPIOB, GPIO_Pin_11);
   while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
   SPI_SendData8(SPI2, reg);
   while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
   SPI_ReceiveData8(SPI2);
   while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
   SPI_SendData8(SPI2, 0x00);
   while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
   regvalue = SPI_ReceiveData8(SPI2);
   GPIO_SetBits(GPIOB, GPIO_Pin_11); 
   return regvalue;
}

/////////  SPI Write Regster

void SPI_writeReg(uint8_t reg, uint8_t regvalue){

   GPIO_ResetBits(GPIOB, GPIO_Pin_11);
   while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
   SPI_SendData8(SPI2, reg);
   while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
   SPI_ReceiveData8(SPI2);
   while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
   SPI_SendData8(SPI2, regvalue);
   while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
   SPI_ReceiveData8(SPI2);
   GPIO_SetBits(GPIOB, GPIO_Pin_11); 
}



/////////  SPI Write one byte

void SPI_TxData(uint8_t value){

   while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
   SPI_SendData8(SPI2, value);
   while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
   SPI_ReceiveData8(SPI2);

}

/////////  SPI Write Burst data

void SPI_writeData(uint8_t reg, uint8_t *Datavalue, uint8_t length){

	if (length <= 1){
	  return;
	} else{
		GPIO_ResetBits(GPIOB, GPIO_Pin_11);
		SPI_TxData(reg);
		for (int i=0; i<length; i++){
		   SPI_TxData(*(Datavalue+i));		
		}
		GPIO_SetBits(GPIOB, GPIO_Pin_11); 
	}

}

