/*
*
* Recibir bloques de matrices por usart, realizar una multiplicación y enviar la matriz resultado
*
* Este programa incrementa gradualmente el tamaño de la matriz hasta encontrar un error
* en la alocación de memoria o en el resultado de la matriz
*
*/
#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include "uart.h"

uint8_t size;
uint8_t* A, * B, * C;

// blink builtin led
void error() {
    while (1) {
        PORTB |= (1 << 7);
        _delay_ms(20);
        PORTB &= ~(1 << 7);
        _delay_ms(200);
    }
}
// perform matrix product
void multiplicar() {
    for (uint8_t i = 0; i < size; i++) {
        for (uint8_t j = 0; j < size; j++) {
            for (uint8_t k = 0; k < size; k++) {
                C[i * size + j] += A[i * size + k] * B[j * size + k];
            }
        }
    }
}
// Validates operation result
void validar() {
    for (uint16_t i = 0; i < size * size; i++) {
        if (C[i] != size) {
            error(); //bloqueante
        }
    }
}

int main() {
    // LED integrado como salida
    DDRB |= (1 << 7);
    // Setear baudrate en 1M (maximo)
    UART_Init(1000000L);
    //Recibir tamaño de fila (en una matriz de 32x32 recibe 32)
    size = UART_ReceiveByte();
    
    // Repite: Esperar bloque, multiplicar, enviar resultado
    while (1) {
        // Alocar espacio para matrices productos (las que recibe)
        A = (uint8_t*)malloc(size * size * sizeof(uint8_t));
        B = (uint8_t*)malloc(size * size * sizeof(uint8_t));
        // Alocar e inicializar (en 0) matriz resultado
        C = (uint8_t*)calloc(size * size, sizeof(uint8_t));

        // Chequear error de alocacion
        if (A == NULL || B == NULL || C == NULL) error();

        //Recibe 32x32 enteros, dos veces
        UART_ReceiveBlock(A, size * size);
        UART_ReceiveBlock(B, size * size);

        // Realiza la multiplicacion
        multiplicar();

        // Envia el resultado
        UART_SendBlock(C, size * size);

        free(A);
        free(B);
        free(C);
    }
    return 0;
}