/**************************************************************************
 * @file arduino_scad.ino
 * @brief Programme principal Arduino SCAD - Version simplifiée
 * Fonctionnalités : capteurs MAX6675, encodeurs, pompes, UART, RTC, OLED
 **************************************************************************/

#include <Arduino.h>
#include "config.h"
#include "sensors.h"
#include "pump.h"
#include "pins.h"
#include "oled.h"
#include <RTClib.h>

#define BAUD_USB  115200
#define BAUD_UART 9600   // UART Pi (pins 16/17)

// === DÉFINITION DES VARIABLES GLOBALES ===
unsigned long dernier_affichage_serial = 0;
unsigned long dernier_affichage_oled   = 0;
unsigned long dernier_controle_pompes  = 0;
unsigned long dernier_envoi_uart       = 0;

// === INSTANCES DES MODULES ===
SensorModule sensorEau(PTC1_CLK, PTC1_CS, PTC1_SO);
SensorModule sensorPanneau(PTC2_CLK, PTC2_CS, PTC2_SO);
SensorModule sensorCuve1(CUVE1_CLK, CUVE1_CS, CUVE1_SO);
SensorModule sensorCuve2(CUVE2_CLK, CUVE2_CS, CUVE2_SO);

PumpController pompes;
EncoderController enc1(ENCODER1_CLK, ENCODER1_DT);
EncoderController enc2(ENCODER2_CLK, ENCODER2_DT);

RTC_DS3231 rtc;

/**************************************************************************
 * Horodatage RTC
 **************************************************************************/
String getTimestamp() {
  DateTime now = rtc.now();
  char buffer[20];
  sprintf(buffer, "%04d-%02d-%02d %02d:%02d:%02d",
          now.year(), now.month(), now.day(),
          now.hour(), now.minute(), now.second());
  return String(buffer);
}

/**************************************************************************
 * Lecture tension PV simplifiée
 **************************************************************************/
float readPVVoltage() {
  int adc = analogRead(PV_VOUT);
  return (adc * 5.0) / 1023.0;
}

/**************************************************************************
 * Envoi frame UART (CSV) sur USB + Serial2
 **************************************************************************/
void sendFrameUART() {
  String timestamp = getTimestamp();
  float t_eau     = sensorEau.getSensor();
  float t_panneau = sensorPanneau.getSensor();
  float t_cuve1   = sensorCuve1.getSensor();
  float t_cuve2   = sensorCuve2.getSensor();
  int   encoder1  = enc1.getValue();
  int   encoder2  = enc2.getValue();
  float pv_volts  = readPVVoltage();

  auto writeCSV = [&](Print &p){
    p.print("DATA,");
    p.print(timestamp); p.print(",");
    p.print(t_eau, 2); p.print(",");
    p.print(t_panneau, 2); p.print(",");
    p.print(t_cuve1, 2); p.print(",");
    p.print(t_cuve2, 2); p.print(",");
    p.print(encoder1);  p.print(",");
    p.print(encoder2);  p.print(",");
    p.print(pv_volts, 3);
    p.println();
  };

  writeCSV(Serial);   // USB monitor
  writeCSV(Serial2);  // Pi UART
}

/**************************************************************************
 * Affiche les données complètes sur le flux choisi
 **************************************************************************/
void afficherDonneesCompletes(Print &out) {
  out.println("=== SYSTÈME SCAD - DONNÉES ===");
  out.print("Horodatage: "); out.println(getTimestamp());

  out.println("--- Températures ---");
  out.print("Eau: ");     out.print(sensorEau.getSensor(), 2);     out.println(" °C");
  out.print("Panneau: "); out.print(sensorPanneau.getSensor(), 2); out.println(" °C");
  out.print("Cuve1: ");   out.print(sensorCuve1.getSensor(), 2);   out.println(" °C");
  out.print("Cuve2: ");   out.print(sensorCuve2.getSensor(), 2);   out.println(" °C");

  out.println("--- Encodeurs ---");
  out.print("Enc1: "); out.println(enc1.getValue());
  out.print("Enc2: "); out.println(enc2.getValue());

  out.println("--- PV ---");
  out.print("Tension: "); out.print(readPVVoltage(), 2); out.println(" V");

  out.println("--- Pompes ---");
  out.print("Pompe A: "); out.print(pompes.getA()); out.println(" %");
  out.print("Pompe B: "); out.print(pompes.getB()); out.println(" %");

  out.println("===============================");
}

/**************************************************************************
 * Affichage OLED
 **************************************************************************/
void afficherOLED() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Titre avec timestamp
  display.setCursor(0, 0);
  display.print("SCAD ");
  String time = getTimestamp();
  if (time.length() > 15) time = time.substring(time.length() - 8);
  display.println(time);

  display.drawLine(0, 9, 127, 9, SSD1306_WHITE);

  // Températures
  display.setCursor(0, 12);
  display.print("Eau:");
  display.print(sensorEau.getSensor(), 1);
  display.print(" Pan:");
  display.print(sensorPanneau.getSensor(), 1);

  display.setCursor(0, 21);
  display.print("C1:");
  display.print(sensorCuve1.getSensor(), 1);
  display.print(" C2:");
  display.print(sensorCuve2.getSensor(), 1);

  // Encodeurs
  display.setCursor(0, 30);
  display.print("Enc1:");
  display.print(enc1.getValue());
  display.print(" Enc2:");
  display.print(enc2.getValue());

  // PV
  display.setCursor(0, 39);
  display.print("PV:");
  display.print(readPVVoltage(), 1);
  display.print("V");

  // Pompes
  display.setCursor(0, 48);
  display.print("PompeA:");
  display.print(pompes.getA());
  display.print("% B:");
  display.print(pompes.getB());
  display.print("%");

  display.display();
}

/**************************************************************************
 * Gestion des commandes série (USB + UART2)
 **************************************************************************/
void gererCommandesPort(Stream &in, Print &out) {
  if (!in.available()) return;

  String commande = in.readStringUntil('\n');
  commande.trim();
  commande.toUpperCase();

  if (commande == "STATUS") {
    afficherDonneesCompletes(out);
  }
  else if (commande == "ENCODEURS") {
    out.print("Enc1: "); out.print(enc1.getValue());
    out.print(" / Enc2: "); out.println(enc2.getValue());
  }
  else if (commande == "POMPES") {
    out.print("PompeA: "); out.print(pompes.getA());
    out.print("% / PompeB: "); out.print(pompes.getB()); out.println("%");
  }
  else if (commande == "STOP") {
    pompes.stopAll();
    out.println("Pompes arrêtées");
  }
  else if (commande == "UART") {
    sendFrameUART();
    out.println("Frame UART envoyée");
  }
  else if (commande == "HELP") {
    out.println("=== COMMANDES ===");
    out.println("STATUS - État système");
    out.println("ENCODEURS - État encodeurs");
    out.println("POMPES - État pompes");
    out.println("STOP - Arrêt pompes");
    out.println("UART - Envoi UART manuel (CSV)");
    out.println("HELP - Cette aide");
  }
}

/**************************************************************************
 * Setup
 **************************************************************************/
void setup() {
  Serial.begin(BAUD_USB);     // USB
  Serial2.begin(BAUD_UART);   // Pi (pins 16/17)

  Serial.println("=== DÉMARRAGE SYSTÈME SCAD ===");
  Serial2.println("SCAD UART2 READY");

  // Init capteurs
  sensorEau.begin();
  sensorPanneau.begin();
  sensorCuve1.begin();
  sensorCuve2.begin();

  // Init encodeurs + pompes
  enc1.begin();
  enc2.begin();
  pompes.begin();

  // Init RTC
  if (!rtc.begin()) {
    Serial.println("RTC introuvable !");
    while (1);
  }
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  // Init OLED
  initOLED();
  splashABMI();

  delay(2000);
  Serial.println("=== SYSTÈME PRÊT ===");
  Serial2.println("=== SYSTÈME PRÊT ===");
}

/**************************************************************************
 * Loop
 **************************************************************************/
void loop() {
  // MàJ encodeurs
  enc1.update();
  enc2.update();
  pompes.setA(enc1.getValue());
  pompes.setB(enc2.getValue());

  // MàJ capteurs
  sensorEau.readAll();
  sensorPanneau.readAll();
  sensorCuve1.readAll();
  sensorCuve2.readAll();

  // Envoi périodique UART
  if (millis() - dernier_envoi_uart >= INTERVALLE_UART) {
    sendFrameUART();
    dernier_envoi_uart = millis();
  }

  // Commandes des deux ports
  gererCommandesPort(Serial,  Serial);
  gererCommandesPort(Serial2, Serial2);

  // Affichage série (USB)
  if (millis() - dernier_affichage_serial >= INTERVALLE_SERIAL) {
    afficherDonneesCompletes(Serial);
    dernier_affichage_serial = millis();
  }

  // OLED
  if (millis() - dernier_affichage_oled >= INTERVALLE_OLED) {
    afficherOLED();
    dernier_affichage_oled = millis();
  }

  delay(5);
}
