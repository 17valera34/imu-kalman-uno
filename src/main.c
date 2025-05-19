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
  MPU6050_Data d;
  sei();
  mpu6050_init();

  while (1)
  {
    mpu6050_read_all();
    mpu6050_get_data(&d);

    usart_send_string("AX = ");
    usart_send_int(d.accel_x);
    usart_send_string(", AY = ");
    usart_send_int(d.accel_y);
    usart_send_string(", AZ = ");
    usart_send_int(d.accel_z);
    usart_send_string("\r\n");

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
