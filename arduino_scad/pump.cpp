#include "pump.h"
#include <Arduino.h>
#include "pins.h"

// PumpController class implementation
// Converts percentage (0-100) to PWM value (0-255)

static inline uint8_t pct2pwm(int pct) {
  pct = constrain(pct, 0, 100);
  return map(pct, 0, 100, 0, 255);
}

// PumpController methods
void PumpController::begin() {
    pinMode(POMPE1_IN1, OUTPUT); pinMode(POMPE1_IN2, OUTPUT); pinMode(POMPE1_ENA, OUTPUT); // Pump A
    pinMode(POMPE2_IN3, OUTPUT); pinMode(POMPE2_IN4, OUTPUT); pinMode(POMPE2_ENB, OUTPUT);// Pump B
    pinMode(POMPE1_ENA, OUTPUT); pinMode(POMPE2_ENB, OUTPUT); // Enable pins for PWM
    // Set initial states for the pump control pins
    digitalWrite(POMPE1_ENA, LOW); digitalWrite(POMPE2_ENB, LOW); // Disable PWM initially
    // Set the direction pins for both pumps
    digitalWrite(POMPE1_IN1, LOW); digitalWrite(POMPE1_IN2, LOW); // Pump A direction
    digitalWrite(POMPE2_IN3, LOW); digitalWrite(POMPE2_IN4, LOW); // Pump B direction
    // Set the initial direction for both pumps
    // Set the direction pins for both pumps
    // sens “avant” pour les deux pompes (adapter si inversé)
    digitalWrite(POMPE1_IN1, HIGH); digitalWrite(POMPE1_IN2, LOW);
    digitalWrite(POMPE2_IN3, LOW);  digitalWrite(POMPE2_IN4, HIGH);

  setA(0); setB(0);
}
// Set the PWM value for each pump
void PumpController::applyA() { analogWrite(POMPE1_ENA, pct2pwm(a_pct)); } // Set PWM for Pump A
void PumpController::applyB() { analogWrite(POMPE2_ENB, pct2pwm(b_pct)); }// Set PWM for Pump B

// Set the percentage for Pump and apply the PWM

void PumpController::setA(int pct) { a_pct = constrain(pct,0,100); applyA(); }
void PumpController::setB(int pct) { b_pct = constrain(pct,0,100); applyB(); }

void PumpController::stopAll() { a_pct = b_pct = 0; applyA(); applyB(); }
