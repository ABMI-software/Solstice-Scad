// === PINS DE L'ENCODEUR ===
#define ENCODER_CLK 3
#define ENCODER_DT 5

// === PIN DU RELAIS ===

#define RELAIS_PIN 4

int lastClkState;
int compteur = 0;

void setup() {
  pinMode(ENCODER_CLK, INPUT);
  pinMode(ENCODER_DT, INPUT);

  pinMode(RELAIS_PIN, OUTPUT);
  digitalWrite(RELAIS_PIN, LOW); // relais OFF au démarrage

  Serial.begin(9600);
  lastClkState = digitalRead(ENCODER_CLK); // état initial
}

void loop() {
  int currentClk = digitalRead(ENCODER_CLK);
  
  if (currentClk != lastClkState) {
    if (digitalRead(ENCODER_DT) != currentClk) {
      compteur++;
    } else {
      compteur--;
    }

    compteur = constrain(compteur, 0, 100); // limite entre 0 et 100%

    Serial.print("Débit: ");
    Serial.print(compteur);
    Serial.println(" %");

    /*// Activation/désactivation du relais selon la valeur
    if (compteur > 0) {
      digitalWrite(RELAIS_PIN, HIGH); // relais ON
    } else {
      digitalWrite(RELAIS_PIN, LOW);  // relais OFF
    }*/
  }
  lastClkState = currentClk;
  delay(1);
} 