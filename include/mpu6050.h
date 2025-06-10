#ifndef __MPU6050_H
#define __MPU6050_H

#include "avr/io.h"
#define MPU6050_ADDR 0x68
#define ACCEL_SCALE 16384.0f
#define GYRO_SCALE 131.0f

typedef struct
{
    int16_t accel_x, accel_y, accel_z;
    int16_t temp;
    int16_t gyro_x, gyro_y, gyro_z;
}MPU6050_Data;

typedef struct
{
    float a_x, a_y, a_z;
    float temp;
    float g_x, g_y, g_z;
    float roll, pitch;
    float roll_accel, pitch_accel;
    float roll_gyro, pitch_gyro;
}MPU6050_Values;


void mpu6050_init(void);
uint8_t mpu6050_write_reg(uint8_t address, uint8_t reg, uint8_t data);
//uint8_t mpu6050_read_reg(uint8_t address, uint8_t reg, uint8_t *data);
void mpu6050_read_all(MPU6050_Data *d);
void mpu6050_convert_data(const MPU6050_Data *data, MPU6050_Values *value);
MPU6050_Values mpu6050_read(void);
void mpu6050_compute__angles(MPU6050_Values *v);
void mpu6050_integrate_gyro(MPU6050_Values *v, float dt);

#endif