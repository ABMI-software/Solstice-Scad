#define STEP_PIN 2     // Pin STEP connectée à l'Arduino
#define DIR_PIN 5      // Pin DIR connectée à l'Arduino
#define ENABLE_PIN 8   // Pin ENABLE connectée à l'Arduino (peut être mis à LOW pour activer)

void setup() {
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);

  // Désactive les drivers au début
  digitalWrite(ENABLE_PIN, LOW);  // Active le moteur (ENABLE_PIN à LOW)
  
  // Initialisation de la direction (forward ou reverse)
  digitalWrite(DIR_PIN, HIGH); // HIGH pour avancer, LOW pour reculer

  Serial.begin(9600);
  Serial.println("Test moteur pas-à-pas sans AccelStepper.");
}

void loop() {
  // Faire avancer le moteur d'un pas à la fois
  digitalWrite(STEP_PIN, HIGH);  // Envoie une impulsion à la broche STEP
  delayMicroseconds(1000);       // Attendre un court instant avant de passer à l'état bas
  digitalWrite(STEP_PIN, LOW);   // Fin de l'impulsion
  delayMicroseconds(1000);       // Attendre avant d'envoyer une nouvelle impulsion
  
  // Si tu veux changer la direction, inverse la valeur de DIR_PIN
  // Pour inverser la direction :
  // digitalWrite(DIR_PIN, !digitalRead(DIR_PIN)); // Inverse la direction
}
