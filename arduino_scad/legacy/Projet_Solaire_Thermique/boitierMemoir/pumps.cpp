/**
 * pumps.cpp - Implémentation de la gestion des pompes
 */

 #include <Arduino.h>
 #include "hardware.h"
 #include "pumps.h"
 
 // Tableau pour stocker les vitesses actuelles des pompes (0-255)
 uint8_t pumpSpeeds[NUM_PUMPS] = {0, 0};
 
 /**
  * Initialise les pompes
  */
 void initPumps() {
   // Configuration des moteurs en mode avance
   digitalWrite(PUMP1_IN1, HIGH);
   digitalWrite(PUMP1_IN2, LOW);
   digitalWrite(PUMP2_IN3, HIGH);
   digitalWrite(PUMP2_IN4, LOW);
   
   // Initialisation des vitesses à 0
   analogWrite(PUMP1_ENA, 0);
   analogWrite(PUMP2_ENB, 0);
   
   Serial.println(F("Pompes initialisées"));
 }
 
 /**
  * Définit la vitesse d'une pompe spécifique
  * @param pumpIndex Index de la pompe (0 ou 1)
  * @param speed Vitesse (0-255)
  */
 void setPumpSpeed(uint8_t pumpIndex, uint8_t speed) {
   // Vérification des limites
   if (pumpIndex >= NUM_PUMPS) {
     return;
   }
   
   // Stockage de la nouvelle valeur
   pumpSpeeds[pumpIndex] = speed;
   
   // Application de la vitesse à la pompe
   if (pumpIndex == 0) {
     analogWrite(PUMP1_ENA, speed);
   } else {
     analogWrite(PUMP2_ENB, speed);
   }
   
   // Affichage pour débogage
   Serial.print(F("Pompe "));
   Serial.print(pumpIndex + 1);
   Serial.print(F(" : vitesse = "));
   Serial.println(speed);
 }
 
 /**
  * Retourne la vitesse actuelle d'une pompe
  * @param pumpIndex Index de la pompe (0 ou 1)
  * @return Vitesse actuelle (0-255)
  */
 uint8_t getPumpSpeed(uint8_t pumpIndex) {
   if (pumpIndex < NUM_PUMPS) {
     return pumpSpeeds[pumpIndex];
   }
   return 0;
 }
 
 /**
  * Met à jour les pompes (appel périodique)
  * Cette fonction peut être utilisée pour implémenter des comportements
  * spécifiques comme des rampes de vitesse
  */
 void updatePumps() {
   // Pour l'instant, cette fonction ne fait rien de plus
   // Dans une version avancée, on pourrait implémenter des rampes
   // d'accélération/décélération pour éviter les à-coups
 }