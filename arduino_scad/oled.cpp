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

void afficherInfos(float ptc1, float ptc2, float cuve1, float cuve2, const String& heure, bool mode_ete) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    display.setCursor(0, 0);// Set cursor to the top-left corner
    display.println("Informations:");
    display.setCursor(0, 10);
    display.print("Mode ");
    display.print("Heure ");
    display.print(mode_ete ? "(ETE): " : "(HIVER): ");
    display.println(heure);
    display.setCursor(0, 20); // Display sensor readings
    display.println("Températures:"); // Display the label for temperature readings0
    display.setCursor(0, 15);
    
    display.print("PTC1 : "); display.print(ptc1, 1); display.println(" C"); // Display PTC1 temperature
    display.setCursor(0, 25);

    display.print("PTC2 : "); display.print(ptc2, 1); display.println(" C"); // Display PTC2 temperature
    display.setCursor(0, 35);

    display.print("Cuve1: "); display.print(cuve1, 1); display.println(" C"); // Display CUVE1 temperature
    display.setCursor(0, 45);

    display.print("Cuve2: "); display.print(cuve2, 1); display.println(" C");// Display CUVE2 temperature
    display.setCursor(0, 55);
    display.println("ABMI SCAD Project"); // Display project name
    display.setCursor(0, 65);
    display.println("Version 1.0"); // Display version information
    display.setCursor(0, 75);
    display.println("2025"); // Display year
    display.setCursor(0, 85);
   
    display.display();
}
