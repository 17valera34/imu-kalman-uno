#include "timer.h"
#include <avr/interrupt.h>

volatile uint8_t flag_10ms = 0;

void timer1_init_ctc(void)
{
    TCCR1A = 0;
    TCCR1B = (1 << WGM12);
    TCCR1B |= (1 << CS11) | (1 << CS10);
    OCR1A = 2499;
    TIMSK1 |= (1 << OCIE1A);
}

ISR(TIMER1_COMPA_vect)
{
    flag_10ms = 1;
}