#include "handler.h"

#define MATRIX_SIZE 4
#define BLOCK_SIZE 2

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

// Función para mostrar una matriz por el puerto serial
void printMatrix(int M[MATRIX_SIZE][MATRIX_SIZE]) {
  Serial.println("Matriz:");
  for (int i = 0; i < MATRIX_SIZE; i++) {
    for (int j = 0; j < MATRIX_SIZE; j++) {
      Serial.print(M[i][j]);
      Serial.print(" ");
    }
    Serial.println();
  }
}

void loopHandler(){
  // Recorrer cada elemento de la matriz C
  for (int i = 0; i < MATRIX_SIZE; i++) {
    for (int j = 0; j < MATRIX_SIZE; j++) {
      // Calcular el índice del bloque correspondiente
      int bi = i / BLOCK_SIZE;
      int bj = j / BLOCK_SIZE;
      // Enviar el bloque de A y el bloque de B al slave
      //sendBlock(A, bi, bj); 
      //sendBlock(B, bi, bj);
      // Recibir el resultado del slave
      //C[i][j] = receiveResult();
    }
  }

  Serial.println("La Matriz Resultado C ha sido procesado de forma exitosa!!");
  //printMatrix(C); //muestra la matriz resultado por el puerto serial

  //detiene el programa para que termine el procesamiento
  while(true);
}