#include "hardware.h"

void initEncoders();
void readEncoders();
int getDebitPompe1();
int getDebitPompe2();
volatile int compteur1 = 0;
volatile int compteur2 = 0;

int lastClk1 = LOW;
int lastClk2 = LOW;

void initEncoders() {
  pinMode(ENCODER1_CLK, INPUT);
  pinMode(ENCODER1_DT, INPUT);
  pinMode(ENCODER2_CLK, INPUT);
  pinMode(ENCODER2_DT, INPUT);

  lastClk1 = digitalRead(ENCODER1_CLK);
  lastClk2 = digitalRead(ENCODER2_CLK);
}

void readEncoders() {
  int currentClk1 = digitalRead(ENCODER1_CLK);
  if (currentClk1 != lastClk1) {
    if (digitalRead(ENCODER1_DT) != currentClk1) {
      compteur1++;
    } else {
      compteur1--;
    }
    compteur1 = constrain(compteur1, 0, 100);
  }
  lastClk1 = currentClk1;

  int currentClk2 = digitalRead(ENCODER2_CLK);
  if (currentClk2 != lastClk2) {
    if (digitalRead(ENCODER2_DT) != currentClk2) {
      compteur2++;
    } else {
      compteur2--;
    }
    compteur2 = constrain(compteur2, 0, 100);
  }
  lastClk2 = currentClk2;
}

int getDebitPompe1() {
  return compteur1;
}

int getDebitPompe2() {
  return compteur2;
}
