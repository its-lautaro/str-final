#ifndef TIMER_H_
#define TIMER_H_

#include <avr/io.h>
#include <avr/interrupt.h>

void timer1_init();
uint32_t timer1_getCount();

#endif //TIMER_H_