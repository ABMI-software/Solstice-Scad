# ☀️ Solstice - SCAD – Smart Climate and Autonomous Distribution

**Solstice** est un système embarqué intelligent de gestion climatique (SCAD) capable de surveiller la température, de calculer la position du soleil à partir d’un module GPS, et d’activer des modules de pompe ou de ventilation en fonction de données environnementales. Il est conçu pour fonctionner de manière autonome ou connectée, avec un enregistrement local (SD Card) ou distant (serveur + dashboard).

---

## 🌐 Objectifs

- Lire des capteurs de température de type MAX6675 (thermocouple K).
- Utiliser un module GPS pour déterminer :
  - Latitude / Longitude
  - Heure UTC → convertie en heure locale (TimerSCAD)
- Déterminer automatiquement s’il fait **jour/nuit** en fonction de la position du soleil.
- Activer une pompe / ventilation selon des conditions configurables.
- Envoyer les données en temps réel à un serveur distant via Raspberry Pi (optionnel).
- Sauvegarder les logs localement sur carte SD (optionnel).
- Visualiser les données via un dashboard (optionnel).

---

## ⚙️ Architecture du Projet

### 📁 Arduino

arduino/
├── include/
│ ├── config.h # Configuration (offset UTC, seuils, pins…)
│ ├── TimerSCAD.h # Convertit UTC → Heure locale (gestion été/hiver)
│ └── GPS.h # Module GPS (coordonnées, date, heure)
│
├── lib/
│ ├── Sensors/ # Gestion des capteurs MAX6675
│ ├── Pump/ # Contrôle de la pompe
│ └── OLED/ # Affichage OLED (optionnel)
│
├── src/
│ ├── serial_comm.cpp # Envoi des données vers Raspberry
│ ├── logger.cpp # Sauvegarde dans carte SD (optionnel)
│ └── main.ino # Point d'entrée du programme Arduino

shell
Copier
Modifier

### 🧠 Raspberry Pi (en C++)

raspberry/
├── include/
│ ├── SerialReader.h # Lecture UART depuis Arduino
│ ├── Logger.h # Écriture dans fichier CSV
│ ├── NetworkSender.h # Envoi HTTP ou MQTT vers serveur
│
├── src/
│ ├── main.cpp
│ ├── SerialReader.cpp
│ ├── Logger.cpp
│ └── NetworkSender.cpp

yaml
Copier
Modifier

---

## 🔧 Dépendances

### Arduino

- [TinyGPS++](https://github.com/mikalhart/TinyGPSPlus)
- [Adafruit MAX6675](https://github.com/adafruit/MAX6675-library)
- [SSD1306 OLED (optionnel)](https://github.com/adafruit/Adafruit_SSD1306)
- [SD library (optionnel)](https://www.arduino.cc/reference/en/libraries/sd/)

### Raspberry Pi

- g++ (C++17 ou +)
- WiringPi ou libserial (selon méthode de lecture UART)
- MySQL client ou bibliothèque MQTT (si serveur distant activé)

---

## 🚀 Déploiement

### Étape 1 : Arduino

1. Brancher les capteurs (MAX6675), GPS, pompe (relais), et éventuellement OLED/SD.
2. Configurer `config.h` :
   ```cpp
   #define UTC_OFFSET +2  // Heure d’été France
Compiler et uploader main.ino via l’IDE Arduino.

Étape 2 : Raspberry Pi (optionnel)
Brancher en UART RX/TX avec Arduino.

Compiler avec :

bash
Copier
Modifier
g++ -std=c++17 -o solstice src/*.cpp
Lancer le programme :

bash
Copier
Modifier
./solstice
