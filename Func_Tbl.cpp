// 2020-01-11  Markus+Gerhard  Zugriffsfunktionen auf die Pseudodatenbank namens Tbl_heat.h
// 
// 

#include "Func_Tbl.h"



//--------------------------------------------------------------------------------------------
uint16_t T_Idx(uint16_t okz, uint16_t Nr, uint8_t FieldNr, int8_t LngNr) { //, uint8_t LngNr)  
    uint16_t Idx = okz;         // ermittelter Index beginnt beim OFFSET der hier gleich zugewiesen wird.
    if (LngNr == 0) LngNr = CurrLanguage();

    // if (Nr > 0) Nr --;          // da die Tabelle bei 0 beginnt 
    switch (okz) {
    case _S_LNG:
        if (LngNr > _S00_LNGCOUNT)  LngNr = 0;
        Idx += (Nr * _S00_FIELDS) + (LngNr * _S00_FIELDS * _S00_RECORDS); break;  // Sprachkürzel  
    case _S_LNL:
        if (LngNr > _S01_LNGCOUNT)  LngNr = 0;
        Idx += (Nr * _S01_FIELDS) + (LngNr * _S01_FIELDS * _S01_RECORDS); break;  // Sprachlangtext  
    case _S_MSL:
        if (LngNr > _S02_LNGCOUNT)  LngNr = 0;
        Idx += (Nr * _S02_FIELDS) + (LngNr * _S01_FIELDS * _S02_RECORDS); break;  // Meldungen für Notify()
    case _S_HXL:
        if (LngNr > _S03_LNGCOUNT)  LngNr = 0;
        Idx += (Nr * _S03_FIELDS) + (LngNr * _S03_FIELDS * _S03_RECORDS); break;  // DRUCK Sensor HX711 Module Sprachabhängiger Teil 
    case _S_HXD:
        if (LngNr > _S04_LNGCOUNT)  LngNr = 0;
        Idx += (Nr * _S04_FIELDS) + (LngNr * _S04_FIELDS * _S04_RECORDS); break;  // DRUCK Sensor HX711 Module
    case _S_FMD:
        if (LngNr > _S07_LNGCOUNT)  LngNr = 0;
        Idx += (Nr * _S07_FIELDS) + (LngNr * _S07_FIELDS * _S07_RECORDS); break;  // Formatierungs-Strings für Datumausgaben
    case _S_CMD:
        if (LngNr > _S09_LNGCOUNT)  LngNr = 0;
        Idx += (Nr * _S09_FIELDS) + (LngNr * _S09_FIELDS * _S09_RECORDS); break;  // Commandos sind z.zt nur in Sprache 0 definiert 
    case _S_TMP:
        if (LngNr > _S10_LNGCOUNT)  LngNr = 0;
        Idx += (Nr * _S10_FIELDS) + (LngNr * _S10_FIELDS * _S10_RECORDS); break;  // DS18 Temperaturfühler Sprachdaten
    case _S_TMD:
        if (LngNr > _S11_LNGCOUNT)  LngNr = 0;
        Idx += (Nr * _S11_FIELDS) + (LngNr * _S11_FIELDS * _S11_RECORDS); break;  // DS18 Temperaturfühler Daten PIN, Adresse  
    case _S_MNU:
        if (LngNr > _S12_LNGCOUNT)  LngNr = 0;
        Idx += (Nr * _S12_FIELDS) + (LngNr * _S12_FIELDS * _S12_RECORDS); break;  // Menüs Sprachen
    case _S_MNS:
        if (LngNr > _S13_LNGCOUNT)  LngNr = 0;
        Idx += (Nr * _S13_FIELDS) + (LngNr * _S13_FIELDS * _S13_RECORDS); break;  // Menüs Sprungpunkte    
    case _S_MEH:
        if (LngNr > _S14_LNGCOUNT)  LngNr = 0;
        Idx += (Nr * _S14_FIELDS) + (LngNr * _S14_FIELDS * _S14_RECORDS); break;  // Mengeneinheiten 
    case _S_DAY:
        if (LngNr > _S15_LNGCOUNT)  LngNr = 0;
        Idx += (Nr * _S15_FIELDS) + (LngNr * _S15_FIELDS * _S15_RECORDS); break;  // Wochentage Langform
    case _S_DAK:
        if (LngNr > _S16_LNGCOUNT)  LngNr = 0;
        Idx += (Nr * _S16_FIELDS) + (LngNr * _S16_FIELDS * _S16_RECORDS); break;  // Wochentage Kurzform
    case _S_MON:
        if (LngNr > _S17_LNGCOUNT)  LngNr = 0;
        Idx += (Nr * _S17_FIELDS) + (LngNr * _S17_FIELDS * _S17_RECORDS); break;  // Monat Langform
    case _S_MOK:
        if (LngNr > _S18_LNGCOUNT)  LngNr = 0;
        Idx += (Nr * _S18_FIELDS) + (LngNr * _S18_FIELDS * _S18_RECORDS); break;  // Monat Kurzform
    case _S_PAL:
        if (LngNr > _S20_LNGCOUNT)  LngNr = 0;
        Idx += (Nr * _S20_FIELDS) + (LngNr * _S20_FIELDS * _S20_RECORDS); break;  // Parameternamen Sprachdaten
    case _S_PAD:
        if (LngNr > _S21_LNGCOUNT)  LngNr = 0;
        Idx += (Nr * _S21_FIELDS) + (LngNr * _S21_FIELDS * _S21_RECORDS); break;  // Parameter Daten
    case _S_ERL:
        if (LngNr > _S22_LNGCOUNT)  LngNr = 0;
        Idx += (Nr * _S22_FIELDS) + (LngNr * _S22_FIELDS * _S22_RECORDS); break;  // ERROR Sprachdaten
    case _S_ERD:
        if (LngNr > _S23_LNGCOUNT)  LngNr = 0;
        Idx += (Nr * _S23_FIELDS) + (LngNr * _S23_FIELDS * _S23_RECORDS); break;  // ERROR Daten
    case _S_INL:
        if (LngNr > _S28_LNGCOUNT)  LngNr = 0;
        Idx += (Nr * _S28_FIELDS) + (LngNr * _S28_FIELDS * _S28_RECORDS); break;  // INPUT Sprachdaten
    case _S_IND:
        if (LngNr > _S29_LNGCOUNT)  LngNr = 0;
        Idx += (Nr * _S29_FIELDS) + (LngNr * _S29_FIELDS * _S29_RECORDS); break;  // INPUT Daten
    case _S_GEL:
        if (LngNr > _S30_LNGCOUNT)  LngNr = 0;
        Idx += (Nr * _S30_FIELDS) + (LngNr * _S30_FIELDS * _S30_RECORDS); break;  // GERÄTE Sprachen
    case _S_GED:
        if (LngNr > _S31_LNGCOUNT)  LngNr = 0;
        Idx += (Nr * _S31_FIELDS) + (LngNr * _S31_FIELDS * _S31_RECORDS); break;  // GERÄTE Daten
    case _S_KOL:
        if (LngNr > _S32_LNGCOUNT)  LngNr = 0;
        Idx += (Nr * _S32_FIELDS) + (LngNr * _S32_FIELDS * _S32_RECORDS); break;  // KOMBINATION Sprachen
    case _S_KOD:
        if (LngNr > _S33_LNGCOUNT)  LngNr = 0;
        Idx += (Nr * _S33_FIELDS) + (LngNr * _S33_FIELDS * _S33_RECORDS); break;  // KOMBINATION DAten
    case _S_ISL:
        if (LngNr > _S34_LNGCOUNT)  LngNr = 0;
        Idx += (Nr * _S34_FIELDS) + (LngNr * _S34_FIELDS * _S34_RECORDS); break;  // Spezial-Eingänge Sprachen
    case _S_ISD:
        if (LngNr > _S35_LNGCOUNT)  LngNr = 0;
        Idx += (Nr * _S35_FIELDS) + (LngNr * _S35_FIELDS * _S35_RECORDS); break;  // Spezial-Eingänge Daten 
    case _S_MCL:
        if (LngNr > _S40_LNGCOUNT)  LngNr = 0;
        Idx += (Nr * _S40_FIELDS) + (LngNr * _S40_FIELDS * _S40_RECORDS); break;  // MACRO Kopf Sprachdaten
    case _S_MCD:
        if (LngNr > _S41_LNGCOUNT)  LngNr = 0;
        Idx += (Nr * _S41_FIELDS) + (LngNr * _S41_FIELDS * _S41_RECORDS); break;  // MACRO Kopf Daten
    case _S_MCS:
        if (LngNr > _S43_LNGCOUNT)  LngNr = 0;
        Idx += (Nr * _S43_FIELDS) + (LngNr * _S43_FIELDS * _S43_RECORDS); break;  // MACRO Positionen Macro-Schritte


    default:
        Serial.print(F(" not declared Offset in Fkt T_Idx missing case:")); Serial.print(okz);
        Serial.print(F(" Nr:")); Serial.print(Nr); Serial.print(F(" FieldNr:")); Serial.print(FieldNr);
        Serial.println();
        break;
    }
    Idx += FieldNr;   // noch die Feldnummer hinzuzählen
        // Serial.print("Index: "); Serial.println(Idx);
    return Idx;
}



//------------------------------------------------------------------ 
uint8_t FlashGet_Byte(uint16_t LfdNr) {
    uint8_t c;
    //do {
    c = pgm_read_byte(LfdNr); // ein Byte von der Position LfdNr
  // } while (c);
    return c;
}

//-------------------------------------------------------------------
uint16_t FlashGet_uint16(uint16_t LfdNr) {
    uint16_t c;
    c = pgm_read_word(LfdNr); // 
    return c;
}

//-------------------------------------------------------------------------
char* F2buffer(uint16_t LfdNr, uint8_t empty) {
    if (empty == 1) { for (uint8_t i = 0; i < 10; i++) { Buffer1[i] = 0x00; } } // falls nötig kann hier vor dem Lesen der Buffer1 geleert werden.
      // oder  memset(Buffer1, 0, 10); // oder memset(Buffer1, 0, sizeof(Buffer1));
    strcpy_P(Buffer1, (char*)pgm_read_word(&(F_TBL[LfdNr])));
    char* buffer_pt = Buffer1;
    return buffer_pt;
}

//-------------------------------------------------------------------------
char* F2X(uint16_t LfdNr, char xbuffer[]) {
    strcpy_P(xbuffer, (char*)pgm_read_word(&(F_TBL[LfdNr])));
    char* buffer_pt = xbuffer;
    return buffer_pt;
}
char* F2X(uint16_t LfdNr) {
    strcpy_P(Buffer1, (char*)pgm_read_word(&(F_TBL[LfdNr])));
    char* buffer_pt = Buffer1;
    return buffer_pt;
}

//------------------------------------------------------------------------
int8_t TBL_Numbers(uint16_t Adr) {
    sbufferNr[0] = 0; sbufferNr[1] = 0; sbufferNr[2] = 0; sbufferNr[3] = 0; sbufferNr[4] = 0;  // Initialisieren des Zahlenarrays
    int16_t  bStartPos = 0;              // Startposition im Buffer1
    int16_t  bEndPos = 0;              // EndePosition im  Buffer1
    char     Char1;
    int8_t   NrI = 0;              // Zähler von 0...4 für die 5 Nummern
    char* F_pt = &Buffer2[0];   // Zeiger auf Buffer2[0]     
    F2X(Adr, Buffer2);
    Buffer2Len = strlen(Buffer2);
    if (Buffer2Len > 0) {
        bEndPos = Buffer2Len;
        do {
            bStartPos = GetPosNEQ(Buffer2, bStartPos, bEndPos, Char1, ' ');   // nächstes gültiges ZEICHEN
            if (Char1 != 0x00) {                                          // es wurde ein nächstes ZEICHEN gefunden
                sbufferNr[NrI] = atol(F_pt + bStartPos);                    // n-te Zahl beginnend bei 0 ...4  merken                 
                NrI++;                                                     // Index erhöhen
                bStartPos = GetPosEQ(Buffer2, bStartPos, bEndPos, Char1, ' ');  // wo kommt nach der Zahl der nächste TRENNER
                if (Char1 != 0x00) { bStartPos++; }                        // TRENNER gefunden       
            }
        } while (Char1 != 0x00);                                        // solange noch eine passende Position im buffer gefunden wird
    }
    return NrI; // Liefert Anzahl gefundener Zahlen abgelegt in sbufferNr[0..4] 
}

//-------------------------------------------------------------------------
int16_t TBL_Int(uint16_t Adr) {   // -32000 bis +32000 
    return atoi(F2X(Adr, Buffer2));
}

//-------------------------------------------------------------------------
int32_t TBL_Long(uint16_t Adr) {   // -2.147.483.647 bis +  
    return atol(F2X(Adr, Buffer2));
}

//----------------------------------------------------------------------------------------------------
int16_t F_bufflen(char xbuffer[]) {
    int16_t BufLen = 0;
    // BufLen = strlen(xbuffer); // Methode A
    for (uint16_t i = 0; i < sizeof(xbuffer); i++) { if (xbuffer[i] != '\x00') { BufLen++; } else { break; } }   // Methode B
    return BufLen;
}
int16_t F_bufflen() {
    int16_t BufLen = 0;
    BufLen = strlen(Buffer1); // Methode A
    return BufLen;
}

//---------------------------------------------------------------------
int8_t CurrLanguage(int8_t = -1);  // Stadardwert für Sprachenparameter wenn beim Aufruf nichts angegeben wird.
int8_t CurrLanguage(int8_t SetLng) {
    static int8_t Lng;
    if (SetLng >= 0 && SetLng < _LNGCOUNT) {  // nur wenn ein Parameterwert mitgegeben wurde wird die Sprache gestellt
        if (Lng != SetLng) {
            Lng = SetLng;
            EE_ParaLet(2, Lng);         // eingestellte Sprache auch im EEPROM speichern.
        }
    }
    return Lng;
}