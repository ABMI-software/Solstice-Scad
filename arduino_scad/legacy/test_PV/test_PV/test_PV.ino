#include <SPI.h>
#include <SD.h>

// === Capteur analogique ===
const int pin_temp = A0;

// === SD Card Configuration ===
#define SD_CS 10  // Chip Select pin for SD card

// === Constantes physiques ===
const float masseEau = 3.5;                   // en kg
const float capaciteThermiqueEau = 4180.0;    // en J/(kg·°C)
const float surfacePanneau = 0.15;            // en m²

// === Variables pour le calcul ===
float lastTemp = 0.0;
unsigned long lastTime = 0;
bool firstRead = true;

// === Timing ===
unsigned long previousMillis = 0;
const unsigned long interval = 30000; // 30 sec

void setup() {
  Serial.begin(9600);
  delay(1000);

  pinMode(pin_temp, INPUT);

  pinMode(SD_CS, OUTPUT);
  digitalWrite(SD_CS, HIGH);

  if (!SD.begin(SD_CS)) {
    Serial.println("Erreur SD !");
  } else {
    Serial.println("Carte SD initialisée.");
    File dataFile = SD.open("puissance.csv", FILE_WRITE);
    if (dataFile) {
      dataFile.println("Time(s);Temp(°C);Puissance(W/m²)");
      dataFile.close();
    }
  }

  lastTime = millis();
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Lecture température analogique (ex: TMP36)
    int analogValue = analogRead(pin_temp);
    float voltage = analogValue * (5.0 / 1023.0);
    float temperature = (voltage - 0.5) * 100.0; // pour TMP36

    if (firstRead) {
      lastTemp = temperature;
      lastTime = currentMillis;
      firstRead = false;
      return;
    }

    float deltaT = temperature - lastTemp;
    float deltaTime = (currentMillis - lastTime) / 1000.0; // en secondes

    float puissance = (deltaT / deltaTime) * masseEau * capaciteThermiqueEau / surfacePanneau;

    Serial.print("Temp = ");
    Serial.print(temperature, 2);
    Serial.print(" °C | ΔT = ");
    Serial.print(deltaT, 2);
    Serial.print(" °C | Puissance = ");
    Serial.print(puissance, 2);
    Serial.println(" W/m²");

    // Enregistrement sur SD
    File dataFile = SD.open("puissance.csv", FILE_WRITE);
    if (dataFile) {
      dataFile.print(currentMillis / 1000); // temps depuis démarrage en secondes
      dataFile.print(";");
      dataFile.print(temperature, 2);
      dataFile.print(";");
      dataFile.println(puissance, 2);
      dataFile.close();
    } else {
      Serial.println("Erreur écriture SD !");
    }

    // Mise à jour
    lastTemp = temperature;
    lastTime = currentMillis;
  }
}
