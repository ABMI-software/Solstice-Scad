/**
 * encoders.cpp - Implémentation de la gestion des encodeurs
 */

 #include <Arduino.h>
 #include "hardware.h"
 #include "encoders.h"
 #include "pumps.h"
 
 // Tableau pour stocker les valeurs actuelles des encodeurs (0-100%)
 volatile uint8_t encoderValues[NUM_ENCODERS] = {0, 0};
 
 // Variables pour stocker l'état précédent des encodeurs
 volatile uint8_t lastEncoderState[NUM_ENCODERS] = {0, 0};
 volatile unsigned long lastDebounceTime[NUM_ENCODERS] = {0, 0};
 const unsigned long debounceDelay = 5;  // Délai anti-rebond en ms
 
 // Déclaration des fonctions d'interruption
 void encoder1ISR();
 void encoder2ISR();
 
 /**
  * Initialise les encodeurs et attache les interruptions
  */
 void initEncoders() {
   // Configuration des broches (déjà fait dans hardware.cpp)
   
   // Lecture de l'état initial des encodeurs
   lastEncoderState[0] = digitalRead(ENCODER1_CLK);
   lastEncoderState[1] = digitalRead(ENCODER2_CLK);
   
   // Attacher les interruptions (corrigé: on passe la référence, pas l'appel)
   attachInterrupt(digitalPinToInterrupt(ENCODER1_CLK), encoder1ISR, CHANGE);
   attachInterrupt(digitalPinToInterrupt(ENCODER2_CLK), encoder2ISR, CHANGE);
   
   Serial.println(F("Encodeurs initialisés"));
 }
 
 /**
  * Fonction d'interruption pour l'encodeur 1
  */
 void encoder1ISR() {
   unsigned long currentTime = millis();
   if (currentTime - lastDebounceTime[0] > debounceDelay) {
     uint8_t currentState = digitalRead(ENCODER1_CLK);
     if (currentState != lastEncoderState[0]) {
       // Si le changement d'état est détecté
       if (digitalRead(ENCODER1_DT) != currentState) {
         // Rotation dans le sens horaire
         if (encoderValues[0] < ENCODER_MAX_VALUE) {
           encoderValues[0] += ENCODER_STEP_VALUE;
           if (encoderValues[0] > ENCODER_MAX_VALUE) {
             encoderValues[0] = ENCODER_MAX_VALUE;
           }
         }
       } else {
         // Rotation dans le sens anti-horaire
         if (encoderValues[0] > ENCODER_MIN_VALUE) {
           encoderValues[0] -= ENCODER_STEP_VALUE;
           if (encoderValues[0] < ENCODER_MIN_VALUE || encoderValues[0] > 200) { // Détection de débordement
             encoderValues[0] = ENCODER_MIN_VALUE;
           }
         }
       }
       lastEncoderState[0] = currentState;
     }
     lastDebounceTime[0] = currentTime;
   }
 }
 
 /**
  * Fonction d'interruption pour l'encodeur 2
  */
 void encoder2ISR() {
   unsigned long currentTime = millis();
   if (currentTime - lastDebounceTime[1] > debounceDelay) {
     uint8_t currentState = digitalRead(ENCODER2_CLK);
     if (currentState != lastEncoderState[1]) {
       // Si le changement d'état est détecté
       if (digitalRead(ENCODER2_DT) != currentState) {
         // Rotation dans le sens horaire
         if (encoderValues[1] < ENCODER_MAX_VALUE) {
           encoderValues[1] += ENCODER_STEP_VALUE;
           if (encoderValues[1] > ENCODER_MAX_VALUE) {
             encoderValues[1] = ENCODER_MAX_VALUE;
           }
         }
       } else {
         // Rotation dans le sens anti-horaire
         if (encoderValues[1] > ENCODER_MIN_VALUE) {
           encoderValues[1] -= ENCODER_STEP_VALUE;
           if (encoderValues[1] < ENCODER_MIN_VALUE || encoderValues[1] > 200) { // Détection de débordement
             encoderValues[1] = ENCODER_MIN_VALUE;
           }
         }
       }
       lastEncoderState[1] = currentState;
     }
     lastDebounceTime[1] = currentTime;
   }
 }
 
 /**
  * Met à jour les pompes en fonction des valeurs des encodeurs
  * Cette fonction doit être appelée régulièrement dans la boucle principale
  */
 void updateEncoders() {
   // Lecture de l'état des boutons des encodeurs
   static bool lastButtonState1 = HIGH;
   static bool lastButtonState2 = HIGH;
   
   bool buttonState1 = digitalRead(ENCODER1_SW);
   bool buttonState2 = digitalRead(ENCODER2_SW);
   
   // Détection d'appui sur le bouton de l'encodeur 1
   if (buttonState1 == LOW && lastButtonState1 == HIGH) {
     // Reset de la valeur à 0
     encoderValues[0] = 0;
     Serial.println(F("Encodeur 1 remis à zéro"));
   }
   lastButtonState1 = buttonState1;
   
   // Détection d'appui sur le bouton de l'encodeur 2
   if (buttonState2 == LOW && lastButtonState2 == HIGH) {
     // Reset de la valeur à 0
     encoderValues[1] = 0;
     Serial.println(F("Encodeur 2 remis à zéro"));
   }
   lastButtonState2 = buttonState2;
   
   // Mise à jour des vitesses des pompes en fonction des valeurs des encodeurs
   // Conversion de 0-100% à 0-255 pour le PWM
   uint8_t pump1Speed = map(encoderValues[0], 0, 100, 0, 255);
   uint8_t pump2Speed = map(encoderValues[1], 0, 100, 0, 255);
   
   setPumpSpeed(0, pump1Speed);
   setPumpSpeed(1, pump2Speed);
 }
 
 /**
  * Retourne la valeur actuelle d'un encodeur
  * @param encoderIndex Index de l'encodeur (0 ou 1)
  * @return Valeur actuelle en pourcentage (0-100%)
  */
 uint8_t getEncoderValue(uint8_t encoderIndex) {
   if (encoderIndex < NUM_ENCODERS) {
     return encoderValues[encoderIndex];
   }
   return 0;
 }
 
 /**
  * Définit la valeur d'un encodeur
  * @param encoderIndex Index de l'encodeur (0 ou 1)
  * @param value Nouvelle valeur (0-100%)
  */
 void setEncoderValue(uint8_t encoderIndex, uint8_t value) {
   if (encoderIndex < NUM_ENCODERS) {
     // Limitation aux bornes min/max
     if (value > ENCODER_MAX_VALUE) {
       value = ENCODER_MAX_VALUE;
     }
     
     encoderValues[encoderIndex] = value;
     
     // Mise à jour immédiate de la pompe correspondante
     uint8_t pumpSpeed = map(value, 0, 100, 0, 255);
     setPumpSpeed(encoderIndex, pumpSpeed);
   }
 }