#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"

int main() {
  UART_Init();
  uint8_t* A, * B, * C;
  uint8_t numbers;

  while (1) {

    numbers = UART_ReceiveByte();
    A = (uint8_t*)malloc(sizeof(uint8_t) * numbers);
    B = (uint8_t*)malloc(sizeof(uint8_t) * numbers);
    C = (uint8_t*)malloc(sizeof(uint8_t) * numbers);

    UART_ReceiveBlock(A, numbers);
    UART_ReceiveBlock(B, numbers);
    UART_ReceiveBlock(C, numbers);

    UART_PrintStr("\nA:");
    UART_SendBlock(A,numbers);
    UART_PrintStr("\nB:");
    UART_SendBlock(B,numbers);
    UART_PrintStr("\nC:");
    UART_SendBlock(C,numbers);

    free(A);
    free(B);
    free(C);
  }

  return 0;
}