/*
*
* Transmisor-receptor usando UART1
* Normal mode. Async.
* Polling.
* Datasheet capitulo 22.
*
*/
#include "uart1.h"

void UART1_Init(long baud) {
    // Set baud rate
    long ubrr = (F_CPU/(16*baud))-1;
    UBRR1H = (uint8_t) ubrr>>8;
    UBRR1L = (uint8_t) ubrr;

    UCSR1C = (1 << UCSZ01) | (1 << UCSZ00); //8 bit data format
    UCSR1B = (1 << TXEN1) | (1 << RXEN1); //enable tx rx
}

void UART1_SendByte(uint8_t n) {
    //wait for empty buffer
    while (!(UCSR1A & (1 << UDRE1)));
    UDR1 = n;
}

uint8_t UART1_ReceiveByte() {
    //wait for data to be received
    while (!(UCSR1A & (1 << RXC1)));
    return UDR1;
}

void UART1_SendBlock(uint8_t* blk, uint8_t numbers) {
    for (int i = 0; i < numbers; i++) {
        UART1_SendByte(blk[i]);
    }
}

void UART1_ReceiveBlock(uint8_t* blk, uint8_t numbers) {
    for (int i = 0; i < numbers; i++) {
        blk[i] = UART1_ReceiveByte();
    }
}

// The following functions are meant to interface with a text based terminal and serves the purpose of testing the above functions

// void UART1_PrintStr(const char* str) {
//     while (*str) {
//         UART1_SendByte(*str);
//         str++;
//     }
// }

// void UART1_PrintNumber(const uint32_t n) {
//     char buffer[11]; // Maximum length of uint32_t in decimal is 10 digits, plus null terminator
//     snprintf(buffer, sizeof(buffer), "%lu ", n);
//     UART1_PrintStr(buffer);
// }