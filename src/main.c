#include <stdio.h>
#include <stdlib.h>
#include "funktionen.h"   
#include "schreibliste.h" 
#include "datenbank.h"  
#include "lookup.h"
#include "fun.h"

int main() {
    int wahl = 0;

    // Initialisierungen
    dateiVorbereiten();
    
    // Datenbank starten
    if (datenbank_initialisieren() != 0) {
        printf("Warnung: Datenbank konnte nicht gestartet werden.\n");
    }

    // Lookup-Tabelle laden (falls vorhanden)
    ladeLookupTabelle("lookup.csv");

    while (1) {
        printf("\n=== LABOR PROJEKT (Aufgabe 1-5) ===\n");
        printf("1. Simulation (Aufgabe 1)\n");
        printf("2. Messung mit Sensor (Aufgabe 2)\n");
        printf("3. Datenbank Exportieren (Aufgabe 3)\n");
        printf("4. U-Boot Spiel (Aufgabe 5)\n");
        printf("5. Beenden\n");
        printf("Ihre Wahl: ");

        if (scanf("%d", &wahl) != 1) {
            while(getchar() != '\n'); 
            continue;
        }
        while(getchar() != '\n'); 

        switch (wahl) {
            case 1: starteSimulation(); break;
            case 2: starteMessung(); break;
            case 3: datenbank_export_csv("export.csv"); break;
            case 4: starteSpiel(); break;
            case 5: datenbank_schliessen(); return 0;
            default: printf("Bitte 1-5 wählen.\n"); break;
        }
    }
    return 0;
}