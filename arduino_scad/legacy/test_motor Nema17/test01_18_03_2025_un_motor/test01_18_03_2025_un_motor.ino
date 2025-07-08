//#include <grbl.h>
#include <AccelStepper.h>
#include <AccelStepperWithDistance.h>
//#include <TimerLib.h>

//declaration des pin sue le CNC sheild V3
#define X_STP 2
#define Y_STP 3

#define X_DIR 5
#define Y_DIR 6

#define Z_STP 4
#define Z_DIR 7

#define Enable_PIN 8  //LOW = Actif

#define BUTTON_ON 28  // Bouton ON
#define BUTTON_ON 26  // Bouton OFF

//Déclaration des moteurs
AccelStepper stepper1(AccelStepper::DRIVER, Z_STP, Z_DIR);
AccelStepper stepper2(AccelStepper::DRIVER, X_STP, X_DIR);
AccelStepper stepper3(AccelStepper::DRIVER, Y_STP, Y_DIR);

//Déclaration des capteurs

//Définition des limites de mouvement en gradian °
const int X_MIN = 0;
const int X_90 = 90;
const int X_MAX = 160;  // 180 - 20
const int Y_MIN = 0;
const int Y_MAX = 77;  // 90 - 13 pour évité la collision

int posX = 0;  // Position initial du moteur X (en degrés)
int posY = 0;  // Position initial du moteur Y (en degrés)
int posZ = 0;

void setup() {
  Serial.begin(9600);
  
  pinMode(Enable_PIN, OUTPUT);
  digitalWrite(Enable_PIN, LOW);
  
  //configurer la vitesse maximale et l'accélération des moteurs X et Y
    stepper2.setMaxSpeed(1000);
    stepper2.setAcceleration(500);
    stepper3.setMaxSpeed(1000);
    stepper3.setAcceleration(500);

 
  PositionInitialSCAD();
}

void loop() {
}

void PositionInitialSCAD() {
    Serial.println("[Initialisation] Mise en position initiale...");

    // Revenir aux positions de base
    stepper2.moveTo(0);
    stepper1.moveTo(100);
    digitalWrite(Y_STP, LOW);

    while (stepper2.distanceToGo() != 0 || stepper3.distanceToGo() != 0) {
        stepper2.run();
        stepper1.run();
    }

    delay(500);

}





















/*
#define DIR_PIN 2  // Broche pour la direction
#define STEP_PIN 3 // Broche pour le signal de pas
void setup(){
  pinMode(DIR_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  Serial.begin(9600);
}
void loop(){
  Serial.println("Moteur tourne en avant");
  stepMoteurPanneau(1, DIR_PIN,STEP_PIN, 200);
  delay(1000);

  Serial.println("Moteur tourne en arriére");
  stepMoteurPanneau(0, DIR_PIN,STEP_PIN, 200);
  delay(1000);
  }
void stepMoteurPanneau(bool dir, byte dirPin, byte stepperPin, int steps) {
  digitalWrite(dirPin, dir);
  delay(100);
  for (int i = 0; i < steps; i++) {
    digitalWrite(stepperPin, HIGH);
    delayMicroseconds(700);
    digitalWrite(stepperPin, LOW);
    delayMicroseconds(700);
  }
}
/*
void positionnerMoteurX() {
    Serial.print("Position actuelle: ");
    Serial.println(currentPosition);
    float angle=90;
    Serial.print("Déplacement demandé: ");
    Serial.println(angle);
    // Calculer le nombre de pas nécessaires pour l'angle donné
    float turns = angle / degreesPerRevolution;
    int steps = turns * stepsPerRevolution;

    // Mettre la direction en avant si l'angle est positif, sinon en arrière
    bool direction = (angle >= 0);

    // Effectuer le mouvement
    digitalWrite(X_DIR, direction); // Définir la direction
    delay(100); // Petite pause pour stabiliser la direction

    for (int i = 0; i < abs(steps); i++) {
        digitalWrite(X_STP, HIGH); // Émettre un pas
        delayMicroseconds(700); // Durée du pas
        digitalWrite(X_STP, LOW); // Arrêter le pas
        delayMicroseconds(700); // Durée de l'arrêt
    }

    // Mettre à jour la position actuelle
    currentPosition += angle; // Mettre à jour la position actuelle
    savePosition(currentPosition); // Sauvegarder la nouvelle position
    Serial.print("Nouvelle position: ");
    Serial.println(currentPosition);
}
*/
/* */