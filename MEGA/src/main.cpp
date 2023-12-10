#include <avr/io.h>
#include <util/delay.h>
#include "uart1.h"
#include "serial.h"

uint8_t* A; // Fila de bloques A (viene de una matriz ordenada por filas)
uint8_t* B; // Columna de bloques B (viene de una matriz ordenada por columnas)
uint8_t* C; // Matriz resultado
uint8_t n = 0; // Cantidad de bytes a recibir
uint8_t bs = 0; // Tamaño de bloque

// blink builtin led
void error() {
  while (1) {
    PORTB |= (1 << 7);
    _delay_ms(20);
    PORTB &= ~(1 << 7);
    _delay_ms(200);
  }
}

void multiplicar() {
  int i, j, k;
  for (i = 0; i < bs; i++) {
    for (j = 0; j < bs; j++) {
      for (k = 0; k < n; k++) {
        C[i * n + j] += A[i * n + k] * B[j * n + k];
      }
    }
  }
}

int main() {
  DDRB |= (1 << 7); // Led debug
  DDRB |= (1 << 6); // Job Req pin 12
  UART1_Init(1000000L);
  SERIAL_Begin(9600);

  n = UART1_ReceiveByte();
  SERIAL_Print("Recibi n ");
  SERIAL_Print(n);
  bs = UART1_ReceiveByte();
  SERIAL_Print("Recibi bs ");
  SERIAL_Print(bs);
  // Alocar espacio
  A = (uint8_t*)malloc(n * bs * sizeof(uint8_t));
  B = (uint8_t*)malloc(n * bs * sizeof(uint8_t));
  C = (uint8_t*)calloc(bs * bs, sizeof(uint8_t)); // iniciarlizar en 0;
  if (A == NULL || B == NULL || C == NULL) error();

  UART1_ReceiveBlock(A, n * bs);
  UART1_ReceiveBlock(B, n * bs);

  while (1) {
    // Recibir columnas de bloques B y calcular bloque resultado
    for (int i = 0; i < n / bs; i++) {
      // Limpiar matriz resultado
      for (int i = 0; i < bs * bs; i++) {
        C[i] = 0;
      }

      multiplicar();
      // Avisar calculo completo
      PORTB &= ~(1 << 6);
      // Enviar resultado
      UART1_SendBlock(C, bs * bs);
      UART1_ReceiveBlock(B, n * bs);
    }
    // Recibir fila de bloques A
    //SERIAL_Print("Esperando recibir bloque de A\n");
    UART1_ReceiveBlock(A, n * bs);
  }
  return 0;
}