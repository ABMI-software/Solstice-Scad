/*
#ifndef PUMP_H
#define PUMP_H

#include <Arduino.h>

class Pump {
public:
    Pump(int pin_in1, int pin_in2, int pin_en); // pin du L298N
    void begin();
    void start();
    void stop();
    void setSpeed(int pwm);

private:
    int _in1, _in2, _en;
};

#endif*/
