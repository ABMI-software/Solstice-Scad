/**
 * Projet_Solaire_Thermique - Boîtier de test pour panneau solaire thermique
 * 
 * Ce programme permet de:
 * - Mesurer 4 températures avec des thermocouples K + modules MAX6675
 * - Lire et contrôler 2 débits via des encodeurs rotatifs KY-040
 * - Piloter 2 pompes DC via un driver L298N
 * - Afficher les valeurs sur un écran OLED I2C
 * - Enregistrer les données sur une carte SD
 * - Mesurer la tension et la puissance du panneau solaire
 * 
 * Matériel: Arduino Mega 2560
 */

// Inclusion des bibliothèques système
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <RTClib.h>

// Inclusion des fichiers d'en-tête du projet
#include "hardware.h"
#include "sensors.h"
#include "pumps.h"
#include "encoders.h"
#include "display.h"
#include "sdlogger.h"
#include "power.h"

// Définition des constantes
#define SERIAL_BAUD 115200
#define LOG_INTERVAL 1000  // Intervalle d'enregistrement en ms (1s)

// Variables globales
unsigned long lastLogTime = 0;
RTC_DS3231 rtc;  // Instance de l'horloge RTC

// Fonction d'initialisation
void setup() {
  // Initialisation du port série
  Serial.begin(SERIAL_BAUD);
  Serial.println(F("Démarrage du système de test pour panneau solaire thermique..."));
  
  // Initialisation du matériel
  initHardware();
  
  // Initialisation des composants
  initSensors();
  initPumps();
  initEncoders();
  initPower();
  
  // Initialisation de l'affichage
  if (!initDisplay()) {
    Serial.println(F("Erreur d'initialisation de l'écran OLED"));
  }
  
  // Initialisation de la carte SD
  if (!initSDLogger()) {
    Serial.println(F("Erreur d'initialisation de la carte SD"));
  }
  
  // Initialisation de l'horloge RTC
  if (!rtc.begin()) {
    Serial.println(F("Horloge RTC non détectée"));
  } else if (rtc.lostPower()) {
    Serial.println(F("RTC a perdu l'alimentation, réglage de l'heure à la compilation"));
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  
  Serial.println(F("Initialisation terminée"));
  
  // Affichage initial
  updateDisplay();
}

// Boucle principale
void loop() {
  // Lecture des capteurs
  readAllTemperatures();
  
  // Mesure de la puissance
  managePower();
  
  // Mise à jour des encodeurs et des pompes
  updateEncoders();
  updatePumps();
  
  // Mise à jour de l'affichage
  updateDisplay();
  
  // Enregistrement périodique des données
  unsigned long currentMillis = millis();
  if (currentMillis - lastLogTime >= LOG_INTERVAL) {
    lastLogTime = currentMillis;
    
    // Obtention de l'horodatage
    String timestamp;
    DateTime now;
    
    if (rtc.begin()) {
      now = rtc.now();
      char dateTimeBuffer[20];
      sprintf(dateTimeBuffer, "%04d-%02d-%02d %02d:%02d:%02d", 
              now.year(), now.month(), now.day(), 
              now.hour(), now.minute(), now.second());
      timestamp = String(dateTimeBuffer);
    } else {
      // Utilisation de millis() si RTC non disponible
      timestamp = String(currentMillis / 1000);
    }
    
    // Enregistrement des données sur la carte SD
    logData(timestamp);
    
    // Affichage des données sur le port série (pour débogage)
    printSensorData();
    
    // Affichage des données de puissance
    Serial.print(F("Tension PV: "));
    Serial.print(getTensionPV());
 
    Serial.print(F("V, Puissance PV: "));
    Serial.print(getPuissancePV());
    Serial.println(F("W"));
  }
  
  // Courte pause pour éviter une utilisation excessive du processeur
  delay(1000);
}