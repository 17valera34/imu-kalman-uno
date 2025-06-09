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

  while (1)
  {
    MPU6050_Values v = mpu6050_read();

    usart_send_string("AX = ");
    usart_send_float(v.a_x, 3);
    usart_send_string(", AY = ");
    usart_send_float(v.a_y, 3);
    usart_send_string(", AZ = ");
    usart_send_float(v.a_z, 3);
   
    usart_send_string(",  GX = ");
    usart_send_float(v.g_x, 3);
    usart_send_string(", GY = ");
    usart_send_float(v.g_y, 3);
    usart_send_string(", GZ = ");
    usart_send_float(v.g_z, 3);

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
