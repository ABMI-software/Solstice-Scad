/**
 * encoders.h - Gestion des encodeurs rotatifs KY-040
 */

#ifndef ENCODERS_H
#define ENCODERS_H

#include <Arduino.h>

// Nombre d'encodeurs
#define NUM_ENCODERS 2

// Valeurs min/max pour les encodeurs (0-100%)
#define ENCODER_MIN_VALUE 0
#define ENCODER_MAX_VALUE 100
#define ENCODER_STEP_VALUE 5  // Incrément par pas d'encodeur

// Prototypes des fonctions
void initEncoders();
void updateEncoders();
uint8_t getEncoderValue(uint8_t encoderIndex);
void setEncoderValue(uint8_t encoderIndex, uint8_t value);

// Variables externes pour les valeurs des encodeurs
extern volatile uint8_t encoderValues[NUM_ENCODERS];

#endif