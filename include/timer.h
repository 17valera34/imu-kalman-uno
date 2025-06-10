#ifndef __TIMER_H
#define __TIMER_H

#include "avr/io.h"
extern volatile uint8_t flag_10ms;

void timer1_init_ctc(void);

#endif