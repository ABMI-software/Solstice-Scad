#include <Wire.h>
#include <math.h>
#include <max6675.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TimeLib.h>

// Variables diverses (non utilisées directement ici mais conservées pour extension possible)
float temperature_read = 0.0;
float previous_error = 0;
float elapsedTime, Time, timePrev;

// === Définition des broches pour les sondes MAX6675 ===
#define PTC1_CS 28
#define PTC1_SO 30
#define PTC1_CLK 31
float ptc1Temperature = 0;
MAX6675 thermocouple0(PTC1_CLK, PTC1_CS, PTC1_SO);

#define PTC2_CS 34
#define PTC2_SO 35
#define PTC2_CLK 37
float ptc2Temperature = 0;
MAX6675 thermocouple1(PTC2_CLK, PTC2_CS, PTC2_SO);

#define CUVE1_CS 33
#define CUVE1_SO 29
#define CUVE1_CLK 32
float cuve1Temperature = 0;
MAX6675 thermocouple2(CUVE1_CLK, CUVE1_CS, CUVE1_SO);

#define CUVE2_CS 26
#define CUVE2_SO 25
#define CUVE2_CLK 27
float cuve2Temperature = 0;
MAX6675 thermocouple3(CUVE2_CLK, CUVE2_CS, CUVE2_SO);

// === Tableaux de lecture pour faire une moyenne sur 4 mesures ===
float tab_ptc1[4] = {0};
float tab_ptc2[4] = {0};
float tab_cuve1[4] = {0};
float tab_cuve2[4] = {0};

// === Paramètres pour l'écran OLED ===
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  delay(500);
  Serial.begin(9600);  // Initialisation de la communication série

  // Initialisation de l'écran OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("Échec de l'allocation de l'écran OLED"));
    for(;;); // Blocage si erreur d'initialisation
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Logger Pret");
  display.display();
  delay(2000);
  display.clearDisplay();

  Serial.println("== DÉMARRAGE DE LA LECTURE DES TEMPÉRATURES ==");
  delay(250);
}

void loop() {
  // Réinitialisation des totaux avant la moyenne
  float total_ptc1 = 0;
  float total_ptc2 = 0;
  float total_cuve1 = 0;
  float total_cuve2 = 0;
  const int readings = 4; // Nombre de lectures pour la moyenne

  // Lecture des capteurs
  for (int i = 0; i < readings; i++) {
    tab_ptc1[i] = thermocouple0.readCelsius();
    tab_ptc2[i] = thermocouple1.readCelsius();
    tab_cuve1[i] = thermocouple2.readCelsius();
    tab_cuve2[i] = thermocouple3.readCelsius();

    total_ptc1 += tab_ptc1[i];
    total_ptc2 += tab_ptc2[i];
    total_cuve1 += tab_cuve1[i];
    total_cuve2 += tab_cuve2[i];

    delay(1000); // Pause entre chaque lecture pour plus de stabilité
  }

  // Calcul des moyennes
  ptc1Temperature = total_ptc1 / readings;
  ptc2Temperature = total_ptc2 / readings;
  cuve1Temperature = total_cuve1 / readings;
  cuve2Temperature = total_cuve2 / readings;

  // Récupération de l'heure actuelle simulée
  String timeStr = String(hour()) + ":" + String(minute()) + ":" + String(second());

  // Affichage dans le moniteur série
  Serial.print("Heure : "); Serial.print(timeStr); Serial.print(" ; ");
  Serial.print("PTC1 : "); Serial.print(ptc1Temperature); Serial.print(" °C ; ");
  Serial.print("PTC2 : "); Serial.print(ptc2Temperature); Serial.print(" °C ; ");
  Serial.print("Cuve1 : "); Serial.print(cuve1Temperature); Serial.print(" °C ; ");
  Serial.print("Cuve2 : "); Serial.print(cuve2Temperature); Serial.println(" °C");

  // Affichage OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Heure: "); display.println(timeStr);

  display.setCursor(0, 12);
  display.print("PTC1: "); display.print(ptc1Temperature); display.write(167); display.println("C");

  display.setCursor(0, 24);
  display.print("PTC2: "); display.print(ptc2Temperature); display.write(167); display.println("C");

  display.setCursor(0, 36);
  display.print("Cuve1: "); display.print(cuve1Temperature); display.write(167); display.println("C");

  display.setCursor(0, 48);
  display.print("Cuve2: "); display.print(cuve2Temperature); display.write(167); display.println("C");

  display.display();
  delay(100); // Pause avant la prochaine boucle
}
