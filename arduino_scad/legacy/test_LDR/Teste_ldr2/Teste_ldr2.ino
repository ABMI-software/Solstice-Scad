int ldrPin = A1;
float Vref = 5.0;          // Tension de référence de l'Arduino
int R = 10000;             // Résistance fixe de 10 kΩ
 
void setup() {
  Serial.begin(9600);
}
 
void loop() {
  int adcValue = analogRead(ldrPin);
  float voltage = (adcValue * Vref) / (R + adcValue);
  float resistanceLDR = (Vref * R / voltage) - R;
  float LDR = (voltage * R) / (Vref - voltage);
 
  Serial.print("ADC: ");
  Serial.print(adcValue);
  Serial.print(" | Voltage: ");
  Serial.print(voltage, 2);
  Serial.print(" V | Resistance LDR: ");
  Serial.print(resistanceLDR);
  Serial.print(" ohms");
  Serial.print("  LDR: ");
  Serial.print(LDR);
  Serial.println(" Lux");
 
  delay(1000);
}
 