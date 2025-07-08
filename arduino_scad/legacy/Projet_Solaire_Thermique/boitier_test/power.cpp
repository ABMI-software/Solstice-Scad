#include "hardware.h"
#include <Arduino.h>
//Déclarations de fonctions
void initPower();
void managePower();
float getTensionPV();

float getPuissancePV();

//Variables globales 
float tensionPV = 0;
float puissancePV = 0;

void initPower() {
  pinMode(PV_VOUT, INPUT);
  
}

void managePower() {
  tensionPV = analogRead(PV_VOUT) * (5.0 / 1023.0) ;  // Suppose diviseur 1:5

  puissancePV = tensionPV * 0.5; // Si courant constant estimé à 0.5 A
}

float getTensionPV() {
  return tensionPV;
}

float getPuissancePV() {
  return puissancePV;
}
