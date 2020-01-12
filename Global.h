#ifndef _GLOBAL_h
#define _GLOBAL_h

#include <Arduino.h>


#define _UART_BUFFLEN    50         // Maximale L�nge eines einzelnen mit LF abzuschlie�enden Kommandos das in den buffer UART_buffer[] aufgenommen werden kann/soll
#define _BUFFERLEN1     200         // Maximale L�nge die f�r einen Zugriff auf den Flash-Speicher zur Verf�gung stehen soll buffer[]
#define _BUFFERLEN2      21         // Maximale L�nge f�r 'kurze' Zugriffe auf den Flash-Speicher, nur zum Einlesen von kurzen Inhalten wie Zahlen od. LCD-Zeilen
#define _TASKS_COUNT     10         // Anzahl Tasks die gleichzeitig laufen k�nnen

extern char    Buffer1[_BUFFERLEN1];       // Haupts�chlich verwendet zum Einlesen aus dem Flash aber auch f�r anderes verwendet
extern char    Buffer2[_BUFFERLEN2];       // ein 2. kurzer BUFFER f�r Flash-Inhalte zum parallelen Abarbeiten mit Buffer1[]

extern int16_t Buffer1Len; // = 0;               // L�nge der Buffer1[] 
extern int16_t Buffer2Len; // = 0;               // L�nge des Buffer2[] 

extern volatile uint32_t   lastIntTime; // = 0;  // Hilfsvariable zum Entprellen von Schaltern innerhalb einer ISR (Ein/Aus/Notaus/..) // aktuell: Endschalter, Power-Taste, Not-Aus-Taste 
extern volatile uint32_t   aktIntTime;  // = 0;   // weitere Hilfsvariable zum Entprellen gemeinsam mit lastIntTime innerhalb einer ISR


extern char     UART_buffer[_UART_BUFFLEN];      // eine Zeichenkette einzeln vom UART eingelesen bis zum ENTER = 10
extern uint8_t  UAIdx; // = 0;                         // Zechenanzahl bzw. Hilfszeiger f�r die Bearbeitung der Zeichenkette in UART_buffer[]
extern uint8_t  UART_bufferlen; // = 0;                // L�nge der Zeichenkette

extern int32_t  sbufferNr[5]; // = { 0,0,0,0,0 };     // Nummern-Array f�r diverse Zwecke 













#define DEFAULT_TEMPERATUR 22

#define PI_TEMPERATURE_1 23



enum DeviceStatus
{
	Init = 10,
	Working = 20,
	Error = 90
};


extern DeviceStatus gStatus;
extern float gfDeviceTemperature;


void Global_SetDefaultValue();

#endif