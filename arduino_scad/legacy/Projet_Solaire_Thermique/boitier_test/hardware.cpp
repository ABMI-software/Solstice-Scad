/**
 * hardware.cpp - Implémentation des fonctions d'initialisation matérielle
 */

#include <Arduino.h>
#include "hardware.h"
#include <SPI.h>


/**
 * Initialise toutes les broches et le matériel de base
 */
void initHardware() {
  // Initialisation des broches du bus SPI
  pinMode(SENSORS_SCK, OUTPUT);
  pinMode(SENSORS_MISO, INPUT);
  
  // Initialisation des broches CS pour les modules MAX6675
  pinMode(SENSORS1_CS, OUTPUT);
  pinMode(SENSORS2_CS, OUTPUT);
  pinMode(SENSORS3_CS, OUTPUT);
  pinMode(SENSORS4_CS, OUTPUT);
  
  // Désactivation initiale des SENSORSs (CS à HIGH)
  digitalWrite(SENSORS1_CS, HIGH);
  digitalWrite(SENSORS2_CS, HIGH);
  digitalWrite(SENSORS3_CS, HIGH);
  digitalWrite(SENSORS4_CS, HIGH);
  
  // Initialisation des broches des encodeurs
  pinMode(ENCODER1_CLK, INPUT_PULLUP);
  pinMode(ENCODER1_DT, INPUT_PULLUP);
  pinMode(ENCODER1_SW, INPUT_PULLUP);
  pinMode(ENCODER2_CLK, INPUT_PULLUP);
  pinMode(ENCODER2_DT, INPUT_PULLUP);
  pinMode(ENCODER2_SW, INPUT_PULLUP);
  
  // Initialisation des broches du driver de moteur L298N
  pinMode(PUMP1_ENA, OUTPUT);
  pinMode(PUMP1_IN1, OUTPUT);
  pinMode(PUMP1_IN2, OUTPUT);
  pinMode(PUMP2_ENB, OUTPUT);
  pinMode(PUMP2_IN3, OUTPUT);
  pinMode(PUMP2_IN4, OUTPUT);
  
  // Configuration initiale des moteurs à l'arrêt
  digitalWrite(PUMP1_IN1, LOW);
  digitalWrite(PUMP1_IN2, LOW);
  analogWrite(PUMP1_ENA, 0);
  digitalWrite(PUMP2_IN3, LOW);
  digitalWrite(PUMP2_IN4, LOW);
  analogWrite(PUMP2_ENB, 0);
  
  // Initialisation de la broche CS pour la carte SD
  pinMode(SD_CS, OUTPUT);
  digitalWrite(SD_CS, HIGH);
  
  // Initialisation du bus I2C (pour l'OLED)
  Wire.begin();
  
  // Initialisation du bus SPI
  SPI.begin();
  
  Serial.println(F("Matériel initialisé"));
}