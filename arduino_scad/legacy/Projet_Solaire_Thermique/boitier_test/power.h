/**
 * power.h - Gestion de la puissance et des tensions
 */

#ifndef POWER_H
#define POWER_H

#include <Arduino.h>

// Prototypes des fonctions
void initPower();
void managePower();
float getTensionPV();
float getTensionRegulee();
float getPuissancePV();

// Variables externes
extern float tensionPV;
extern float tensionRegulee;
extern float puissancePV;

#endif