
#include "config.h" // Include configuration settings
#include <max6675.h> // Include MAX6675 library for temperature sensor
#include "sensors.h"
/**************************************************************************/
/*!
    @brief  Constructor for SensorModule
    Initializes the MAX6675 sensor with the pins defined in pins.h
*//**************************************************************************/

SensorModule::SensorModule(int clkPin, int csPin, int soPin)
    : sensor(clkPin, csPin, soPin), read_avg(0) {
    // Initializing the readings array to zero
    for (int i = 0; i < NUM_READINGS; i++) {
        readings[i] = 0.0;
    }
}
/**************************************************************************/
/*!       
    @brief  Destructor for SensorModule
    Cleans up resources if necessary
*//**************************************************************************/  

SensorModule::~SensorModule() {} 
/**************************************************************************/
/*!   
    @brief  Begin method for SensorModule
    Initializes the sensor, if needed
*//**************************************************************************/          

void SensorModule::begin() {
    // No specific setup needed for MAX6675, but can be added if necessary
}   

/**************************************************************************/
/*!
    @brief  Reads all sensors and updates the average temperature
    This method reads the temperature from the MAX6675 sensor and updates the average
*//**************************************************************************/      


void SensorModule::readAll() {
    // Read the temperature from the MAX6675 sensor
    float temp = sensor.readCelsius();
    
    // Shift readings to make room for the new reading
    for (int i = NUM_READINGS - 1; i > 0; i--) {
        readings[i] = readings[i - 1];
    }
    
    // Store the new reading
    readings[0] = temp;

    // Calculate the average temperature
    read_avg = 0.0;
    for (int i = 0; i < NUM_READINGS; i++) {
        read_avg += readings[i];
    }
    read_avg /= NUM_READINGS;
}

/**************************************************************************/
/*!             
    @brief  Returns the average temperature reading
    @returns The average temperature from the readings
*//**************************************************************************/
float SensorModule::getSensor() {
    return read_avg;
}
/**************************************************************************/
/*!           
    @brief  Reads the average temperature from the sensor
    This method is kept for compatibility, but it now uses getSensor()
    @returns The average temperature reading
*//**************************************************************************/    
float SensorModule::readAverage() {
    // Calculate the average of the readings
    float sum = 0.0;
    for (int i = 0; i < NUM_READINGS; i++) {
        sum += readings[i];
    }
    return sum / NUM_READINGS;
}
/**************************************************************************/


SolarPanelModule::SolarPanelModule(int pinTensionPanneau) 
    : pin_tension(pinTensionPanneau), temperature_eau_precedente(0.0), 
      dernier_temps_mesure(0), premiere_mesure(true) {
    
    // Initialisation de la structure des mesures
    mesures.temp_eau = 0.0;
    mesures.temp_panneau = 0.0;
    mesures.elevation_temp_mesuree = 0.0;
    mesures.elevation_temp_theorique = 0.0;
    mesures.puissance_electrique = 0.0;
    mesures.tension_panneau = 0.0;
    mesures.efficacite_panneau = 0.0;
    mesures.irradiance_estimee = 0.0;
    mesures.donnees_valides = false;
}

SolarPanelModule::~SolarPanelModule() {}

void SolarPanelModule::begin() {
    pinMode(pin_tension, INPUT);
    reset();
    Serial.println("SolarPanelModule initialisé");
    
    // Affichage des paramètres
    Serial.print("Puissance max datasheet: "); Serial.print(PUISSANCE_MAX_PANNEAU); Serial.println(" W");
    Serial.print("Élévation théorique max: "); 
    Serial.print(calculerElevationTheorique(PUISSANCE_MAX_PANNEAU) * 1000, 3); 
    Serial.println(" m°C/s");
}

void SolarPanelModule::reset() {
    premiere_mesure = true;
    temperature_eau_precedente = 0.0;
    dernier_temps_mesure = 0;
    mesures.donnees_valides = false;
}

void SolarPanelModule::update(float temp_eau_actuelle, float temp_panneau_actuelle) {
    unsigned long temps_actuel = millis();
    
    // Mesure de la tension et puissance du panneau
    mesures.tension_panneau = mesurerTensionPanneau();
    mesures.puissance_electrique = estimerPuissanceElectrique(mesures.tension_panneau);
    
    // Stockage des températures
    mesures.temp_eau = temp_eau_actuelle;
    mesures.temp_panneau = temp_panneau_actuelle;
    
    if (!premiere_mesure) {
        unsigned long delta_temps = temps_actuel - dernier_temps_mesure;
        
        if (delta_temps >= 1000) { // Au moins 1 seconde écoulée
            // Calcul de l'élévation mesurée
            mesures.elevation_temp_mesuree = calculerElevationMesuree(
                temp_eau_actuelle, temperature_eau_precedente, delta_temps);
            
            // Calcul de l'élévation théorique avec la puissance mesurée
            mesures.elevation_temp_theorique = calculerElevationTheorique(mesures.puissance_electrique);
            
            // Calcul de l'efficacité et irradiance
            calculerEfficaciteEtIrradiance();
            
            // Mise à jour des variables pour le prochain calcul
            temperature_eau_precedente = temp_eau_actuelle;
            dernier_temps_mesure = temps_actuel;
            
            mesures.donnees_valides = true;
        }
    } else {
        // Première mesure : initialisation
        temperature_eau_precedente = temp_eau_actuelle;
        dernier_temps_mesure = temps_actuel;
        premiere_mesure = false;
        mesures.donnees_valides = false;
    }
}

/**************************************************************************/
// === MÉTHODES PUBLIQUES D'ACCÈS AUX DONNÉES ===
/**************************************************************************/

MesuresSolaires SolarPanelModule::getMesures() const {
    return mesures;
}

float SolarPanelModule::getElevationMesuree() const {
    return mesures.elevation_temp_mesuree;
}

float SolarPanelModule::getElevationTheorique() const {
    return mesures.elevation_temp_theorique;
}

float SolarPanelModule::getPuissanceElectrique() const {
    return mesures.puissance_electrique;
}

float SolarPanelModule::getTensionPanneau() const {
    return mesures.tension_panneau;
}

float SolarPanelModule::getEfficacite() const {
    return mesures.efficacite_panneau;
}

float SolarPanelModule::getIrradiance() const {
    return mesures.irradiance_estimee;
}

bool SolarPanelModule::sontDonneesValides() const {
    return mesures.donnees_valides;
}

void SolarPanelModule::afficherInfos() const {
    Serial.println("=== PANNEAU SOLAIRE ===");
    Serial.print("Tension: "); Serial.print(mesures.tension_panneau, 2); Serial.println(" V");
    Serial.print("Puissance: "); Serial.print(mesures.puissance_electrique, 3); Serial.println(" W");
    Serial.print("Élévation mesurée: "); Serial.print(mesures.elevation_temp_mesuree * 1000, 3); Serial.println(" m°C/s");
    Serial.print("Élévation théorique: "); Serial.print(mesures.elevation_temp_theorique * 1000, 3); Serial.println(" m°C/s");
    Serial.print("Efficacité: "); Serial.print(mesures.efficacite_panneau, 1); Serial.println(" %");
    Serial.print("Irradiance: "); Serial.print(mesures.irradiance_estimee, 0); Serial.println(" W/m²");
    Serial.println("=======================");
}

/**************************************************************************/
// === MÉTHODES PRIVÉES DE CALCUL ===
/**************************************************************************/

float SolarPanelModule::calculerElevationTheorique(float puissance_watts) const {
    if (puissance_watts <= 0) return 0.0;
    
    // ΔT/Δt = P / (m × c)
    float elevation = puissance_watts / (MASSE_EAU * CAPACITE_THERMIQUE);
    return elevation; // °C/s
}

float SolarPanelModule::calculerElevationMesuree(float temp_actuelle, float temp_precedente, unsigned long delta_temps) const {
    if (delta_temps == 0) return 0.0;
    
    float delta_temp = temp_actuelle - temp_precedente;
    float delta_temps_sec = delta_temps / 1000.0;
    
    return delta_temp / delta_temps_sec; // °C/s
}

float SolarPanelModule::mesurerTensionPanneau() const {
    int lecture_adc = analogRead(pin_tension);
    
    // Conversion ADC vers tension (0-1023 -> 0-5V)
    float tension = (lecture_adc * 5.0) / 1023.0;
    
    // Ajustement si diviseur de tension utilisé
    // Exemple pour diviseur 2:1 : tension *= 2.0;
    
    return tension;
}

float SolarPanelModule::estimerPuissanceElectrique(float tension_mesuree) const {
    if (tension_mesuree < 1.0) return 0.0; // Panneau non éclairé
    
    // Approximation basée sur la courbe I-V du panneau
    // P = U × I, avec I qui dépend de U et de l'irradiance
    
    float ratio_tension = tension_mesuree / TENSION_CIRCUIT_OUVERT;
    
    // Modèle simplifié (à affiner selon les tests réels)
    float puissance_estimee = PUISSANCE_MAX_PANNEAU * ratio_tension * 0.8;
    
    return constrain(puissance_estimee, 0.0, PUISSANCE_MAX_PANNEAU);
}

void SolarPanelModule::calculerEfficaciteEtIrradiance() {
    // Calcul de l'efficacité thermique
    float elevation_theo_max = calculerElevationTheorique(PUISSANCE_MAX_PANNEAU);
    
    if (elevation_theo_max > 0 && mesures.elevation_temp_theorique > 0) {
        mesures.efficacite_panneau = (mesures.elevation_temp_mesuree / mesures.elevation_temp_theorique) * 100.0;
        
        // Limitation de l'efficacité à des valeurs réalistes
        mesures.efficacite_panneau = constrain(mesures.efficacite_panneau, 0.0, 200.0);
    } else {
        mesures.efficacite_panneau = 0.0;
    }
    
    // Estimation de l'irradiance (1000 W/m² = plein soleil)
    if (PUISSANCE_MAX_PANNEAU > 0) {
        float ratio_puissance = mesures.puissance_electrique / PUISSANCE_MAX_PANNEAU;
        mesures.irradiance_estimee = ratio_puissance * 1000.0; // W/m²
        mesures.irradiance_estimee = constrain(mesures.irradiance_estimee, 0.0, 1200.0);
    } else {
        mesures.irradiance_estimee = 0.0;
    }
}