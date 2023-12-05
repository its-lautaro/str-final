#ifndef UART_H_
#define UART_H_

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>

void UART_Init(long baud);
void UART_SendByte(uint8_t n);
void UART_PrintStr(const char* str);
void UART_PrintNumber(const uint32_t n);
uint8_t UART_ReceiveByte();
void UART_ReceiveBlock(uint8_t* blk,uint8_t numbers);
void UART_SendBlock(uint8_t* blk,uint8_t numbers);

#endif //UART_H_