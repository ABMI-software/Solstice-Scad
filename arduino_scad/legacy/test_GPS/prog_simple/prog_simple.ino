#include <SoftwareSerial.h>
#include <TinyGPS++.h>

// Broches à modifier selon ton câblage !
// GPS TX --> Arduino 4
// GPS RX --> Arduino 3
SoftwareSerial gpsSerial(18, 19); // RX, TX
TinyGPSPlus gps;

void setup() {
  Serial.begin(9600);         // Moniteur série (USB)
  gpsSerial.begin(9600);      // Port série GPS (essaye 4800 si 9600 ne marche pas)

  Serial.println("=== Initialisation du GPS ===");
  Serial.println("Attente des données GPS...");
}

void loop() {
  // Vérifie si des données arrivent du GPS
  if (gpsSerial.available()) {
    char c = gpsSerial.read();
    Serial.write(c); // Affiche les données brutes du GPS (NMEA)

    if (gps.encode(c)) {
      Serial.println("\n>> Données GPS décodées !");

      if (gps.location.isValid()) {
        Serial.println("✅ Position GPS valide !");
        Serial.print("Latitude : ");
        Serial.println(gps.location.lat(), 6);
        Serial.print("Longitude : ");
        Serial.println(gps.location.lng(), 6);

        Serial.print("Altitude : ");
        Serial.print(gps.altitude.meters());
        Serial.println(" m");

        Serial.print("Date : ");
        Serial.print(gps.date.day());
        Serial.print("/");
        Serial.print(gps.date.month());
        Serial.print("/");
        Serial.println(gps.date.year());

        Serial.print("Heure : ");
        Serial.print(gps.time.hour());
        Serial.print(":");
        Serial.print(gps.time.minute());
        Serial.print(":");
        Serial.print(gps.time.second());
        Serial.println(" UTC");
      } else {
        Serial.println("❌ Position GPS non valide. Attente du signal...");
      }
    }
  }
}
