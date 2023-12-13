#include <Arduino.h>

uint8_t n = 32; // Cant filas/columnas
uint8_t bs = 8; // Tamaño de bloque
uint8_t fil_disponible = 0; // Proxima fila a procesar
uint8_t col_esclavo = 0; // Proxima columna de bloques B a enviar al esclavo
uint8_t fil_esclavo = 255; // Ultima fila de bloques A enviada al esclavo
bool esperando_resultados = false; // Si se espera recibir info del esclavo
uint8_t* A, * B, * C;
uint32_t start_time, compute_time, elapsed_time;

// Byte reservados para control de flujo de la transmision
#define _JOB_REQ_ 255

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
  else {
    Serial.println("Exito");

  }
  while (1) { delay(1000); }
}
// Enviar columna de bloques B
void enviarCol() {
  for (int i = col_esclavo; i < (col_esclavo + bs); i++) {
    for (int j = 0; j < n; j++) {
      Serial3.write(B[i * n + j]);
    }
  }
  col_esclavo += bs;
}
// Enviar fila de bloques A
void enviarFila() {
  fil_esclavo = fil_disponible;
  for (int i = fil_esclavo; i < (fil_esclavo + bs); i++) {
    for (int j = 0; j < n; j++) {
      Serial3.write(A[i * n + j]);
    }
  }
  fil_disponible += bs;
}

void recibirBloque() {
  // Maestro avisa que esta listo para recibir
  Serial.println("Recibiendo trabajo");
  Serial3.write(_JOB_REQ_);
  // Recibir bloque resultado
  uint8_t blk[bs * bs];
  if (Serial3.readBytes(blk, bs * bs) < bs * bs) {
    Serial.println("Error recibiendo bloque resultado");
    error();
  }
  // Guardar bloque resultado
  for (int i = 0; i < bs; i++) {
    for (int j = 0; j < bs; j++) {
      C[(fil_esclavo + i) * n + ((col_esclavo - bs) + j)] = blk[i * bs + j];
    }
  }
  // Imprimir bloque recibido
  for (int i = 0; i < bs; i++) {
    Serial.print("C: ");
    for (int j = 0; j < bs; j++) {
      Serial.print(blk[i * bs + j]);
      Serial.print(" ");
    }
    Serial.println();
  }
  //Continuar procesando la peticion de trabajo
  Serial3.write(_JOB_REQ_);
}

void enviarTrabajo() {
  Serial.println("Pedido de trabajo..");
  // Si hay resultados que deben llegar del esclavo los enviará antes de pedir trabajo
  if (esperando_resultados) {
    esperando_resultados = false;
    recibirBloque();
  }
  // Esto es importante que se haga aca porque cuando espero resultados necesito sabe cual fue la ultima columna enviada (y si fue n y la reinicio ante pierdo esa info)
  if (col_esclavo == n) {
    col_esclavo = 0;
  }
  // Si debe arrancar a procesar una fila nueva y hay filas disponibles
  if (col_esclavo == 0 && fil_disponible < n) {
    enviarFila();
    enviarCol();
    esperando_resultados = true;
  }
  // Si esta actualmente procesando una fila
  else if (col_esclavo != 0) {
    // Enviar siguiente columna
    enviarCol();
    esperando_resultados = true;
  }
}
// Calcula una fila de bloques de la matriz resultado mientras chequea por pedidos de trabajo
void calcular_fila() {
  uint8_t fil_maestro = fil_disponible;
  fil_disponible += bs;
  for (uint8_t i = fil_maestro; i < (fil_maestro + bs); i++) {
    for (uint8_t j = 0; j < n; j++) {
      for (uint8_t k = 0; k < n; k++) {
        C[i * n + j] += A[i * n + k] * B[j * n + k];
        if (Serial3.read() == _JOB_REQ_) enviarTrabajo();
      }
    }
  }
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT); // Led Debug
  Serial.begin(115200UL); // Inicializa el puerto para comunicacion serial (debug)
  Serial3.begin(250000UL); // Comunicacion Arduino
  Serial.println("Enviando informacion inicial");
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
  while (Serial3.read() != _JOB_REQ_) { ; }
  start_time = millis();
  enviarTrabajo();
}

void loop() {
  if (fil_disponible < n) {
    Serial.print("Calculando fila: ");
    Serial.println(fil_disponible);
    calcular_fila();
  }
  else {
    compute_time = millis()-start_time;
    // Si finalizo pero faltan resultados del esclavo lo espera
    while (esperando_resultados) {
      while (Serial3.read() != _JOB_REQ_) { ; }
      enviarTrabajo();
    }
    Serial.print("Tiempo de calculo local: ");
    Serial.print(compute_time);
    Serial.println(" ms");
    // Tiempo total
    elapsed_time = millis() - start_time;
    Serial.print("Tiempo total transcurrido: ");
    Serial.print(elapsed_time);
    Serial.println(" ms");

    validar();
  }
}