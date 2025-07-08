#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <max6675.h>

// === OLED I2C ===
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// === MAX6675 ===
#define MAX6675_SO   35
#define MAX6675_CS   34
#define MAX6675_SCK  37
MAX6675 thermo(MAX6675_SCK, MAX6675_CS, MAX6675_SO);

void setup() {
  Serial.begin(9600);

  // OLED
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.clearDisplay();

  Serial.println(">> Test MAX6675 + OLED prêt !");
}

void loop() {
  float temperature = thermo.readCelsius();

  // Affichage OLED
  display.clearDisplay();
  display.setCursor(0, 10);
  display.print("Temp:");
  display.setCursor(0, 35);
  display.print(temperature, 1);
  display.print(" C");
  display.display();

  // Affichage série
  Serial.print("Température : ");
  Serial.print(temperature);
  Serial.println(" °C");

  delay(1000);
}
