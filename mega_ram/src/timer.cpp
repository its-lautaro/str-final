/*
* Contador con TIMER1
* Normal mode
* Sin interrupciones. Datasheet capitulo 17.
*/
#include "timer.h"
uint16_t fac;

void timer1_init() {

    uint8_t prescaler;

    // A medida que se aumenta el prescaler se reduce la resolucion (el paso minimo es cada vez mas grande) pero se aumenta el rango

    //prescaler = (1 << CS11);fac = 500; // 8 Cada tick son 500ns, cuenta hasta 32.7675 ms 
    //prescaler = (1 << CS11) | (1 << CS10);fac = 4; // 64 Cada tick son 4us cuenta hasta 65.535 ms
    prescaler = (1 << CS12);fac = 16; // 256 Cada tick son 16us cuenta hasta 1.048 seg
    //prescaler = (1 << CS12) | (1 << CS10);fac = 64;  // 1024 Cada tick son 64us cuenta hasta 4.19424 seg

    // Normal mode
    TCCR1A = 0x00;
    TCNT1 = 0;
    // Iniciar el timer
    TCCR1B = prescaler;
}

// Función para obtener el tiempo transcurrido
uint32_t timer1_getCount() {
    uint32_t time = TCNT1 * fac;
    return time;
}