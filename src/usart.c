#include "usart.h"

#define BAUD 9600
#define BAUD_RATE (F_CPU / (16UL * BAUD) - 1)

void usart_init(void)
{
    UBRR0H = (uint8_t)(BAUD_RATE >> 8);
    UBRR0L = (uint8_t)BAUD_RATE;

    UCSR0B |= (1 << TXEN0) | (1 << RXEN0);   // Enable TX, RX
    UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01); // 8-bit data, 1 stop bit, no parity
}

// Sends a single byte over USART
void usart_send_char(uint8_t data)
{
    while (!(UCSR0A & (1 << UDRE0)))
    {
        /* code */
    }
    UDR0 = data;
}

// Sends a null-terminated string over USART
void usart_send_string(const char *str)
{
    while (*str)
    {
        usart_send_char(*str++);
    }
}

void usart_send_hex(uint8_t val)
{
    const char hex_chars[] = "0123456789ABCDEF";
    usart_send_char(hex_chars[val >> 4]);
    usart_send_char(hex_chars[val & 0x0F]);
}

void usart_send_int(int16_t value)
{
    if (value == 0)
    {
        usart_send_char('0');
        return;
    }

    if (value < 0)
    {
        usart_send_char('-');
        value = -value;
    }

    char digits[6]; // max 5
    uint8_t i = 0;

    while (value > 0)
    {
        digits[i++] = '0' + (value % 10);
        value /= 10;
    }

    while (i--)
    {
        usart_send_char(digits[i]);
    }
}

void usart_send_float(float value, uint8_t precision)
{
    if (value < 0)
    {
        usart_send_char('-');
        value = -value;
    }

    uint32_t int_part = (uint32_t)value;
    float frac_part = value - int_part;

    char buf[11];
    int i = 0;

    if (int_part == 0)
    {
        usart_send_char('0');
    }
    else
    {
        while (int_part > 0 && i < 10)
        {
            buf[i++] = '0' + (int_part % 10);
            int_part /= 10;
        }

        while (i--)
        {
            usart_send_char(buf[i]);
        }
    }
    if (precision > 0)
    {
        usart_send_char('.');
    }

    for (uint8_t i = 0; i < precision; i++)
    {
        frac_part *= 10;
        uint8_t digit = (uint8_t)frac_part;
        usart_send_char('0' + digit);
        frac_part -= digit;
    }
}
