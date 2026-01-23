#include "stm32f4xx_hal.h"

#include <string.h>
#include <stdio.h>


#include "OV7670.h"
#include "config_usart.h"
#include "config_i2c.h"
#include "config_xclk.h"

/*
        GY-87 10DOF

  MPU6050:  A 6-axis gyroscope
  HMC5883L: A 3-axis digital magnetometer or compass
  BMP180:   barometer.

  PB6 ======> SCL
  PB7 ======> SDA

  PA9  ======> UART-Tx
  PA10  =====> UART-Rx

  PA0   => Switch key0
  PC13  => Led  

  OV7670    Addr = 0x43 (01000011: Read), 0x42(01000010 : Write)

*/

char debug[255];

int main(void)
{

  HAL_Init();

  /* Configure the system clock to 100 MHz */
  SystemClock_Config();

  USART_config();
  I2C_config();
 
  /* Configure GPIOC LED13 */
  BSP_LED_Init();  

  strcpy(debug,"\tINIT OV7670 CAMERA PROGRAM\n\r");
  usart_send((uint8_t*)debug,strlen(debug));

  if (begin_xclk() < 0)
  {
      memset(debug,0x00,255);
      strcpy(debug,"Error initializing xclk\n\t");
      usart_send((uint8_t*)debug,strlen(debug));
  }

  /*
  strcpy(debug,"\tCamera OV7670 initializing\n\r");
  usart_send((uint8_t*)debug,strlen(debug));
  if (ov7670_init() < 0)
  {
      memset(debug,0x00,255);
      strcpy(debug,"Error init camera\n\t");
      usart_send((uint8_t*)debug,strlen(debug));
  }
  */
  HAL_Delay(1000);
  uint8_t _COM7=0x06;
  uint8_t RxCOM7=0x00;

  while (1)
  {
      HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
      /*
      if (i2c_received(0x43,&_COM7, &RxCOM7, 1) < 0)
      {
        strcpy(debug,"Error read register\n\r");
        usart_send((uint8_t*)debug,strlen(debug));
        return -1;
      }

      sprintf(debug,"Register COM7 is %x\n\r",RxCOM7);
      usart_send((uint8_t*)debug,strlen(debug));
      */
      HAL_Delay(1000);
  }
}

/*
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  //BSP_LED_Toggle(LED4);
  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_8);
}
*/