#include "oled.h"
#include "pins.h"

/**************************************************************************/
/*!
    @file oled.cpp
    @brief Implementation file for OLED display functions
    Contains function definitions for initializing and displaying information on the OLED screen
*/
/**************************************************************************/

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);  // Create an instance of the display

/**************************************************************************/
/*!
    @brief  Initializes the OLED display
    Sets up the display with the specified address and clears it
*/
/**************************************************************************/

void initOLED() {
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("Erreur OLED"));// If the display initialization fails, print an error message
        while (true);   // Loop forever if initialization fails
    }
    display.clearDisplay(); // Clear the display buffer
    display.setTextSize(1); // Set text size to 1
    display.setTextColor(SSD1306_WHITE); // Set text color to white
    display.setCursor(0, 0); // Set cursor to the top-left corner
    display.println("OLED Initialised");
    display.display(); // Update the display with the buffer content
    delay(1000); // Wait for a second to show the initialization message
    
}

/**************************************************************************/
/*!
    @brief  Displays the splash screen with ABMI logo and text
    Shows the ABMI logo and a brief message on the OLED display
*/
/**************************************************************************/

void splashABMI() {
    display.clearDisplay(); 
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(15, 20);// Set cursor position for the logo
    display.println("  ABMI"); // Display the ABMI logo text
    display.setTextSize(1);
    display.setCursor(20, 45);
    display.println("Innovation Industrielle");

    display.display();
    delay(2000);
    display.clearDisplay();
}

/**************************************************************************/
/*!
    @brief  Displays sensor readings on the OLED screen
    @param   ptc1 Temperature reading from PTC1 sensor
    @param   ptc2 Temperature reading from PTC2 sensor
    @param   cuve1 Temperature reading from CUVE1 sensor
    @param   cuve2 Temperature reading from CUVE2 sensor
    @param   heure Current time in HH:MM:SS format
    @param   mode_ete Boolean indicating if it's summer mode (true) or winter mode (false)
*/
/**************************************************************************/

void afficherInfos(float t1, float t2, float c1, float c2, GPSModule& gpsModule) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    display.setCursor(0, 0);
    display.println("Informations:");
    display.setCursor(0, 10);
    display.println("Date: " + gpsModule.getDate());
    display.setCursor(0, 20);
    display.println("Time: " + gpsModule.getTime());
    display.setCursor(0, 30);
    display.println("Températures:");
    display.setCursor(0, 40);
    display.print("PTC1 : "); display.print(t1, 1); display.println("°C");
    display.setCursor(0, 50);
    display.print("PTC2 : "); display.print(t2, 1); display.println("°C");
    display.setCursor(0, 60);
    display.print("Cuve1: "); display.print(c1, 1); display.println("°C");
    display.setCursor(0, 70);
    display.print("Cuve2: "); display.print(c2, 1); display.println("°C");
    display.display();
}
/**************************************************************************/