#ifndef _MOU6050_GYRO_ACCEL_H_ 
#define _MOU6050_GYRO_ACCEL_H_

#include "stm32f4xx_hal.h"

#define dec16(msb,lsb)( ( msb << 8 ) | lsb)

#define MPU6050_Addr            (0x68<<1)

#define ACCEL_XOUT_MSB          0x3B
#define ACCEL_XOUT_LSB          0x3C
#define ACCEL_YOUT_MSB          0x3D
#define ACCEL_YOUT_LSB          0x3E
#define ACCEL_ZOUT_MSB          0x3F
#define ACCEL_ZOUT_LSB          0x40
#define TEMP_OUT_MSB            0x41
#define TEMP_OUT_LSB            0x42
#define GYRO_XOUT_MSB           0x43
#define GYRO_XOUT_LSB           0x44
#define GYRO_YOUT_MSB           0x45
#define GYRO_YOUT_LSB           0x46
#define GYRO_ZOUT_MSB           0x47
#define GYRO_ZOUT_LSB           0x48

#define WHO_AM_I                0x75

#define numSamplesGyro          50




int8_t mpu6050_init(void);
int8_t mpu6050_gyro_calibrate(void);
int MPU6050_data();

#endif