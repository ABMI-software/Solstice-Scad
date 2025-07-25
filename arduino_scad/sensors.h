// sensors.h
/**************************************************************************/
/*!
    @file sensors.h
    @brief Header file for the SensorModule class
    Contains the definition of the SensorModule class which handles MAX6675 sensors
*/


/**************************************************************************/
#include "pins.h" // Include pin definitions
#include "config.h" // Include configuration settings
/**************************************************************************/
#include <Arduino.h> // Include Arduino core library
/**************************************************************************/
#include <max6675.h> // Include MAX6675 library for temperature sensor
/**************************************************************************/


// === Définition de la classe SensorModule ===
class SensorModule {
    public:
        SensorModule(); // Constructor
        ~SensorModule(); // Destructor
        void begin(); // Method to initialize the sensor
        void readAll(); // Method to read all sensors and update average temperature
        float getSensor(); // Method to get the current sensor reading
        float readAverage(); // Method to get the average reading

    private:
        MAX6675 sensor; // Instance of the MAX6675 sensor
        float read_avg; // Variable to store the average reading
        float readings[NUM_READINGS]; // Array to store multiple readings for averaging
};
/**************************************************************************/

