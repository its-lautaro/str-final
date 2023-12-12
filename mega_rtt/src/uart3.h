#ifndef UART1_H_
#define UART1_H_

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>

void UART3_Init(long baud);
void UART3_SendByte(uint8_t n);
uint8_t UART3_ReceiveByte();

#endif //UART1_H_