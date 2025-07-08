/**
 * sdlogger.h - Gestion de l'enregistrement des données sur carte SD
 */

#ifndef SDLOGGER_H
#define SDLOGGER_H

#include <Arduino.h>
#include <SD.h>

// Nom du fichier de log
#define LOG_FILENAME "log.csv"

// Prototypes des fonctions
bool initSDLogger();
bool logData(String timestamp);
String createLogEntry(String timestamp);

#endif