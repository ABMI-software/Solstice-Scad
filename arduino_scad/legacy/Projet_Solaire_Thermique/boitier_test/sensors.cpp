/**
 * sensors.cpp - Implémentation de la gestion des capteurs de température
 */

 #include <Arduino.h>
 #include "sensors.h"
 
 // Instances des capteurs MAX6675 (SPI logiciel)
 MAX6675 thermocouple1(37, 34, 35);  // SCK, CS, SO
 MAX6675 thermocouple2(29, 30, 31);
 MAX6675 thermocouple3(32, 33, 34);
 MAX6675 thermocouple4(27, 26, 25);
 
 // Tableau pour stocker les températures
 float temperatures[NUM_SENSORS] = {0.0, 0.0, 0.0, 0.0};
 
 /**
  * Initialise les capteurs de température
  */
 void initSensors() {
   Serial.println(F("Initialisation des capteurs de température..."));
   delay(1000); // Attente pour stabiliser les capteurs
   readAllTemperatures(); // Première lecture
   Serial.println(F("Capteurs initialisés."));
 }
 
 /**
  * Lit toutes les températures et les stocke dans le tableau
  */
 void readAllTemperatures() {
   temperatures[0] = thermocouple1.readCelsius();
   delay(10);
   temperatures[1] = thermocouple2.readCelsius();
   delay(10);
   temperatures[2] = thermocouple3.readCelsius();
   delay(10);
   temperatures[3] = thermocouple4.readCelsius();
 
   for (uint8_t i = 0; i < NUM_SENSORS; i++) {
     if (isnan(temperatures[i]) || temperatures[i] < -10.0 || temperatures[i] > 1000.0) {
       Serial.print(F("Erreur de lecture capteur "));
       Serial.println(i + 1);
       temperatures[i] = 0.0; // Valeur de secours
     }
   }
 }
 
 /**
  * Retourne la température d'un capteur spécifique
  */
 float getTemperature(uint8_t sensorIndex) {
   if (sensorIndex < NUM_SENSORS) {
     return temperatures[sensorIndex];
   }
   return 0.0;
 }
 
 /**
  * Affiche les températures sur le port série
  */
 void printSensorData() {
   Serial.println(F("--- Températures ---"));
   for (uint8_t i = 0; i < NUM_SENSORS; i++) {
     Serial.print(F("Sonde "));
     Serial.print(i + 1);
     Serial.print(F(" : "));
     Serial.print(temperatures[i]);
     Serial.println(F(" °C"));
   }
 }
 