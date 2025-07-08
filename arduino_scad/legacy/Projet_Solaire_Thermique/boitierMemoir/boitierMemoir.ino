#include <Arduino.h>
#include "hardware.h"
#include "sensors.h"
#include "encoders.h"
#include "power.h"
#include "sdlogger.h"

// Intervalle d'enregistrement des données (5 minutes = 300000 ms)
const unsigned long LOG_INTERVAL = 300000;
unsigned long lastLogTime = 0;

// Horodatage simplifié (compteur de minutes depuis démarrage)
unsigned long minuteCounter = 0;

// Variables globales pour détecter changement débit
uint8_t lastEncoderValue1 = 0;
uint8_t lastEncoderValue2 = 0;

void setup() {
  Serial.begin(9600);

  // Initialisation matériel
  initHardware();

  // Initialisation capteurs
  initSensors();

  // Initialisation encodeurs
  initEncoders();

  // Initialisation gestion puissance
  initPower();

  // Initialisation carte SD
  if (!initSDLogger()) {
    Serial.println(F("Erreur: Impossible d'initialiser la carte SD!"));
  }
}

void loop() {
  unsigned long currentMillis = millis();

  // Mise à jour périodique des capteurs
  readAllTemperatures();
  delay(1000);

  // Mise à jour périodique de la gestion de puissance
  managePower();
  delay(1000);

  // Mise à jour périodique des encodeurs
  updateEncoders();
  delay(1000);

  // Vérification du changement de débit pour affichage immédiat
  uint8_t currentEncoderValue1 = getEncoderValue(0);
  uint8_t currentEncoderValue2 = getEncoderValue(1);

  if (currentEncoderValue1 != lastEncoderValue1 || currentEncoderValue2 != lastEncoderValue2) {
    Serial.println(F("\n--- Changement détecté dans le débit ---"));
    Serial.print(F("Débit 1 changé à : "));
    Serial.print(currentEncoderValue1);
    Serial.println(F("%"));

    Serial.print(F("Débit 2 changé à : "));
    Serial.print(currentEncoderValue2);
    Serial.println(F("%"));

    lastEncoderValue1 = currentEncoderValue1;
    lastEncoderValue2 = currentEncoderValue2;
  }

  // Affichage régulier sur Serial
  Serial.println(F("\n--- État actuel des capteurs ---"));
  for (uint8_t i = 0; i < NUM_SENSORS; i++) {
    Serial.print(F("Température "));
    Serial.print(i + 1);
    Serial.print(F(": "));
    Serial.print(getTemperature(i), 1);
    Serial.println(F(" °C"));
  }

  Serial.print(F("Débit 1: "));
  Serial.print(currentEncoderValue1);
  Serial.println(F("%"));

  Serial.print(F("Débit 2: "));
  Serial.print(currentEncoderValue2);
  Serial.println(F("%"));

  Serial.print(F("Puissance PV: "));
  Serial.print(getPuissancePV(), 2);
  Serial.println(F(" W"));

  // Enregistrement périodique des données
  if (currentMillis - lastLogTime >= LOG_INTERVAL) {
    lastLogTime = currentMillis;

    // Création timestamp (simplifié pour test)
    char timestamp[20];
    unsigned long hours = minuteCounter / 60;
    unsigned long minutes = minuteCounter % 60;
    snprintf(timestamp, sizeof(timestamp), "%02lu:%02lu", hours, minutes);

    // Construction chaîne CSV
    String dataLog = String(timestamp) + ",";

    // Ajout températures
    for (uint8_t i = 0; i < NUM_SENSORS; i++) {
      dataLog += String(getTemperature(i), 1) + ",";
    }

    // Ajout débits (encodeurs)
    dataLog += String(currentEncoderValue1) + ",";
    dataLog += String(currentEncoderValue2) + ",";

    // Ajout puissance PV
    dataLog += String(getPuissancePV(), 2);

    // Écriture des données sur SD
    if (logData(dataLog)) {
      Serial.print(F("Données enregistrées: "));
      Serial.println(dataLog);
    } else {
      Serial.println(F("Erreur écriture carte SD"));
    }

    // Incrément minuteCounter (5 min écoulées)
    minuteCounter += 5;
  }

  delay(5000);
}
