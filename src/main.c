#include <avr/io.h>
#include "util/delay.h"
#include "usart.h"
#include "i2c.h"
#include "mpu6050.h"
#include "gpio.h"
#include <avr/interrupt.h>

int main(void)
{
  gpio_init();
  usart_init();
  i2c_init();
  sei();
  mpu6050_init();

  uint8_t who_am_i;
  uint8_t status = mpu6050_read_reg(0x68, 0x75, &who_am_i);
  if (status == 0)
  {
    usart_send_string("WHO_AM_I = 0x");
    usart_send_hex(who_am_i);
    usart_send_string("\r\n");
  }
  else
  {
    usart_send_string("Ошибка чтения регистра!\r\n");
  }

  while (1)
  {
    //mpu6050_read_bytes();
    _delay_ms(500);
  }
}

// if (result == 0)
// {
//   while (1)
//   {
//     PORTB ^= (1 << PORTB5);
//     _delay_ms(100);
//   }
// }
// else if (result == 1)
// {

//   while (1)
//   {
//     PORTB ^= (1 << PORTB5);
//     _delay_ms(800);
//   }
// }
// else if (result == 2)
// {
//   PORTB |= (1 << PORTB5);
//   i2c_stop();
//   while (1)
//   {
//   }
// }
