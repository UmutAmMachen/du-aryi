#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>     
#include <termios.h>    
#include <fcntl.h>
#include <sys/select.h> 
#include <sys/time.h>   
#include "fun.h"

#define BREITE 40
#define HOEHE 15

// Hilfsfunktion: Tastendruck erkennen
int kbhit(void) {
    struct timeval tv;
    fd_set fds;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
    return FD_ISSET(STDIN_FILENO, &fds);
}

// Terminal Modus umschalten
void setNonBlockingMode(int enable) {
    static struct termios oldt, newt;
    if (enable) {
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    } else {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    }
}

void starteSpiel(void) {
    int playerY = HOEHE / 2;
    int score = 0;
    
    int decke[BREITE];
    int boden[BREITE];

    srand(time(NULL));
    
    // Initialisierung: Breiter, gerader Tunnel
    for (int x = 0; x < BREITE; x++) {
        decke[x] = 1;
        boden[x] = HOEHE - 2;
    }

    printf("\n=== U-BOOT GAME ===\n");
    printf("Steuerung: 'w'=Hoch, 's'=Runter. \n");
    printf("Druecke Enter zum Starten...");
    while(getchar()!='\n');

    setNonBlockingMode(1);

    while(1) {
        // --- 1. Zeichnen ---
        printf("\033[H\033[J"); 

        printf("Score: %d | Tiefe: %d\n", score, playerY);
        printf("########################################\n");

        for (int y = 0; y < HOEHE; y++) {
            for (int x = 0; x < BREITE; x++) {
                if (x == 5 && y == playerY) printf(">"); //PLAYER 
                else if (y <= decke[x] || y >= boden[x]) printf("#");
                else printf(" ");
            }
            printf("\n");
        }
        printf("########################################\n");

        // --- 2. Eingabe ---
        if (kbhit()) {
            char c = getchar();
            if (c == 'e') break;
            if (c == 'w' && playerY > 1) playerY--;
            if (c == 's' && playerY < HOEHE-2) playerY++;
            tcflush(STDIN_FILENO, TCIFLUSH); 
        }

        // --- 3. Kollision ---
        if (playerY <= decke[5] || playerY >= boden[5]) {
            setNonBlockingMode(0);
            printf("\nCRASH! Score: %d\n", score);
            printf("Enter druecken...");
            while(getchar()!='\n'); 
            return;
        }

        // --- 4. Scrolling ---
        for (int x = 0; x < BREITE - 1; x++) {
            decke[x] = decke[x+1];
            boden[x] = boden[x+1];
        }

        // --- 5. NEUE GENERIERUNG (Der Fairness-Patch) ---
        int basisDecke = decke[BREITE-2];
        int basisBoden = boden[BREITE-2];
        
        int neuesDecke = basisDecke;
        int neuesBoden = basisBoden;

        // Chance auf Änderung (Chaos reduziert auf 5%)
        int zufall = rand() % 100;

        if (zufall < 5) { 
            // Hindernis spawnt, aber nicht zu extrem
            if (rand() % 2 == 0) {
                neuesDecke += 2; // Decke kommt etwas runter
            } else {
                neuesBoden -= 2; // Boden kommt etwas hoch
            }
        } else {
            // Normaler Wackler (+1, 0, -1)
            neuesDecke += (rand() % 3 - 1);
            neuesBoden += (rand() % 3 - 1);
        }

        // --- SICHERHEITS-CHECK (WICHTIG!) ---
        // 1. Spielfeldgrenzen einhalten
        if (neuesDecke < 1) neuesDecke = 1;
        if (neuesBoden > HOEHE - 2) neuesBoden = HOEHE - 2;

        // 2. Mindestabstand erzwingen (Die "Anti-Frust"-Regel)
        // Wir sorgen dafür, dass immer mindestens 5 Zeilen Platz sind
        while ((neuesBoden - neuesDecke) < 5) {
            if (neuesDecke > 1) neuesDecke--; // Decke hochziehen
            if (neuesBoden < HOEHE - 2) neuesBoden++; // Boden runterdrücken
        }

        decke[BREITE-1] = neuesDecke;
        boden[BREITE-1] = neuesBoden;

        score++;
        
        // Geschwindigkeit: Startet gemütlich, wird schneller
        // Start: 200ms -> Max Speed: 80ms
        int speed = 200000 - (score * 200); 
        if (speed < 80000) speed = 80000; 
        usleep(speed); 
    }

    setNonBlockingMode(0);
}