#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>  
#include <sys/time.h>

void setup() {
  Serial.begin(9600); //Inicializa el puerto para comunicacion serial
  pinMode(13,OUTPUT);
}


// Multiplicación de matrices por bloques
/*void matmulblks(int *a, int *b, int *c, int n, int bs){
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
*/

void loop() {

  int i, j, n=185;//Valor máximo 181
  int* A = (int *) malloc(n*n*sizeof(int));
  int* B = (int *) malloc(n*n*sizeof(int));
  int* C = (int *) malloc(n*n*sizeof(int));
  //Inicializacion
  for (i = 0; i < n; i++) {
    for (j = 0; j < n; j++) {
      A[i * n + j] = 1;
      B[j * n + i] = 1;
      C[i * n + j] = 0;
    }
  }
  if (A == NULL || B == NULL || C == NULL) Serial.println("Se rompió todo padre");

  Serial.println("Las matrices fueron iniciadas correctamente.");
  Serial.print("Multiplicando matrices de ");
  Serial.print(n);
  Serial.print("x ");
  Serial.print(n);

  //long long int timeStart=micros();//64 bits long long int
  //long long int timeStop=micros();

  long timeStart=millis();
  delay(1000);
  long timeStop=millis();

  Serial.print("El tiempo de cálculo es de ");
  Serial.print(timeStart-timeStop);
  Serial.print(" us");




	free(A);
	free(B);
	free(C);
  digitalWrite(13,HIGH);
  delay(1000);
  digitalWrite(13,LOW);
}


