/**************************************************************************/
/*!
    @file config.h
    @brief Configuration file for the Arduino SCAD project
    Contains global constants and settings
*/
#ifndef CONFIG_H
#define CONFIG_H

/**************************************************************************/
// === LIBRAIRIES ===

// Libraries for OLED display
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
// Libraries for pumps 
#include <SPI.h>
#include <TimeLib.h>
#include <math.h>
// Libraries for sensors and modules

#include <max6675.h>
// Libraries for GPS module
#include <SoftwareSerial.h>
#include <TinyGPS++.h>


/**************************************************************************/

// === DÉFINITION DES BROCHES ===
#include "pins.h"   
// === DÉFINITION DE L'ÉCRAN OLED ===
#include "oled.h"
// === DÉFINITION DES POMPES ===
#include "pump.h"
// === DÉFINITION DE LA COMMUNICATION SÉRIE ===
#include "serial_comm.h"

// === DÉFINITION DES CONSTANTES ===
const int NUM_SENSORS = 4; // Nombre total de capteurs
const int NUM_PUMPS = 2; // Nombre total de pompes
const int NUM_ENCODERS = 2; // Nombre total d'encodeurs
const int NUM_READINGS = 4; // Nombre de lectures pour la moyenne
const unsigned long SENSOR_INTERVAL = 5000; // Intervalle entre les lectures en ms
const float TEMP_THRESHOLD = 100.0; // Seuil de température pour l'alerte


// === CONFIGURATION AFFICHAGE ===
const unsigned long INTERVALLE_SERIAL = 3000;    // 3 secondes
const unsigned long INTERVALLE_OLED = 1000;      // 1 seconde  
const unsigned long INTERVALLE_POMPES = 2000;    // 2 secondes
const unsigned long INTERVALLE_UART = 5000;      //  5 secondes pour UART
const unsigned long INTERVALLE_SYNC_TIME = 60000; //  1 minute pour sync GPS->RTC

/**************************************************************************/
#endif // CONFIG_H
/**************************************************************************/
