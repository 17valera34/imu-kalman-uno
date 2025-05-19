#ifndef __MPU6050_H
#define __MPU6050_H

#include "avr/io.h"
#define MPU6050_ADDR 0x68


void mpu6050_init(void);
uint8_t mpu6050_write_reg(uint8_t address, uint8_t reg, uint8_t data);
uint8_t mpu6050_read_reg(uint8_t address, uint8_t reg, uint8_t *data);
void mpu6050_read_bytes(void);

#endif