#define IN1 43
#define IN2 39
#define IN3 41
#define IN4 45
#define ENA 8
#define ENB 9

void setup() {
  // Configuration des broches
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);

  Serial.begin(9600);
  Serial.println("Pompes ON");

}

void loop() {
  // Pompe 1 tourne
  digitalWrite(IN1, HIGH); // direction
  digitalWrite(IN2, LOW);
  digitalWrite(ENA, HIGH); // ON

  // Pompe 2 tourne
  digitalWrite(IN4, HIGH); // direction
  digitalWrite(IN3, LOW);
  digitalWrite(ENB, HIGH); // ON

  delay(20000); // Marche 20 sec

  // Stop les deux pompes
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
  delay(2000);
}
