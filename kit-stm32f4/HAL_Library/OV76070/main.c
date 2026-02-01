#include "stm32f4xx_hal.h"

#include <string.h>
#include <stdio.h>


#include "OV7670.h"
#include "config_usart.h"
#include "config_i2c.h"
#include "config_spi.h"
#include "config_xclk.h"
#include "ST7796.h"

SPI_HandleTypeDef SPIHandle;

/*

  PB6   =>  SCL
  PB7   =>  SDA
  PA9   =>  UART-Tx
  PA10  =>  UART-Rx
  PA0   =>  Switch key0
  PC13  =>  Led 
  PB10  =>  Command
  PA4   =>  NSS
  PA5   =>  SCK1
  PA6   =>  MISO  Optional
  PA7   =>  MOSI

*/

//  OV7670    Addr = 0x43 (01000011: Read), 0x42(01000010 : Write)



char debug[255];
uint8_t bit1;

int main(void)
{

  HAL_Init();

  /* Configure the system clock to 100 MHz */
  SystemClock_Config();

  USART_config();
  //I2C_config();
  SPI_config();
 
  /* Configure GPIOC LED13 */
  BSP_LED_Init();  

  strcpy(debug,"\tINIT OV7670 CAMERA PROGRAM\n\r");
  usart_send((uint8_t*)debug,strlen(debug));

  //if (begin_xclk() < 0)
  //{
  //    memset(debug,0x00,255);
  //    strcpy(debug,"Error initializing xclk\n\t");
  //    usart_send((uint8_t*)debug,strlen(debug));
  //}

  strcpy(debug,"\tCamera OV7670 initializing\n\r");
  usart_send((uint8_t*)debug,strlen(debug));
  //if (ov7670_init() < 0)
  //{
  //    memset(debug,0x00,255);
  //    strcpy(debug,"Error init camera\n\t");
  //    usart_send((uint8_t*)debug,strlen(debug));
  //}
  
  //config_pixels();
  //config_vsyn(); 

  st7796_init();

  HAL_Delay(1000);

    uint8_t size_display[2*320];
    //memset(size_display,0xF0CD,2*50*50);
    uint8_t cont = 0;

  while (1)
  {
  
      HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);   
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET); // ---> nss
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET); // -->> Write comnad
		  SPI_TxData(0x2C);
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);  // -->> Write data/register
		  for (int i=0; i<480; i++){
          memset(size_display,cont,2*320);
          //for (int j=0; j<320; i++){
            HAL_SPI_Transmit(&SPIHandle, size_display, 2*320, 0);
          //}
		  }
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET); // ---> nss   
      HAL_Delay(500);
      cont +=1;
  
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == GPIO_PIN_0)
  {
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
    bit1  = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7); //0000000 0|1
    //dec16(msb,lsb)
  }
}

/*
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  //BSP_LED_Toggle(LED4);
  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_8);
}
*/