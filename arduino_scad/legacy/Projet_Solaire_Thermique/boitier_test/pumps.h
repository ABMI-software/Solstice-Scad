/**
 * pumps.h - Gestion des pompes via le driver L298N
 */

#ifndef PUMPS_H
#define PUMPS_H

#include <Arduino.h>


// Nombre de pompes
#define NUM_PUMPS 2

// Prototypes des fonctions
void initPumps();
void setPumpSpeed(uint8_t pumpIndex, uint8_t speed);
uint8_t getPumpSpeed(uint8_t pumpIndex);
void updatePumps();

// Variables externes pour les vitesses des pompes
extern uint8_t pumpSpeeds[NUM_PUMPS];

#endif