#ifndef __USART_H
#define __USART_H

#include "avr/io.h"

void usart_init(void);
void usart_send_char(uint8_t data);
void usart_send_string(const char *str);
void usart_send_hex(uint8_t val);
void usart_send_int(int16_t value);
void usart_send_float(float value, uint8_t precision);

#endif

