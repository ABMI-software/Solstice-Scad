/*! 
    @file pins.h
    @brief Pin definitions for the Arduino SCAD project
    Contains pin assignments for sensors, pumps, encoders, and OLED display
*/

/**************************************************************************/
// pins.h
// This file contains the pin definitions for the Arduino SCAD project.
// It includes definitions for sensors, pumps, encoders, and the OLED display.
// It is included in the config.h file to centralize all pin assignments.
// This file is part of the Arduino SCAD project and is used to configure the hardware setup.
// It is included in the config.h file to centralize all pin assignments.
/**************************************************************************/

#ifndef PINS_H
#define PINS_H  
/**************************************************************************/

// === Définition des broches pour les Encodeurs ===
// Encodeur 1
#define ENCODER1_CLK 2
#define ENCODER1_DT 3
#define ENCODER1_SW 4

// Encodeur 2
#define ENCODER2_CLK 5
#define ENCODER2_DT 6
#define ENCODER2_SW 7

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
// Les pompes sont contrôlées par des ponts en H, donc 2 broches par pompe
// Les broches IN1 et IN2 sont pour la pompe 1, IN3 et IN4 pour la pompe 2
// Les broches ENA et ENB sont pour le contrôle de la vitesse des pompes
// Pompe 1
#define POMPE1_IN1 22
#define POMPE1_IN2 23
#define POMPE1_ENA 26

// Pompe 2
#define POMPE2_IN3 24
#define POMPE2_IN4 25
#define POMPE2_ENB 27

/**************************************************************************/
//  === Définition des broches pour le panneau PV

#define PV_VOUT A0

/**************************************************************************/

// === Définition des broches pour le module GPS ===
#define GPS_RX 18  // Pin RX du GPS 
#define GPS_TX 19  // Pin TX du GPS 



#endif // PINS_H

