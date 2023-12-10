#include <Arduino.h>
#define OUTPUT_ENABLE 8;

uint8_t n = 80; // Cant filas/columnas
uint8_t bs = 4; // Tamaño de bloque
uint8_t* A, * B, * C;
uint8_t fil_disponible = 0;
uint8_t fil_maestro;
uint8_t fil_esclavo;
uint8_t col_esclavo = 0;
uint8_t filas_procesadas = 0;
bool volatile job_req = false;
bool fil_disp = true;
uint32_t start_time, elapsed_time;


void error() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(50);
  digitalWrite(LED_BUILTIN, LOW);
  delay(200);
}
// Validar resultado de la operacion
void validar() {
  for (uint16_t i = 0; i < n * n; i++) {
    if (C[i] != n) {
      Serial.println("Error: fallo en la validación");
      error(); //bloqueante
    }
  }
  Serial.println("Operación completada con éxito");
}

/*
* A partir de la fila que le corresponde al esclavo, envio bs filas completas (bs*n elementos)
*/
void sendFil() {
  Serial.print("Enviando filas ");
  Serial.print(fil_esclavo);
  Serial.print("...");
  Serial.println(fil_disponible);
  for (uint8_t i = fil_esclavo; i < fil_disponible; i++) {
    for (uint8_t j = 0; j < n; j++) {
      Serial1.write(A[i * n + j]);
    }
  }
}

/*
* A partir de la columna que le corresponde al esclavo, envio bs columnas completas (bs*n elementos)
*/
void sendCol() {
  Serial.print("Enviando columnas ");
  Serial.print(col_esclavo);
  Serial.print("...");
  Serial.println(col_esclavo+bs);
  for (uint8_t i = col_esclavo; i < col_esclavo + bs; i++) {
    for (uint8_t j = 0; j < n; j++) {
      Serial1.write(B[i * n + j]);
    }
  }
}
/*
* Recibo un bloque de la matriz resultado (bs*bs elementos)
* y lo guardo en la posicion correspondiente
*/
void recvBlock() {
  Serial.print("Bloque recibido: ");
  for (int i = 0; i < bs; i++) {
    for (int j = 0; j < bs; j++) {
      C[(fil_esclavo + i) * n + (col_esclavo + j)]=Serial1.read();
      Serial.print(C[(fil_esclavo + i) * n + (col_esclavo + j)]);
      Serial.print(" ");
    }
    Serial.println();
  }
  Serial.println();
}

/*
* Ante una solicitud de trabajo, recibo el bloque recien calculado (si no es la primera solicitud)
* Luego, si se proceso completa una fila de bloques resultado y aun hay filas resultado que falten calcular, se envian
* Si no se proceso completa una fila de bloques resultado se envia la columna de bloques B correspondiente
*/
void enviarTrabajo() {
  // Recibir bloque
  recvBlock();
  col_esclavo += bs;
  // Si el bloque que recibi fue el ultimo de la fila reinicio el contador
  if (col_esclavo >= n) {
    col_esclavo = 0;
    filas_procesadas++;
  }
  // Si termino de procesar una fila y todavia quedan envio la proxima
  if (col_esclavo != 0) {
    sendCol();
  } // Si no, si todavia quedan filas disponibles
  else if (fil_disp) {
    // Tomar la proxima fila disponible
    fil_esclavo = fil_disponible;
    fil_disponible += bs;
    if (fil_disponible >= n) {
      fil_disp = false;
    }
    sendCol();
    sendFil();
  }
}
/*
* Calcula una fila de bloques de la matriz resultado mientras chequea por pedidos de trabajo
*/
void calcular_fila() {
  uint8_t i, j;
  for (i = fil_maestro; i < (fil_maestro + bs); i++) {
    for (j = 0; j < n; j++) {
      C[i * n + j] += A[i * n + j] * B[i * n + j];
      if (job_req) enviarTrabajo();
    }
  }
}

void rutina() {
  job_req = true;
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT); // Led Debug
  Serial.begin(9600); // Inicializa el puerto para comunicacion serial
  Serial1.begin((uint32_t)1000000); // Comunicación Arduino
  attachInterrupt(digitalPinToInterrupt(12), rutina, CHANGE); // Interrupcion de PIN CHANGE para solicitud de trabajo

  A = (uint8_t*)malloc(n * n * sizeof(uint8_t));
  B = (uint8_t*)malloc(n * n * sizeof(uint8_t));
  C = (uint8_t*)calloc(n * n, sizeof(uint8_t));
  //Inicializacion
  for (uint8_t i = 0; i < n; i++) {
    for (uint8_t j = 0; j < n; j++) {
      A[i * n + j] = 1;
      B[i * n + j] = 1;
    }
  }

  //Enviar info inicial al esclavo
  //start_time = micros();
  Serial.print("Enviando n:");
  Serial.println(n);
  Serial1.write(n);
  Serial.print("Enviando bs:");
  Serial.println(bs);
  Serial1.write(bs);
  Serial.println("Enviando fila y columna de bloques inicial");
  fil_esclavo = fil_disponible;
  fil_disponible += bs;
  sendFil();
  sendCol();
  enviarTrabajo();
}

void loop() {
  // Repetir hasta completar el calculo de la matriz
  if (filas_procesadas < n / bs) {
    if (job_req) {
      enviarTrabajo();
      job_req = false;
    }
    if (fil_disp) {
      fil_maestro = fil_disponible;
      fil_disponible += bs;
      Serial.print("Procesando fila ");
      Serial.println(fil_maestro);
      if (fil_disponible >= n) fil_disp = false;
      calcular_fila();
      filas_procesadas++;
    }
  }
  else {
    //elapsed_time = micros() - start_time;
    validar();
  }
}