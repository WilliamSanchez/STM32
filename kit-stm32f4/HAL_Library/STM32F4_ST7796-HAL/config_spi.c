#include "config_spi.h"
#include "stm32f4xx_hal.h"


extern SPI_HandleTypeDef SPIHandle;

void nss_config()
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  // Enable the GPIO_LED Clock //
  __HAL_RCC_GPIOA_CLK_ENABLE();

  // Configure the GPIO_LED pin //
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);  
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET); 
}

void SPI_config(void)
{

    SPIHandle.Instance              = SPI;

    SPIHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    SPIHandle.Init.Direction         = SPI_DIRECTION_2LINES;
    SPIHandle.Init.CLKPhase          = SPI_PHASE_2EDGE;
    SPIHandle.Init.CLKPolarity       = SPI_POLARITY_HIGH;
    SPIHandle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
    SPIHandle.Init.CRCPolynomial     = 7;
    SPIHandle.Init.DataSize          = SPI_DATASIZE_8BIT;
    SPIHandle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
    SPIHandle.Init.NSS               = SPI_NSS_SOFT;
    SPIHandle.Init.TIMode            = SPI_TIMODE_DISABLE; 
    SPIHandle.Init.Mode              = SPI_MODE_MASTER;
    while(HAL_SPI_Init(&SPIHandle) != HAL_OK);

    nss_config();

}

int8_t SPI_TxData(uint8_t value)
{
    if(HAL_SPI_Transmit(&SPIHandle, &value, 1, 10000)!= HAL_OK)
        return -1;
    return 0;
}


int8_t SPI_writeReg(uint8_t reg, uint8_t regvalue)
{
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET); // -->> Write comnad
		SPI_TxData(reg);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET); // -->> Write comnad
		SPI_TxData(regvalue);
    return 0;
}

int8_t SPI_writeData(uint8_t reg, uint8_t *Datavalue, uint32_t length)
{
    if (length <= 1){
	  return -1;
	} else{
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET); // -->> Write comnad
		SPI_TxData(reg);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);  // -->> Write data/register
		for (int i=0; i<length; i++){
		   SPI_TxData(*(Datavalue+i));		
		}
	};
    return 0;
}

int8_t SPI_writePixel(uint16_t *Datavalue, uint32_t length)
{
    if (length <= 1){
	  return -1;
	} else{
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET); // -->> Write comnad
		SPI_TxData(0x2C);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);  // -->> Write data/register
		for (int i=0; i<length; i++){
		   SPI_TxData(0x56);		
           //SPI_TxData((uint8_t*)*(Datavalue+i));
		}
	};
    return 0;
}