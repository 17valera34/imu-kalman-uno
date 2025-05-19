#ifndef __I2C_H
#define __I2C_H

#include "avr/io.h"

// Initialize I2C (TWI) hardware, set bitrate, enable TWI
void i2c_init(void);

// Send START condition and address + R/W bit
// Returns 0 on success, 1 on failure (no ACK)
uint8_t i2c_start(uint8_t address);

// Send STOP condition to release the bus
void i2c_stop(void);

// Write one byte to the bus
// Returns 0 on success (ACK received), 1 on failure (NACK)
uint8_t i2c_write(uint8_t data);

// Read one byte and send ACK to slave (expect more bytes)
uint8_t i2c_read_ack(void);

// Read one byte and send NACK to slave (last byte read)
uint8_t i2c_read_nack(void);


uint8_t i2c_read_bytes(uint8_t address, uint8_t reg, uint8_t *buffer, uint8_t len);
#endif