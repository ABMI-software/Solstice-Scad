// sensors.h
/**************************************************************************/
/*!
    @file sensors.h
    @brief Header file for the SensorModule class
    Contains the definition of the SensorModule class which handles MAX6675 sensors
    and solar panel power calculations
*/
/**************************************************************************/

#ifndef SENSORS_H
#define SENSORS_H

#include "pins.h" // Include pin definitions
#include "config.h" // Include configuration settings
#include <Arduino.h> // Include Arduino core library
#include <max6675.h> // Include MAX6675 library for temperature sensor

/**************************************************************************/
// === CONSTANTES PHYSIQUES PANNEAU SOLAIRE ===
/**************************************************************************/
const float MASSE_EAU = 3.5;              // kg (3.5L d'eau)
const float CAPACITE_THERMIQUE = 4180.0;  // J/(°C.kg) à 20°C
const float SURFACE_PANNEAU = 0.15;       // m² (0.3m x 0.5m)

// === CARACTÉRISTIQUES PANNEAU (DATASHEET) ===
const float PUISSANCE_MAX_PANNEAU = 3.0;     // W (puissance max du panneau)
const float TENSION_FONCTIONNEMENT = 5.0;    // V
const float COURANT_MAX = 0.6;               // A (600mA)
const float TENSION_CIRCUIT_OUVERT = 6.5;    // V

/**************************************************************************/
// === STRUCTURE POUR MESURES PANNEAU SOLAIRE ===
/**************************************************************************/
struct MesuresSolaires {
  float temp_eau;
  float temp_panneau;
  float elevation_temp_mesuree;      // °C/s mesurée
  float elevation_temp_theorique;    // °C/s calculée depuis la datasheet
  float puissance_electrique;        // W mesurée
  float tension_panneau;             // V mesurée
  float efficacite_panneau;          // %
  float irradiance_estimee;          // W/m²
  bool donnees_valides;              // Indique si les calculs sont valides
};

/**************************************************************************/
// === DÉFINITION DE LA CLASSE SensorModule ===
/**************************************************************************/
class SensorModule {
    public:
        SensorModule(int clkPin, int csPin, int soPin); // Constructeur 
        ~SensorModule(); // Destructor
        void begin(); // Method to initialize the sensor
        void readAll(); // Method to read all sensors and update average temperature
        float getSensor(); // Method to get the current sensor reading
        float readAverage(); // Method to get the average reading

    private:
        MAX6675 sensor; // Instance of the MAX6675 sensor
        float read_avg; // Variable to store the average reading
        float readings[NUM_READINGS]; // Array to store multiple readings for averaging
};

/**************************************************************************/
// === DÉFINITION DE LA CLASSE SolarPanelModule ===
/**************************************************************************/
class SolarPanelModule {
    public:
        SolarPanelModule(int pinTensionPanneau); // Constructeur
        ~SolarPanelModule(); // Destructeur
        
        void begin(); // Initialisation du module
        void update(float temp_eau_actuelle, float temp_panneau_actuelle); // Mise à jour des calculs
        void reset(); // Reset des mesures
        
        // Méthodes de lecture des données
        MesuresSolaires getMesures() const; // Retourne toutes les mesures
        float getElevationMesuree() const; // Retourne l'élévation mesurée
        float getElevationTheorique() const; // Retourne l'élévation théorique
        float getPuissanceElectrique() const; // Retourne la puissance électrique
        float getTensionPanneau() const; // Retourne la tension du panneau
        float getEfficacite() const; // Retourne l'efficacité en %
        float getIrradiance() const; // Retourne l'irradiance estimée
        
        // Méthodes utilitaires
        bool sontDonneesValides() const; // Vérifie si les données sont valides
        void afficherInfos() const; // Affiche les informations sur le Serial
        
    private:
        int pin_tension; // Pin de lecture de tension
        MesuresSolaires mesures; // Structure contenant toutes les mesures
        
        // Variables pour les calculs temporels
        float temperature_eau_precedente;
        unsigned long dernier_temps_mesure;
        bool premiere_mesure;
        
        // Méthodes privées de calcul
        float calculerElevationTheorique(float puissance_watts) const;
        float calculerElevationMesuree(float temp_actuelle, float temp_precedente, unsigned long delta_temps) const;
        float mesurerTensionPanneau() const;
        float estimerPuissanceElectrique(float tension_mesuree) const;
        void calculerEfficaciteEtIrradiance();
};

#endif // SENSORS_H