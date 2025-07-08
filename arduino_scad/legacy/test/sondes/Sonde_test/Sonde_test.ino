#include <SPI.h>
#include <Adafruit_MAX31865.h>

// Initialiser trois capteurs avec les broches CS différentes
Adafruit_MAX31865 thermo1 = Adafruit_MAX31865(10, 11, 12, 13);
Adafruit_MAX31865 thermo2 = Adafruit_MAX31865(9, 11, 12, 13);
Adafruit_MAX31865 thermo3 = Adafruit_MAX31865(8, 11, 12, 13);

void setup() {
  Serial.begin(9600);
  // Configurez chaque module en fonction de votre sonde (par exemple, 4 fils)
  thermo1.begin(MAX31865_4WIRE);
  thermo2.begin(MAX31865_4WIRE);
  thermo3.begin(MAX31865_4WIRE);
}

void loop() {
  // Exemple de lecture et affichage de la température
  float t1 = thermo1.temperature(100.0, 430.0);
  float t2 = thermo2.temperature(100.0, 430.0);
  float t3 = thermo3.temperature(100.0, 430.0);

  float Delta_t1_t2 = t1-t2;
  float Delta_t2_t3 = t2-t3;
  
  Serial.print("Température bleu : ");   Serial.print(t1, 1); Serial.println(" °C"); Serial.print("\t");
  Serial.print("Température rouge : ");   Serial.print(t2, 1); Serial.println(" °C"); Serial.print("\t");
  Serial.print("Température blanche : "); Serial.print(t3, 1); Serial.println(" °C"); Serial.print("\t");
 
  //Serial.print("Delta T Entrée-Sortie: "); Serial.print( Delta_t1_t2, 1);  Serial.println(" °C"); Serial.print("\t");
  //Serial.print("Delta T Sortie-Cuve: ");   Serial.print( Delta_t2_t3, 1);  Serial.println(" °C"); Serial.print("\t");
  
  delay(100);
}
