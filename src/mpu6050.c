#include "mpu6050.h"
#include "i2c.h"
#include "gpio.h"

uint8_t data[14];

void mpu6050_init(void)
{
    mpu6050_write_reg(MPU6050_ADDR, 0x6B, 0);
    mpu6050_write_reg(MPU6050_ADDR, 0x1B, 0);
    mpu6050_write_reg(MPU6050_ADDR, 0x1C, 0);
}

uint8_t mpu6050_write_reg(uint8_t address, uint8_t reg, uint8_t data)
{
    if (i2c_start((address << 1) | 0))
    {
        return 1;
    }

    if (i2c_write(reg))
    {
        return 2;
    }

    if (i2c_write(data))
    {
        return 3;
    }

    i2c_stop();

    return 0;
}

uint8_t mpu6050_read_reg(uint8_t address, uint8_t reg, uint8_t *data)
{
    if (i2c_start((address << 1) | 0))
    {
        return 1;
    }

    if (i2c_write(reg))
    {
        return 2;
    }

    if (i2c_start((address << 1) | 1))
    {
        return 3;
    }

    *data = i2c_read_nack();

    i2c_stop();

    return 0;
}

void mpu6050_read_bytes(void)
{
    uint8_t res = i2c_read_bytes(MPU6050_ADDR, 0x3B, data, 14);
    if (res == 0)
    {
        PORTB &= ~(1 << PORTB5);
    }
    else
    {
        PORTB |= (1 << PORTB5);
    }
}