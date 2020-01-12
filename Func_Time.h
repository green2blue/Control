// Func_Time.h

#ifndef _FUNC_TIME_h
#define _FUNC_TIME_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#ifndef _TIMESTAMP_
#define _TIMESTAMP_
// Zeitstempel für zum Beispiel den letzten Error-Log-Eintrag pro Fehler im EEPROM 2018-03-29 
typedef struct packed_timestamp_bits {
    unsigned int JJJJ : 12; // Jahr (reicht bis 4095)
    unsigned int MO : 4;  // Monat(0..12) 
    unsigned int DD : 5;  // Tag  (0..31) geht sich gerade aus
    unsigned int HH : 5;  // Stunde (0..24) 
    unsigned int MM : 6;  // Minute (0..60)
    unsigned int SS : 6;  // Sekunde(0..60)
    unsigned int FF : 2;  // 2 Restbits auf 40 Bit = 5 Byte für den Zeitstempel
} timestampstruct;
extern  timestampstruct timestamp; // die Variable timestamp.JJJJ  .MM usw. ist im Programm zu verwenden
#endif



//------------------------------------------------------------------------------------------
// liefert 1 wenn das Jahr ein Schaltjahr ist, wenn der Februal also 29 anstatt 28 Tage hat
//         0 wenn kein Schaltjahr vorliegt
// bin kein Fan von true od. false da sowieso ein ganzes Byte verbraucht wird, kann man auch gleiche eine Zahl liefern.
//------------------------------------------------------------------------------------------
int8_t isYearSwitchYear(uint16_t iJJJJ);

//--------------------------------------------------------------------------
// liefert die Anzahl Tage die ein Monat hat. Bsp: 1=31, 2=28/29, 3=31 usw.
// bei den Monatszahlen 1,3,5,7,8,10,12  ist BIT0 und BIT3 immer <>  --> 31 Tage
// bei den Moantszahlen 4,6,9,11         ist BIT0 und BIT3 immer ==  --> 30 Tage
// Monatszahl 2 hat 28 + isYearSwitchYear Tage 
// Monatstagezahl=  iMM(Bit0) XOR iMM(BIT3) + 30
//--------------------------------------------------------------------------
int8_t MonthhasDays(int16_t iJJJJ, uint8_t iMM);

//-----------------------------------------------------------
// Prüft das Datum in der Struktur timestamp auf Gültigkeit.
//-----------------------------------------------------------
int8_t checkTimestamp();







#endif