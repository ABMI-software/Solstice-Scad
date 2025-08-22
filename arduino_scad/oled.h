#ifndef OLED_H
#define OLED_H

/**************************************************************************/
/*!
    @file oled.h
    @brief Header file for OLED display functions
    Contains function declarations for initializing and displaying information on the OLED screen
*/
/**************************************************************************/
#include "config.h"
#include "pins.h" // Include pin definitions
/**************************************************************************/
#include <Arduino.h>
/**************************************************************************/

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include "GPS.h" // Include GPS module header
// === Initialisation de l’écran global ===
extern Adafruit_SSD1306 display;

void initOLED();
void afficherTemp(float ptc1, float ptc2, float cuve1, float cuve2, const String& heure);

void splashABMI();
void afficherInfos(float t1, float t2, float c1, float c2, GPSModule& gpsModule);

/**************************************************************************/
#endif
