#ifndef UART_H_
#define UART_H_

#include <avr/io.h>

void UART_Init();
void UART_SendInt8(uint8_t n);
void UART_SendStr(const char* str);
uint8_t UART_ReceiveInt8();

#endif //UART_H_