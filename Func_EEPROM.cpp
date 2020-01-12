// 
// 
// 

#include "Func_EEPROM.h"

//------------------------------------------------------------------------------------
int32_t EE_ParaGet(uint8_t ParaNr) {
    int32_t ParaVal;
    EEPROM.get(_EE_OFFS_PARA + (ParaNr - 1) * _EE_PARA_SIZE, ParaVal);  // Einlesen der 4 Byte in den Integer RetVal
    return ParaVal;
}


// ---------------------------------------------------------------------------------------
void EE_ParaLet(uint8_t ParaNr, int32_t ParaVal) {
    EEPROM.put(_EE_OFFS_PARA + (ParaNr - 1) * _EE_PARA_SIZE, ParaVal);    // Schreiben der 4 Byte ins EEPROM (nur wenn sich etwas geändert hat)
}

//---------------------------------------------------------------------------------------
int32_t EE_ParaWork(uint8_t ParaNr, int32_t ParaWorkVal) {
    int32_t ParaVal;
    EEPROM.get(_EE_OFFS_PARA + (ParaNr - 1) * _EE_PARA_SIZE, ParaVal);   // Einlesen aktuell gespeicherter Wert (4 Byte)
    ParaVal = ParaVal + ParaWorkVal;                                    // hinzurechnen/abziehen  des mitgegebenen Wertes 
    EEPROM.put(_EE_OFFS_PARA + (ParaNr - 1) * _EE_PARA_SIZE, ParaVal);   // Speichern des neuen Wertes
    return ParaVal;                                                     // Liefert den neuen gespeicherten Wert im EEPROM
}

//------------------------------------------------------------------------
void FactoryParameters() {
    int32_t ParaVal = 0;
    for (uint8_t PNr = 1; PNr < _PARAMETER_COUNT; PNr++) {
        ParaVal = TBL_Long(T_Idx(_S_PAD, PNr, _S21_DEF));     // Parameter Defaultwert aus TBL Parameter-DATEN lesen und als LONG liefern
        Serial.print(F("Parameter:")); Serial.print(PNr); Serial.print(F(" set to:")); Serial.println(ParaVal);
        EE_ParaLet(PNr, ParaVal);
    }
}