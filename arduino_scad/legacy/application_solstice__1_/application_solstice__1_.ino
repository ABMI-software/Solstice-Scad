#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <math.h>
#include <Arduino.h>
#include <EEPROM.h>
#include <Adafruit_MAX31865.h>
// Pins pour les moteurs pas à pas
#define EN 8
#define X_DIR 5
#define Y_DIR 6
#define Z_DIR 7
#define X_STP 2
#define Y_STP 3
#define Z_STP 4

#define EEPROM_ADDRESS_POSITION 0
#define EEPROM_ADDRESS_PANNEAU_SORTI 4
#define EEPROM_ADDRESS_PANNEAU_RENTRER 8
#define EMERGENCY_STOP_PIN 28  // Exemple de broche pour le bouton d'arrêt d'urgence
// Pins pour les capteurs LDR
#define LDR_NORD A8
#define LDR_SUD A9
// La valeur de la résistance de référence. Utilisez 430.0 pour PT100 et 4300.0 pour PT1000
#define RREF 430.0
// La résistance nominale à 0 degré Celsius
// 100.0 pour PT100, 1000.0 pour PT1000
#define RNOMINAL 100.0
#define TEMP_TARGET 100.0         // Température cible
#define TEMP_THRESHOLD 2.0        // Seuil de tolérance
#define MAX_CYCLES_NO_INCREASE 0  // Nombre maximum de cycles sans augmentation de température
// Utilisation du SPI matériel : CS (SS), DI (MOSI), DO (MISO), CLK (SCK)
Adafruit_MAX31865 thermo1 = Adafruit_MAX31865(53, 51, 50, 52);  // Sonde à l'entrée du tube
Adafruit_MAX31865 thermo2 = Adafruit_MAX31865(49, 47, 48, 46);  // Sonde à la sortie du tube
Adafruit_MAX31865 thermo3 = Adafruit_MAX31865(45, 43, 44, 42);  // Sonde dans la cuve de stockage de stockage

// Constantes pour les paramètres des moteurs pas à pas
const int stepsPerRevolution = 200;
const float degreesPerRevolution = 360.0;
const int stepVis = 100;
const int StepVisPanneau = 10;
bool panneauEnTrainDeRentrer = false;
bool panneauEstRentre = false;
const int LED_PIN = 13;
// Décalages horaires pour le calcul du fuseau horaire local
const int summerOffset = 2;
const int winterOffset = 1;
// Déclaration des pins pour les capteurs d'énergie
const int ENERGY_SENSOR_PIN = A0;  // Pin analogique pour lire l'énergie du panneau
// Variable pour suivre l'état du panneau (true = extérieur, false = intérieur)
bool panneauExterieur = true;
// Constantes pour les calculs des LDR
const float Rref = 10000;
const float RL10 = 50;
const float GAMMA = 0.7;

// Variables globales pour l'état des moteurs et la position actuelle
float currentPosition = 0.0;  // Position actuelle du moteur en degrés
bool panneauSorti = false;
bool panneauRentrer = false;
bool panneauEtatAffiche = false;  // Variable de contrôle pour afficher l'état une seule fois
float altitude_solaire;
float startAltitude;
float endAltitude;
float bestAltitude = 0.0;  // Initialisation de la meilleure altitude
bool emergencyStopActivated = false;
float previousTempSortie = 0;
float previousTempCuve = 0;
int noIncreaseCycles = 0;

// Création d'un objet GPS
TinyGPSPlus gps;

// Initialisation du port série pour le GPS (pins 11 et 10 pour SoftwareSerial)
SoftwareSerial ss(10, 11);

// Prototypes de fonctions
void updateGPSData();
void savePosition(float position);
float loadPosition();
void step(bool dir, byte dirPinX, byte stepperPinX, byte dirPinY, byte stepperPinY, int steps);
void stepMoteurPanneau(bool dir, byte dirPin, byte stepperPin, int steps);
void rentrerPanneau();
void sortirPanneau();
void afficherDonneesGPS(int jour, int mois, int annee, int heure, int minute, int seconde, float latitude, float longitude, float altitude_solaire);
bool isSummerTime(int day, int month, int hour, int year);
int jour_de_l_an(int jour, int mois, int annee);
float calculAltitudeSolaire(int jour, float latitude, int heure, int minute);
void initialiserSysteme();
void afficherEtatPanneau();
void delayMinutes(unsigned long minutes);
float lireLDR_NORD();
float lireLDR_SUD();
void readAndPrintTemperatures();
void controleDelta(float tempEntree, float tempSortie, float tempCuve);
void monitorTemperatureIncrease(float currentTempSortie, float currentTempCuve);
void positionnerMoteurX();
void allumerLED();
void arreterProgramme();
float lireEnergie();
bool toutesLesTemperaturesInferieuresA(float seuil);
bool toutesLesTemperaturesSuperieuresA(float seuil);
void arreterProgramme();
// Déclaration des variables pour la temporisation
unsigned long previousUpdateTime = 0;
const unsigned long updateInterval = 60000;  // Intervalle de 1 minutes en millisecondes

// Fonction pour l'initialisation du système
void initialiserSysteme() {
  // Chargement de la position actuelle et de l'état du panneau depuis l'EEPROM
  currentPosition = loadPosition();

  // Si le panneau n'est pas sorti au démarrage, le sortir
  if (!panneauSorti && !panneauRentrer) {
    sortirPanneau();
  }
}
// Fonction pour le traitement des données GPS
void updateGPSData() {
  // Date et Heure
  int year = gps.date.year();
  int month = gps.date.month();
  int day = gps.date.day();
  int hour = gps.time.hour();
  int minute = gps.time.minute();
  int second = gps.time.second();

  // Calcul du jour de l'année
  int day_of_year = jour_de_l_an(day, month, year);

  // Calcul du décalage horaire en fonction de l'heure d'été ou d'hiver
  int timeOffset = isSummerTime(day, month, hour, year) ? summerOffset : winterOffset;

  // Ajustement de l'heure pour le fuseau horaire local
  hour += timeOffset;
  if (hour >= 24) {
    hour -= 24;
    day += 1;
  } else if (hour < 0) {
    hour += 24;
    day -= 1;
  }

  // Latitude et Longitude
  float latitude = gps.location.lat();
  float longitude = gps.location.lng();

  // Calcul de l'altitude solaire
  altitude_solaire = calculAltitudeSolaire(day_of_year, latitude, hour, minute);

  // Appel à la fonction pour afficher les données GPS et altitude solaire
  afficherDonneesGPS(day, month, year, hour, minute, second, latitude, longitude, altitude_solaire);
}

// Fonction pour afficher les données GPS et altitude solaire
void afficherDonneesGPS(int jour, int mois, int annee, int heure, int minute, int seconde, float latitude, float longitude, float altitude_solaire) {
  Serial.print("Date : ");
  Serial.print(jour);
  Serial.print("/");
  Serial.print(mois);
  Serial.print("/");
  Serial.print(annee);
  Serial.print(" Heure : ");
  Serial.print(heure);
  Serial.print(":");
  Serial.print(minute);
  Serial.print(":");
  Serial.print(seconde);

  Serial.print(" Latitude : ");
  Serial.print(latitude, 6);
  Serial.print(" Longitude : ");
  Serial.print(longitude, 6);

  Serial.print(" Jour de l'année : ");
  Serial.print(jour_de_l_an(jour, mois, annee));

  Serial.print(" Altitude solaire : ");
  Serial.println(altitude_solaire);
  delay(2000);
}

// Fonction pour déterminer si on est en heure d'été
bool isSummerTime(int day, int month, int hour, int year) {
  if (month > 3 && month < 10) {
    return true;
  } else if (month == 3) {
    int lastSunday = (31 - (5 + year * 5 / 4) % 7);
    return (day > lastSunday) || (day == lastSunday && hour >= 2);
  } else if (month == 10) {
    int lastSunday = (31 - (5 + year * 5 / 4) % 7);
    return (day < lastSunday) || (day == lastSunday && hour < 3);
  }
  return false;
}

// Fonction pour calculer le jour de l'année
int jour_de_l_an(int jour, int mois, int annee) {
  int jours_dans_mois[] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };
  int jour_de_l_an = jours_dans_mois[mois - 1] + jour;

  if ((annee % 4 == 0 && annee % 100 != 0) || annee % 400 == 0) {
    // Année bissextile, ajuster pour février
    if (mois > 2) {
      jour_de_l_an += 1;
    }
  }

  return jour_de_l_an;
}

// Fonction pour calculer l'altitude solaire avec arrondi à 2 décimales
float calculAltitudeSolaire(int jour, float latitude, int heure, int minute) {
  // Convertir l'heure en heures décimales
  float heures_decimales = heure + minute / 60.0;

  // Calcul de la déclinaison solaire
  float delta = 23.45 * sin((jour - 81) * 2 * M_PI / 365.24);

  // Calcul de l'angle horaire.
  float w = 15 * (heures_decimales - 12);

  // Calcul d'altitude solaire
  float altitude_radians = asin(sin(radians(delta)) * sin(radians(latitude)) + cos(radians(delta)) * cos(radians(latitude)) * cos(radians(w)));

  // Conversion de l'altitude solaire en degrés
  float altitude_solaire = degrees(altitude_radians);

  return altitude_solaire;
}
void allumerLED() {
  digitalWrite(LED_PIN, HIGH);  // Allumer la LED (assurez-vous que LED_PIN est bien défini)
  delay(2000);
}
bool toutesLesTemperaturesInferieuresA(float seuil) {
  // Lecture des températures actuelles des trois capteurs
  float tempEntree = thermo1.temperature(RNOMINAL, RREF);
  float tempSortie = thermo2.temperature(RNOMINAL, RREF);
  float tempCuve = thermo3.temperature(RNOMINAL, RREF);

  // Vérifier que toutes les températures sont inférieures au seuil donné
  return (tempEntree < seuil && tempSortie < seuil && tempCuve < seuil);
}
bool toutesLesTemperaturesSuperieuresA(float seuil) {
  // Lecture des températures actuelles des trois capteurs
  float tempEntree = thermo1.temperature(RNOMINAL, RREF);
  float tempSortie = thermo2.temperature(RNOMINAL, RREF);
  float tempCuve = thermo3.temperature(RNOMINAL, RREF);

  // Vérifier que toutes les températures sont supérieures au seuil donné
  return (tempEntree > seuil && tempSortie > seuil && tempCuve > seuil);
}
void arreterProgramme() {
  Serial.println("Arrêt d'urgence activé !");
  while (true) {
    // Désactiver les moteurs, éteindre les LEDs, etc.
    digitalWrite(EN, HIGH);  // Désactiver les moteurs en mettant EN à HIGH
    // LED clignotante supprimée, uniquement l'arrêt des moteurs
  }
}
// Fonctions pour le contrôle des moteurs pas à pas
void step(bool dir, byte dirPinX, byte stepperPinX, byte dirPinY, byte stepperPinY, int steps) {
  digitalWrite(dirPinX, dir);
  digitalWrite(dirPinY, dir);
  delay(100);
  for (int i = 0; i < steps; i++) {
    digitalWrite(stepperPinX, HIGH);
    digitalWrite(stepperPinY, HIGH);
    delayMicroseconds(700);
    digitalWrite(stepperPinX, LOW);
    digitalWrite(stepperPinY, LOW);
    delayMicroseconds(700);
  }
}

void stepMoteurPanneau(bool dir, byte dirPin, byte stepperPin, int steps) {
  digitalWrite(dirPin, dir);
  delay(100);
  for (int i = 0; i < steps; i++) {
    digitalWrite(stepperPin, HIGH);
    delayMicroseconds(700);
    digitalWrite(stepperPin, LOW);
    delayMicroseconds(700);
  }
}
/*
void positionnerMoteurX() {
    Serial.print("Position actuelle: ");
    Serial.println(currentPosition);
    float angle=90;
    Serial.print("Déplacement demandé: ");
    Serial.println(angle);
    // Calculer le nombre de pas nécessaires pour l'angle donné
    float turns = angle / degreesPerRevolution;
    int steps = turns * stepsPerRevolution;

    // Mettre la direction en avant si l'angle est positif, sinon en arrière
    bool direction = (angle >= 0);

    // Effectuer le mouvement
    digitalWrite(X_DIR, direction); // Définir la direction
    delay(100); // Petite pause pour stabiliser la direction

    for (int i = 0; i < abs(steps); i++) {
        digitalWrite(X_STP, HIGH); // Émettre un pas
        delayMicroseconds(700); // Durée du pas
        digitalWrite(X_STP, LOW); // Arrêter le pas
        delayMicroseconds(700); // Durée de l'arrêt
    }

    // Mettre à jour la position actuelle
    currentPosition += angle; // Mettre à jour la position actuelle
    savePosition(currentPosition); // Sauvegarder la nouvelle position

    Serial.print("Nouvelle position: ");
    Serial.println(currentPosition);
}
*/
float lireEnergie() {
  // Lire et retourner l'énergie captée
  return analogRead(ENERGY_SENSOR_PIN);
}
void sortirPanneau() {
  Serial.println("Sortie du panneau...");
  stepMoteurPanneau(false, Z_DIR, Z_STP, StepVisPanneau);
  panneauSorti = true;
  panneauRentrer = false;
  EEPROM.put(EEPROM_ADDRESS_PANNEAU_SORTI, panneauSorti);
  EEPROM.put(EEPROM_ADDRESS_PANNEAU_RENTRER, panneauRentrer);
  delay(5000);  // Ajustez le délai selon le temps nécessaire pour le mouvement
  //Serial.print("Panneau sorti. Valeur EEPROM : ");
  //Serial.print(panneauSorti);
  //Serial.print(" Valeur EEPROM Panneau Rentré : ");
  //Serial.println(panneauRentrer);
}
void rentrerPanneau() {
  // Vérifier si le panneau est déjà rentré
  if (panneauRentrer) {
    Serial.println("Le panneau est déjà rentré.");
    return;
  }
  Serial.println("Rentrée du panneau...");
  // Calculer le nombre de pas nécessaires pour rentrer le panneau
  int stepsPanneau = StepVisPanneau * stepsPerRevolution;
  // Effectuer le mouvement pour rentrer le panneau
  stepMoteurPanneau(true, Z_DIR, Z_STP, stepsPanneau);
  // Mettre à jour l'état du panneau
  panneauRentrer = true;
  panneauSorti = false;
  // Sauvegarder l'état du panneau dans l'EEPROM
  savePanneauState(panneauSorti, panneauRentrer);
  Serial.println("Panneau rentré.");
  delay(2000);  // Ajustez le délai selon le temps nécessaire pour le mouvement
}

// Fonction pour sauvegarder l'état du panneau
void savePanneauState(bool sorti, bool rentrer) {
  EEPROM.write(EEPROM_ADDRESS_PANNEAU_SORTI, sorti ? 1 : 0);
  EEPROM.write(EEPROM_ADDRESS_PANNEAU_RENTRER, rentrer ? 1 : 0);
}

// Fonction pour lire l'état du panneau
void loadPanneauState(bool& sorti, bool& rentrer) {
  sorti = EEPROM.read(EEPROM_ADDRESS_PANNEAU_SORTI) == 1;
  rentrer = EEPROM.read(EEPROM_ADDRESS_PANNEAU_RENTRER) == 1;
}
// Fonctions pour la gestion de la position
void savePosition(float position) {
  EEPROM.put(EEPROM_ADDRESS_POSITION, position);
  //Serial.print("Position sauvegardée dans EEPROM : ");
  //Serial.println(position);
}

float loadPosition() {
  float position;
  EEPROM.get(EEPROM_ADDRESS_POSITION, position);
  //Serial.print("Position lue depuis EEPROM : ");
  //Serial.println(position);
  return position;
}

// Fonction pour lire et retourner la valeur moyenne des LDR
float lireLDR_NORD() {
  int analogValue1 = analogRead(LDR_NORD);
  float voltage1 = analogValue1 / 1024. * 5;
  float resistance1 = 2000 * voltage1 / (1 - voltage1 / 5);
  float lux_nord = pow(RL10 * 1e3 * pow(10, GAMMA) / resistance1, (1 / GAMMA));
  Serial.print("lux_nord :");
  Serial.println(lux_nord);
  return lux_nord;
}

float lireLDR_SUD() {
  int analogValue2 = analogRead(LDR_SUD);
  float voltage2 = analogValue2 / 1024. * 5;
  float resistance2 = 2000 * voltage2 / (1 - voltage2 / 5);
  float lux_sud = pow(RL10 * 1e3 * pow(10, GAMMA) / resistance2, (1 / GAMMA));
  Serial.print("lux_sud :");
  Serial.println(lux_sud);
  return lux_sud;
}

void rotateMotor(float degrees, float& currentPosition) {
  float difference = degrees - currentPosition;  // Calculer la différence entre la position cible et la position actuelle
  // Si la différence est positive, avancer
  if (difference > 0) {
    float turns = difference / degreesPerRevolution * stepVis;
    int steps = turns * stepsPerRevolution;
    step(false, X_DIR, X_STP, Y_DIR, Y_STP, steps);  // Avancer
    currentPosition += difference;

    // Afficher les informations sur la rotation effectuée
    Serial.print("Rotation ajoutée de ");
    Serial.print(turns);
    Serial.println(" tours complets.");
  }
  // Si la différence est négative, reculer
  else if (difference < 0) {
    float turns = abs(difference) / degreesPerRevolution * stepVis;
    int steps = turns * stepsPerRevolution;
    step(true, X_DIR, X_STP, Y_DIR, Y_STP, steps);  // Reculer
    currentPosition += difference;

    // Afficher les informations sur la rotation effectuée
    Serial.print("Rotation retirée de ");
    Serial.print(turns);
    Serial.println(" tours complets.");
  }
  //savePosition(currentPosition);
}

void balayerEtTrouverMaxLux(float altitudeActuelle, float& bestAltitude, float& currentPosition) {
  // Déclaration des variables locales
  float scanRange = 10.0;                              // Définition de la plage de balayage en degrés
  float scanIncrement = 1.0;                           // Incrément de balayage en degrés
  float startAltitude = altitudeActuelle - scanRange;  // Définition de l'altitude de départ du balayage
  float endAltitude = altitudeActuelle + scanRange;    // Définition de l'altitude de fin du balayage
  float maxLux = 0;                                    // Initialisation de la valeur maximale de lux trouvée
  float previousBestAltitude = bestAltitude;           // Sauvegarde de l'ancienne meilleure altitude
  bestAltitude = altitudeActuelle;                     // Initialisation de la meilleure altitude trouvée à l'altitude actuelle

  // Sauvegarde de la position initiale
  float initialPosition = currentPosition;

  // Balayage positif (+10 degrés)
  for (float altitude = altitudeActuelle; altitude <= endAltitude; altitude += scanIncrement) {
    rotateMotor(altitude, currentPosition);  // Positionne les moteurs X et Y à l'altitude actuelle du balayage

    int lux_nord = lireLDR_NORD();  // Mesure la luminosité à cette altitude
    // Compare avec le maximum actuel
    if (lux_nord > maxLux) {
      maxLux = lux_nord;        // Met à jour la valeur maximale de lux trouvée
      bestAltitude = altitude;  // Met à jour la meilleure altitude trouvée
    }

    delay(100);  // Attend un court délai entre chaque mesure
  }
  // Retour à la position initiale après balayage positif
  rotateMotor(initialPosition, currentPosition);
  // Balayage négatif (-10 degrés)
  for (float altitude = altitudeActuelle; altitude >= startAltitude; altitude -= scanIncrement) {
    rotateMotor(altitude, currentPosition);  // Positionne les moteurs X et Y à l'altitude actuelle du balayage

    int lux_sud = lireLDR_SUD();  // Mesure la luminosité à cette altitude
    // Compare avec le maximum actuel
    if (lux_sud > maxLux) {
      maxLux = lux_sud;         // Met à jour la valeur maximale de lux trouvée
      bestAltitude = altitude;  // Met à jour la meilleure altitude trouvée
    }
    delay(100);  // Attend un court délai entre chaque mesure
  }
  // Retourner à la meilleure altitude trouvée
  rotateMotor(bestAltitude, currentPosition);
  savePosition(currentPosition);
  Serial.print("Valeur maximale de lux trouvée : ");
  Serial.println(maxLux);
  Serial.print("currentPosition :");
  Serial.println(currentPosition);
}
// Fonction principale Arduino
void setup() {
  // Initialisation des pins des moteurs
  pinMode(EN, OUTPUT);
  pinMode(X_DIR, OUTPUT);
  pinMode(Y_DIR, OUTPUT);
  pinMode(Z_DIR, OUTPUT);
  pinMode(X_STP, OUTPUT);
  pinMode(Y_STP, OUTPUT);
  pinMode(Z_STP, OUTPUT);
  pinMode(EMERGENCY_STOP_PIN, INPUT_PULLUP);
  // Initialisation des capteurs de température
  thermo1.begin(MAX31865_3WIRE);
  thermo2.begin(MAX31865_3WIRE);
  thermo3.begin(MAX31865_3WIRE);

  // Initialisation des pins pour les LDR
  pinMode(LDR_NORD, INPUT);
  pinMode(LDR_SUD, INPUT);

  // Initialisation du port série
  Serial.begin(9600);

  // Initialisation du port série pour le GPS
  ss.begin(9600);

  // Chargement de la position actuelle et de l'état du panneau depuis l'EEPROM
  currentPosition = loadPosition();  // Assurez-vous que cette fonction est définie ailleurs
  loadPanneauState(panneauSorti, panneauRentrer);

  // Vérifier l'état du panneau (sorti ou rentré)
  if (panneauSorti) {
    Serial.println("Panneau déjà sorti.");
  } else if (panneauRentrer) {
    Serial.print("Panneau déjà rentré. Sortir le panneau maintenant...");
    // Sortir le panneau s'il est rentré
    int stepsPanneau = StepVisPanneau * stepsPerRevolution;  // Ajustez selon votre besoin
    stepMoteurPanneau(false, Z_DIR, Z_STP, stepsPanneau);
    panneauSorti = true;
    panneauRentrer = false;                          // Mettre à jour l'état
    savePanneauState(panneauSorti, panneauRentrer);  // Sauvegarder l'état du panneau
    Serial.println("Panneau sorti.");
    // Attendre que le panneau soit complètement sorti avant de continuer
    delay(2000);  // Ajustez le délai selon le temps nécessaire pour le mouvement
  } else {
    Serial.println("Panneau non sorti. Sortir le panneau...");
    int stepsPanneau = StepVisPanneau * stepsPerRevolution;  // Ajustez selon votre besoin
    stepMoteurPanneau(false, Z_DIR, Z_STP, stepsPanneau);
    panneauSorti = true;
    panneauRentrer = false;                          // Mettre à jour l'état
    savePanneauState(panneauSorti, panneauRentrer);  // Sauvegarder l'état du panneau
    Serial.println("Panneau sorti.");
  }

  // Afficher la position de démarrage
  Serial.print("Position de démarrage : ");
  Serial.println(currentPosition);
}
void readAndPrintTemperatures() {
  // Lecture des températures actuelles des trois capteurs
  float tempEntree = thermo1.temperature(RNOMINAL, RREF);
  float tempSortie = thermo2.temperature(RNOMINAL, RREF);
  float tempCuve = thermo3.temperature(RNOMINAL, RREF);
  // Affichage des températures actuelles sur la même ligne
  Serial.print("Température Entrée: ");
  Serial.print(tempEntree, 1);
  Serial.print(" °C\t");
  Serial.print("Température Sortie: ");
  Serial.print(tempSortie, 1);
  Serial.print(" °C\t");
  Serial.print("Température Cuve: ");
  Serial.print(tempCuve, 1);
  Serial.println(" °C");

  // Calcul et affichage des delta T
  controleDelta(tempEntree, tempSortie, tempCuve);

  // Surveillance de l'augmentation des températures
  monitorTemperatureIncrease(tempSortie, tempCuve);

  // Mise à jour des températures précédentes pour la prochaine itération
  previousTempSortie = tempSortie;
  previousTempCuve = tempCuve;

  delay(1000);
}
// Fonction pour calculer et afficher les delta T
void controleDelta(float tempEntree, float tempSortie, float tempCuve) {
  // Ca/lcul des delta T entre l'entrée et la sortie du tube, et entre la sortie du tube et la cuve
  float deltaT_EntreeSortie = tempSortie - tempEntree;
  float deltaT_SortieCuve = tempCuve - tempSortie;

  // Affichage des delta T calculés
  Serial.print("Delta T Entrée-Sortie: ");
  Serial.print(deltaT_EntreeSortie, 1);
  Serial.print(" °C\t");
  Serial.print("Delta T Sortie-Cuve: ");
  Serial.print(deltaT_SortieCuve, 1);
  Serial.println(" °C");

  // Vérification des conditions sur les delta T
  if (deltaT_EntreeSortie > 0) {
    Serial.println("Augmentation de température détectée entre l'entrée et la sortie du tube.");
    delay(5000);
  } else {
    Serial.println("Aucune augmentation de température détectée entre l'entrée et la sortie du tube.");
    delay(5000);
  }

  if (deltaT_SortieCuve == 0) {
    Serial.println("Aucune perte de chaleur détectée entre la sortie du tube et la cuve.");
    delay(2000);
  } else if (deltaT_SortieCuve < 0) {
    Serial.println("Perte de chaleur détectée entre la sortie du tube et la cuve.");
    delay(5000);
  }
}
// Fonction pour surveiller l'augmentation des températures dans la sortie du tube et la cuve
void monitorTemperatureIncrease(float currentTempSortie, float currentTempCuve) {
  if (currentTempSortie <= previousTempSortie && currentTempCuve <= previousTempCuve) {
    noIncreaseCycles++;
    Serial.print("Pas d'augmentation de la température détectée.");
    delay(5000);
    Serial.println(noIncreaseCycles);
  } else {
    noIncreaseCycles = 0;
    //Serial.println("Réinitialisation du compteur car la température augmente.");
  }
  previousTempSortie = currentTempSortie;
  previousTempCuve = currentTempCuve;
}


void loop() {
  unsigned long currentMillis = millis();

  // Lire les données GPS si disponibles
  while (ss.available() > 0) {
    gps.encode(ss.read());  // Lecture des données GPS
  }

  // Vérifier la communication série pour les commandes
  if (Serial.available() > 0) {
    String received = Serial.readStringUntil('\n');  // Lire jusqu'à une nouvelle ligne
    //Serial.print("Received data: ");
    //Serial.println(received); // Pour déboguer et voir ce qui est reçu

    if (received == "true") {                       // Signal pour l'arrêt d'urgence
      Serial.println("Emergency stop triggered.");  // Débogage
      arreterProgramme();                           // Appeler la fonction d'arrêt d'urgence
      return;                                       // Sortir immédiatement pour éviter d'exécuter le reste du code
    } else if (received == "1") {                   // Signal pour la réinitialisation
      Serial.println("Reset command received.");    // Débogage
      asm volatile("  jmp 0");                      // Redémarre le microcontrôleur
      return;                                       // Sortir immédiatement après réinitialisation
    }
  }

  // Vérifier le bouton d'arrêt d'urgence
  if (digitalRead(EMERGENCY_STOP_PIN) == LOW) {
    arreterProgramme();  // Appeler la fonction d'arrêt d'urgence
    return;              // Sortir immédiatement pour éviter d'exécuter le reste du code
  }

  // Vérifier si c'est le moment de mettre à jour les données
  if (currentMillis - previousUpdateTime >= updateInterval) {
    // Vérifier si les données GPS sont mises à jour
    if (gps.location.isUpdated()) {
      updateGPSData();  // Appel de la fonction de traitement des données GPS
    }

    float position_ptc_pv = 90 - altitude_solaire;  // Calcul de la position pour le panneau solaire
    Serial.print("position_ptc_pv :");
    Serial.println(position_ptc_pv);

    rotateMotor(position_ptc_pv, currentPosition);
    balayerEtTrouverMaxLux(currentPosition, bestAltitude, currentPosition);
    readAndPrintTemperatures();

    if (noIncreaseCycles >= MAX_CYCLES_NO_INCREASE) {
      Serial.println("Rentrer le panneau car pas d'augmentation de la température détectée.");
      float energieExterieur = lireEnergie();  // Lire l'énergie avant de rentrer le panneau

      rentrerPanneau();
      panneauExterieur = false;  // Met à jour l'état du panneau

      // Allumer la LED
      digitalWrite(LED_PIN, HIGH);
      Serial.println("Led ON.");
      delay(2000);
      float energieInterieur = lireEnergie();  // Lire l'énergie après avoir rentré le panneau

      // Vérifiez si toutes les températures sont supérieures à 30°C et l'énergie intérieure est supérieure à l'énergie extérieure
      if (toutesLesTemperaturesSuperieuresA(30) && energieInterieur > energieExterieur) {
        Serial.println("Problème d'ensoleillement détecté.");
        delay(5000);
      } else if (toutesLesTemperaturesInferieuresA(30)) {
        Serial.println("PTC occulté, intervention nécessaire.");
        digitalWrite(LED_PIN, LOW);
        // Boucle infinie pour arrêter le programme
        while (true) {
          delay(100);
        }
      } else {
        Serial.println("Températures vérifiées, reprise du programme.");
        // Éteindre la LED
        digitalWrite(LED_PIN, LOW);
        Serial.println("Led OFF.");
        sortirPanneau();
        panneauExterieur = true;  // Le panneau est prêt à être sorti à nouveau
      }
    }

    previousUpdateTime = currentMillis;  // Mettre à jour le temps de la dernière mise à jour
  }
}