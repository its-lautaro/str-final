/*
*
* Transmisor-receptor usando USART0
* Modo normal-speed Asynchronus 1 Mbps
* Polling. Datasheet capitulo 22.
*
*/
#include "uart.h"

void UART_Init() {
    // Set baud rate to 1M
    UBRR0L = 0;

    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); //8 bit data format
    UCSR0B = (1 << TXEN0) | (1 << RXEN0); //enable tx rx

}

void UART_SendByte(uint8_t n) {
    //wait for empty buffer
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = n;
}

void UART_PrintStr(const char* str) {
    while (*str) {
        UART_SendByte(*str);
        str++;
    }
}

void UART_PrintNumber(uint32_t n){
    char buffer[11]; // Maximum length of uint32_t in decimal is 10 digits, plus null terminator
    snprintf(buffer, sizeof(buffer), "%lu", n);
    UART_PrintStr(buffer);
}

uint8_t UART_ReceiveByte() {
    //wait for empty buffer
    while (!(UCSR0A & (1 << RXC0)));
    return UDR0;
}