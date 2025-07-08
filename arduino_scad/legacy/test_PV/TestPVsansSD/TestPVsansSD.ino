#include <Arduino.h>
// === Constantes physiques ===

const float surfacePanneau = 0.15;         // m²
const float masseEau = 3.5;                // kg
const float capaciteEau = 4180.0;          // J/(kg·°C)


void setup() {
  Serial.begin(9600);  // Démarre le moniteur série
  pinMode(A1, INPUT);  // Broche reliée au panneau solaire
  Serial.println("=== Test PV ===");
}

void loop() {
  int rawValue = analogRead(A1);  // Lecture analogique
  float voltage = rawValue * (5.0 / 1023.0) ;  // Conversion avec diviseur de tension
  float lux = voltage * 100.0;
  float puissancePV_Wm2 = lux / 120.0;
  float dT_dt = (puissancePV_Wm2 * surfacePanneau) / (masseEau * capaciteEau);

  Serial.print("Tension du panneau: ");
  Serial.print(voltage, 2);
  Serial.print(" V     Lux : ");
  Serial.print(lux);
  Serial.print(" lux    Puissance solaire reçue : ");
  Serial.print(puissancePV_Wm2, 2);
  Serial.print(" W/m²\t");

  Serial.print("  Élévation temp. estimée : ");
  Serial.print(dT_dt, 6);
  Serial.println(" °C/s");

  delay(1000);  // 1 mesure par seconde
}
