#include <avr/io.h>
#include <util/delay.h>
#include "uart3.h"
int main() {
  UART3_Init(250000L);
  char incomingByte;
  while (1) {
    // Espera mensaje
    do{
      incomingByte = UART3_ReceiveByte();
    }while(incomingByte!='a');
    // Enviar respuesta
    UART3_SendByte('a');
  }
  return 0;
}