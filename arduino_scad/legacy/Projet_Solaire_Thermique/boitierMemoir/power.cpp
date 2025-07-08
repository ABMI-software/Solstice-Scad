#include "hardware.h"
#include <Arduino.h>
//Déclarations de fonctions
void initPower();
void managePower();
float getTensionPV();
float getTensionRegulee();
float getPuissancePV();

//Variables globales 
float tensionPV = 0;
float tensionRegulee = 0;
float puissancePV = 0;

void initPower() {
  pinMode(PV_VOUT, INPUT);
  pinMode(REGULATEUR_VOUT, INPUT);
}

void managePower() {
  tensionPV = analogRead(PV_VOUT) * (5.0 / 1023.0) * 6.0;  // Suppose diviseur 1:5
  tensionRegulee = analogRead(REGULATEUR_VOUT) * (5.0 / 1023.0) * 3.0;  // Suppose 1:2
  puissancePV = tensionPV * 0.5; // Si courant constant estimé à 0.5 A
}

float getTensionPV() {
  return tensionPV;
}

float getTensionRegulee() {
  return tensionRegulee;
}

float getPuissancePV() {
  return puissancePV;
}
