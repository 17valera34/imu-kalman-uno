#include "gpio.h"

void gpio_init()
{
    DDRB |= (1 << DDB5);
}