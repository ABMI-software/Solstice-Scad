#ifndef CONFIG_H
#define CONFIG_H

// === LIBRAIRIES GLOBALES ===
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <max6675.h>

// Nombre de lectures pour la moyenne
const int NUM_READINGS = 4;

// Intervalle entre les lectures en ms
const unsigned long SENSOR_INTERVAL = 5000;

// Seuil de température 
const float TEMP_THRESHOLD = 100.0;

#endif
