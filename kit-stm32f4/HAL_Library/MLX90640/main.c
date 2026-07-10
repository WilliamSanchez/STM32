#include "stm32f4xx_hal.h"

#include <string.h>
#include <stdio.h>

#include "MLX90640_I2C_Driver.h"
#include "MLX90640_API.h"
#include "config_usart.h"

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

#define MLX90640_DEVICEID1  0x2407
paramsMLX90640 _params;
char debug[2024];
uint8_t slaveAddr = 0x33;

int main(void)
{

  HAL_Init();

  /* Configure the system clock to 100 MHz */
  SystemClock_Config();

  USART_config();
  MLX90640_I2CInit();
 
  /* Configure GPIOC LED13 */
  BSP_LED_Init();  

  uint16_t serialNumberID[3];

  
  uint8_t reg[2];
  reg[0] = (MLX90640_DEVICEID1>>8);
  reg[1] = MLX90640_DEVICEID1 & 0x00FF;
 
  if (MLX90640_I2CRead(slaveAddr,(uint16_t)MLX90640_DEVICEID1, 3, serialNumberID) == 1)
  {
      strcpy(debug,"No read data\n\r");
      usart_send((uint8_t*)debug,strlen(debug));
      return -1;
  }
  for(int i=0; i<3; i++){
      sprintf(debug,"%d> %x\n\r",i,serialNumberID[i]);
      usart_send((uint8_t*)debug,strlen(debug));
  }

  uint16_t eeMLX90640[832];
  if (MLX90640_I2CRead(slaveAddr, MLX90640_EEPROM_START_ADDRESS, MLX90640_EEPROM_DUMP_NUM, eeMLX90640) == 1)
  {
      strcpy(debug,"No read data calibration\n\r");
      usart_send((uint8_t*)debug,strlen(debug));
      return -1;
  }
  
  //while (MLX90640_DumpEE(slaveAddr, eeMLX90640) != 0) {
  //    strcpy(debug,"MLX90640_DumpEE Error\n\r");
  //    usart_send((uint8_t*)debug,strlen(debug));
  //    HAL_Delay(30000);
  //}
  //}else{
  //    for(int i=0; i< 832; i++)
  //    {
  //         sprintf(debug,"%d] %x\n\r",i, eeMLX90640[i]);
  //         usart_send((uint8_t*)debug,strlen(debug));
  //    }
    
  if (MLX90640_ExtractParameters(eeMLX90640, &_params) < 0)
  {
    strcpy(debug,"Extract parameters error\n\r");
    usart_send((uint8_t*)debug,strlen(debug));
    return -1;
  }
  //};

  sprintf(debug,"Parameters\n\rVDD: %d\n\rKvPTAT: %.2f\n\r",_params.kVdd,(float)_params.KvPTAT);
  usart_send((uint8_t*)debug,strlen(debug));
/*
  MLX90640_SetChessMode(slaveAddr);
  MLX90640_SetResolution(slaveAddr,0);
  MLX90640_SetRefreshRate(slaveAddr,2);
  sprintf(debug,"Mode %d, resolution: %d RefreshRate: %d\n\r",MLX90640_GetCurMode(slaveAddr), MLX90640_GetCurResolution(slaveAddr),MLX90640_GetRefreshRate(slaveAddr));
  usart_send((uint8_t*)debug,strlen(debug));


  float frameData[768];
  uint16_t mlx90640Frame[834];
  int status;

  float emissivity = 0.95;
  float tr = 23.15;
  float ta = 0.0;
*/
  HAL_Delay(1000);

  while (1)
  {
      HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
      /*
      for(uint8_t page=0; page < 2; page++)
      {
        status = MLX90640_GetFrameData(slaveAddr,mlx90640Frame);
        if(status > 0)
        {
          ta = MLX90640_GetTa(mlx90640Frame, &_params); // Store ambient temp locally
          tr = ta - 8; // For a MLX90640 in the open air the shift is
          MLX90640_CalculateTo(mlx90640Frame, &_params, emissivity, tr, frameData);
          sprintf(debug,"status: %d, ta %.2f, tr: %.2f \n\r",status, ta, tr);
          usart_send((uint8_t*)debug,strlen(debug));
        }  
      }

      for(uint8_t h=0; h<24; h++)
    {
      for(uint8_t w=0; w<32; w++)
      {
          sprintf(debug,"%.2f\t",frameData[h*32+w]);
          usart_send((uint8_t*)debug,strlen(debug));
      }
      strcpy(debug,"\n\r");
      usart_send((uint8_t*)debug,strlen(debug));
    }


      strcpy(debug,"\n\r===============================================\n\r");
      usart_send((uint8_t*)debug,strlen(debug));
      */
      HAL_Delay(1000);
  }
  return 0;
}

