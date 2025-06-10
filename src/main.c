#include <avr/io.h>
#include "util/delay.h"
#include "usart.h"
#include "i2c.h"
#include "mpu6050.h"
#include "gpio.h"
#include "timer.h"
#include <avr/interrupt.h>

int main(void)
{
  gpio_init();
  usart_init();
  i2c_init();
  timer1_init_ctc();
  mpu6050_init();
  sei();

  while (1)
  {
    if (flag_10ms != 0)
    {
      flag_10ms = 0;
      MPU6050_Values v = mpu6050_read();

      // usart_send_string("AX = ");
      // usart_send_float(v.a_x, 3);
      // usart_send_string(", AY = ");
      // usart_send_float(v.a_y, 3);
      // usart_send_string(", AZ = ");
      // usart_send_float(v.a_z, 3);

      usart_send_string("Roll_accel = ");
      usart_send_float(v.roll_accel, 3);
      usart_send_string(",   Pitch_accel = ");
      usart_send_float(v.pitch_accel, 3);

      // usart_send_string(",  GX = ");
      // usart_send_float(v.g_x, 3);
      // usart_send_string(", GY = ");
      // usart_send_float(v.g_y, 3);
      // usart_send_string(", GZ = ");
      // usart_send_float(v.g_z, 3);

      // usart_send_string("Roll = ");
      // usart_send_float(v.roll_gyro, 3);
      // usart_send_string(",   Pitch = ");
      // usart_send_float(v.pitch_gyro, 3);

      usart_send_string("\r\n");
    }
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
