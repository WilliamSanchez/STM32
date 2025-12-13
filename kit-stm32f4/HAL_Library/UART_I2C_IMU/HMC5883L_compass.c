#include "HMC5883L_compass.h"

#include <string.h>
#include <stdio.h>
#include "config_i2c.h"

#include "config_usart.h"

static char debug[255];
int16_t com_x, com_y, com_z = 0;
uint8_t read_reg = 0x03;
uint8_t UC[6];

int8_t HMC5883L_init(void)
{

     char reg[2];
     reg[0] = 0x00; reg[1] = 0x70; 
     if (i2c_send(HMC5883L_Addr, (uint8_t *)reg, 2))
        return -1;
         
     reg[0] = 0x01; reg[1] = 0x20;   
     if (i2c_send(HMC5883L_Addr, (uint8_t *)reg, 2))
        return -1;

     reg[0] = 0x02; reg[1] = 0x00;
     if (i2c_send(HMC5883L_Addr, (uint8_t *)reg, 2))
        return -1; 
     
    return 0;
}

int HMC5883L_data()
{

      if (i2c_received(HMC5883L_Addr, &read_reg, UC, 6) < 0)
      {
          strcpy(debug,"Error read HMC5883L Compass\n\r");
          usart_send((uint8_t*)debug,strlen(debug));
          return -1;
      }

      com_x = (signed short)dec16(UC[0],UC[1]);
      com_y = (signed short)dec16(UC[2],UC[3]);    
      com_z = (signed short)dec16(UC[4],UC[5]);
      sprintf(debug,"Hx: %d, Hy: %d Hz: %d\n\r",com_x, com_y, com_z);
      usart_send((uint8_t*)debug,strlen(debug));

    return 0;
}
