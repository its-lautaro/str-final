/*
*
* El tamaño máximo de memoria disponible para matrices en ram es
* 8 Kb
*
*/
#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include <uart.h>
#include <timer.h>
#define F_CPU 16000000

uint8_t const n = 4;
uint8_t* A, * B, * C;

// blink builtin led
void error() {
  while (1) {
    PORTB |= (1 << 7);
    _delay_ms(20);
    PORTB &= ~(1 << 7);
    _delay_ms(200);
  }
}

// perform matrix product
void multiplicar() {
  for (uint8_t i = 0; i < n; i++) {
    for (uint8_t j = 0; j < n; j++) {
      for (uint8_t k = 0; k < n; k++) {
        C[i * n + j] += A[i * n + k] * B[j * n + k];
      }
    }
  }
}

// Validates operation result
void validar() {
  for (uint16_t i = 0; i < n * n; i++) {
    if (C[i] != n) {
      error(); //bloqueante
    }
  }
}

int main() {
  DDRB |= (1<<7); // Led
  UART_Init();

  A=(uint8_t*)malloc(n*n*sizeof(uint8_t));
  B=(uint8_t*)malloc(n*n*sizeof(uint8_t));
  C=(uint8_t*)malloc(n*n*sizeof(uint8_t));

  // Check if there was enough memory
  if (A == NULL || B==NULL || C == NULL){
    error();
  }

  // Initialize matrices
  for (int i = 0; i < n*n; i++)
  {
    A[i]=1;
    B[i]=1;
    C[i]=0;
  }
  timer1_init();
  
  multiplicar();
  uint32_t time = timer1_getCount();
  
  validar();
  
  // Print elapsed time
  UART_PrintStr("Tiempo transcurrido: ");
  UART_PrintNumber(time);
  UART_PrintStr(" us\n");
  
  while (1) {
  }

  return 0;
}