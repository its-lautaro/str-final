#include "handler.h"

#define MATRIX_SIZE 4
#define BLOCK_SIZE 2

void iniziatile(){

}

void receiveResult(){
    while (Serial1.available()){

    }
}

//funcion para enviar un bloque de una matriz al slave
void sendBlock(int M[MATRIX_SIZE][MATRIX_SIZE], int i, int j){
    //Calcular el indice del primer elemento del bloque 
    int i0 = i * BLOCK_SIZE;
    int j0 = j * BLOCK_SIZE;

    //Recorre solo el bloque de matriz que debe enviar
    for (int i = i0; i < i0 + BLOCK_SIZE; i++){
        for (int j = j0; j < j0 + BLOCK_SIZE; j++){
            //Envio el elemento al slave - MEGA
            Serial1.write(M[i][j]);

            //Espero un tiempo para que el slave lo reciba
            //ESTA SENTENCIA ES BLOQUEANTE, ES CRITICO PARA NUESTRO SISTEMA? 
            delay(10);
        }
    }
}

void loopHandler(){

}