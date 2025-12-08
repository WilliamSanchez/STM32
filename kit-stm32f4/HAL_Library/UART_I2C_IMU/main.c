#include "stm32f4xx_hal.h"

#include <string.h>
#include <stdio.h>


#include "bmp180_bar.h"
#include "mpu6050_gyro_accel.h"
#include "config_usart.h"
#include "config_i2c.h"


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
  

  HAL_Delay(1000);
  strcpy(debug,"\tBarometer BMP180 initializing\n\r");
  usart_send((uint8_t*)debug,strlen(debug));
  if (bmp180_init() < 0)
  {
      memset(debug,0x00,255);
      strcpy(debug,"Error init barometer\n\t");
      usart_send((uint8_t*)debug,strlen(debug));
  }
  
  strcpy(debug,"\tAccelerometer and gyrometer mpu6050 initializing\n\r");
  usart_send((uint8_t*)debug,strlen(debug));
  if (mpu6050_init() < 0)
  {
      memset(debug,0x00,255);
      strcpy(debug,"Error init accel and gyro\n\t");
      usart_send((uint8_t*)debug,strlen(debug));
  }

  if (mpu6050_gyro_calibrate() < 0)
  {
      memset(debug,0x00,255);
      strcpy(debug,"Error calibrate gyro\n\t");
      usart_send((uint8_t*)debug,strlen(debug));
  }

/*
  char addr = (0x68<<1);
  char reg = 0x75;
  char who_am_i = 0x00;
  i2c_received(addr,(uint8_t*) &reg, (uint8_t*)&who_am_i, 1);
  sprintf((char *)aTxBuffer,"MPU6050   %x\n\r",who_am_i);
  usart_send(aTxBuffer, strlen((char*)aTxBuffer));

  addr = (0x1E<<1);
  reg = 0x04;
  who_am_i = 0x00;
  i2c_received(addr,(uint8_t*) &reg, (uint8_t*)&who_am_i, 1);
  sprintf((char *)aTxBuffer,"HMC5883L %x\n\r",who_am_i);
  usart_send(aTxBuffer, strlen((char*)aTxBuffer));
*/
 
  long p_calc = 0;
  while (1)
  {
      HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
      /*
      To calculate pession, is necessary first calculate teperature 
      because the preassure depend of the temperaure.
      */
      BMP180_temperature();
      p_calc = BMP180_Preassure();
      altitude(p_calc);
      MPU6050_data();
      HAL_Delay(1000);
  }
}

/*
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == GPIO_PIN_0)
  {
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET); 
  

  } 
  
}
*/