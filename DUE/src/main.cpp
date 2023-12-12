#include <Arduino.h>

uint8_t n = 8; // Cant filas/columnas
uint8_t bs = 2; // Tamaño de bloque
uint8_t fil_disponible = 0; // Proxima fila a procesar
uint8_t* A, * B, * C;
uint32_t start_time, elapsed_time;


// Bytes reservados para control de flujo de la transmision
#define _WAIT_ 255
#define _RESUME_ 254

void error() {
  while (1) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(50);
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
  }
}
// Validar resultado de la operacion
void validar() {
  bool error_ = false;
  Serial.println("Validando matriz");
  for (int i = 0; i < n; i++) {
    Serial.print("C: ");
    for (int j = 0; j < n; j++) {
      if (C[i * n + j] != n) error_ = true;
      Serial.print(C[i * n + j]);
      Serial.print(" ");
    }
    Serial.print('\n');
  }
  if (error_) {
    Serial.println("Error: fallo en la validacion");
    error(); //bloqueante
  }
  while(1){;}
}
/*
* Calcula una fila de bloques de la matriz resultado mientras chequea por pedidos de trabajo
*/
void calcular_fila() {
  for (uint8_t i = fil_disponible; i < (fil_disponible + bs); i++) {
    for (uint8_t j = 0; j < n; j++) {
      for (uint8_t k = 0; k < n; k++) {
        C[i * n + j] += A[i * n + k] * B[j * n + k];
      }
    }
  }
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT); // Led Debug
  Serial.begin(115200); // Inicializa el puerto para comunicacion serial
  Serial3.begin(115200); // Comunicacion Arduino

  // Alocacion e inicializacion
  A = (uint8_t*)malloc(n * n * sizeof(uint8_t));
  B = (uint8_t*)malloc(n * n * sizeof(uint8_t));
  C = (uint8_t*)calloc(n * n, sizeof(uint8_t));
  for (uint8_t i = 0; i < n; i++) {
    for (uint8_t j = 0; j < n; j++) {
      A[i * n + j] = 1;
      B[i * n + j] = 1;
    }
  }
  Serial.println("Enviando valores iniciales");
  Serial3.write(n);
  Serial3.write(bs);
  Serial.println("Esperando confirmacion");

  while (Serial3.read() != _RESUME_) { ; }
  // Enviar fila de bloques A
  for (int i = 0; i < bs; i++) {
    for (int j = 0; j < n; j++) {
      Serial3.write(A[i * n + j]);
    }
  }
  // Enviar matriz B
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      Serial3.write(B[i * n + j]);
    }
  }
  Serial.println("Matrices enviadas");
  fil_disponible += bs;
}

void loop() {
  if (fil_disponible < n) {
    Serial.print("Calculando fila: ");
    Serial.println(fil_disponible);
    calcular_fila();
    fil_disponible += bs;
  }
  else {
    // Finalizo el cálculo, pedir resultados a esclavo
    Serial3.write(_RESUME_);
    // Esto funciona xq se envia la primer fila, caso contrario sera necesario agregar logica adicional para recibir resultado
    if (Serial3.readBytes(C, n * bs) != n * bs) {
      Serial.print("Error recibiendo bloque resultado");
      error();
    }
    validar();
  }
}