/**
 * hardware.h - Définition des constantes et des broches pour le projet
 */

 #ifndef HARDWARE_H
 #define HARDWARE_H
 #include <Wire.h>
 
 // Configuration des broches pour les modules MAX6675 (CAPTEURS)
 // Chaque module MAX6675 nécessite 3 broches: CS, SCK, MISO
 // SCK et MISO sont partagés sur la bus SPI
 #define SENSORS_SCK 27  // Broche SCK partagée pour les capteurs
 #define SENSORS_MISO 25 // Broche MISO partagée pour les capteurs
 #define SENSORS1_CS 26  // Broche CS pour SENSORS 1
 #define SENSORS2_CS 30  // Broche CS pour SENSORS 2
 #define SENSORS3_CS 33  // Broche CS pour SENSORS 3
 #define SENSORS4_CS 36  // Broche CS pour SENSORS 4
 
 // Configuration des broches pour les encodeurs rotatifs KY-040
 #define ENCODER1_CLK 3  // Broche CLK de l'encodeur 1 (interrupt)
 #define ENCODER1_DT 5   // Broche DT de l'encodeur 1
 #define ENCODER1_SW 6   // Broche SW (bouton) de l'encodeur 1
 #define ENCODER2_CLK 2 // Broche CLK de l'encodeur 2 (interrupt)
 #define ENCODER2_DT 4  // Broche DT de l'encodeur 2
 #define ENCODER2_SW 7  // Broche SW (bouton) de l'encodeur 2
 
 // Configuration des broches pour le driver de moteur L298N
 #define PUMP1_ENA 8    // Broche PWM pour la vitesse de la pompe 1
 #define PUMP1_IN1 51    // Broche de contrôle 1 pour la pompe 1
 #define PUMP1_IN2 11   // Broche de contrôle 2 pour la pompe 1
 #define PUMP2_ENB 10   // Broche PWM pour la vitesse de la pompe 2
 #define PUMP2_IN3 12   // Broche de contrôle 1 pour la pompe 2
 #define PUMP2_IN4 51   // Broche de contrôle 2 pour la pompe 2 (corrigé Pump2 -> PUMP2)
 
 // Configuration des broches pour l'écran OLED I2C
 // Utilise les broches SDA et SCL standard de l'Arduino Mega
 #define OLED_RESET -1   // Partage reset avec Arduino
 #define SCREEN_WIDTH 128 // Largeur de l'écran OLED en pixels
 #define SCREEN_HEIGHT 64 // Hauteur de l'écran OLED en pixels
 #define OLED_ADDRESS 0x3C // Adresse I2C de l'écran OLED (généralement 0x3C ou 0x3D)
 
 // Configuration des broches pour le module carte SD
 #define SD_CS 53        // Broche CS pour la carte SD
 
 // Configuration des broches pour mesurer les tensions (pour power.cpp)
 #define PV_VOUT A1      // Tension du panneau solaire

 
 // Autres constantes
 #define PWM_MIN 0       // Valeur PWM minimale
 #define PWM_MAX 255     // Valeur PWM maximale
 #define ENCODER_STEPS 20 // Nombre de pas pour un tour complet de l'encodeur
 
 // Prototypes des fonctions
 void initHardware();
 
 #endif