/**
 * sensors.cpp - Implémentation de la gestion des capteurs de température
 */

 #include <Arduino.h>
 #include "hardware.h"
 #include "sensors.h"
 
 // Instances des capteurs MAX6675
 MAX6675 thermocouple1(27, 26, 25);
 MAX6675 thermocouple2(29, 30, 31);
 MAX6675 thermocouple3(32, 33, 34);
 MAX6675 thermocouple4(37, 36, 35);
 
 // Tableau pour stocker les températures
 float temperatures[NUM_SENSORS] = {0.0, 0.0, 0.0, 0.0};
 
 /**
  * Initialise les capteurs de température
  */
 void initSensors() {
   // Attente pour que les capteurs soient prêts
   delay(500);
   
   // Première lecture pour vérifier que les capteurs sont opérationnels
   readAllTemperatures();
   
   Serial.println(F("Capteurs de température initialisés"));
 }
 
 /**
  * Lit toutes les températures et les stocke dans le tableau
  */
 void readAllTemperatures() {
   // Lecture des capteurs avec délai entre les lectures pour éviter les interférences
   temperatures[0] = thermocouple1.getCelsius();
   delay(10); // Court délai entre les lectures
   temperatures[1] = thermocouple2.getCelsius();
   delay(10);
   temperatures[2] = thermocouple3.getCelsius();
   delay(10);
   temperatures[3] = thermocouple4.getCelsius();
   
   // Détection des valeurs aberrantes (erreur de lecture)
   for (uint8_t i = 0; i < NUM_SENSORS; i++) {
     if (isnan(temperatures[i]) || temperatures[i] < -10.0 || temperatures[i] > 1000.0) {
       Serial.print(F("Erreur de lecture sur le capteur "));
       Serial.println(i + 1);
       // Conserver la dernière valeur valide
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
   return 0.0; // Retourne 0 si l'index est invalide
 }
 
 /**
  * Affiche les données des capteurs sur le port série (débogage)
  */
 void printSensorData() {
   Serial.println(F("--- Données des capteurs ---"));
   for (uint8_t i = 0; i < NUM_SENSORS; i++) {
     Serial.print(F("Température "));
     Serial.print(i + 1);
     Serial.print(F(": "));
     Serial.print(temperatures[i]);
     Serial.println(F(" °C"));
   }
 }