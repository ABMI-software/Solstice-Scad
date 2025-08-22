#ifndef GPS_H
#define GPS_H

#include <SoftwareSerial.h>
#include <TinyGPS++.h>

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

#endif // GPS_H