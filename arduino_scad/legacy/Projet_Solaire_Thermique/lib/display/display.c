#include "hardware.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

void initDisplay();
void updateDisplay();
void nextPage();

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

int currentPage = 0;
const int totalPages = 4;

void initDisplay() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Échec de l'initialisation de l'écran OLED"));
    for (;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Systeme Solaire");
  display.println("Thermique");
  display.display();
  delay(2000);
}

void updateDisplay() {
  display.clearDisplay();
  display.setCursor(0, 0);
  switch (currentPage) {
    case 0:
      display.println("Bienvenue !");
      display.println("Systeme Solaire");
      display.println("Thermique");
      break;
    case 1:
      display.print("Cuve1: ");

::contentReference[oaicite:0]{index=0}
 
