
/**************************************************************************
 * @file arduino_scad.ino
 * @brief Main file for the Arduino SCAD project
 * This file initializes the OLED display and simulates sensor readings.
 * It is part of the Arduino SCAD project and is used to test the OLED display functionality.
 **************************************************************************/

#include <Arduino.h>
#include "config.h"
#include "sensors.h"
#include "GPS.h"
#include "pump.h"
#include "pins.h"

#include "pump.h"
#include <Arduino.h>
#include "pins.h"



/*
// __________________________________________________________________________________________________
//test des pompes et des encodeurs
// Converts percentage (0-100) to PWM value (0-255)
static inline uint8_t pct2pwm(int pct) {
  pct = constrain(pct, 0, 100);
  return map(pct, 0, 100, 0, 255);
}

void PumpController::begin() {
    // Initialize pump control pins
    pinMode(POMPE1_IN1, OUTPUT); 
    pinMode(POMPE1_IN2, OUTPUT); 
    pinMode(POMPE1_ENA, OUTPUT); 
    pinMode(POMPE2_IN3, OUTPUT); 
    pinMode(POMPE2_IN4, OUTPUT); 
    pinMode(POMPE2_ENB, OUTPUT);
    
    // Set initial states - all LOW initially
    digitalWrite(POMPE1_ENA, LOW); 
    digitalWrite(POMPE2_ENB, LOW);
    digitalWrite(POMPE1_IN1, LOW); 
    digitalWrite(POMPE1_IN2, LOW);
    digitalWrite(POMPE2_IN3, LOW); 
    digitalWrite(POMPE2_IN4, LOW);
    
    // Set direction for both pumps (adjust if reversed)
    digitalWrite(POMPE1_IN1, HIGH); 
    digitalWrite(POMPE1_IN2, LOW);
    digitalWrite(POMPE2_IN3, LOW);  
    digitalWrite(POMPE2_IN4, HIGH);

    // Initialize pumps to 0%
    setA(0); 
    setB(0);
}

void PumpController::applyA() { 
    analogWrite(POMPE1_ENA, pct2pwm(a_pct)); 
}

void PumpController::applyB() { 
    analogWrite(POMPE2_ENB, pct2pwm(b_pct)); 
}

void PumpController::setA(int pct) { 
    a_pct = constrain(pct, 0, 100); 
    applyA(); 
}

void PumpController::setB(int pct) { 
    b_pct = constrain(pct, 0, 100); 
    applyB(); 
}

void PumpController::stopAll() { 
    a_pct = 0; 
    b_pct = 0; 
    applyA(); 
    applyB(); 
}

// ========== ENCODERCONTROLLER IMPLEMENTATION ==========

EncoderController::EncoderController(int clk, int dt) 
    : clkPin(clk), dtPin(dt), compteur(0), lastClkState(LOW) {
}

void EncoderController::begin() {
    pinMode(clkPin, INPUT);
    pinMode(dtPin, INPUT);
    
    lastClkState = digitalRead(clkPin);
}

void EncoderController::update() {
    // Lecture de l'encodeur rotatif
    int currentClk = digitalRead(clkPin);
    
    if (currentClk != lastClkState) {
        if (digitalRead(dtPin) != currentClk) {
            compteur++;  // Rotation horaire
        } else {
            compteur--;  // Rotation anti-horaire
        }
        
        // Limiter entre 0 et 100 (pourcentage PWM)
        compteur = constrain(compteur, 0, 100);
        
        // Optional: Serial debugging
        Serial.print("Encodeur: ");
        Serial.println(compteur);
    }
    lastClkState = currentClk;
}

void EncoderController::setValue(int value) {
    compteur = constrain(value, 0, 100);
}

//________________________________________________________________________________  
//test des capteurs et de l'afficheur OLED
/*
//Create an instance of SensorModule
SensorModule sensorPtc1(31, 28, 30);
SensorModule sensorPtc2(37, 34, 35);
SensorModule sensorCuve1(32, 33, 29);
SensorModule sensorCuve2(27, 26, 25);

// Create an instance of GPSModule
GPSModule gpsModule;
void setup() {
    Serial.begin(9600);

    // Initialize the GPS module
    gpsModule.begin();
    // Initialize the  OLED display
    initOLED();
    splashABMI();

   delay(1000); // Wait for a second before starting the main loop
    Serial.println("Setup complete. Starting main loop...");
}


void loop() {

    // Read GPS data
    // Check if GPS data is valid 
    if (gpsModule.update()) { // Remplace 'update' par le nom correct de ta méthode
    Serial.println("Date: " + gpsModule.getDate());
    Serial.println("Time: " + gpsModule.getTime());
    } else {
        Serial.println("GPS data not valid");
    }
    delay(1000); 

    // Simule des données en boucle
    sensorPtc1.readAll(); // Read PTC1 sensor
    sensorPtc2.readAll(); // Read PTC2 sensor
    sensorCuve1.readAll(); // Read Cuve1 sensor
    sensorCuve2.readAll(); // Read Cuve2 sensor
    // Print the average temperatures to the Serial Monitor

    bool mode_ete = true;

    float t1 = sensorPtc1.getSensor();
    float t2 = sensorPtc2.getSensor();
    float c1 = sensorCuve1.getSensor();
    float c2 = sensorCuve2.getSensor();
    

    Serial.print("PTC1 Average Temperature: ");
    Serial.println(t1);
    Serial.print("PTC2 Average Temperature: ");
    Serial.println(t2);
    Serial.print("Cuve1 Average Temperature: ");
    Serial.println(c1);
    Serial.print("Cuve2 Average Temperature: ");
    Serial.println(c2);
    delay(1000); // Wait for a second before the next reading

    // Heure simulée pour test
    String heure = "15:40:00";// Simulated time for testing

    // Affichage OLED
    afficherInfos(t1, t2, c1, c2, gpsModule);

    delay(2000);

    
}*/

