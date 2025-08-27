/**************************************************************************
 * @file arduino_scad.ino
 * @brief Programme principal Arduino SCAD - Version simplifiée
 * Fonctionnalités : capteurs MAX6675, encodeurs, pompes, UART, RTC, OLED
 **************************************************************************/

#include <Arduino.h>
#include "config.h"
#include "sensors.h"
#include "pump.h"
#include "pins.h"
#include "oled.h"
#include <RTClib.h>

// === DÉFINITION DES VARIABLES GLOBALES ===
unsigned long dernier_affichage_serial = 0;
unsigned long dernier_affichage_oled = 0;
unsigned long dernier_controle_pompes = 0;
unsigned long dernier_envoi_uart = 0;

// === INSTANCES DES MODULES ===
SensorModule sensorEau(PTC1_CLK, PTC1_CS, PTC1_SO);
SensorModule sensorPanneau(PTC2_CLK, PTC2_CS, PTC2_SO);
SensorModule sensorCuve1(CUVE1_CLK, CUVE1_CS, CUVE1_SO);
SensorModule sensorCuve2(CUVE2_CLK, CUVE2_CS, CUVE2_SO);

PumpController pompes;
EncoderController enc1(ENCODER1_CLK, ENCODER1_DT);
EncoderController enc2(ENCODER2_CLK, ENCODER2_DT);

RTC_DS3231 rtc;

/**************************************************************************/
/*!
    @brief  Fonction timestamp avec RTC
*/
/**************************************************************************/
String getTimestamp() {
    DateTime now = rtc.now();
    char buffer[20];
    sprintf(buffer, "%04d-%02d-%02d %02d:%02d:%02d",
            now.year(), now.month(), now.day(),
            now.hour(), now.minute(), now.second());
    return String(buffer);
}

/**************************************************************************/
/*!
    @brief  Lecture tension PV simplifiée
*/
/**************************************************************************/
float readPVVoltage() {
    int adc = analogRead(PV_VOUT);
    return (adc * 5.0) / 1023.0;
}

/**************************************************************************/
/*!
    @brief  Envoi frame UART vers Raspberry Pi
*/
/**************************************************************************/
void sendFrameUART() {
    String timestamp = getTimestamp();
    float t_eau = sensorEau.getSensor();
    float t_panneau = sensorPanneau.getSensor();
    float t_cuve1 = sensorCuve1.getSensor();
    float t_cuve2 = sensorCuve2.getSensor();
    int encoder1 = enc1.getValue();
    int encoder2 = enc2.getValue();
    float pv_volts = readPVVoltage();
    
    // Format CSV pour Raspberry Pi
    Serial.print("DATA,");
    Serial.print(timestamp); Serial.print(",");
    Serial.print(t_eau, 2); Serial.print(",");
    Serial.print(t_panneau, 2); Serial.print(",");
    Serial.print(t_cuve1, 2); Serial.print(",");
    Serial.print(t_cuve2, 2); Serial.print(",");
    Serial.print(encoder1); Serial.print(",");
    Serial.print(encoder2); Serial.print(",");
    Serial.print(pv_volts, 3);
    Serial.println();
}

/**************************************************************************/
/*!
    @brief  Affiche les données sur le moniteur série
*/
/**************************************************************************/
void afficherDonneesCompletes() {
    Serial.println("=== SYSTÈME SCAD - DONNÉES ===");
    Serial.print("Horodatage: "); Serial.println(getTimestamp());
    
    Serial.println("--- Températures ---");
    Serial.print("Eau: "); Serial.print(sensorEau.getSensor(), 2); Serial.println(" °C");
    Serial.print("Panneau: "); Serial.print(sensorPanneau.getSensor(), 2); Serial.println(" °C");
    Serial.print("Cuve1: "); Serial.print(sensorCuve1.getSensor(), 2); Serial.println(" °C");
    Serial.print("Cuve2: "); Serial.print(sensorCuve2.getSensor(), 2); Serial.println(" °C");
    
    Serial.println("--- Encodeurs ---");
    Serial.print("Enc1: "); Serial.println(enc1.getValue());
    Serial.print("Enc2: "); Serial.println(enc2.getValue());
    
    Serial.println("--- PV ---");
    Serial.print("Tension: "); Serial.print(readPVVoltage(), 2); Serial.println(" V");
    
    Serial.println("--- Pompes ---");
    Serial.print("Pompe A: "); Serial.print(pompes.getA()); Serial.println(" %");
    Serial.print("Pompe B: "); Serial.print(pompes.getB()); Serial.println(" %");
    
    Serial.println("===============================");
}

/**************************************************************************/
/*!
    @brief  Affichage OLED
*/
/**************************************************************************/
void afficherOLED() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    
    // Titre avec timestamp
    display.setCursor(0, 0);
    display.print("SCAD ");
    String time = getTimestamp();
    if (time.length() > 15) time = time.substring(time.length() - 8);
    display.println(time);
    
    display.drawLine(0, 9, 127, 9, SSD1306_WHITE);
    
    // Températures
    display.setCursor(0, 12);
    display.print("Eau:");
    display.print(sensorEau.getSensor(), 1);
    display.print(" Pan:");
    display.print(sensorPanneau.getSensor(), 1);
    
    display.setCursor(0, 21);
    display.print("C1:");
    display.print(sensorCuve1.getSensor(), 1);
    display.print(" C2:");
    display.print(sensorCuve2.getSensor(), 1);
    
    // Encodeurs
    display.setCursor(0, 30);
    display.print("Enc1:");
    display.print(enc1.getValue());
    display.print(" Enc2:");
    display.print(enc2.getValue());
    
    // PV
    display.setCursor(0, 39);
    display.print("PV:");
    display.print(readPVVoltage(), 1);
    display.print("V");
    
    // Pompes
    display.setCursor(0, 48);
    display.print("PompeA:");
    display.print(pompes.getA());
    display.print("% B:");
    display.print(pompes.getB());
    display.print("%");
    
    display.display();
}

/**************************************************************************/
/*!
    @brief  Contrôle des pompes via encodeurs
*/
/**************************************************************************/

/**************************************************************************/
/*!
    @brief  Gestion des commandes série simples
*/
/**************************************************************************/
void gererCommandesSerial() {
    if (Serial.available()) {
        String commande = Serial.readStringUntil('\n');
        commande.trim();
        commande.toUpperCase();
        
        if (commande == "STATUS") {
            afficherDonneesCompletes();
        }
        else if (commande == "ENCODEURS") {
            Serial.print("Enc1: "); Serial.print(enc1.getValue());
            Serial.print(" / Enc2: "); Serial.println(enc2.getValue());
        }
        else if (commande == "POMPES") {
            Serial.print("PompeA: "); Serial.print(pompes.getA());
            Serial.print("% / PompeB: "); Serial.print(pompes.getB()); Serial.println("%");
        }
        else if (commande == "STOP") {
            pompes.stopAll();
            Serial.println("Pompes arrêtées");
        }
        else if (commande == "UART") {
            sendFrameUART();
        }
        else if (commande == "HELP") {
            Serial.println("=== COMMANDES ===");
            Serial.println("STATUS - État système");
            Serial.println("ENCODEURS - État encodeurs");
            Serial.println("POMPES - État pompes");
            Serial.println("STOP - Arrêt pompes");
            Serial.println("UART - Envoi UART manuel");
            Serial.println("HELP - Cette aide");
        }
    }
}

/**************************************************************************/
/*!
    @brief  Setup - Initialisation
*/
/**************************************************************************/
void setup() {
    Serial.begin(9600);
    Serial.println("=== DÉMARRAGE SYSTÈME SCAD ===");
    
    // Initialisation capteurs
    Serial.println("Init capteurs...");
    sensorEau.begin();
    sensorPanneau.begin();
    sensorCuve1.begin();
    sensorCuve2.begin();
    
    // Initialisation encodeurs et pompes
    Serial.println("Init encodeurs et pompes...");
    enc1.begin();
    enc2.begin();
    pompes.begin();
    
    // Initialisation RTC
    Serial.println("Init RTC...");
    if (rtc.begin()) {
        Serial.println("RTC OK");
        if (rtc.lostPower()) {
            Serial.println("RTC sync nécessaire");
            rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Décommentez si besoin
        }
    } else {
        Serial.println("RTC non détecté");
    }
    
    // Initialisation OLED
    Serial.println("Init OLED...");
    initOLED();
    splashABMI();
    
    delay(2000);
    Serial.println("=== SYSTÈME PRÊT ===");
    Serial.println("Tapez HELP pour les commandes");
}

/**************************************************************************/
/*!
    @brief  Loop principal
*/
/**************************************************************************/
void loop() {
    // MISE À JOUR ENCODEURS (PRIORITÉ!)
    enc1.update();
    enc2.update();
    
    int vitesseA = enc1.getValue();
    int vitesseB = enc2.getValue();
    
    pompes.setA(vitesseA);
    pompes.setB(vitesseB);
    
    // Lecture capteurs
    sensorEau.readAll();
    sensorPanneau.readAll();
    sensorCuve1.readAll();
    sensorCuve2.readAll();

 
    
    // Envoi UART périodique
    if (millis() - dernier_envoi_uart >= INTERVALLE_UART) {
        sendFrameUART();
        dernier_envoi_uart = millis();
    }
    
    // Gestion commandes série
    gererCommandesSerial();
    
    // Affichage date et heure 
    getTimestamp() ;

    // Affichage série périodique
    if (millis() - dernier_affichage_serial >= INTERVALLE_SERIAL) {
        afficherDonneesCompletes();
        dernier_affichage_serial = millis();
    }
    
    // Mise à jour OLED
    if (millis() - dernier_affichage_oled >= INTERVALLE_OLED) {
        afficherOLED();
        dernier_affichage_oled = millis();
    }
    
    // Contrôle pompes
    /*if (millis() - dernier_controle_pompes >= INTERVALLE_POMPES) {
        controlerPompes();
        dernier_controle_pompes = millis();
    }
    */
    delay(5); // Délai court pour réactivité encodeurs
}