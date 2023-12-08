/*
*
* A diferencia de la implementación para el Arduino MEGA
* la función malloc no retorna null en caso de fallo de alocación
* por lo que no es posible aumentar el tamaño de forma dinámica.
* 
* Para encontrar el tamaño máximo, el valor de n debe ser aumentado manualmente
* hasta encontrar un valor en el que el programa no emita resultado
* (esto sucede debido a un error de alocación que nunca es anunciado)
*
* Luego de alocar el espacio en memoria realiza la operación e informa el tiempo transcurrido
*/
#include <Arduino.h>

uint8_t n = 176;
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