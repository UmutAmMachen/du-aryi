#ifndef DATENBANK_H
#define DATENBANK_H
int datenbank_initialisieren(void);
void messung_speichern(int nr, float real, float sensor, float abweichung);
void datenbank_export_csv(const char* dateiname);
void datenbank_schliessen(void);
#endif