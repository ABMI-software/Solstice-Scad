#ifndef oled
#define oled

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

class OLEDDisplay {
public:
    OLEDDisplay();
    void begin();
    void afficher(float ptc1, float ptc2, float cuve1, float cuve2, const String& heure);
    ~OLEDDisplay();
    
private:
    Adafruit_SSD1306 _display;
};

#endif
