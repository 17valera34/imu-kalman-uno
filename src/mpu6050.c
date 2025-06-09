#include "mpu6050.h"
#include "i2c.h"
#include "gpio.h"
#include "usart.h"

static uint8_t data[14];

void mpu6050_init(void)
{
    mpu6050_write_reg(MPU6050_ADDR, 0x6B, 0x01);
    mpu6050_write_reg(MPU6050_ADDR, 0x1A, 0x03); // DLPF ~44Hz
    mpu6050_write_reg(MPU6050_ADDR, 0x19, 49);   // SMPLRT_DIV = 49 → 1000 / (1 + 49) = 20 Гц
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

// uint8_t mpu6050_read_reg(uint8_t address, uint8_t reg, uint8_t *data)
// {
//     if (i2c_start((address << 1) | 0))
//     {
//         return 1;
//     }

//     if (i2c_write(reg))
//     {
//         return 2;
//     }

//     if (i2c_start((address << 1) | 1))
//     {
//         return 3;
//     }

//     *data = i2c_read_nack();

//     i2c_stop();

//     return 0;
// }

void mpu6050_read_all(MPU6050_Data *d)
{
    i2c_read_bytes(MPU6050_ADDR, 0x3B, data, 14);

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
    value->a_x = (data->accel_x / ACCEL_SCALE) - 0.150f;
    value->a_y = (data->accel_y / ACCEL_SCALE) + 0.003f;
    value->a_z = (data->accel_z / ACCEL_SCALE)  - 0.030f;
    value->temp = data->temp / 340.0f + 36.53f;
    value->g_x = (data->gyro_x / GYRO_SCALE) + 6.150f;
    value->g_y = (data->gyro_y / GYRO_SCALE) ;
    value->g_z = (data->gyro_z / GYRO_SCALE) - 0.700;
}

MPU6050_Values mpu6050_read(void)
{
    MPU6050_Values values;
    MPU6050_Data d;

    mpu6050_read_all(&d);
    mpu6050_convert_data(&d, &values);
    return values;
}