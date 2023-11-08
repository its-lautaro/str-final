#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>

void error() {
    while (1) {
        PORTB ^= 1 << 7;
        _delay_ms(100);
        PORTB ^= 1 << 7;
        _delay_ms(100);
    }
}

void validar(int* matriz, int res, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (matriz[i * N + j] != res) {
                error();
            }
        }
    }
}

// Calcula el producto matricial de dos matrices de tamaño nxn, en bloques de tamaño bsxbs
void multBloques(int* A, int* B, int* C, int n, int bs) {
    int* ablk, * bblk, * cblk;
    for (int I = 0; I < n; I += bs) {
        for (int J = 0; J < n; J += bs) {
            cblk = &C[I * n + J];
            for (int K = 0; K < n; K += bs) {
                ablk = &A[I * n + K];
                bblk = &B[J * n + K];
                for (int i = 0; i < bs; i++) {
                    for (int j = 0; j < bs; j++) {
                        for (int k = 0; k < bs; k++) {
                            cblk[i * n + j] += ablk[i * n + k] * bblk[j * n + k];
                        }
                    }
                }
            }
        }
    }
}

int main(void) {
    // LED integrado para comunicacion
    DDRB = 1 << 7;
    // Pin 2 para comunicarse con DUE
    DDRE = 1 << 4;

    int8_t i, j; // 2 bytes
    int8_t n = 36;
    int* A, * B, * C; // 6 bytes
    A = (int*)(malloc(n * n * sizeof(int))); //36x36
    B = (int*)(malloc(n * n * sizeof(int)));
    C = (int*)(malloc(n * n * sizeof(int)));

    while (1) {
        //chequear overflow memoria
        if (A == NULL || B == NULL || C == NULL) error();
        //inicializacion
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
                A[i * n + j] = 1;
                B[j * n + i] = 1;
                C[i * n + j] = 0;
            }
        }
        //Señal para sincronizar
        PORTD = 1<<4;
        _delay_ms(1);
        PORTD &= ~(1<<4);

        multBloques(A, B, C, n, 8);

        //Señal de finalizacion
        PORTD = 1<<4;
        _delay_ms(1);
        PORTD &= ~(1<<4);

        validar(C, n, n);
    }

    return 0;
}