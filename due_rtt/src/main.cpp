/*
* Calcula el round trip time
*/
#include <Arduino.h>
char incomingByte;
void setup() {
  Serial.begin(115200UL); // interfaz usb
  Serial3.begin(250000UL); // comunicacion arduino
  
}

void loop() {
  unsigned long last_micros = micros();
  
  // Enviar mensaje
  Serial3.write('a');
  
  // Esperar respuesta
  do
  {
    incomingByte = Serial3.read();
  } while (incomingByte!='a');
  
  unsigned long roundTripTime = micros() - last_micros;
  
  Serial3.read();

  // Informar round trip time por consola
  Serial.print("Round Trip Time: ");
  Serial.print(roundTripTime);
  Serial.println(" microseconds");

  delay(1000);
}