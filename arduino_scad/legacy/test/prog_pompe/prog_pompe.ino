//Pompe: 

//Déclaration des broches

const int pumpPin = 3;      // Broche PWM pour commander la pompe via le MOSFET
const int buttonPin = 2;    // Broche pour le bouton (avec INPUT_PULLUP)
const int potPin = A0;      // Broche pour le potentiomètre (contrôle de la vitesse)

// Variables de contrôle
bool pumpState = false;     // Pompe éteinte initialement
int lastButtonState = HIGH; // État précédent du bouton (active LOW)
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50; // Délai de debounce (ms)

void setup() {
  pinMode(pumpPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(potPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  // Lecture du bouton avec gestion du rebond
  int reading = digitalRead(buttonPin);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // Si le bouton est pressé (état LOW)
    if (reading == LOW) {
      pumpState = !pumpState; // Inverse l'état de la pompe
      Serial.print("Nouvel état de la pompe : ");
      Serial.println(pumpState ? "ON" : "OFF");
      delay(200); // Petit délai pour éviter les rebonds multiples
    }
  }
  lastButtonState = reading;

  if (pumpState) {
    // Pompe activée : lecture du potentiomètre pour régler la vitesse
    int potValue = analogRead(potPin);
    int pwmValue = map(potValue, 0, 1023, 0, 255);
    analogWrite(pumpPin, pwmValue);
    Serial.print("Vitesse de la pompe (PWM) : ");
    Serial.println(pwmValue);
  } else {
    // Pompe éteinte
    analogWrite(pumpPin, 0);
    Serial.println("Pompe éteinte.");
  }
  
  delay(100); // Petit délai de rafraîchissement
}
