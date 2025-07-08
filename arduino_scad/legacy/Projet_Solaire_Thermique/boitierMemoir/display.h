/**
 * display.h - Gestion de l'affichage sur l'écran OLED
 */

 #ifndef DISPLAY_H
 #define DISPLAY_H
 
 #include <Adafruit_SSD1306.h>
 #include <Adafruit_GFX.h>
 
 // Prototypes des fonctions
 bool initDisplay();
 void updateDisplay();
 void displayError(const char* message);
 
 // Variable externe pour l'écran OLED
 extern Adafruit_SSD1306 display;
 
 #endif