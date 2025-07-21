#include "Pump.h"

Pump::Pump(int pin_in1, int pin_in2, int pin_en)
    : _in1(pin_in1), _in2(pin_in2), _en(pin_en) {}

void Pump::begin() {
    pinMode(_in1, OUTPUT);
    pinMode(_in2, OUTPUT);
    pinMode(_en, OUTPUT);
}

void Pump::start() {
    digitalWrite(_in1, HIGH);
    digitalWrite(_in2, LOW);
    digitalWrite(_en, HIGH);
}

void Pump::stop() {
    digitalWrite(_en, LOW);
}

void Pump::setSpeed(int pwm) {
    analogWrite(_en, pwm);
}
