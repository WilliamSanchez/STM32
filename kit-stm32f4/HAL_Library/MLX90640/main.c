#include "stm32f4xx_hal.h"

#include <string.h>
#include <stdio.h>

#include "MLX90640_I2C_Driver.h"
#include "MLX90640_API.h"
#include "config_usart.h"
//#include "config_i2c.h"




/*
        GY-87 10DOF

  MPU6050:  A 6-axis gyroscope
  HMC5883L: A 3-axis digital magnetometer or compass
  BMP180:   barometer.

  PB6 ======> SCL
  PB7 ======> SDA

  PA9  ======> Tx
  PA10  =====> Rx

  PA0   => Switch key0
  PC13  => Led  

  BMP180    Addr = 0xEF (11101111: Read), 0xEE(11101110 : Write)
  HMC5883L  Addr = 0x3D (00111101 : Read), 0x3C (00111100 : Write)
  MPU6050   Addr = 0x68 

*/

char debug[2024];

int main(void)
{

  HAL_Init();

  /* Configure the system clock to 100 MHz */
  SystemClock_Config();

  USART_config();
  MLX90640_I2CInit();
 
  /* Configure GPIOC LED13 */
  BSP_LED_Init();  
  /*
  strcpy(debug,"\tIR sensor MLX90640 initializing\n\r");
  usart_send((uint8_t*)debug,strlen(debug));
  if (MLX90640_init() < 0)
  {
      memset(debug,0x00,255);
      strcpy(debug,"Error init IR Sensor\n\t");
      usart_send((uint8_t*)debug,strlen(debug));
  }
  
*/
  HAL_Delay(1000);
  uint8_t slaveAddr = 0x33;
  uint16_t frameData[768];

  while (1)
  {
      HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
      MLX90640_GetFrameData(slaveAddr, frameData);
      for(int i=0; i < 32; i++){
        for(int j=0; j < 24; j++){
          sprintf(debug,"%x|",frameData[24*i+j]);
          usart_send((uint8_t*)debug,strlen(debug));
        }
        strcpy(debug,"\n\r");
        usart_send((uint8_t*)debug,strlen(debug));
      }
      strcpy(debug,"\n\r====================================\n\r");
      usart_send((uint8_t*)debug,strlen(debug));
      memset(debug,0x00,2024);
      memset(debug,0x0000,768);
      HAL_Delay(1000);
  }
}

