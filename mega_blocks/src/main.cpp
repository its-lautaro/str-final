#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"

int main() {
  UART_Init(1000000L);
  uint8_t* A;
  uint8_t numbers;

  while (1) {

    //receive size
    numbers = UART_ReceiveByte();
    A = (uint8_t*)malloc(sizeof(uint8_t) * numbers);

    //receive block
    UART_ReceiveBlock(A, numbers);

    UART_PrintStr("\nEcho: ");
    //send block
    UART_SendBlock(A,numbers);
    UART_PrintStr("\n");
    free(A);
  }

  return 0;
}