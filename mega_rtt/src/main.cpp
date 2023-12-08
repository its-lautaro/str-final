#include <avr/io.h>
#include <util/delay.h>
#include "uart1.h"

int main() {
  UART1_Init(1000000L);

  while (1) {
    // Espera mensaje
    UART1_ReceiveByte(); // operación bloqueante
    // Enviar respuesta
    UART1_SendByte('a'); // operación bloqueante
  }

  return 0;
}