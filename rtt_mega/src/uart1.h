#ifndef UART1_H_
#define UART1_H_

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>

void UART1_Init(long baud);
void UART1_SendByte(uint8_t n);
uint8_t UART1_ReceiveByte();
void UART1_ReceiveBlock(uint8_t* blk,uint8_t numbers);
void UART1_SendBlock(uint8_t* blk,uint8_t numbers);

#endif //UART1_H_