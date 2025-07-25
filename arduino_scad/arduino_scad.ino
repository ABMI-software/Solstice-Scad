
/**************************************************************************
 * @file arduino_scad.ino
 * @brief Main file for the Arduino SCAD project
 * This file initializes the OLED display and simulates sensor readings.
 * It is part of the Arduino SCAD project and is used to test the OLED display functionality.
 **************************************************************************/
#include <Arduino.h>
#include "config.h"


/**************************************************************************/
/*!
    @brief  Method to display sensor readings on the OLED screen
    @param   ptc1 Temperature reading from PTC1 sensor
    @param   ptc2 Temperature reading from PTC2 sensor
    @param   cuve1 Temperature reading from CUVE1 sensor
    @param   cuve2 Temperature reading from CUVE2 sensor
    @param   heure Current time in HH:MM:SS format
*/ 
/**************************************************************************/
/*!
    @brief  Main setup function
    Initializes the serial communication and OLED display
*/
void setup() {
    Serial.begin(9600);
    initOLED();
    splashABMI();

    // Test d'affichage
    afficherInfos(42.3, 41.7, 38.2, 37.8, "12:45:30", true);
}

/**************************************************************************/
/*!

    @brief  Main loop function
    Continuously updates the OLED display with simulated sensor readings
*/  
void loop() {
    // Simule des données en boucle
    float ptc1 = 42.3;
    float ptc2 = 41.7;
    float cuve1 = 38.2;
    float cuve2 = 37.8;
    String heure = "14:06:55";
    bool mode_ete = true; // à switcher selon config

    afficherInfos(ptc1, ptc2, cuve1, cuve2, heure, mode_ete);
    delay(2000);
}

