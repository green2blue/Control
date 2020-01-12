// 
// 
// 

#include "Func_Help.h"

//-----------------------------------------------------------------------------------------------------------------------------
int16_t GetPosNEQ(char xbuffer[], int16_t& StartPos, int16_t& EndPos, char& foundChar, char noSChar, char noSChar2) {
    uint8_t found = 0;
    uint8_t i;
    for (i = StartPos; i < EndPos; i++) {             // suche und untersuche im erlaubten Bereich 
        foundChar = xbuffer[i];
        if (foundChar != noSChar && foundChar != noSChar2) {             // sobald ein nützliches Zeichen entdeckt wird ist man fertig
            StartPos = i;                               // dieser Wert könnte auch 0 sein und damit doch gültig 
            foundChar = xbuffer[i];
            found = 1;
            break;
        }
    }
    if (found == 0) {                               // wenn keine passende Stelle gefunden wurde
        StartPos = -1;                              // 32767; // INTEGER positives Maximum  -32767 negatives Maximum
        foundChar = 0x00;
    }                                           // wenn keine gültige Stelle gefunden wurde wird -1 als Wert geliefert
    else {}                              // gültige Stelle gefunden, StartPos beinhaltet bereits die passende Stelle
    return StartPos;
}

//-----------------------------------------------------------------------------------------------------------------------------
//int16_t GetPosEQ(char xbuffer[], int16_t& StartPos, int16_t& EndPos, char& foundChar, char searchChar, char = 0xfe);
int16_t GetPosEQ(char xbuffer[], int16_t& StartPos, int16_t& EndPos, char& foundChar, char searchChar, char searchChar2) {
    uint8_t found = 0;
    uint8_t i;
    for (i = StartPos; i < EndPos; i++) {                         // suche und untersuche im erlaubten Bereich 
        foundChar = xbuffer[i];
        if (foundChar == searchChar || foundChar == searchChar2) {    // sobald ein Zeichen entdeckt wird ist man fertig
            StartPos = i;                                           // dieser Wert könnte auch 0 sein und damit doch gültig 
            if (StartPos < EndPos) { foundChar = xbuffer[StartPos + 1]; }
            else { foundChar = 0x00; }
            found = 1;
            break;
        }
    }
    if (found == 0) {                                           // wenn keine passende Stelle gefunden wurde
        StartPos = 255;
        foundChar = 0x00;
    }                                                       // wenn keine gültige Stelle gefunden wurde wird 255 als Wert geliefert
    else {}                                          // gültige Stelle gefunden, bufferStartPos beinhaltet bereits die passende Stelle
    return StartPos;
}
