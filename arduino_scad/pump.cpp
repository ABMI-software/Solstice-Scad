#include "pump.h"
#include <Arduino.h>
#include "pins.h"

// ========== PUMPCONTROLLER IMPLEMENTATION ==========

// Converts percentage (0-100) to PWM value (0-255)
static inline uint8_t pct2pwm(int pct) {
  pct = constrain(pct, 0, 100);
  return map(pct, 0, 100, 0, 255);
}

void PumpController::begin() {
    // Initialize pump control pins
    pinMode(POMPE1_IN1, OUTPUT); 
    pinMode(POMPE1_IN2, OUTPUT); 
    pinMode(POMPE1_ENA, OUTPUT); 
    pinMode(POMPE2_IN3, OUTPUT); 
    pinMode(POMPE2_IN4, OUTPUT); 
    pinMode(POMPE2_ENB, OUTPUT);
    
    // Set initial states - all LOW initially
    digitalWrite(POMPE1_ENA, LOW); 
    digitalWrite(POMPE2_ENB, LOW);
    digitalWrite(POMPE1_IN1, LOW); 
    digitalWrite(POMPE1_IN2, LOW);
    digitalWrite(POMPE2_IN3, LOW); 
    digitalWrite(POMPE2_IN4, LOW);
    
    // Set direction for both pumps (adjust if reversed)
    digitalWrite(POMPE1_IN1, HIGH); 
    digitalWrite(POMPE1_IN2, LOW);
    digitalWrite(POMPE2_IN3, LOW);  
    digitalWrite(POMPE2_IN4, HIGH);

    // Initialize pumps to 0%
    setA(0); 
    setB(0);
}

void PumpController::applyA() { 
    analogWrite(POMPE1_ENA, pct2pwm(a_pct)); 
}

void PumpController::applyB() { 
    analogWrite(POMPE2_ENB, pct2pwm(b_pct)); 
}

void PumpController::setA(int pct) { 
    a_pct = constrain(pct, 0, 100); 
    applyA(); 
}

void PumpController::setB(int pct) { 
    b_pct = constrain(pct, 0, 100); 
    applyB(); 
}

void PumpController::stopAll() { 
    a_pct = 0; 
    b_pct = 0; 
    applyA(); 
    applyB(); 
}

// ========== ENCODERCONTROLLER IMPLEMENTATION ==========

EncoderController::EncoderController(int clk, int dt) : clkPin(clk), dtPin(dt), compteur(0), lastClkState(LOW) {
}

void EncoderController::begin() {
    pinMode(clkPin, INPUT);
    pinMode(dtPin, INPUT);
    
    lastClkState = digitalRead(clkPin);
}

void EncoderController::update() {
    // Lecture de l'encodeur rotatif
    int currentClk = digitalRead(clkPin);
    
    if (currentClk != lastClkState) {
        if (digitalRead(dtPin) != currentClk) {
            compteur++;  // Rotation horaire
        } else {
            compteur--;  // Rotation anti-horaire
        }
        
        // Limiter entre 0 et 100 (pourcentage PWM)
        compteur = constrain(compteur, 0, 100);
        
        // Optional: Serial debugging
        Serial.print("Encodeur: ");
        Serial.println(compteur);
    }
    lastClkState = currentClk;
}

void EncoderController::setValue(int value) {
    compteur = constrain(value, 0, 100);
}