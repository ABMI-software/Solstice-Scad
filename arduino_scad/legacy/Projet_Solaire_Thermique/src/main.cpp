#include <Arduino.h>
#include "hardware.h"


void setup() {
  Serial.begin(9600);
  initSensors();
  initDisplay();
  initPumps();
  initEncoders();
  initPower();
  Serial.println("Système solaire thermique prêt !");
}

void loop() {
  readSensors();
  readEncoders();
  controlPumps();
  managePower();
  updateDisplay();
  delay(100);
}
