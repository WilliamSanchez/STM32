#include "bmp180_bar.h"

int8_t bmp180_init(void)
{
    static char addr = 0xEE;
    static char reg = 0xD0;
    static char who_am_i =0x00;
    i2c_received(addr,(uint8_t*) &reg, (uint8_t*)&who_am_i, 1);
    if (who_am_i != 0x55)
        return -1;

    return 0;
}