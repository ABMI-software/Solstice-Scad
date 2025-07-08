#ifndef HARDWARE_H
#define HARDWARE_H

// === LIBRAIRIES GLOBALES ===
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <max6675.h>

// === LIBRAIRIES matérielles===
#include "sensors/sensors.c"
#include "display/display.c"
#include "pumps/pumps.c"
#include "encoders/encoders.c"
#include "power/power.c"

// Encodeur 1
#define ENCODER1_CLK 2
#define ENCODER1_DT 3
#define ENCODER1_SW 4

// Encodeur 2
#define ENCODER2_CLK 5
#define ENCODER2_DT 6
#define ENCODER2_SW 7

// OLED I2C
#define OLED_SDA 20
#define OLED_SCL 21

// Sonde 1
#define THERMO1_CS 10
// Sonde 2
#define THERMO2_CS 11
// Sonde 3
#define THERMO3_CS 8
// Sonde 4
#define THERMO4_CS 9
#define THERMO_SCK 13
#define THERMO_SO 12

// Pompe 1
#define POMPE1_IN1 22
#define POMPE1_IN2 23
#define POMPE1_ENA 26

// Pompe 2
#define POMPE2_IN3 24
#define POMPE2_IN4 25
#define POMPE2_ENB 27

// Panneau PV
#define PV_VOUT A0

// Régulateur
#define REGULATEUR_VOUT A1

// Interrupteur ON/OFF
#define SWITCH_PIN 30

#endif
