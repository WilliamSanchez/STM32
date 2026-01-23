#ifndef _OV7670_H_
#define _OV7670_H_

#include <string.h>
#include <stdio.h>
#include "config_i2c.h"
#include "config_usart.h"

#define OV7670_Addr     0x21 //0x43

#define dec16(msb,lsb)( ( msb << 8 ) | lsb)
#define dec24(msb,lsb, xsb)( ( msb << 16 ) | ( lsb << 8 ) | xsb)

int8_t ov7670_init(void);

#endif