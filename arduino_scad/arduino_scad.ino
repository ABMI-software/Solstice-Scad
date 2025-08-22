
/**************************************************************************
 * @file arduino_scad.ino
 * @brief Main file for the Arduino SCAD project
 * This file initializes the OLED display and simulates sensor readings.
 * It is part of the Arduino SCAD project and is used to test the OLED display functionality.
 **************************************************************************/

#include <Arduino.h>
/*#include "config.h"
#include "sensors.h"
#include "GPS.h"
#include "pump.h"
#include "pins.h"*/

// Create an instance of PumpController
//PumpController pumps;

void setup() {
  Serial.begin(9600);
  pumps.begin();
  //Serial.println(F("Cmds: A=##  B=##  (0..100)  STOP"));
  Serial.println(F("Simulating pump control..."));
}

void loop() {
  /*// Contrôle par le Serial: ex. "A=60" ou "B=30" ou "STOP"
  if (Serial.available()) {
    String s = Serial.readStringUntil('\n'); s.trim();
    if (s.startsWith("A=")) pumps.setA(s.substring(2).toInt());
    else if (s.startsWith("B=")) pumps.setB(s.substring(2).toInt());
    else if (s == "STOP") pumps.stopAll();

    Serial.print(F("A=")); Serial.print(pumps.getA());
    Serial.print(F("%  B=")); Serial.println(pumps.getB());
  }
    */
   Serial.println(F("Simulating pump control..."));
}

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

