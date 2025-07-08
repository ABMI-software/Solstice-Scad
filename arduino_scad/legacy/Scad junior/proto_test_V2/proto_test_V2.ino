// === PINS ENCODEUR ===
#define ENCODER_CLK 3
#define ENCODER_DT 4

// === PINS L298N ===
#define IN1 43
#define ENA 8
#define IN2 45
#define ENB 9   

volatile int compteur = 0;
int lastClk = LOW;

void setup() {
  Serial.begin(9600);

  // Encodeur
  pinMode(ENCODER_CLK, INPUT);
  pinMode(ENCODER_DT, INPUT);
  lastClk = digitalRead(ENCODER_CLK);

  // L298N
  pinMode(IN1, OUTPUT);

  pinMode(ENA, OUTPUT);

  // Sens de rotation fixé
  digitalWrite(IN1, HIGH);

  Serial.println(">> Contrôle moteur 24V via encodeur prêt !");
}

void loop() {
  int currentClk = digitalRead(ENCODER_CLK);
  if (currentClk != lastClk) {
    if (digitalRead(ENCODER_DT) != currentClk) {
      compteur++;
    } else {
      compteur--;
    }

    compteur = constrain(compteur, 0, 100);

    int pwmVal = map(compteur, 0, 100, 0, 255);
    analogWrite(ENA, pwmVal);

    Serial.print("Vitesse PWM : ");
    Serial.print(pwmVal);
    Serial.print(" (");
    Serial.print(compteur);
    Serial.println(" %)");
  }

  lastClk = currentClk;
  delay(1);
}
