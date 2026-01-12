#include <stdio.h>
#include <stdlib.h> // für rand()
#include "schreibliste.h"
#include "datenbank.h"

#define MAX_MESSUNGEN 20

void starteSimulation(void) {
    printf("Starte Simulation (Zufallswerte)...\n");

    for(int i = 1; i <= MAX_MESSUNGEN; i++) {
        float real = 0.0f;
        printf("\nMessung %d/%d - Realer Abstand (m): ", i, MAX_MESSUNGEN);
        
        if (scanf("%f", &real) != 1) {
            while(getchar() != '\n'); i--; continue;
        }
        while(getchar() != '\n');

        // Zufallswert simulieren
        float sensor = ((float)rand() / RAND_MAX) * 2.0f; 
        float diff = real - sensor;

        printf("Simuliert: Real=%.4f | Sensor=%.4f\n", real, sensor);
        
        schreibeZeile(i, real, sensor, diff);
        messung_speichern(i, real, sensor, diff);
    }
}