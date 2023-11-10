#ifndef TIMER_H_
#define TIMER_H_

#include <avr/io.h>
#include <avr/interrupt.h>

/*
* Permite seleccionar prescaler entre 8, 64, 256 y 1024
* A medida que se aumenta el prescaler se reduce la resolucion pero se aumenta el rango
* 8: hasta 32.7675 ms con resolucion 500 ns
* 64: hasta 65.535 ms con resolucion 4 us
* 256: hasta 1.048 s con resolucion 16 us
* 1024: hasta 4.19424 s con resolucion 64 us
*/
void timer1_start(uint16_t);
void timer1_stop();
uint32_t timer1_getCount();

#endif //TIMER_H_