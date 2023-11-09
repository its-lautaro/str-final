/*
*
* Transmisor-receptor usando USART0
* Modo normal-speed Asynchronus 1 Mbps
* Polling. Datasheet chapter 22.
*
*/
#include "uart.h"

void UART_Init() {
    // Set baud rate to 1M
    UBRR0L = 0;

    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); //8 bit data format
    UCSR0B = (1 << TXEN0) | (1 << RXEN0); //enable tx rx

}

void UART_SendInt8(uint8_t n) {
    //wait for empty buffer
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = n;
}

void UART_SendStr(const char* str) {
    while (*str) {
        UART_SendInt8(*str);
        str++;
    }
}

uint8_t UART_ReceiveInt8() {
    while (!(UCSR0A & (1 << RXC0)));
    return UDR0;
}