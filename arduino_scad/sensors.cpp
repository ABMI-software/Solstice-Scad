#include "Sensors.h"

SensorModule::SensorModule()
  : ptc1(PTC1_CLK, PTC1_CS, PTC1_SO),
    ptc2(PTC2_CLK, PTC2_CS, PTC2_SO),
    cuve1(CUVE1_CLK, CUVE1_CS, CUVE1_SO),
    cuve2(CUVE2_CLK, CUVE2_CS, CUVE2_SO),
    ptc1_avg(0), ptc2_avg(0), cuve1_avg(0), cuve2_avg(0) {}

void SensorModule::begin() {
  // Pas besoin de setup pour MAX6675, mais tu peux initialiser ici si besoin
}

void SensorModule::readAll() {
  ptc1_avg   = ptc1.readCelsius();
  ptc2_avg   = ptc2.readCelsius();
  cuve1_avg  = cuve1.readCelsius();
  cuve2_avg  = cuve2.readCelsius();
}

float SensorModule::getPTC1()   { return ptc1_avg; }
float SensorModule::getPTC2()   { return ptc2_avg; }
float SensorModule::getCUVE1()  { return cuve1_avg; }
float SensorModule::getCUVE2()  { return cuve2_avg; }
