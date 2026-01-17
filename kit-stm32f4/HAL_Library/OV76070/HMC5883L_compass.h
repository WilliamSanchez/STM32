#ifndef _HMC5883L_COMPASS_H_ 
#define _HMC5883L_COMPASS_H_

#include "stm32f4xx_hal.h"

#define dec16(msb,lsb)( ( msb << 8 ) | lsb)

#define HMC5883L_Addr            (0x1E<<1)
#define PHI                       (3.1416)




int8_t HMC5883L_init(void);
int HMC5883L_data();

#endif