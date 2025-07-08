#include <max6675.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// === OLED ===
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// === MAX6675 (Thermocouples) ===
#define thermoSO   35
#define thermoSCK  37

MAX6675 thermo1(thermoSCK, 38, thermoSO); // Panneau 1
MAX6675 thermo2(thermoSCK, 40, thermoSO); // Cuve 1
MAX6675 thermo3(thermoSCK, 42, thermoSO); // Panneau 2
MAX6675 thermo4(thermoSCK, 44, thermoSO); // Cuve 2

// === Encodeurs rotatifs ===
// Encodeur 1 : Pompe 1
#define clkPin1 3
#define dtPin1  5

// Encodeur 2 : Pompe 2
#define clkPin2 7
#define dtPin2  8

int lastClk1 = LOW, lastClk2 = LOW;
int encoderValue1 = 0;
int encoderValue2 = 0;

// === Relais ===
#define pumpRelay1 4
#define pumpRelay2 9

void setup() {
  Serial.begin(9600);

  // OLED
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Encodeurs
  pinMode(clkPin1, INPUT);
  pinMode(dtPin1, INPUT);
  pinMode(clkPin2, INPUT);
  pinMode(dtPin2, INPUT);
  lastClk1 = digitalRead(clkPin1);
  lastClk2 = digitalRead(clkPin2);

  // Pompes
  pinMode(pumpRelay1, OUTPUT);
  pinMode(pumpRelay2, OUTPUT);
  digitalWrite(pumpRelay1, LOW);
  digitalWrite(pumpRelay2, LOW);

  Serial.println("Systeme de controle solaire thermique OK");
}

void loop() {
  // === Lecture encodeur 1 ===
  int currentClk1 = digitalRead(clkPin1);
  if (currentClk1 != lastClk1 && currentClk1 == HIGH) {
    if (digitalRead(dtPin1) != currentClk1) encoderValue1++;
    else encoderValue1--;
    encoderValue1 = constrain(encoderValue1, 0, 100);
  }
  lastClk1 = currentClk1;

  // === Lecture encodeur 2 ===
  int currentClk2 = digitalRead(clkPin2);
  if (currentClk2 != lastClk2 && currentClk2 == HIGH) {
    if (digitalRead(dtPin2) != currentClk2) encoderValue2++;
    else encoderValue2--;
    encoderValue2 = constrain(encoderValue2, 0, 100);
  }
  lastClk2 = currentClk2;

  // === Lecture température ===
  float tempP1 = thermo1.readCelsius();
  float tempC1 = thermo2.readCelsius();
  float tempP2 = thermo3.readCelsius();
  float tempC2 = thermo4.readCelsius();

  // === Commande relais ===
  digitalWrite(pumpRelay1, encoderValue1 > 0 ? HIGH : LOW);
  digitalWrite(pumpRelay2, encoderValue2 > 0 ? HIGH : LOW);

  // === Affichage OLED ===
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Cuve1: "); display.print(tempC1, 1); display.println(" C");
  display.print("Debit1: "); display.print(encoderValue1); display.println(" %");
  display.print("Cuve2: "); display.print(tempC2, 1); display.println(" C");
  display.print("Debit2: "); display.print(encoderValue2); display.println(" %");
  display.display();

  // === Serial Debug ===
  Serial.print("P1: "); Serial.print(tempP1);
  Serial.print(" | C1: "); Serial.print(tempC1);
  Serial.print(" | D1: "); Serial.print(encoderValue1);
  Serial.print(" | P2: "); Serial.print(tempP2);
  Serial.print(" | C2: "); Serial.print(tempC2);
  Serial.print(" | D2: "); Serial.println(encoderValue2);

  delay(200);
}
