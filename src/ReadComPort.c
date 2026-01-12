#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "schreibliste.h"
#include "datenbank.h"
#include "lookup.h"

#define MAX_MESSUNGEN 20

void starteMessung(void) {
    const char* port = "/dev/ttyUSB0";
    int serial_port = open(port, O_RDWR);

    if (serial_port < 0) {
        printf("Fehler: Sensor nicht gefunden (%s).\n", strerror(errno));
        printf("TIPP: Nutze Option 1 (Simulation), wenn kein Sensor da ist.\n");
        return;
    }

    struct termios tty;
    tcgetattr(serial_port, &tty);
    cfsetispeed(&tty, B115200);
    cfsetospeed(&tty, B115200);
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8 | CREAD | CLOCAL;
    tty.c_lflag |= ICANON;
    tty.c_lflag &= ~(ECHO | ECHOE | ISIG);
    tcsetattr(serial_port, TCSANOW, &tty);

    printf("Starte Sensor-Messung...\n");

    for (int i = 1; i <= MAX_MESSUNGEN; i++) {
        float real = 0.0f, sensor = 0.0f;
        
        printf("\nMessung %d/%d - Realer Abstand (m): ", i, MAX_MESSUNGEN);
        if (scanf("%f", &real) != 1) {
            while(getchar() != '\n'); i--; continue;
        }
        while(getchar() != '\n');

        printf("Warte auf Sensor...\n");
        tcflush(serial_port, TCIFLUSH);
        char buffer[256];
        int n = read(serial_port, buffer, sizeof(buffer)-1);

        if (n > 0) {
            buffer[n] = '\0';
            float raw = strtof(buffer, NULL);
            
            // Interpolation nutzen (falls Tabelle geladen)
            if (anzahlEintraege > 0) sensor = berechneAbstand((int)raw);
            else sensor = raw;

            float diff = real - sensor;
            printf("Sensor: %.4f | Real: %.4f | Diff: %.4f\n", sensor, real, diff);
            
            schreibeZeile(i, real, sensor, diff);
            messung_speichern(i, real, sensor, diff);
        } else {
            printf("Lesefehler!\n");
            i--;
        }
    }
    close(serial_port);
}