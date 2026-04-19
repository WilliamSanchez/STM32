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
  PA7   =>  MOSI
  PB0   =>  XCLK
  PB1   =>  PCLK
  PB2   =>  HREF
  PB4   =>  VSYNC
  PA1   =>  BIT0
  PA2   =>  BIT1
  PA3   =>  BIT2
  PA6   =>  BIT3 //MISO  Optional
  PB12  =>  BIT4
  PB13  =>  BIT5
  PB14  =>  BIT6
  PB15  =>  BIT7

*/

//  OV7670    Addr = 0x43 (01000011: Read), 0x42(01000010 : Write)



char debug[255];
uint8_t Pixel = 0x00;
uint32_t byte[8];

uint16_t contH = 0;
uint8_t size_display[2*480];
uint8_t cont = 0;

int main(void)
{

  HAL_Init();

  /* Configure the system clock to 100 MHz */
  SystemClock_Config();

  USART_config();
  I2C_config();
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

  HAL_Delay(1000);

  strcpy(debug,"\tCamera OV7670 initializing\n\r");
  usart_send((uint8_t*)debug,strlen(debug));
  //if (ov7670_init() < 0)
  //{
  //    memset(debug,0x00,255);
  //    strcpy(debug,"Error init camera\n\t");
  //    usart_send((uint8_t*)debug,strlen(debug));
  //}
  
  //config_pixels();
  //config_pclk();
  config_href();

  //st7796_init();

  HAL_Delay(1000);

    //uint8_t size_display[2*480];
    //memset(size_display,0xF0CD,2*50*50);
    //uint8_t cont = 0;

  //HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);   
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET); // ---> nss

  while (1)
  {
  
      //HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);   
      //HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET); // ---> nss
      //HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET); // -->> Write comnad
		  //SPI_TxData(0x2C);
      //HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);  // -->> Write data/register
		  //for (int i=0; i<320; i++){
          //memset(size_display,cont,2*480);
          //for (int j=0; j<320; i++){
            //HAL_SPI_Transmit(&SPIHandle, size_display, 2*480, 0);
          //}
		  //}
      //HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET); // ---> nss   
      //HAL_Delay(500);
      //HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
      //cont +=1;  
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  //if(GPIO_Pin == GPIO_PIN_1)
  //{
  //  HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
  //  byte[0]= HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1); //0000000 0|1
  //  byte[1]= HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2);    
  //  byte[2]= HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3);    
  //  byte[3]= HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6);
  //  byte[4]= HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12);
  //  byte[5]= HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13);
  //  byte[6]= HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14);
  //  byte[7]= HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15);
  //  //Pixel =  ((byte[0] << 7) || (byte[1] << 6) || (byte[2] << 5) || (byte[3] << 4) || (byte[4] << 3) || (byte[5] << 2) || (byte[6] << 1) || byte[7]);
  //  Pixel =  ((byte[7] << 7) || (byte[6] << 6) || (byte[5] << 5) || (byte[4] << 4) || (byte[3] << 3) || (byte[2] << 2) || (byte[1] << 1) || byte[0]);
  //  size_display[contH] = cont;  //Pixel;
  //  contH++;
  //}
//
  //if(GPIO_Pin == GPIO_PIN_2)
  //{
  //    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13); 
  //    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET); // -->> Write comnad
	//	  SPI_TxData(0x2C);
  //    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);  // -->> Write data/register
  //    HAL_SPI_Transmit(&SPIHandle, size_display, 2*480, 0);
  //    contH = 0; cont ++;
  //}

  if(GPIO_Pin == GPIO_PIN_0)
  {
      //HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
      HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13); 
      HAL_Delay(1000);
  }
  //else if(GPIO_Pin == GPIO_PIN_0 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 0)
  //{
  //    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
  //}

}


/*
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  //BSP_LED_Toggle(LED4);
  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_8);
}
*/