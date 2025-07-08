#include "hardware.h"


void initPumps();
void controlPumps();


void initPumps() {
  // Pompe 1
  pinMode(POMPE1_IN1, OUTPUT);
  pinMode(POMPE1_IN2, OUTPUT);
  pinMode(POMPE1_ENA, OUTPUT);

  // Pompe 2
  pinMode(POMPE2_IN3, OUTPUT);
  pinMode(POMPE2_IN4, OUTPUT);
  pinMode(POMPE2_ENB, OUTPUT); 

  // Sens de rotation fixe pour chaque pompe
  digitalWrite(POMPE1_IN1, HIGH);
  digitalWrite(POMPE1_IN2, LOW);

  digitalWrite(POMPE2_IN3, HIGH);
  digitalWrite(POMPE2_IN4, LOW);
}

void controlPumps() {
  int pwm1 = map(getDebitPompe1(), 0, 100, 0, 255);
  int pwm2 = map(getDebitPompe2(), 0, 100, 0, 255);

  analogWrite(POMPE1_ENA, pwm1);
  analogWrite(POMPE2_ENB, pwm2);
}
