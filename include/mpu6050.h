#ifndef __MPU6050_H
#define __MPU6050_H

#include "avr/io.h"
#define MPU6050_ADDR 0x68
#define ACCEL_SCALE 16384.0f
#define GYRO_SCALE 131.0f

typedef struct
{
    uint16_t accel_x, accel_y, accel_z;
    uint16_t temp;
    uint16_t gyro_x, gyro_y, gyro_z;
}MPU6050_Data;


void mpu6050_init(void);
uint8_t mpu6050_write_reg(uint8_t address, uint8_t reg, uint8_t data);
uint8_t mpu6050_read_reg(uint8_t address, uint8_t reg, uint8_t *data);
void mpu6050_read_all(void);
void mpu6050_get_data(MPU6050_Data *d);

#endif