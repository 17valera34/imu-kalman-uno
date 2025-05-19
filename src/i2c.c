#include "i2c.h"
#include "avr/io.h"
#define TWI_START_TRANSMITTED 0x08
#define TWI_REP_START_TRANSMITTED 0x10
#define TWI_MT_SLA_ACK 0x18
#define TWI_MR_SLA_ACK 0x40
#define TWI_MT_DATA_ACK 0x28

void i2c_init(void)
{
    TWSR = 0x00;
    TWBR = 72;
    TWCR = (1 << TWEN);
}

uint8_t i2c_start(uint8_t address)
{
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

    while (!(TWCR & (1 << TWINT)))
    {
    }

    uint8_t status = TWSR & 0xF8;
    if ((status != TWI_START_TRANSMITTED) && (status != TWI_REP_START_TRANSMITTED))
    {
        return 1;
    }

    TWDR = address;
    TWCR = (1 << TWINT) | (1 << TWEN);

    while (!(TWCR & (1 << TWINT)))
    {
    }

    status = TWSR & 0xF8;
    if ((status != TWI_MT_SLA_ACK) && (status != TWI_MR_SLA_ACK))
    {
        return 2;
    }

    return 0;
}

void i2c_stop(void)
{
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

uint8_t i2c_write(uint8_t data)
{
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);

    while (!(TWCR & (1 << TWINT)))
    {
    }

    uint8_t status = TWSR & 0xF8;
    if (status != TWI_MT_DATA_ACK)
    {
        return 1;
    }

    return 0;
}

uint8_t i2c_read_ack(void)
{
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);

    while (!(TWCR & (1 << TWINT)))
    {
    }

    return TWDR;
}

uint8_t i2c_read_nack(void)
{
    TWCR = (1 << TWINT) | (1 << TWEN);

    while (!(TWCR & (1 << TWINT)))
    {
    }

    return TWDR;
}

uint8_t i2c_read_bytes(uint8_t address, uint8_t reg, uint8_t *buffer, uint8_t len)
{
    if(i2c_start((0x68 << 1) | 0))
    {
        return 1;
    }

    if(i2c_write(0x3B))
    {
        return 2;
    }

    if(i2c_start((0x68 << 1) | 1))
    {
        return 3;
    }


    for (uint8_t i = 0; i < len; i++)
    {
        if (i < (len - 1))
        {
            buffer[i] = i2c_read_ack();
        }
        else
        {
            buffer[i] = i2c_read_nack();
        }
    }

    i2c_stop();
    return 0;
}