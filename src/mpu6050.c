#include "mpu6050.h"
#include "i2c.h"
#include "gpio.h"
#include "usart.h"

static uint8_t data[14];

void mpu6050_init(void)
{
    mpu6050_write_reg(MPU6050_ADDR, 0x6B, 0x01);
    mpu6050_write_reg(MPU6050_ADDR, 0x1A, 0x03); // DLPF ~44Hz
    mpu6050_write_reg(MPU6050_ADDR, 0x1B, 0);    // ±250°/s
    mpu6050_write_reg(MPU6050_ADDR, 0x1C, 0);    //  ±2g
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

void mpu6050_read_all(void)
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

void mpu6050_get_data(MPU6050_Data *d)
{
    d->accel_x = (int16_t)(data[0] << 8 | data[1]);
    d->accel_y = (int16_t)(data[2] << 8 | data[3]);
    d->accel_z = (int16_t)(data[4] << 8 | data[5]);
    d->temp = (int16_t)(data[6] << 8 | data[7]);
    d->gyro_x = (int16_t)(data[8] << 8 | data[9]);
    d->gyro_y = (int16_t)(data[10] << 8 | data[11]);
    d->gyro_z = (int16_t)(data[12] << 8 | data[13]);
}

void mpu6050_convert_data(const MPU6050_Data *data, MPU6050_Values *value)
{
    value->a_x = data->accel_x / 16384.0f;
    value->a_y = data->accel_y / 16384.0f;
    value->a_z = data->accel_z / 16384.0f;
    value->temp = data->temp / 340.0f + 36.53f;
    value->g_x = data->gyro_x / 131.0f;
    value->g_y = data->gyro_y / 131.0f;
    value->g_z = data->gyro_z / 131.0f;
}

MPU6050_Values mpu6050_read(void)
{
    MPU6050_Values values;
    MPU6050_Data d;

    mpu6050_read_all();
    mpu6050_get_data(&d);
    mpu6050_convert_data(&d, &values);
    return values;
}