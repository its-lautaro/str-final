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
        prescaler = (1 << CS12) | (1 << CS10);fac = 64;  // 1024 Cada tick son 64us cuenta hasta 4.19424 seg
        break;
    }

    // Normal mode
    TCCR1A = 0x00;
    TCNT1 = 0;
    // Iniciar el timer
    TCCR1B = prescaler;
}

void timer1_stop(){
    TCCR1B=0;
}

// Función para obtener el tiempo transcurrido
uint32_t timer1_getCount() {
    uint32_t time = TCNT1;
    time *= fac;
    return time;
}