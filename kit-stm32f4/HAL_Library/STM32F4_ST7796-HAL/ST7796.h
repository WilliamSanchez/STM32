#ifndef _ST7796_H_
#define _ST7796_H_

#include "stm32f4xx_hal.h"
#include "config_spi.h"

#define Horizontal_line   320 //320
#define Vertical_line     480//480;
#define COL_ADDR_SET      0x2A
#define ROW_ADDR_SET      0X2B
#define MEM_WRT           0X2C

#ifndef dec16
    #define dec16(msb,lsb)( ( msb << 8 ) | lsb)
#endif

#ifndef dec24
    #define dec24(msb,lsb, xsb)( ( msb << 16 ) | ( lsb << 8 ) | xsb)
#endif

int8_t st7796_init();
void config_command();

#endif