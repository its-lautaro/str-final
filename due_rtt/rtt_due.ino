/*
* Calcula el round trip time
*/
void setup() {
  Serial.begin(9600); // interfaz usb
  Serial1.begin((uint32_t) 1000000); // comunicacion arduino
  
}

void loop() {
  unsigned long last_micros = micros();
  
  // Enviar mensaje
  Serial1.write('a');
  
  // Esperar respuesta
  while (!Serial1.available()) {
      ;
  }
  
  unsigned long roundTripTime = micros() - last_micros;
  
  Serial1.read();

  // Informar round trip time por consola
  Serial.print("Round Trip Time: ");
  Serial.print(roundTripTime);
  Serial.println(" microseconds");

  delay(1000);
}