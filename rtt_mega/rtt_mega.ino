void setup() {
  Serial1.begin((uint32_t) 1000000);
}

void loop() {
  // Esperar mensaje
  while (!Serial1.available()) {
    ;
  }
  char incomingByte = Serial1.read();
  
  // Enviar respuesta
  Serial1.write('a');
}