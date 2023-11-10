/*
*
* El tamaño máximo de memoria disponible para matrices en ram es
* 8 Kb
*
* Este programa incrementa gradualmente el tamaño de la matriz hasta encontrar un error
* en la alocación de memoria o en el resultado de la matriz
*
*/
#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include <uart.h>
#include <timer.h>

uint8_t n = 2;
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
      UART_PrintStr("Error: fallo en la validacion\n");
      error(); //bloqueante
    }
  }
}

int main() {
  DDRB |= (1 << 7); // Led
  UART_Init();
  uint16_t prescaler = 256;

  while (1) {

    A = (uint8_t*)malloc(n * n * sizeof(uint8_t));
    B = (uint8_t*)malloc(n * n * sizeof(uint8_t));
    C = (uint8_t*)malloc(n * n * sizeof(uint8_t));

    // Check if there was enough memory
    if (A == NULL || B == NULL || C == NULL) {
      UART_PrintStr("Error: memoria insuficiente\n");
      error();
    }

    // Initialize matrices
    for (int i = 0; i < n * n; i++) {
      A[i] = 1;
      B[i] = 1;
      C[i] = 0;
    }

    timer1_start(prescaler);
    multiplicar();
    timer1_stop();

    uint32_t time = timer1_getCount();

    // Check if there was overflow
    // if (TIFR0 & (1 << TOV0)) time = time + ((uint32_t)65536);
    // TIFR0 &= ~(1 << TOV0);

    // Print elapsed time
    UART_PrintStr("Producto matriz ");
    UART_PrintNumber(n);
    UART_PrintStr("x");
    UART_PrintNumber(n);
    UART_PrintStr(", tiempo transcurrido: ");
    UART_PrintNumber(time);
    UART_PrintStr(" us\n");

    validar();

    n++;
    free(A);
    free(B);
    free(C);
  }

  return 0;
}