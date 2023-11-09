/*
* El tamaño máximo de memoria disponible para matrices en ram es
8 Kb

Se utilizaran matrices de enteros de 8bits de tamaño 52x52 (2704 elementos)
*/
#include <avr/io.h>
#include <stdlib.h>

uint16_t const n = 52;
uint8_t A[n*n];
uint8_t B[n*n];
uint8_t C[n*n];

int main() {
  DDRB |= (1 << 7); //led
  for (uint16_t i = 0; i < n*n; i++) {
    A[i] = i % 9;
    B[i] = i % 9;
    C[i] = i % 9;
  }

  while (1) {
    for (uint16_t i = 0; i < n*n; i++) {
      C[i] += A[i] * B[i];
    }
  }
  return 0;
}