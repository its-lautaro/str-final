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

uint8_t n = 51;
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

  A = (uint8_t*)malloc(1 * sizeof(uint8_t));
  B = (uint8_t*)malloc(1 * sizeof(uint8_t));
  C = (uint8_t*)malloc(1 * sizeof(uint8_t));
  while (1) {

    int max = 0;
    // Calcular tamaño maximo de matriz
    for (int i = 2; i < 100; i++) {
      free(A);
      free(B);
      free(C);
      A = (uint8_t*)malloc(i * i * sizeof(uint8_t));
      B = (uint8_t*)malloc(i * i * sizeof(uint8_t));
      C = (uint8_t*)malloc(i * i * sizeof(uint8_t));
      // Check if there was enough memory
      if (A == NULL || B == NULL || C == NULL) {
        max = i - 1;
        UART_PrintStr("Tamaño máximo: ");
        UART_PrintNumber(max);
        UART_PrintStr("x");
        UART_PrintNumber(max);
        UART_PrintStr("\n");
        free(A);
        free(B);
        free(C);
        break;
      }
    }
    // Calcular tiempo de ejecución
    A = (uint8_t*)malloc(max * max * sizeof(uint8_t));
    B = (uint8_t*)malloc(max * max * sizeof(uint8_t));
    C = (uint8_t*)malloc(max * max * sizeof(uint8_t));

    // Initialize matrices
    for (int i = 0; i < n * n; i++) {
      A[i] = 1;
      B[i] = 1;
      C[i] = 0;
    }

    timer1_start(prescaler);
    multiplicar();
    uint32_t time = timer1_stop();

    UART_PrintStr("tiempo transcurrido: ");
    UART_PrintNumber(time);
    UART_PrintStr(" us\n");

    validar();

    for (int i = 0; i < n * n; i++) {
      C[i] = 0;
    }
    _delay_ms(1000);
  }
  return 0;
}