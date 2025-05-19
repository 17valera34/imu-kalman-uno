#include <avr/io.h>
#include "util/delay.h"
#include "usart.h"
#include "i2c.h"
#include <avr/interrupt.h>

int main(void)
{
  DDRB |= (1 << DDB5);
  usart_init();
  i2c_init();
  sei();

  i2c_start((0x68 << 1) | 0);
  i2c_write(0x75);
  i2c_start((0x68 << 1) | 1);
  uint8_t who_am_i = i2c_read_nack();
  i2c_stop();

  usart_send_string("WHO_AM_I: 0x");
  usart_send_hex(who_am_i);
  usart_send_string("\r\n");

  i2c_start((0x68 << 1) | 0);
  i2c_write(0x75);
  i2c_start((0x68 << 1) | 1);
  who_am_i = i2c_read_nack();
  i2c_stop();

  usart_send_string("WHO_AM_I: 0x");
  usart_send_hex(who_am_i);
  usart_send_string("\r\n");

  while (1)
  {
  }
}

// if (result == 0)
// {
//   while (1)
//   {
//     PORTB ^= (1 << DDB5);
//     _delay_ms(100);
//   }
// }
// else if (result == 1)
// {

//   while (1)
//   {
//     PORTB ^= (1 << DDB5);
//     _delay_ms(800);
//   }
// }
// else if (result == 2)
// {
//   PORTB |= (1 << DDB5);
//   i2c_stop();
//   while (1)
//   {
//   }
// }
