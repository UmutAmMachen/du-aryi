#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "schreibliste.h" // Wichtig!


void dateiVorbereiten(void) {
    const char* Döner ="../src/messung.txt";
    if (access(Döner, F_OK) !=-1)
    {
        return;
    }
    FILE *datei = fopen("../src/messung.txt", "w");
    if (datei != NULL) {
        fprintf(datei, "Messung Nr.;Realer Abstand (m);Sensor Wert (m);Abweichung (m)\n");
        fclose(datei);
    }
    else 
    {
        perror("Datei geht net zu erstellen");
    }
}

void schreibeZeile(const char* text) {
    
    FILE *datei = fopen("../src/messung.txt", "a");
    if (datei == NULL) {
        perror("Fehler beim Speichern");
        return;
    }
    
    fprintf(datei, "%s\n", text);
    fclose(datei);
}