#include <Arduino.h>

char incomingByte;

void setup() {
  Serial3.begin(250000UL);
}

void loop() {
  // Esperar mensaje
  do
  {
    incomingByte = Serial3.read();
  } while (incomingByte!='a');
  // Enviar respuesta
  Serial3.write('a');
}