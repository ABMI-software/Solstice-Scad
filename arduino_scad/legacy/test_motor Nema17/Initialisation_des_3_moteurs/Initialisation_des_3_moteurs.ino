#include <Stepper.h>
#define STEPS_PER_REV 200  

// Déclaration des moteurs (X, Y, Z)
  Stepper motorX(STEPS_PER_REV, 2, 3);  // pins IN1, IN2
  Stepper motorY(STEPS_PER_REV, 4, 5);
  Stepper motorZ(STEPS_PER_REV, 6, 7);
/* Définition des pins pour chaque moteur
  const int dirPinX = 2;
  const int stepPinX = 3;

  const int dirPinY = 4;
  const int stepPinY = 5;

  const int dirPinZ = 6;
  const int stepPinZ = 7;
 */
// Nombre de pas à effectuer lors d'une commande
  const int stepsToMoveX = 100;
  const int stepsToMoveY = 50;
  const int stepsToMoveZ = 14000;

// Pins des capteurs LDR
int ldrTopLeft = A0;
int ldrTopRight = A1;
int ldrBottomLeft = A2;
int ldrBottomRight = A3;

// Délai en microsecondes entre chaque impulsion (ajuste selon la vitesse désirée)
  const int stepDelay = 1000;

void setup() {

  // Initialisation de la communication série pour le suivi
  Serial.begin(9600);
  // config des pin 
  ConfigPin();
  Serial.println("Initialisation terminée.");
  Serial.println("Mouvement dans un sens :");
  Serial.print ("test du moteur Z");
  testZMotor();

  delay(1000);  // attendre entre chaque lecture

  /*
  Serial.println("-> Moteur X avance de 180°");
  moveMotor(stepPinX, dirPinX, stepsToMoveX);
  delay(1000);

  Serial.println("-> Moteur Y avance");
  moveMotor(stepPinY, dirPinY, stepsToMoveY);
  delay(1000);

  Serial.println("-> Moteur Z avance");
  moveMotor(stepPinZ, dirPinZ, stepsToMoveZ);
  delay(1000);*/
  //adjustZ();
  //delay((1000));
  //adjustY();

  // Optionnel : 


}

void ConfigPin(){
    // Configuration moteurs
  motorX.setSpeed(60);  // RPM
  motorY.setSpeed(60);
  motorZ.setSpeed(60);
}
// Fonction générique pour déplacer un moteur via les pins STEP et DIR
// Si 'steps' est positif, on tourne dans un sens, s'il est négatif, dans l'autre.
void moveMotor(int stepPin, int dirPin, int steps) {
  if (steps > 0) {
    digitalWrite(dirPin, HIGH);  // Définir la direction (sens 1)
  } 


  // Envoi de la série d'impulsions
  for (int i = 0; i < steps; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(stepDelay);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(stepDelay);
  }
}
void adjustX() {
  int left = analogRead(ldrTopLeft) + analogRead(ldrBottomLeft);
  int right = analogRead(ldrTopRight) + analogRead(ldrBottomRight);

  int diffX = left - right;

  if (abs(diffX) > 50) {
    motorX.step(diffX > 0 ? 10 : -10);
  }
}
void adjustY() {
  int top = analogRead(ldrTopLeft) + analogRead(ldrTopRight);
  int bottom = analogRead(ldrBottomLeft) + analogRead(ldrBottomRight);

  int diffY = top - bottom;

  if (abs(diffY) > 50) {
    motorY.step(diffY > 0 ? 10 : -10);
  }
}
void testZMotor() {
  Serial.println("Monter...");
  motorZ.step(100);  // 100 pas en avant
  delay(500);
}

void loop() {
motorZ.step(100);

}