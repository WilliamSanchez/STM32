#ifndef _BMP180_BAR_H_
#define _BMP180_BAR_H_

#include <string.h>
#include <stdio.h>
#include "config_i2c.h"

#include "config_usart.h"

#define BMP180_Addr     0xEE
#define out_xlsb        0xF8
#define out_lsb         0xF7
#define out_msb         0xF6
#define ctrl_meas       0xF4        
#define soft_reset      0xE0
#define id              0xD0 

#define Temperature     0x2E
#define Pressure_oss1   0x34
#define Pressure_oss2   0x74
#define Pressure_oss3   0xB4
#define Pressure_oss4   0xF4

/*          0xF4
[ oss <7:6> | sco <5> | measurement control <4:0>]
<5> start converison
<7:6> control oversampling
    00 = single
    01 = 2 times
    10 = 4 times
    11 = 8 times
<4:0> Controls measurement

0x2E = Temperature
0x34 = Pressure(oss = 1)  
0x74 = Pressure(oss = 2)
0xB4 = Pressure(oss = 3)
0xF4 = Pressure(oss = 4)
*/


#define dec16(msb,lsb)( ( msb << 8 ) | lsb)
#define dec24(msb,lsb, xsb)( ( msb << 16 ) | ( lsb << 8 ) | xsb)

/*
                BMP180 
    preassure data : 16 to 19 bit
    Temperture data : 16 bit

    Init
    1-> Read calibrartion data [AC1, AC2, AC3, AC4, AC5, AC6, B1, B2, MB, MC, MD] 

    Loop:

    2-> read uncompensated temperature value UT
    3-> wait 4.5ms
    4-> Read reg [0xF6|0xF7].
    5-> Read uncompensated pressure value UP
    6-> Calculate true temperature
        x1 = (UT-AC6)*AC5/2pow(15)
        x2 = (MC*2pow(11))/(X1+MD)
        B5 = X1+x2
    7-> calculate true pressure 
        B6 = B5-4000
        x1 = (B2*(B6*B6/(2pow(12)))/2pow(11))
        x2 = AC2*B6/2pow(11)
        x3 = x1+x2
        B3 = (((AC1*4+x3)<<oss)+2)/4
        x1 = AC3*B6/2pow(13)
        x2 = (B1*(B6*(B6/2pow(12))))/2pow(16)
        x3 = ((x1+x2)+2)/2pow(2)
        B4 = (AC4*(unsigned long)(x3+32768)/2pow(15))
        B7 = ((unsigned long)UP-B3)*(50000 >> oss)
            if(B7 < 0x80000000) {p=(B7*2)/B4}
            else  {p=(B7/B4)*2}
        x1 = (p/2pow(8))*(p/2pow(8))
        x1 = (x1*3038)/2pow(16)
        x2 = (-7357*p)/2pow(16)
        p = p+(x1+x2+3791)/2pow(4)
    8-> Calculating abosulte altitude (Po =1013.25hPa )
        alt = 44330*(1-(P/Po)pow(1/5.255))
    9-> Display Temperature, Pression and altitude.   




*/


int8_t bmp180_init(void);
long BMP180_temperature();
long BMP180_Preassure();
long altitude(const long pres);

#endif