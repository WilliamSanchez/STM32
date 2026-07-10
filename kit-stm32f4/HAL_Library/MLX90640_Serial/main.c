#include "stm32f4xx_hal.h"

#include <string.h>
#include <stdio.h>
#include "config_usart.h"

UART_HandleTypeDef UartHandle;
UART_HandleTypeDef UartHandle_Mlx90640;
__IO ITStatus UartReady = RESET;

/*
        GY-87 10DOF

  MPU6050:  A 6-axis gyroscope
  HMC5883L: A 3-axis digital magnetometer or compass
  BMP180:   barometer.

  PA2  ======> Tx2
  PA3  ======> Rx2

  PA9  ======> Tx1
  PA10  =====> Rx1

  PA0   => Switch key0
  PC13  => Led  

  BMP180    Addr = 0xEF (11101111: Read), 0xEE(11101110 : Write)
  HMC5883L  Addr = 0x3D (00111101 : Read), 0x3C (00111100 : Write)
  MPU6050   Addr = 0x68 

*/

char debug[2024];
char data[1544];

int main(void)
{

  HAL_Init();

  /* Configure the system clock to 100 MHz */
  SystemClock_Config();

  USART_config();
  USART_MLX6040();
  BSP_LED_Init();

  HAL_UART_Receive_IT(&UartHandle_Mlx90640, (uint8_t *)data, 1544);
  //while (UartReady != SET){}

  HAL_Delay(1000);

  while (1)
  {
      HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
      //memcpy(debug,data,1544);
      sprintf(debug,"Header %x%x, num data = %d \n\r\0",data[0],data[1],(int)(256*data[3]+data[2]));
      uint8_t pixel[1540];
      memcpy(pixel,&data[4],1540);
      usart_send(debug, strlen(debug));
      for(int i=0; i< 24; i++)
      {
        for(int j=0; j<32; j++)
        {
                sprintf(debug,"\t%.2f\0",(float)(((int)pixel[24*i+j] + 256*(int)pixel[24*i+j+1])/100));
                usart_send(debug, strlen(debug));
        }
          strcpy(debug,"\n\r\0");
          usart_send(debug, strlen(debug));
      }

      strcpy(debug,"======================================\0");
      usart_send(debug, strlen(debug));

      HAL_Delay(1000);
  }
  return 0;
}

