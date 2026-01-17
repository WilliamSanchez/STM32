#include "mpu6050_gyro_accel.h"

#include <string.h>
#include <stdio.h>
#include "config_i2c.h"

#include "config_usart.h"

static char debug_i2c[255];
float accel_x, accel_y, accel_z =0.0;
float gyro_x, gyro_y, gyro_z =0.0;
signed short c_gyro_x, c_gyro_y, c_gyro_z =0.0;

int8_t mpu6050_init(void)
{
    static char reg = WHO_AM_I;
    static char who_am_i =0x00;
    static char data[2];
    i2c_received(MPU6050_Addr,(uint8_t*) &reg, (uint8_t*)&who_am_i, 1);
    if (who_am_i != 0x68)
        return -1;

    data[0] = 0x1B; data[1] = 0x18;
    if(i2c_send(MPU6050_Addr, (uint8_t*)data, 2))
    {
        strcpy(debug_i2c,"Error config Temperature\n\r");
        usart_send((uint8_t*)debug_i2c,strlen(debug_i2c));
        return -1;
    }

    data[0] = 0x1C; data[1] = 0x00;
    if(i2c_send(MPU6050_Addr, (uint8_t*)data, 2))
    {
        strcpy(debug_i2c,"Error config Temperature\n\r");
        usart_send((uint8_t*)debug_i2c,strlen(debug_i2c));
        return -1;
    }

    data[0] = 0x37; data[1] = 0x02;
    if(i2c_send(MPU6050_Addr, (uint8_t*)data, 2))
    {
        strcpy(debug_i2c,"Error config Temperature\n\r");
        usart_send((uint8_t*)debug_i2c,strlen(debug_i2c));
        return -1;
    }

    data[0] = 0x6A; data[1] = 0x00;
    if(i2c_send(MPU6050_Addr, (uint8_t*)data, 2))
    {
        strcpy(debug_i2c,"Error config Temperature\n\r");
        usart_send((uint8_t*)debug_i2c,strlen(debug_i2c));
        return -1;
    }

    /*  Register for disable sleep mode, necessary to compass 0x00*/
    data[0] = 0x6B; data[1] = 0x00; //data[1] = 0x01;
    if(i2c_send(MPU6050_Addr, (uint8_t*)data, 2))
    {
        strcpy(debug_i2c,"Error config Temperature\n\r");
        usart_send((uint8_t*)debug_i2c,strlen(debug_i2c));
        return -1;
    }
     
    return 0;
}

int8_t mpu6050_gyro_calibrate()
{
    static uint8_t read_reg = GYRO_XOUT_MSB;
    static signed short t_gyro_x, t_gyro_y, t_gyro_z =0.0;
    static uint8_t UG[6];
    static uint8_t s = 0;

    while(s < numSamplesGyro)
    {
        if (i2c_received(MPU6050_Addr, &read_reg, UG, 6) < 0)
        {
            strcpy(debug_i2c,"Error read MPU6050 gyro calibrate\n\r");
            usart_send((uint8_t*)debug_i2c,strlen(debug_i2c));
            return -1;
        }    
        t_gyro_x += (signed short)dec16(UG[0],UG[1]);
        t_gyro_y += (signed short)dec16(UG[2],UG[3]);    
        t_gyro_z += (signed short)dec16(UG[4],UG[5]);
        HAL_Delay(100);
        s++;
    }

    c_gyro_x = t_gyro_x/numSamplesGyro;
    c_gyro_y = t_gyro_y/numSamplesGyro;
    c_gyro_z = t_gyro_z/numSamplesGyro;

    sprintf(debug_i2c,"Gyro calibrate OK\n\rCGx: %02f, CGy: %02f, CGz: %02f\n\r",c_gyro_x, c_gyro_y, c_gyro_z);
    usart_send((uint8_t*)debug_i2c,strlen(debug_i2c));
    return 0;
}

int MPU6050_data()
{
    static uint8_t read_reg = ACCEL_XOUT_MSB;
    static uint8_t UA[14];
    if (i2c_received(MPU6050_Addr, &read_reg, UA, 14) < 0)
    {
        strcpy(debug_i2c,"Error read MPU6050 Accelerometer\n\r");
        usart_send((uint8_t*)debug_i2c,strlen(debug_i2c));
        return -1;
    }
    
    accel_x = ((signed short)dec16(UA[0],UA[1])*.000061f*9.80665f);
    accel_y = ((signed short)dec16(UA[2],UA[3])*.000061f*9.80665f);    
    accel_z = ((signed short)dec16(UA[4],UA[5])*.000061f*9.80665f);

    sprintf(debug_i2c,"Ax: %02f, Ay: %02f Az: %02f\n\r",accel_x, accel_y, accel_z);
    usart_send((uint8_t*)debug_i2c,strlen(debug_i2c));
    
    gyro_x = (((signed short)dec16(UA[8],UA[9])-c_gyro_x)*.060975f);
    gyro_y = (((signed short)dec16(UA[10],UA[11])-c_gyro_y)*.060975f);    
    gyro_z = (((signed short)dec16(UA[12],UA[13])-c_gyro_z)*.060975f);

    sprintf(debug_i2c,"Gx: %02f, Gy: %02f Gz: %02f\n\r",gyro_x, gyro_y, gyro_z);
    usart_send((uint8_t*)debug_i2c,strlen(debug_i2c));
    
    sprintf(debug_i2c,"T: %02f\n\r",(signed short)(dec16(UA[6],UA[7]))/340+36.53);
    usart_send((uint8_t*)debug_i2c,strlen(debug_i2c));
    return 0;
}
