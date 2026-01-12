// 22072025
// C-Programm zum Lesen eines µControllers über USB (Jetson Nano)
// Implementierung von Laboraufgabe 1: Datenaufnahme (DA)
// Port: /dev/ttyUSB0
// Baudrate: 115200

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "schreibliste.h"


#define MAX_MESSUNGEN 20

int main() {
    
    dateiVorbereiten();

    const char* port = "/dev/ttyUSB0";
    int serial_port = open(port, O_RDWR);

    if (serial_port < 0) {
        printf("Fehler beim Öffnen von %s: %s\n", port, strerror(errno));
        return 1;
    }

    struct termios tty;
    if (tcgetattr(serial_port, &tty) != 0) {
        printf("Fehler beim Abrufen der Einstellungen: %s\n", strerror(errno));
        close(serial_port);
        return 1;
    }

   
    cfsetispeed(&tty, B115200);
    cfsetospeed(&tty, B115200);


    tty.c_cflag &= ~PARENB;        
    tty.c_cflag &= ~CSTOPB;       
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;            
    tty.c_cflag |= CREAD | CLOCAL; 

    
    tty.c_lflag |= ICANON; 
    tty.c_lflag &= ~(ECHO | ECHOE | ISIG); 

  
    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
        printf("Fehler beim Setzen der Einstellungen: %s\n", strerror(errno));
        close(serial_port);
        return 1;
    }

    printf("Starte Messreihe (Aufgabe 1)...\n");
    printf("Bitte platzieren Sie den Sensor und messen Sie den Abstand mit einem Lineal.\n");

    
    for (int i = 1; i <= MAX_MESSUNGEN; i++) {
        float realerAbstand = 0.0f;
        float sensorWert = 0.0f;
        
       
        printf("\n--- Messung %d von %d ---\n", i, MAX_MESSUNGEN);
        printf("Bitte gemessenen Abstand in Meter eingeben (z.B. 0.5): ");
        
        
        if (scanf("%f", &realerAbstand) != 1) {
            printf("Ungültige Eingabe! Bitte eine Zahl eingeben.\n");
           
            while(getchar() != '\n'); 
            i--; 
            continue;
        }
        
        
        while(getchar() != '\n');

        printf("Warte auf Sensorwert...\n");

       
        tcflush(serial_port, TCIFLUSH);

        
        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        
        // read blockiert jetzt, bis eine Zeile (Enter) vom Sensor kommt
        int num_bytes = read(serial_port, buffer, sizeof(buffer) - 1);

        if (num_bytes < 0) {
            printf("Lesefehler vom Sensor: %s\n", strerror(errno));
            i--; 
        } else {
            buffer[num_bytes] = '\0'; 
            
             
            sensorWert = strtof(buffer, NULL); 
            
            
            float abweichung = realerAbstand - sensorWert;

            
            printf("Eingabe: %.4f m | Sensor: %.4f m | Abweichung: %.4f m\n", 
                   realerAbstand, sensorWert, abweichung);


            char outputLine[512];
            snprintf(outputLine, sizeof(outputLine), "%d;%.4f;%.4f;%.4f", 
                     i, realerAbstand, sensorWert, abweichung);
            
            schreibeZeile(outputLine);
        }
    }

    printf("\nMessreihe abgeschlossen. Daten wurden gespeichert.\n");
    close(serial_port);
    return 0;
}