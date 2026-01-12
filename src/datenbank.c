#include <stdio.h>
#include <sqlite3.h>
#include "datenbank.h"

static sqlite3 *db = NULL;

int datenbank_initialisieren(void) {
    if (sqlite3_open("messdaten.db", &db) != SQLITE_OK) return -1;
    const char *sql = "CREATE TABLE IF NOT EXISTS messungen (id INTEGER PRIMARY KEY, nr INTEGER, real REAL, sensor REAL, diff REAL);";
    sqlite3_exec(db, sql, 0, 0, 0);
    return 0;
}

void messung_speichern(int nr, float real, float sensor, float diff) {
    if (!db) return;
    char sql[256];
    snprintf(sql, sizeof(sql), "INSERT INTO messungen (nr, real, sensor, diff) VALUES (%d, %.4f, %.4f, %.4f);", nr, real, sensor, diff);
    sqlite3_exec(db, sql, 0, 0, 0);
    printf("(Gespeichert in DB)\n");
}

void datenbank_export_csv(const char* dateiname) {
    if (!db) return;
    FILE *f = fopen(dateiname, "w");
    if (!f) return;
    
    fprintf(f, "Nr;Real;Sensor;Diff\n");
    sqlite3_stmt *res;
    sqlite3_prepare_v2(db, "SELECT nr, real, sensor, diff FROM messungen", -1, &res, 0);
    
    while (sqlite3_step(res) == SQLITE_ROW) {
        fprintf(f, "%d;%f;%f;%f\n", 
            sqlite3_column_int(res, 0), sqlite3_column_double(res, 1),
            sqlite3_column_double(res, 2), sqlite3_column_double(res, 3));
    }
    sqlite3_finalize(res);
    fclose(f);
    printf("Export fertig: %s\n", dateiname);
}

void datenbank_schliessen(void) {
    if (db) sqlite3_close(db);
}