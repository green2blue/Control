// 
// 
// 

#include "Func_Time.h"
timestampstruct timestamp;  // so muß eine Struct im cpp-File definiert werden, im .h File wird diese deklariert. 


//-----------------------------------------------------------
// Prüft das Datum in der Struktur timestamp auf Gültigkeit.
//-----------------------------------------------------------
int8_t checkTimestamp() {
    int8_t result = 0;
    if (timestamp.JJJJ > 2000) { result = 1; }
    else { return result; }
    if (timestamp.MO < 13) { if (timestamp.DD <= MonthhasDays(timestamp.JJJJ, timestamp.MO)) { result = 1; } else { return 0; } }
    if (timestamp.HH < 24 && timestamp.MM < 60 && timestamp.SS < 60 && timestamp.HH >= 0 && timestamp.MM >= 0 && timestamp.SS >= 0) { result = 1; }
    else { return 0; }
    return result;
}

//--------------------------------------------------------------------------
// liefert die Anzahl Tage die ein Monat hat. Bsp: 1=31, 2=28/29, 3=31 usw.
// bei den Monatszahlen 1,3,5,7,8,10,12  ist BIT0 und BIT3 immer <>  --> 31 Tage
// bei den Moantszahlen 4,6,9,11         ist BIT0 und BIT3 immer ==  --> 30 Tage
// Monatszahl 2 hat 28 + isYearSwitchYear Tage 
// Monatstagezahl=  iMM(Bit0) XOR iMM(BIT3) + 30
//--------------------------------------------------------------------------
int8_t MonthhasDays(int16_t iJJJJ, uint8_t iMM) {
    if (iMM == 2) { return 28 + isYearSwitchYear(iJJJJ); }
    if (bitRead(iMM, 0) == bitRead(iMM, 3)) { return 30; }
    else { return 31; }
}

//------------------------------------------------------------------------------------------
// liefert 1 wenn das Jahr ein Schaltjahr ist, wenn der Februal also 29 anstatt 28 Tage hat
//         0 wenn kein Schaltjahr vorliegt
// bin kein Fan von true od. false da sowieso ein ganzes Byte verbraucht wird, kann man auch gleiche eine Zahl liefern.
//------------------------------------------------------------------------------------------
int8_t isYearSwitchYear(uint16_t iJJJJ) {
    if (iJJJJ % 400 == 0 || (iJJJJ % 100 != 0 && iJJJJ % 4 == 0)) { return 1; }
    return 0;
}
