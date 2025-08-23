#ifndef PUMP_H
#define PUMP_H

#include <Arduino.h>

class PumpController {
public:
  void begin();
  void setA(int pct);   // 0..100
  void setB(int pct);   // 0..100
  int  getA() const { return a_pct; }
  int  getB() const { return b_pct; }
  void stopAll();

private:
  int a_pct = 0, b_pct = 0;
  void applyA();
  void applyB();
};

class EncoderController {
public:
    EncoderController(int clkPin, int dtPin);
    void begin();
    void update();
    int getValue() const { return compteur; }
    void reset() { compteur = 0; }
    void setValue(int value);

private:
    int clkPin, dtPin;
    int lastClkState;
    int compteur;
};

#endif // PUMP_H