/**
 * sensors.h - Gestion des capteurs de température (MAX6675)
 */

 #ifndef SENSORS_H
 #define SENSORS_H
 #include <Wire.h>
 #include <Adafruit_GFX.h>
 #include <Adafruit_SSD1306.h>
 #include <max6675.h>
 
 // Nombres de capteurs de température
 #define NUM_SENSORS 4
 
 // Prototypes des fonctions
 void initSensors();
 void readAllTemperatures();
 float getTemperature(uint8_t sensorIndex);
 void printSensorData();
 
 // Variables externes pour les températures
 extern float temperatures[NUM_SENSORS];
 
 #endif