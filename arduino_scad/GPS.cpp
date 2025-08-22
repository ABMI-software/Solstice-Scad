#include "GPS.h"
#include "config.h"
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include "pins.h"

/**************************************************************************/
/*!
    @file GPS.cpp
    @brief Implementation of GPS module functions
*//**************************************************************************/

// GPSModule class constructor

GPSModule::GPSModule() : gpsSerial(GPS_RX, GPS_TX) {}

void GPSModule::begin() {
  gpsSerial.begin(9600);
}

/**************************************************************************/
// Function to update GPS data
// Returns true if the data is valid, false otherwise
/**************************************************************************/

bool GPSModule::update() {
  while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
  }
  return gps.location.isValid();
}

String GPSModule::getDate() {
  if (gps.date.isValid()) {
    return String(gps.date.day()) + "/" + String(gps.date.month()) + "/" + String(gps.date.year());
  } else {
    return "Invalid date";
  }
}

String GPSModule::getTime() {
  if (gps.time.isValid()) {
    char buffer[9];
    sprintf(buffer, "%02d:%02d:%02d", gps.time.hour(), gps.time.minute(), gps.time.second());
    return String(buffer);
  } else {
    return "Invalid time";
  }
}