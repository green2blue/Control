
#ifndef _FUNC_EEPROM_h
    #define _FUNC_EEPROM_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <EEPROM.h> 
#include "Tbl_heat.h"
#include "Func_Tbl.h"

// --------------------------------------------------------------------------------
//  O F F S E T    Compilervariablen  
// --------------------------------------------------------------------------------
#define _EE_OFFS_HX711    312   // Offset ab der HX711 Daten gespeichert werden pro Modul 10 Byte (2 Byte Calibrated:12345, 4 Byte float Fact, 4 Byte int32_t Offset)
#define _EE_HX711_SIZE     10   // benötigte BYTE pro HX711 Modul    bis 512 können also 20 Stück gespeichert werden.

#define _EE_OFFS_PARA     512   // Offset ab der die  PARAMETER -Werte LONG INTEGER(also je zu 4 Byte)  zu finden sind
#define _EE_PARA_SIZE       4   // Anzahl Byte die ein Parameter einnimt

#define _EE_SEG_STA_OFFS 1536   // Offset für die Statistikdaten pro Gerät/Kombination  2018-06-10 begonnen einzubauen
#define _EE_STA_SIZE       10   // Anzahl Byte pro Statistiksatz .OnYears .OnDays .OnHours .OnMinutes .OnSeconds .Onms .OnCnt .free 5+9+5+6+6+10+32+7=80Bit=10Byte

#define _EE_OFFS_ERR	 1024   // Offset für die Error-Häufigkeit pro Err-Nr (1 Byte(Cnt1) + 2 Byte(Cnt2) + 5 Byte(timestamp)) 
#define _EE_ERR_SIZE        8   // Anzahl Byte pro Errordatensatz 

#define _EE_DS18_areasize  30   // für wieviele DS18-Sensor Adressen ist im EEPROM Platz reserviert



// ----------------------------------------------------------------------------------------
//  Lesen  P A R A M E T E R - N U M M E R - W E R T E  Format LONG INTEGER, also 4 Byte
//  Die Stelle(adresse) an der nach dem Wert zu greifen ist errechnet sich aus:
//  Bsp: ParaNr=1   addr= (ParaNr-1) * AnzahlByteEinesWertes  
//       das ergibt die Reihe 0, 4, 8, 12 und das sind für die Parameter-Nummern die Startadressen im EEPROM (+Offset) 
// ----------------------------------------------------------------------------------------
int32_t EE_ParaGet(uint8_t ParaNr);

// ---------------------------------------------------------------------------------------
//  Speichern  von  P A R A M E T E R - N U M M E R - W E R T E  Format LONG INTEGER
//  SPEICHERT einen Integer-Wert zum Parameter mit der Nummer ParaNr.
//  siehe auch EE_ParaGet()
// ---------------------------------------------------------------------------------------
void EE_ParaLet(uint8_t ParaNr, int32_t ParaVal);

//---------------------------------------------------------------------------------------
//  ADDIEREN SUBTRAHIEREN von Werte zu einem gespeicherten Parameterwert 
//---------------------------------------------------------------------------------------
int32_t EE_ParaWork(uint8_t ParaNr, int32_t ParaWorkVal);

//------------------------------------------------------------------------
// Alle in TBL definierten Parameter auf DEFAULT-WERT im EEPROM schreiben
//------------------------------------------------------------------------
void FactoryParameters();

#endif