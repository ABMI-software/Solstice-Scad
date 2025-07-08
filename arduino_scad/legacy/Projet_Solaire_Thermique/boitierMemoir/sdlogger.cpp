/**
 * sdlogger.cpp - Implémentation de la gestion de l'enregistrement sur carte SD
 */

#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include "hardware.h"
#include "sdlogger.h"
#include "sensors.h"
#include "encoders.h"

// Variable pour indiquer si la carte SD est disponible
bool sdCardAvailable = false;

/**
 * Initialise le module de carte SD
 * @return true si l'initialisation est réussie, false sinon
 */
bool initSDLogger() {
  // Initialisation de la carte SD
  Serial.print(F("Initialisation de la carte SD... "));
  
  // Initialisation du module SD
  if (!SD.begin(SD_CS)) {
    Serial.println(F("échec!"));
    sdCardAvailable = false;
    return false;
  }
  
  Serial.println(F("réussie."));
  sdCardAvailable = true;
  
  // Création de l'en-tête du fichier CSV si nécessaire
  if (!SD.exists(LOG_FILENAME)) {
    File dataFile = SD.open(LOG_FILENAME, FILE_WRITE);
    if (dataFile) {
      // Écriture de l'en-tête du fichier CSV
      dataFile.println(F("Horodatage,Temp1,Temp2,Temp3,Temp4,Debit1,Debit2"));
      dataFile.close();
      Serial.println(F("Fichier de log créé avec en-tête"));
    } else {
      Serial.println(F("Erreur lors de la création du fichier de log"));
      sdCardAvailable = false;
      return false;
    }
  }
  
  return true;
}

/**
 * Crée une entrée de log au format CSV
 * @param timestamp Horodatage pour l'entrée
 * @return Chaîne formatée au format CSV
 */
String createLogEntry(String timestamp) {
  // Créer la ligne de données au format CSV
  String dataString = timestamp + ",";
  
  // Ajout des températures
  for (uint8_t i = 0; i < NUM_SENSORS; i++) {
    dataString += String(temperatures[i], 1);
    dataString += ",";
  }
  
  // Ajout des débits (valeurs des encodeurs)
  dataString += String(getEncoderValue(0)) + ",";
  dataString += String(getEncoderValue(1));
  
  return dataString;
}

/**
 * Enregistre les données actuelles dans le fichier de log
 * @param timestamp Horodatage pour l'entrée
 * @return true si l'enregistrement est réussi, false sinon
 */
bool logData(String timestamp) {
  // Vérification de la disponibilité de la carte SD
  if (!sdCardAvailable) {
    return false;
  }
  
  // Création de la ligne de données
  String dataString = createLogEntry(timestamp);
  
  // Ouverture du fichier en mode ajout
  File dataFile = SD.open(LOG_FILENAME, FILE_WRITE);
  
  // Écriture des données dans le fichier
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    
    // Affichage des données sur le port série (pour débogage)
    Serial.print(F("Enregistrement: "));
    Serial.println(dataString);
    
    return true;
  } else {
    Serial.println(F("Erreur lors de l'ouverture du fichier de log"));
    return false;
  }
}