import serial
import csv
from datetime import datetime

# === CONFIGURATION ===
PORT = "COM3"         # Remplace COM4 par le port série de ton Arduino (ex : COM3, COM5...)
BAUD_RATE = 9600
OUTPUT_FILE = "donnees_capteurs.csv"  # Nom du fichier à générer

# === OUVERTURE DU PORT SÉRIE ===
ser = serial.Serial(PORT, BAUD_RATE, timeout=1)

# === ÉCRITURE DANS LE FICHIER CSV ===
with open(OUTPUT_FILE, mode='w', newline='') as file:
    writer = csv.writer(file)
    # Entête du fichier CSV
    writer.writerow(["Horodatage", "PTC1 (°C)", "PTC2 (°C)", "Cuve1 (°C)", "Cuve2 (°C)"])
    
    print(" Enregistrement en cours... (Ctrl+C pour arreter)\n")

    try:
        while True:
            line = ser.readline().decode('utf-8').strip()
            if line:
                # Exemple attendu : "Heure : 14:25:11 ; PTC1 : 123.5 °C ; PTC2 : 130.1 °C ; Cuve1 : 100.2 °C ; Cuve2 : 95.3 °C"
                values = [x.strip() for x in line.split(';') if ':' in x]
                if len(values) == 5:
                    horodatage = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
                    # Extraction des valeurs numériques
                    temp_vals = []
                    for val in values[1:]:
                        try:
                            temp = float(val.split(':')[1].replace('°C','').strip())
                            temp_vals.append(temp)
                        except:
                            temp_vals.append('Erreur')
                    writer.writerow([horodatage] + temp_vals)
                    print(f"{horodatage} -> {temp_vals}")
    except KeyboardInterrupt:
        print("\n Enregistrement arrete par l'utilisateur.")
    finally:
        ser.close()
