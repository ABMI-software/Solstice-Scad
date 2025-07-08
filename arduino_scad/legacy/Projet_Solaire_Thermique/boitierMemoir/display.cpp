/**
 * display.cpp - Implémentation de la gestion de l'écran OLED
 */

 #include <Arduino.h>
 #include "hardware.h"
 #include "display.h"
 #include "sensors.h"
 #include "encoders.h"
 #include "power.h"
 
 // Instance de l'écran OLED
 Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
 
 /**
  * Initialise l'écran OLED
  * @return true si l'initialisation est réussie, false sinon
  */
 bool initDisplay() {
   // Initialisation de l'écran OLED
   if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
     Serial.println(F("Échec d'initialisation de l'écran SSD1306"));
     return false;
   }
   
   // Configuration initiale de l'écran
   display.clearDisplay();
   display.setTextSize(1);
   display.setTextColor(SSD1306_WHITE);
   display.setCursor(0, 0);
   display.println(F("Boitier test solaire"));
   display.println(F("Initialisation..."));
   display.display();
   
   delay(1000);  // Pause pour afficher le message d'initialisation
   
   Serial.println(F("Écran OLED initialisé"));
   return true;
 }
 
 /**
  * Met à jour l'affichage avec les données actuelles
  */
 void updateDisplay() {
   display.clearDisplay();
   display.setTextSize(1);
   display.setTextColor(SSD1306_WHITE);
   display.setCursor(0, 0);
   
   // Affichage du titre
   display.println(F("Test Panneau Solaire"));
   display.drawLine(0, 9, display.width(), 9, SSD1306_WHITE);
   
   // Affichage des températures
   display.setCursor(0, 12);
   display.print(F("T1:"));
   display.print(temperatures[0], 1);
   display.print(F("C "));
   
   display.print(F("T2:"));
   display.print(temperatures[1], 1);
   display.println(F("C"));
   
   display.print(F("T3:"));
   display.print(temperatures[2], 1);
   display.print(F("C "));
   
   display.print(F("T4:"));
   display.print(temperatures[3], 1);
   display.println(F("C"));
   
   // Ligne de séparation
   display.drawLine(0, 30, display.width(), 30, SSD1306_WHITE);
   
   // Affichage des débits (valeurs des encodeurs)
   display.setCursor(0, 33);
   display.print(F("Débit 1: "));
   display.print(getEncoderValue(0));
   display.println(F("%"));
   
   display.print(F("Débit 2: "));
   display.print(getEncoderValue(1));
   display.println(F("%"));
   
   // Barre de progression pour le débit 1
   display.drawRect(0, 50, 128, 5, SSD1306_WHITE);
   int width1 = map(getEncoderValue(0), 0, 100, 0, 126);
   display.fillRect(1, 51, width1, 3, SSD1306_WHITE);
   
   // Barre de progression pour le débit 2
   display.drawRect(0, 58, 128, 5, SSD1306_WHITE);
   int width2 = map(getEncoderValue(1), 0, 100, 0, 126);
   display.fillRect(1, 59, width2, 3, SSD1306_WHITE);
   
   // Ajout des mesures de puissance
   if (tensionPV > 0 || tensionRegulee > 0) {
     display.setCursor(32, 1);
     display.print(F("PV:"));
     display.print(tensionPV, 1);
     display.print(F("V "));
     display.print(puissancePV, 1);
     display.print(F("W"));
   }
   
   // Mise à jour de l'affichage
   display.display();
 }
 
 /**
  * Affiche un message d'erreur sur l'écran
  * @param message Message d'erreur à afficher
  */
 void displayError(const char* message) {
   display.clearDisplay();
   display.setTextSize(1);
   display.setTextColor(SSD1306_WHITE);
   display.setCursor(0, 0);
   
   display.println(F("ERREUR:"));
   display.println();
   display.println(message);
   
   display.display();
 }