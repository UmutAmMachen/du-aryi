#include <stdio.h>
#include <unistd.h>
#include "schreibliste.h"

#define DATEI "messung.txt"

void dateiVorbereiten(void) {
    if (access(DATEI, F_OK) != -1) return;
    FILE *f = fopen(DATEI, "w");
    if (f) {
        fprintf(f, "| Nr | Real | Sensor | Diff |\n|----|------|--------|------|\n");
        fclose(f);
    }
}

void schreibeZeile(int nr, double real, double sensor, double abweichung) {
    FILE *f = fopen(DATEI, "a");
    if (f) {
        fprintf(f, "| %d | %.4f | %.4f | %.4f |\n", nr, real, sensor, abweichung);
        fclose(f);
    }
}