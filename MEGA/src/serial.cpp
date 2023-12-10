/*
*
* Transmisor-receptor usando UART0
* Para hacer interfaz con la consola de la pc
*
*/
#include "serial.h"

void SERIAL_Begin(long baud) {
    // Set baud rate
    long ubrr = (F_CPU/(16*baud))-1;
    UBRR0H = (uint8_t) ubrr>>8;
    UBRR0L = (uint8_t) ubrr;

    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); //8 bit data format
    UCSR0B = (1 << TXEN0) | (1 << RXEN0); //enable tx rx
}

void UART0_SendByte(uint8_t n) {
    //wait for empty buffer
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = n;
}

uint8_t UART0_ReceiveByte() {
    //wait for data to be received
    while (!(UCSR0A & (1 << RXC0)));
    return UDR0;
}

// The following functions are meant to interface with a text based terminal and serves the purpose of testing the above functions
void SERIAL_Print(const char* str) {
     while (*str) {
         UART0_SendByte(*str);
         str++;
     }
}

void SERIAL_Print(const uint32_t n) {
    char buffer[11]; // Maximum length of uint32_t in decimal is 10 digits, plus null terminator
    snprintf(buffer, sizeof(buffer), "%lu ", n);
    SERIAL_Print(buffer);
}