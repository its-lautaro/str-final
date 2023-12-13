/*
* Para simplificar la sincronización, el esclavo recibe unicamente al inicio tantas filas de bloques A y columnas de bloques B como puede almacenar
* Calcula la matriz y envía el resultado al maestro
* Supongo que read es bloqueante
*/
#include <Arduino.h>

// Byte reservado para control de flujo de la transmisión
#define _JOB_REQ_ 255

uint8_t* A; // Fila de bloques A (viene de una matriz ordenada por filas)
uint8_t* B; // Columna de bloques B (viene de una matriz ordenada por columnas)
uint8_t* C; // Matriz resultado
uint8_t n = 0; // Cantidad de bytes a recibir
uint8_t bs = 0; // Tamaño de bloque
uint8_t col_procesadas = 0; // Lleva la cuenta de las columnas procesadas para saber cuando tiene que pedir una fila de bloques A

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
    for (uint8_t j = 0; j < bs; j++) {
      for (uint8_t k = 0; k < n; k++) {
        C[i * bs + j] += A[i * n + k] * B[j * n + k];
      }
    }
  }
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT); // LED que parpadea para indicar estado de error
  Serial.begin(115200UL); // Consola serie para debug
  Serial3.begin(250000UL); // Comunicacion serie con otro micro
  Serial3.setTimeout(10000); // Timeout de 10 seg
  Serial.println("Esperando valores iniciales..");
  // Obtener valores para alocar memoria
  char bytes[2];
  if (Serial3.readBytes(bytes, 2) != 2) {
    Serial.println("Timeout");
    error(); // Bloqueante con timeout
  }

  n = bytes[0];
  bs = bytes[1];
  // Serial.print("Recibi n y bs: ");
  // Serial.print(n);
  // Serial.print(", ");
  // Serial.println(bs);
  A = (uint8_t*)malloc(n * bs * sizeof(uint8_t)); // Fila de bloques A
  B = (uint8_t*)malloc(n * bs * sizeof(uint8_t)); // Matriz B
  C = (uint8_t*)calloc(bs * bs, sizeof(uint8_t)); // Fila de bloques C
  if (A == NULL || B == NULL || C == NULL) error();
  // Pedir trabajo
  Serial3.write(_JOB_REQ_);
}

void loop() {
  // Si debe comenzar a procesar una fila nueva
  if (col_procesadas == 0) {
    // Recibir filas A
    if (Serial3.readBytes(A, n * bs) != n * bs) {
      Serial.println("Timeout recibiendo A");
      error();
    }
  }
  // Recibir columnas B
  if (Serial3.readBytes(B, n * bs) != n * bs) {
    Serial.println("Timeout recibiendo B");
    error();
  }
  // Imprimir matrices recibidas
  // if (col_procesadas == 0) {
  //   // Imprimir filas A
  //   Serial.println("Recibi las matrices");
  //   for (int i = 0; i < bs; i++) {
  //     Serial.print("A: ");
  //     for (int j = 0; j < n; j++) {
  //       Serial.print(A[i * n + j]);
  //       Serial.print(" ");
  //     }
  //     Serial.print("\n");
  //   }
  //   Serial.println();
  // }
  // // Imprimir columnas B
  // for (int i = 0; i < bs; i++) {
  //   Serial.print("B: ");
  //   for (int j = 0; j < n; j++) {
  //     Serial.print(B[i * n + j]);
  //     Serial.print(" ");
  //   }
  //   Serial.print("\n");
  // }
  multiplicar();
  col_procesadas++;
  if(col_procesadas==(n/bs)) col_procesadas=0;
  // Imprimir matriz resultado
  // Serial.println("Calculo finalizado");
  // for (int i = 0; i < bs; i++) {
  //   for (int j = 0; j < bs; j++) {
  //     Serial.print(C[i * bs + j]);
  //     Serial.print(" ");
  //   }
  //   Serial.print("\n");
  // }
  // Avisar calculo finalizado
  Serial3.write(_JOB_REQ_);
  // Esperar a que el maestro este listo para recibir
  while (Serial3.read() != _JOB_REQ_) { ; }
  // Enviar bloque resultado
  for (int i = 0; i < bs; i++) {
    for (int j = 0; j < bs; j++) {
      Serial3.write(C[i * bs + j]);
    }
  }
  // Reiniciar matriz resultado
  for (int i = 0; i < bs; i++) {
    for (int j = 0; j < bs; j++) {
      C[i * bs + j] = 0;
    }
  }
  // Esperar a que el maestro este listo para seguir enviando trabajo
  while (Serial3.read() != _JOB_REQ_) { ; }
}