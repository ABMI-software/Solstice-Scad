#ifndef PINS_H
#define PINS_H

// Encodeur 1
#define ENCODER1_CLK 2
#define ENCODER1_DT 3
#define ENCODER1_SW 4

// Encodeur 2
#define ENCODER2_CLK 5
#define ENCODER2_DT 6
#define ENCODER2_SW 7

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

// OLED I2C
#define OLED_SDA 20
#define OLED_SCL 21

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


#endif
