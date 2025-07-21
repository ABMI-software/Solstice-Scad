#include "GPS.h"

GPSModule::GPSModule() : gpsSerial(GPS_RX, GPS_TX) {}

void GPSModule::begin() {
  gpsSerial.begin(9600);
}

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