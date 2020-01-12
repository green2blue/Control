// Func_Help.h

#ifndef _FUNC_HELP_h
#define _FUNC_HELP_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
//-----------------------------------------------------------------------------------------------------------------------------
// liefert die StartPos ab der in der Zeichenkette &xbuffer bis zu EndPos ein Inhalt UNGLEICH noSChar und UNGLEICH noSChar2
// zu finden ist;  gedachte Anwendung: Das Auffinden der ersten relevanten Stelle in einer Zeichenkette.
// Liefert in foundChar das gefundenen Zeichen, damit kann man gleich feststellen ob eine Zahl oder sonstiges Zeichen vorliegt.
// Die Funktion könnte auch heißen   'GetNextRelevantPosAndChar()'
// Üblicherweise ist noSChar ' ', ein LEERZEICHEN  Üblicherweise trennt ' ' die zu interpretiernden Zeichen voneinander.
// wenn 255 geliefert wird, dann gibt es keine passende Stelle, ebenfalls wird in diesem Fall foundChar auf 0x00 gesetzt.
// ACHTUNG &bufferStartPos wird  hier bei einem Treffer eines noSChar Zeichens an die Position dieses Zeichens gesetzt.
//-----------------------------------------------------------------------------------------------------------------------------
int16_t GetPosNEQ(char xbuffer[], int16_t & StartPos, int16_t & EndPos, char & foundChar, char noSChar, char = 0xfe);
int16_t GetPosNEQ(char xbuffer[], int16_t& StartPos, int16_t& EndPos, char& foundChar, char noSChar, char noSChar2);

//-----------------------------------------------------------------------------------------------------------------------------
// Liefert die StartPos ab der in der Zeichenkette ein Inhalt GLEICH searchChar oder searchChar2 vorliegt 
// und liefert das nächstgelegene Zeichen Gegenstück zu GetPosNEQ  
//-----------------------------------------------------------------------------------------------------------------------------
int16_t GetPosEQ(char xbuffer[], int16_t& StartPos, int16_t& EndPos, char& foundChar, char searchChar, char = 0xfe);
int16_t GetPosEQ(char xbuffer[], int16_t& StartPos, int16_t& EndPos, char& foundChar, char searchChar, char searchChar2);

#endif

