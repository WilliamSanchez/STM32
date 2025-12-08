#include "bmp180_bar.h"
#include <math.h>

static float Po                 = 1013.25;
static float alt_coefficient    = 1/5.255;


short AC1,AC2,AC3;
unsigned short AC4,AC5,AC6;
short B1, B2;
short MB, MC, MD;
long B5 = 0;

char debug_i2c[255];

int8_t bmp180_init(void)
{
    static char addr = 0xEE;
    static char reg = 0xD0;
    static char who_am_i =0x00;
    i2c_received(addr,(uint8_t*) &reg, (uint8_t*)&who_am_i, 1);
    if (who_am_i != 0x55)
        return -1;

    reg = 0xAA;
    uint8_t calibration_coefficient[22];
    
    if (i2c_received(addr,(uint8_t*) &reg, calibration_coefficient, 22) < 0)
        return -1;

    AC1 = dec16(calibration_coefficient[0], calibration_coefficient[1]); 
    AC2 = dec16(calibration_coefficient[2], calibration_coefficient[3]); 
    AC3 = dec16(calibration_coefficient[4], calibration_coefficient[5]); 
    AC4 = dec16(calibration_coefficient[6], calibration_coefficient[7]); 
    AC5 = dec16(calibration_coefficient[8], calibration_coefficient[9]); 
    AC6 = dec16(calibration_coefficient[10], calibration_coefficient[11]); 
    B1 = dec16(calibration_coefficient[12], calibration_coefficient[13]); 
    B2 = dec16(calibration_coefficient[14], calibration_coefficient[15]); 
    MB = dec16(calibration_coefficient[16], calibration_coefficient[17]); 
    MC = dec16(calibration_coefficient[18], calibration_coefficient[19]); 
    MD = dec16(calibration_coefficient[20], calibration_coefficient[21]);

    sprintf(debug_i2c,"AC : %d|%d|%d|%d|%d|%d\n\rB : %d|%d\n\rMB : %d\n\rMC : %d\n\rMD %d\n\r", AC1, AC2, AC3, AC4, AC5, AC6, B1, B2, MB, MC, MD);
    usart_send((uint8_t*)debug_i2c,strlen(debug_i2c));

    return 0;
}

long BMP180_temperature()
{
    static uint8_t read_reg = out_msb;
    static uint8_t UT[2];
    static long BMP180_T, Temp = 0;
    static long x1, x2 = 0;
    static char data[2] = {ctrl_meas, Temperature};
    if(i2c_send(BMP180_Addr, (uint8_t*)data, 2))
    {
        strcpy(debug_i2c,"Error config Temperature\n\r");
        usart_send((uint8_t*)debug_i2c,strlen(debug_i2c));
        return -1;
    }
       
    HAL_Delay(10);

    if (i2c_received(BMP180_Addr,(uint8_t*) &read_reg, UT, 2) < 0)
    {
        strcpy(debug_i2c,"Error read Temperature\n\r");
        usart_send((uint8_t*)debug_i2c,strlen(debug_i2c));
        return -1;
    }

    BMP180_T = dec16(UT[0],UT[1]);
    sprintf(debug_i2c,"UT : %x%x|%ld\n\r", UT[0], UT[1], BMP180_T);
    usart_send((uint8_t*)debug_i2c,strlen(debug_i2c));

    x1 = ((BMP180_T - AC6)*AC5)>>15; ///pow(2,15);
    x2 = (MC<<11)/(x1+MD);
    B5 = x1 + x2;

    Temp = (B5+8)>>4;

    sprintf(debug_i2c,"Temperature : %ld x1 : %ld, x2: %ld, B5 : %ld\n\r", Temp,x1,x2,B5);
    usart_send((uint8_t*)debug_i2c,strlen(debug_i2c));

    return Temp;
}

long BMP180_Preassure()
{
    uint8_t oss = 0;
    static uint8_t read_reg = out_msb;
    static uint8_t UP[3];
    static long BMP180_P, Pres = 0;
    static char data[2] = {ctrl_meas, Pressure_oss1};
    static long B6, xp1, xp2, xp3, B3 = 0;
    static unsigned long B4, B7 = 0;

    if(i2c_send(BMP180_Addr, (uint8_t*)data, 2))
    {
        strcpy(debug_i2c,"Error config TPreassure\n\r");
        usart_send((uint8_t*)debug_i2c,strlen(debug_i2c));
        return -1;
    }
        
    HAL_Delay(10);

    if (i2c_received(BMP180_Addr,(uint8_t*) &read_reg, UP, 3) < 0)
    {
        strcpy(debug_i2c,"Error read Temperature\n\r");
        usart_send((uint8_t*)debug_i2c,strlen(debug_i2c));
        return -1;
    }

    BMP180_P = (dec24(UP[0],UP[1],UP[2]) >> (8-oss));
    sprintf(debug_i2c,"UP : %x%x%x|%ld\n\r", UP[0], UP[1], UP[2], BMP180_P);
    usart_send((uint8_t*)debug_i2c,strlen(debug_i2c));

    B6 = B5 - 4000;
    xp1 = ((B2*B6*(B6>>12))>>11);
    xp2 = ((AC2*B6)>>11);
    xp3 = xp1 + xp2;
    B3 = ((((AC1*4)+xp3)<<oss)+2)/4;
    xp1 = ((AC3*B6)>>13);
    xp2 = ((B1*B6*(B6>>12))>>16);
    xp3 = ((xp1+xp2)+2)>>2;
    B4 = (AC4*(unsigned long)(xp3+32768))>>15;
    B7 = ((unsigned long)BMP180_P-B3)*(50000>>oss);
    if(B7 < 0x80000000)
        Pres = (B7*2)/B4;
    else
        Pres = (B7/B4)*2; 

    xp1 = (Pres>>8)*(Pres>>8);
    xp1 = (xp1*3038)>>16;
    xp2 = (-7357*Pres)>>16;
    Pres += ((xp1+xp2+3791)>>2);

    sprintf(debug_i2c,"preassure: %ld x1 : %ld, x2: %ld\n\r", Pres,xp1,xp2);
    usart_send((uint8_t*)debug_i2c,strlen(debug_i2c));

    return Pres;
}

long altitude()
{
    long alt = 0;
    float P = (float)(BMP180_Preassure()/100);
    alt = 44330*(1 - pow((P/Po),alt_coefficient));
    sprintf(debug_i2c,"Altitude: %ld m\n\r", alt);
    usart_send((uint8_t*)debug_i2c,strlen(debug_i2c));
    return alt;

}