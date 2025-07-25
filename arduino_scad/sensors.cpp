
#include "config.h" // Include configuration settings
#include <max6675.h> // Include MAX6675 library for temperature sensor
#include "sensors.h"
/**************************************************************************/
/*!
    @brief  Constructor for SensorModule
    Initializes the MAX6675 sensor with the pins defined in pins.h
*//**************************************************************************/

SensorModule::SensorModule(int clkPin, int csPin, int soPin)
    : sensor(clkPin, csPin, soPin), read_avg(0) {
    // Initializing the readings array to zero
    for (int i = 0; i < NUM_READINGS; i++) {
        readings[i] = 0.0;
    }
}
/**************************************************************************/
/*!       
    @brief  Destructor for SensorModule
    Cleans up resources if necessary
*//**************************************************************************/  

SensorModule::~SensorModule() {} 
/**************************************************************************/
/*!   
    @brief  Begin method for SensorModule
    Initializes the sensor, if needed
*//**************************************************************************/          

void SensorModule::begin() {
    // No specific setup needed for MAX6675, but can be added if necessary
}   

/**************************************************************************/
/*!
    @brief  Reads all sensors and updates the average temperature
    This method reads the temperature from the MAX6675 sensor and updates the average
*//**************************************************************************/      


void SensorModule::readAll() {
    // Read the temperature from the MAX6675 sensor
    float temp = sensor.readCelsius();
    
    // Shift readings to make room for the new reading
    for (int i = NUM_READINGS - 1; i > 0; i--) {
        readings[i] = readings[i - 1];
    }
    
    // Store the new reading
    readings[0] = temp;

    // Calculate the average temperature
    read_avg = 0.0;
    for (int i = 0; i < NUM_READINGS; i++) {
        read_avg += readings[i];
    }
    read_avg /= NUM_READINGS;
}

/**************************************************************************/
/*!             
    @brief  Returns the average temperature reading
    @returns The average temperature from the readings
*//**************************************************************************/
float SensorModule::getSensor() {
    return read_avg;
}
/**************************************************************************/
/*!           
    @brief  Reads the average temperature from the sensor
    This method is kept for compatibility, but it now uses getSensor()
    @returns The average temperature reading
*//**************************************************************************/    
float SensorModule::readAverage() {
    // Calculate the average of the readings
    float sum = 0.0;
    for (int i = 0; i < NUM_READINGS; i++) {
        sum += readings[i];
    }
    return sum / NUM_READINGS;
}
/**************************************************************************/
/*!
    @brief  Constructor for SensorModule with specific MAX6675 pins
    Initializes the MAX6675 sensor with the specified SCK, CS, and SO pins
*//**************************************************************************/

/*// Définition des capteurs MAX6675
SensorModule::SensorModule(MAX6675 ptc1):    
  SensorModule::SensorModule(int sck, int cs, int so) // Initialisation du d'un capteur avec les broches définies dans pins.h
    : ptc1(sck, cs, so), read_avg(0) {}

   

SensorModule::~SensorModule(){} 

void SensorModule::begin() {
  // Pas besoin de setup pour MAX6675, mais tu peux initialiser ici si besoin
}

void SensorModule::readAll() {
  read_avg   = ptc1.readCelsius(); 

}

float SensorModule::getSensor()   { return read_avg; }
*/