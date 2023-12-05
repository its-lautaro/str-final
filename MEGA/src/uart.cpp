/*
*
* Transmisor-receptor usando USART0
* Normal mode. Async.
* Polling.
* Datasheet capitulo 22.
*
*/
#include "uart.h"

void UART_Init(long baud) {
    // Set baud rate
    long ubrr = (F_CPU/(16*baud))-1;
    UBRR0H = (uint8_t) ubrr>>8;
    UBRR0L = (uint8_t) ubrr;

    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); //8 bit data format
    UCSR0B = (1 << TXEN0) | (1 << RXEN0); //enable tx rx
}

void UART_SendByte(uint8_t n) {
    //wait for empty buffer
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = n;
}

uint8_t UART_ReceiveByte() {
    //wait for data to be received
    while (!(UCSR0A & (1 << RXC0)));
    return UDR0;
}

// The following functions are meant to interface with a text based terminal and serves the purpose of testing the above functions

void UART_PrintStr(const char* str) {
    while (*str) {
        UART_SendByte(*str);
        str++;
    }
}

void UART_PrintNumber(const uint32_t n) {
    char buffer[11]; // Maximum length of uint32_t in decimal is 10 digits, plus null terminator
    snprintf(buffer, sizeof(buffer), "%lu ", n);
    UART_PrintStr(buffer);
}

void UART_SendBlock(uint8_t* blk, uint8_t numbers) {
    for (int i = 0; i < numbers; i++) {
        UART_SendByte(blk[i]);
    }
}

void UART_ReceiveBlock(uint8_t* blk, uint8_t numbers) {
    for (int i = 0; i < numbers; i++) {
        blk[i] = UART_ReceiveByte();
    }
}