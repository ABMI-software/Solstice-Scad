/*! 
    @file pins.h
    @brief Pin definitions for the Arduino SCAD project
    Contains pin assignments for sensors, pumps, encoders, and OLED display
*/
/**************************************************************************/

#ifndef PINS_H
#define PINS_H  

/**************************************************************************/
// === Définition des broches pour les Encodeurs ===
// Encodeur 1
#define ENCODER1_CLK 2
#define ENCODER1_DT 5
#define ENCODER1_SW 7

// Encodeur 2
#define ENCODER2_CLK 3
#define ENCODER2_DT 19  // Changé pour éviter conflit avec GPS_TX
#define ENCODER2_SW 4

/**************************************************************************/
// === Définition des broches pour les sondes MAX6675 ===
#define PTC1_CS  28
#define PTC1_SO  30
#define PTC1_CLK 31

#define PTC2_CS  34
#define PTC2_SO  35
#define PTC2_CLK 37

#define CUVE1_CS  33
#define CUVE1_SO  29
#define CUVE1_CLK 32

#define CUVE2_CS  26
#define CUVE2_SO  25
#define CUVE2_CLK 27

/**************************************************************************/
// === Définition des broches pour OLED I2C ===
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C

/**************************************************************************/
// === Définition des broches pour les pompes ===
// Pompe 1
#define POMPE1_IN1 22
#define POMPE1_IN2 23
#define POMPE1_ENA 8  // Changé de 26 à 8 (conflit résolu)

// Pompe 2
#define POMPE2_IN3 24
#define POMPE2_IN4 25
#define POMPE2_ENB 9  // Changé de 27 à 9 (conflit résolu)

/**************************************************************************/
// === Définition des broches pour le panneau PV ===
#define PV_VOUT A1

/**************************************************************************/
// === Définition des broches pour le module GPS ===
#define GPS_RX 14  // Pin RX du GPS 
#define GPS_TX 15  // Pin TX du GPS 

#endif // PINS_H