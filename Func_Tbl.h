// Func_Tbl.h

#ifndef _FUNC_TBL_h
#define _FUNC_TBL_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Global.h"
#include "Tbl_heat.h"
#include "Func_Help.h"
#include "Func_EEPROM.h"

//---------------------------------------------------------------------------
// Tbl Funktionen und Beispiele zum Auslesen von Inhalten aus der Tbl
//---------------------------------------------------------------------------
#define BIT_NOT(i)                 (~(i))       // bitweises INVERTIEREN
#define BIT_OR(a, b)               ((a) | (b))  // bitweises ODER   
#define BIT_AND(a, b)              ((a) & (b))  // bitweises UND
#define BIT_XOR(a, b)              ((a) ^ (b))  // exclusives ODER
#define BIT_SHL(a, n)              ((a) << (n)) // BIT verschieben um n Stellen nach LINKS
#define BIT_SHR(a, n)              ((a) >> (n)) // BIT verschieben um n Stellen nach RECHTS
//--------------------------------------------------------------------------------------------
// ZENTRALE Funktion um zum richtigen Satz und damit INHALT aus TBL zu kommen.
// Jeder Bereich aus TBL ist hier anzuführen.
// Index zu einem Bereich / einen Datensatz / ein Feld / in Sprache  Eintrag in Tbl ermitteln
//   okz    = _S_LNG / _S_TMP / _S_MEH / S_...  eines der OFFSET-Hilfsvariablen für den Bereich
//   Nr     = Das wievielte Element aus dem Bereich von 1...n  beginnend bei 1
//   FNr    = Feldnummer, welches Feld                         beginnend bei 0 
//   LngNr  = SprachNummer 0,... _S00_RECORDS                  beginnend bei 0
// nur wenn Sprache auch definiert ist in TBL darf darauf zugegriffen werden, gilt für alle folgenden Zeilen
//  if (LngNr > ...
//--------------------------------------------------------------------------------------------
uint16_t T_Idx(uint16_t okz, uint16_t Nr, uint8_t FieldNr, int8_t = 0);
uint16_t T_Idx(uint16_t okz, uint16_t Nr, uint8_t FieldNr, int8_t LngNr);


//------------------------------------------------------------------ 
// 2018-03-04  1 BYTE aus der Tbl holen und liefern 
//------------------------------------------------------------------ 
uint8_t FlashGet_Byte(uint16_t LfdNr);

//-------------------------------------------------------------------
// 2018-03-04  2 Byte aus der Tbl holen  siehe auch, falls benötigt:
//  c= pgm_read_dword(int16_t LfdNr) 
//  c= pgm_read_float(int16_t LfdNr)
//-------------------------------------------------------------------
uint16_t FlashGet_uint16(uint16_t LfdNr);

//-------------------------------------------------------------------------
// EINE  Zeile aus Tbl in den F_buffer[] und Zeiger auf F_buffer liefern
//-------------------------------------------------------------------------
char* F2buffer(uint16_t LfdNr, uint8_t empty);

//-------------------------------------------------------------------------
// EINE  Zeile aus Tbl in einen char buffer holen und Zeiger darauf liefern 
//  Unterschied zu F2buffer, man muß als 2.Parameter den zu befüllenden buffer[] angeben.
// Bsp: F2X(10, Buffer1)  füllt Buffer1[] 
//      F2X(10, Buffer2)  füllt Buffer2[] 
//-------------------------------------------------------------------------
char* F2X(uint16_t LfdNr, char xbuffer[]);

char* F2X(uint16_t LfdNr);

//------------------------------------------------------------------------
// einzelne Zahlen von einer bestimmten Adresse aus TBL liefern
// Inhalt in TBL-Zeile Bsp: n,56,1  --> soll die 3 Zahlen 0,56,1 in das Array schreiben
// Die Fkt befördert die Zahlen in das Array sbufferNr[0..4] und liefert die Anzahl gefundener Positionen im Buffer1[]
//------------------------------------------------------------------------
int8_t TBL_Numbers(uint16_t Adr);

//-------------------------------------------------------------------------
// Integer Zahl von einer bestimmten Adresse in TBL liefern
//-------------------------------------------------------------------------
int16_t TBL_Int(uint16_t Adr);
//-------------------------------------------------------------------------
// long Zahl von einer bestimmten Adresse aus TBL liefern
//-------------------------------------------------------------------------
int32_t TBL_Long(uint16_t Adr);

//----------------------------------------------------------------------------------------------------
// ermittelt/liefert die Buffer1Len (BUFFER-LÄNGE) mit unterschiedlichen Verfahren
// 0=Zeichen ungleich 0x00 zählen    1=strlen(buffer)   ich weiß nicht was besser ist.
//----------------------------------------------------------------------------------------------------
int16_t F_bufflen(char xbuffer[]);

int16_t F_bufflen(); 


//---------------------------------------------------------------------
// Sprache einstellen/lesen  
// 0=Standard=engl.  1=deutsch  2=französisch  siehe Tbl_ Bereich LNG
// Wenn man im Parameter SetLng einen Wert >= 0 angibt wird diese Nummer
// zur aktuellen Sprache gemacht.
// Bsp: CurrLanguage(1)  stellt die Sprache auf Deutsch
//                  (0)  stellt die Sprache auf engl.
//                  (-1) stellt nichts sondern liefert nur die aktuell eingestellte Sprache
//---------------------------------------------------------------------
int8_t CurrLanguage(int8_t = -1);  // Stadardwert für Sprachenparameter wenn beim Aufruf nichts angegeben wird.
int8_t CurrLanguage(int8_t SetLng);


#endif