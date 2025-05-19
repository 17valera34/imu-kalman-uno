#include "usart.h"

#define BAUD 9600
#define BAUD_RATE (F_CPU / (16UL * BAUD) - 1)

void usart_init(void)
{
    UBRR0H = (uint8_t)(BAUD_RATE >> 8);
    UBRR0L = (uint8_t)BAUD_RATE;

    UCSR0B |= (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0); // Enable TX, RX and RX complete interrupt
    UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);               // 8-bit data, 1 stop bit, no parity
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
