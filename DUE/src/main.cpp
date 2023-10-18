#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>  
#include <sys/time.h>

void setup() {
  Serial.begin(9600); //Inicializa el puerto para comunicacion serial
}

//Funcion que calcula el tiempo de ejecución
double dwalltime(){
	double sec;
	struct timeval tv;

	gettimeofday(&tv,NULL);
	sec = tv.tv_sec + tv.tv_usec/1000000.0;
	return sec;
}

// Multiplicación de matrices por bloques
void matmulblks(int *a, int *b, int *c, int n, int bs){
int *ablk, *bblk, *cblk;
int I, J, K;    
int i, j, k; 
 
  for(I = 0; I < n; I += bs){
    for(J = 0; J < n; J += bs){
      cblk = &c[I*n + J];
      for(K = 0; K < n; K += bs){
        ablk = &a[I*n + K];
        bblk = &b[J*n + K];
        for (i = 0; i < bs; i++){
          for (j = 0; j < bs; j++){
            for  (k = 0; k < bs; k++){
            cblk[i*n + j] += ablk[i*n + k] * bblk[j*n + k];
            }
          }
			  }
		  }
    }
  }
}

//el n y el tamaño de bloque bs lo debemos setear nosotros segun el tamaño maximo de matriz a procesar
int i, j, bs=4 ,n=88; 
int* A = (int *) malloc(n*n*sizeof(int));
int* B = (int *) malloc(n*n*sizeof(int));
int* C = (int *) malloc(n*n*sizeof(int));
int c=10;
char msj[20];
double timetick;

void loop() {
  //Inicializacion
	for (i =0; i < n; i++){
		for (j = 0; j < n; j++){
			A[i*n + j] = i;
			B[j*n + i] = i;
			C[i*n + j] = i;

      if (A[i*n + j] != i){
        Serial.println("Error en A");
      }
      
      if (B[j*n + i] != i){
        Serial.println("Error en B");
      }
      
      if (C[i*n + j] != i){
        Serial.println("Error en C");
      }
		}
	}
  Serial.println("Las matrices fueron iniciadas correctamente.");
  Serial.print("Multiplicando matrices de ");
  Serial.print(n);
  Serial.print("x ");
  Serial.print(n);
  Serial.print("en bloques de ");
  Serial.print(bs);
  Serial.print("x ");
  Serial.println(bs);

  timetick = dwalltime();
  
  matmulblks(A, B, C, n, bs);
  
  double totalTime = dwalltime() - timetick;

  // Validando
  for (i = 0; i < n; i++){
    for (j = 0; j < n; j++){
      if (C[i*n + j] != n){
        Serial.print("Error en ");
        Serial.print(i, j);
        Serial.print("valor: ");
        Serial.println(C[i*n + j]);
      }
    }
  }

	Serial.print("Tiempo en segundos ");
  Serial.println(totalTime);

	free(A);
	free(B);
	free(C);
}




