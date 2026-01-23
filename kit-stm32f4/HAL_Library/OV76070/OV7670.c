#include "OV7670.h"
#include <math.h>

static char debug_i2c[255];

int8_t ov7670_init(void)
{
    static uint8_t COM7=0x06; //00000110

    if(i2c_send(OV7670_Addr, &COM7, 1))
    {
        strcpy(debug_i2c,"Error config camera\n\r");
        usart_send((uint8_t*)debug_i2c,strlen(debug_i2c));
        return -1;
    }
    return 0;
}