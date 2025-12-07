#ifndef _BMP180_BAR_H_
#define _BMP180_BAR_H_

#include "config_i2c.h"

/*
                BMP180 
    preassure data : 16 to 19 bit
    Temperture data : 16 bit

    Init
    1-> Read calibrartion data [AC1, AC2, ] 

    Loop
    2-> read uncompensated temperature value
    3-> wait 4.5ms
    4-> Read reg [0xF6|0xF7].
    5-> Read uncompensated pressure value


*/


int8_t bmp180_init(void);

#endif