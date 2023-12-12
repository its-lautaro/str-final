/*
* Para simplificar la sincronización, el esclavo recibe unicamente al inicio tantas filas de bloques A y columnas de bloques B como puede almacenar
* Calcula la matriz y envía el resultado al maestro
* Supongo que read es bloqueante
*/
#include <Arduino.h>

// Bytes reservados para control de flujo de la transmisión
#define _WAIT_ 255 // Byte reservado para indicar al maestro que debe esperar
#define _RESUME_ 254 // Byte reservado para indicar al maestro que puede continuar enviando información

uint8_t* A; // Fila de bloques A (viene de una matriz ordenada por filas)
uint8_t* B; // Columna de bloques B (viene de una matriz ordenada por columnas)
uint8_t* C; // Matriz resultado
uint8_t n = 0; // Cantidad de bytes a recibir
uint8_t bs = 0; // Tamaño de bloque

// Parpadea el LED integrado indefinidamente
void error() {
  while (1) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(20);
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
  }
}

void multiplicar() {
  for (uint8_t i = 0; i < bs; i++) {
    for (uint8_t j = 0; j < n; j++) {
      for (uint8_t k = 0; k < n; k++) {
        C[i * n + j] += A[i * n + k] * B[j * n + k];
      }
    }
  }
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT); // LED que parpadea para indicar estado de error
  Serial.begin(115200); // Consola serie para debug
  Serial3.begin(115200); // Comunicacion serie con otro micro
  Serial.println("Esperando valores iniciales..");
  Serial3.setTimeout(10000); // Timeout de 3 seg

  // Obtener valores para alocar memoria
  char bytes[2];
  if (Serial3.readBytes(bytes, 2) != 2) {
    Serial.println("Timeout");
    error(); // Bloqueante con timeout
  }
  Serial3.write(_WAIT_); // Indicar al maestro que espere alocación de memoria

  n = bytes[0];
  bs = bytes[1];
  Serial.print("Recibi n y bs: ");
  Serial.print(n);
  Serial.print(", ");
  Serial.println(bs);
  A = (uint8_t*)malloc(n * bs * sizeof(uint8_t)); // Fila de bloques A
  B = (uint8_t*)malloc(n * n * sizeof(uint8_t)); // Matriz B
  C = (uint8_t*)calloc(n * bs, sizeof(uint8_t)); // Fila de bloques C
  if (A == NULL || B == NULL || C == NULL) error();

  Serial3.write(_RESUME_);
  if (Serial3.readBytes(A, n * bs) != n * bs) {
    Serial.println("Timeout recibiendo A");
    error(); // Bloqueante con timeout
  }
  if (Serial3.readBytes(B, n * n) != n * n) {
    Serial.println("Timeout recibiendo B");
    error(); // Bloqueante con timeout
  }
  
  Serial.println("Recibi las matrices");
  for (int i = 0; i < bs; i++) {
    Serial.print("A: ");
    for (int j = 0; j < n; j++) {
      Serial.print(A[i * n + j]);
      Serial.print(" ");
    }
    Serial.print("\n");
  }
  Serial.println();
  for (int i = 0; i < n; i++) {
    Serial.print("B: ");
    for (int j = 0; j < n; j++) {
      Serial.print(B[i * n + j]);
      Serial.print(" ");
    }
    Serial.print("\n");
  }
}

void loop() {
  multiplicar();
  Serial.println("Se termino el calculo");
  // Esperar que el maestro este listo para recibir
  while (Serial3.read() != _RESUME_) { ; }
  for (int i = 0; i < n * bs; i++) {
    Serial3.write(C[i]);
  }
  Serial.println("Se envio el resultado");
  // Imprimir resultado
  for (int i = 0; i < bs; i++) {
    Serial.print("C: ");
    for (int j = 0; j < n; j++) {
      Serial.print(C[i * n + j]);
      Serial.print(" ");
    }
    Serial.print('\n');
  }
  // Reiniciar matriz resultado
  for (int i = 0; i < n * bs; i++) {
    C[i] = 0;
  }
  // Esperar mensaje del maestro para volver a enviar el resultado
  while (Serial3.read() != _RESUME_) { ; }
}