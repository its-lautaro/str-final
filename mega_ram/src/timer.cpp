/*
* Contador con TIMER1
* Normal mode
* Sin interrupciones. Datasheet capitulo 17.
*/
#include "timer.h"
uint16_t fac;

void timer1_start(uint16_t prescaler) {
    switch (prescaler)
    {
    case 8:
        prescaler = (1 << CS11);fac = 500;
        break;
    case 64:
        prescaler = (1 << CS11) | (1 << CS10);fac = 4;
        break;
    case 256:
        prescaler = (1 << CS12);fac = 16;
        break;    
    case 1024:
        prescaler = (1 << CS12) | (1 << CS10);fac = 64;
        break;
    }

    // Normal mode
    TCCR1A = 0x00;
    TCNT1 = 0;
    // Iniciar el timer
    TCCR1B = prescaler;
}

uint32_t timer1_stop(){
    TCCR1B &= ~(1<<CS10) & ~(1<<CS11) & ~(1<<CS12);
    uint32_t time = TCNT1;
    time *= fac;
    return time;
}