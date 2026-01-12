#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h> // für rand()
#include "schreibliste.h"


#define MAX_MESSUNGEN 20

int main() 
{
    
    dateiVorbereiten();
    
    printf("Starte Simulation (Aufgabe 1)...\n");
    printf("Dies ist der Simulations-Modus ohne echten Sensor.\n");

    
    for(int i = 1; i <= MAX_MESSUNGEN; i++)
    {
        float realerAbstand = 0.0f;
        
       
        printf("\n--- Messung %d von %d ---\n", i, MAX_MESSUNGEN);
        printf("Bitte gemessenen Abstand in Meter eingeben (z.B. 0.5): ");
        
       
        if (scanf("%f", &realerAbstand) != 1) {
            printf("Ungültige Eingabe! Bitte eine Zahl eingeben.\n");
            while(getchar() != '\n'); 
            i--; 
            continue;
        }
        while(getchar() != '\n'); 

        
        float sensorWert = ((float)rand() / RAND_MAX) * 2.0f; 
        
        
        float abweichung = realerAbstand - sensorWert;

        printf("Simuliert: Real=%.4f m | Sensor=%.4f m | Diff=%.4f m\n", 
               realerAbstand, sensorWert, abweichung);

        // 5. Speichern
        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        
        
        snprintf(buffer, sizeof(buffer), "%d;%.4f;%.4f;%.4f", 
                 i, realerAbstand, sensorWert, abweichung);
                 
        schreibeZeile(buffer);
    }

    printf("\nSimulation abgeschlossen. Daten gespeichert.\n");
    return 0;
}