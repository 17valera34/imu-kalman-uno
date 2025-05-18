#include <avr/io.h>
#include "util/delay.h"
#include "usart.h"
#define RX_BUF_SIZE 64
volatile uint8_t msCounter = 0;

int main(void)
{
  DDRB |= (1 << DDB5);
  PORTB |= (1 << PORTB5);


  while (1)
  {
    /* code */
  }
  
}