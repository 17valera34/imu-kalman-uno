#include "mpu6050.h"
#include "i2c.h"
#include "gpio.h"
#include "usart.h"
#include <math.h>

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
    value->a_x = (float)data->accel_x / ACCEL_SCALE - 0.00f;
    value->a_y = (float)data->accel_y / ACCEL_SCALE + 0.003f;
    value->a_z = (float)data->accel_z / ACCEL_SCALE - 0.030f;
    value->temp = (float)data->temp / 340.0f + 36.53f;
    value->g_x = (float)data->gyro_x / GYRO_SCALE + 6.150f;
    value->g_y = (float)data->gyro_y / GYRO_SCALE;
    value->g_z = (float)data->gyro_z / GYRO_SCALE - 0.700;
}

void mpu6050_compute__angles(MPU6050_Values *v)
{
    const float epsilon = 1e-6f;

    float denom_roll = sqrtf(v->a_x * v->a_x + v->a_z * v->a_z);
    if (denom_roll < epsilon)
    {
        denom_roll = epsilon;
    }

    v->roll_accel = atan2f(v->a_y, denom_roll) * (180.f / M_PI);

    float denom_pitch = sqrtf(v->a_y * v->a_y + v->a_z * v->a_z);
    if (denom_pitch < epsilon)
    {
        denom_pitch = epsilon;
    }

    v->pitch_accel = atan2f(-v->a_x, denom_pitch) * (180.f / M_PI);
}

void mpu6050_integrate_gyro(MPU6050_Values *v, float dt)
{
    v->roll_gyro += v->g_x * dt;
    v->pitch_gyro += v->g_y * dt;
}

MPU6050_Values mpu6050_read(void)
{
    static MPU6050_Values values = {0};
    static uint8_t initialised = 0;
    MPU6050_Data d;

    mpu6050_read_all(&d);
    mpu6050_convert_data(&d, &values);
    mpu6050_compute__angles(&values);

    const float dt = 0.01f;

    if (!initialised)
    {
        values.roll_gyro = values.roll_accel;
        values.pitch_gyro = values.pitch_accel;
        initialised = 1;
    }

    mpu6050_integrate_gyro(&values, dt);
    return values;
}