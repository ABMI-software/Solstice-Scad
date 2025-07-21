// ===== GPS.h =====
#ifndef GPS_H
#define GPS_H

#include <SoftwareSerial.h>
#include <TinyGPS++.h>

#define GPS_RX 18  // Pin RX du GPS 
#define GPS_TX 19  // Pin TX du GPS 

class GPSModule {
  public:
    GPSModule();
    void begin();
    bool update();
    String getDate();
    String getTime();

  private:
    SoftwareSerial gpsSerial;
    TinyGPSPlus gps;
};

#endif