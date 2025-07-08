#include "hardware.h"


// Initialisation des capteurs
void initSensors();

// Lecture de toutes les températures
void readSensors();

// Accès aux températures (°C)
float getTempCuve1();
float getTempCuve2();
float getTempPanneau1();
float getTempPanneau2();

// Déclaration des capteurs
MAX6675 capteurCuve1(THERMO_SCK, THERMO1_CS, THERMO_SO);
MAX6675 capteurCuve2(THERMO_SCK, THERMO2_CS, THERMO_SO);
MAX6675 capteurPanneau1(THERMO_SCK, THERMO3_CS, THERMO_SO);
MAX6675 capteurPanneau2(THERMO_SCK, THERMO4_CS, THERMO_SO);

// Températures stockées en mémoire
float tempCuve1 = 0;
float tempCuve2 = 0;
float tempPanneau1 = 0;
float tempPanneau2 = 0;

void initSensors() {
  // Rien à faire ici pour les MAX6675
  delay(500); // petit délai de stabilisation
}

void readSensors() {
  tempCuve1     = capteurCuve1.readCelsius();
  tempCuve2     = capteurCuve2.readCelsius();
  tempPanneau1  = capteurPanneau1.readCelsius();
  tempPanneau2  = capteurPanneau2.readCelsius();
}

float getTempCuve1()     { return tempCuve1; }
float getTempCuve2()     { return tempCuve2; }
float getTempPanneau1()  { return tempPanneau1; }
float getTempPanneau2()  { return tempPanneau2; }
