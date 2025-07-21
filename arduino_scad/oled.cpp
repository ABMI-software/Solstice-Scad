#include "oled.h"
#include "pins.h"


OLEDDisplay::OLEDDisplay() : _display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {}

void OLEDDisplay::begin() {
    if (!_display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("Erreur d'initialisation OLED"));
        while (true); // blocage
    }
    _display.clearDisplay();
    _display.display();
}

void OLEDDisplay::afficher(float ptc1, float ptc2, float cuve1, float cuve2, const String& heure) {
    _display.clearDisplay();
    _display.setTextSize(1);
    _display.setTextColor(SSD1306_WHITE);
    _display.setCursor(0, 0);

    _display.print("Heure: "); _display.println(heure);

    _display.print("PTC1: "); _display.print(ptc1); _display.println(" C");
    _display.print("PTC2: "); _display.print(ptc2); _display.println(" C");
    _display.print("Cuve1: "); _display.print(cuve1); _display.println(" C");
    _display.print("Cuve2: "); _display.print(cuve2); _display.println(" C");

    _display.display();
}
