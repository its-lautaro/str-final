#ifndef UART_H_
#define UART_H_

#include <avr/io.h>
#include <stdio.h>

void UART_Init();
void UART_SendByte(uint8_t n);
void UART_PrintStr(const char* str);
void UART_PrintNumber(uint32_t n);
uint8_t UART_ReceiveByte();

#endif //UART_H_