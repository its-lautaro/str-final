/*
*
* Este programa incrementa gradualmente el tamaño de la matriz hasta encontrar un error
* en la alocación de memoria o en el resultado de la matriz
*
* Una vez encontrado el tamaño maximo mide el tiempo de ejecución
*/
#include <Arduino.h>

uint8_t n = 51;
uint8_t* A, * B, * C;

// blink builtin led
void error() {
  while (1) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(20);
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
  }
}
// perform matrix product
void multiplicar() {
  for (uint8_t i = 0; i < n; i++) {
    for (uint8_t j = 0; j < n; j++) {
      for (uint8_t k = 0; k < n; k++) {
        C[i * n + j] += A[i * n + k] * B[j * n + k];
      }
    }
  }
}
// Validates operation result
void validar() {
  for (uint16_t i = 0; i < n * n; i++) {
    if (C[i] != n) {
      Serial.println("Error: fallo en la validacion\n");
      error(); //bloqueante
    }
  }
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);

  A = (uint8_t*)malloc(n * n * sizeof(uint8_t));
  B = (uint8_t*)malloc(n * n * sizeof(uint8_t));
  C = (uint8_t*)malloc(n * n * sizeof(uint8_t));

  if (A == NULL || B == NULL || C == NULL) {
    error();
  }
}

void loop() {
  // Initialize matrices
  for (int i = 0; i < n * n; i++) {
    A[i] = 1;
    B[i] = 1;
    C[i] = 0;
  }

  Serial.println("Calculando..");
  uint32_t start = millis();
  multiplicar();
  uint32_t time = millis() - start;

  Serial.print("tiempo transcurrido: ");
  Serial.print(time);
  Serial.println(" ms");

  validar();

  delay(1000);
}