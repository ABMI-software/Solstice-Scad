// Définition des pins pour chaque moteur
#define STEP_PIN1 2
#define DIR_PIN1 5
#define ENABLE_PIN1 8

#define STEP_PIN2 3
#define DIR_PIN2 6
#define ENABLE_PIN2 9

#define STEP_PIN3 4
#define DIR_PIN3 7
#define ENABLE_PIN3 10

void setup() {
  // Initialisation des pins pour chaque moteur
  pinMode(STEP_PIN1, OUTPUT);
  pinMode(DIR_PIN1, OUTPUT);
  pinMode(ENABLE_PIN1, OUTPUT);

  pinMode(STEP_PIN2, OUTPUT);
  pinMode(DIR_PIN2, OUTPUT);
  pinMode(ENABLE_PIN2, OUTPUT);

  pinMode(STEP_PIN3, OUTPUT);
  pinMode(DIR_PIN3, OUTPUT);
  pinMode(ENABLE_PIN3, OUTPUT);

  // Activer les drivers
  digitalWrite(ENABLE_PIN1, LOW);
  digitalWrite(ENABLE_PIN2, LOW);
  digitalWrite(ENABLE_PIN3, LOW);

  // Initialisation des directions
  digitalWrite(DIR_PIN1, HIGH);  // Moteur 1 tourne dans un sens
  digitalWrite(DIR_PIN2, HIGH);  // Moteur 2 tourne dans un sens
  digitalWrite(DIR_PIN3, HIGH);  // Moteur 3 tourne dans un sens

  Serial.begin(9600);
  Serial.println("Test de trois moteurs pas-à-pas sans AccelStepper.");
}

void loop() {
  // Faire avancer les trois moteurs d'un pas à la fois

  // Moteur 1
  digitalWrite(STEP_PIN1, HIGH);
  delayMicroseconds(1000);  // Attendre avant de descendre l'impulsion
  digitalWrite(STEP_PIN1, LOW);
  delayMicroseconds(1000);  // Attendre avant la prochaine impulsion

  // Moteur 2
  digitalWrite(STEP_PIN2, HIGH);
  delayMicroseconds(1000);
  digitalWrite(STEP_PIN2, LOW);
  delayMicroseconds(1000);

  // Moteur 3
  digitalWrite(STEP_PIN3, HIGH);
  delayMicroseconds(1000);
  digitalWrite(STEP_PIN3, LOW);
  delayMicroseconds(1000);

  // Si tu veux inverser la direction des moteurs après un certain nombre de pas
  // Inverser la direction pour chaque moteur
  static int steps = 0;
  static int maxSteps = 1000;

  steps++;
  if (steps >= maxSteps) {
    // Inverser la direction de chaque moteur
    digitalWrite(DIR_PIN1, !digitalRead(DIR_PIN1));  // Inverse direction moteur 1
    digitalWrite(DIR_PIN2, !digitalRead(DIR_PIN2));  // Inverse direction moteur 2
    digitalWrite(DIR_PIN3, !digitalRead(DIR_PIN3));  // Inverse direction moteur 3
    steps = 0;  // Réinitialiser le compteur de pas
  }
}
