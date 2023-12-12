/*
*
* Transmisor-receptor usando UART1
* Normal mode. Async.
* Polling.
* Datasheet capitulo 22.
*
*/
#include "uart3.h"

void UART3_Init(long baud) {
    // Set baud rate
    long ubrr = (F_CPU/(16*baud))-1;
    UBRR3H = (uint8_t) ubrr>>8;
    UBRR3L = (uint8_t) ubrr;

    UCSR3C = (1 << UCSZ01) | (1 << UCSZ00); //8 bit data format
    UCSR3B = (1 << TXEN3) | (1 << RXEN3); //enable tx rx
}

void UART3_SendByte(uint8_t n) {
    //wait for empty buffer
    while (!(UCSR3A & (1 << UDRE3)));
    UDR3 = n;
}

uint8_t UART3_ReceiveByte() {
    //wait for data to be received
    while (!(UCSR3A & (1 << RXC3)));
    return UDR3;
}