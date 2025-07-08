# Boîtier de Test pour Panneau Solaire Thermique

## Description
Ce projet Arduino permet de créer un système complet de test pour panneau solaire thermique. Il permet la mesure et l'enregistrement de paramètres clés tels que la température (à 4 points de mesure) et le débit d'eau (via 2 contrôles indépendants).

## Fonctionnalités
- Mesure en temps réel de 4 températures via capteurs thermocouples type K + modules MAX6675
- Contrôle du débit d'eau via 2 encodeurs rotatifs KY-040 (PWM 0-100%)
- Pilotage de 2 pompes DC via driver L298N
- Affichage des valeurs en temps réel sur écran OLED I2C (128x64)
- Enregistrement horodaté des mesures sur carte SD au format CSV

## Matériel nécessaire
- Arduino Mega 2560
- 4 modules MAX6675 avec sondes thermocouples type K
- 2 encodeurs rotatifs KY-040
- 1 module driver L298N pour moteurs DC
- 2 pompes DC
- 1 écran OLED I2C 128x64
- 1 module carte SD
- Module RTC DS3231 (optionnel)
- Résistances, câbles et alimentation adaptée

## Bibliothèques requises
- Adafruit_MAX6675.h
- Adafruit_SSD1306.h
- Adafruit_GFX.h
- SD.h
- SPI.h
- RTClib.h (optionnel, pour horodatage)

## Structure du projet
```
/Projet_Solaire_Thermique/
├── main.ino                 // Programme principal
├── hardware.h               // Configuration matérielle
├── hardware.cpp             // Implémentation hardware
├── sensors/                 // Gestion des capteurs de température
│   ├── sensors.h
│   └── sensors.cpp
├── pumps/                   // Gestion des pompes
│   ├── pumps.h
│   └── pumps.cpp
├── encoders/                // Gestion des encodeurs
│   ├── encoders.h
│   └── encoders.cpp
├── display/                 // Gestion de l'affichage OLED
│   ├── display.h
│   └── display.cpp
├── sdlogger/                // Gestion de l'enregistrement sur SD
│   ├── sdlogger.h
│   └── sdlogger.cpp
└── README.md                // Ce fichier
```

## Format des données enregistrées
Les données sont enregistrées dans un fichier CSV avec la structure suivante:
```
Horodatage,Temp1,Temp2,Temp3,Temp4,Debit1,Debit2
2025-04-28 14:03:00,45.2,46.1,49.5,43.2,38,42
```

## Installation
1. Installez les bibliothèques requises via le gestionnaire de bibliothèques Arduino
2. Connectez les composants selon le schéma fourni dans hardware.h
3. Téléversez le programme dans votre Arduino Mega 2560
4. Vérifiez que la carte SD est correctement formatée (FAT16/FAT32)

## Utilisation
1. Mettez le système sous tension
2. L'écran OLED affichera les températures et les débits
3. Utilisez les encodeurs pour régler le débit des pompes
4. Les données sont automatiquement enregistrées sur la carte SD

## Licence
Ce projet est distribué sous licence libre.