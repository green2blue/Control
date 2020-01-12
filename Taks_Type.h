#ifndef _TASK_TYPE_
#define _TASK_TYPE_
//--------------------------------------------------------------------------------------------------
// Struktur die zu jedem Task per Pointer in einer Initialisierungsroutine verknüpft wird.
// Anhand dieser Datenstruktur weiß jeder Task was er als nächstes zu tun hat. Hier wird meist von einem Task gesprochen der ein Macro
// abarbeiten soll. Es gibt auch andere Tasks die keine komplexen Aufgaben ausführen sollen sondern einfach nur periodisch einfache Dinge erledigen sollen.
//--------------------------------------------------------------------------------------------------
typedef struct {              // 18 Bytes insgesamt, bei 10 Tasks werden hier 180 Byte verbraucht
    uint8_t    IsRunning;     // 1 Byte Merker welche Makro-Nummer der Task gerade abarbeitet.
                              //      0=Task ist FREI und steht für neue Aufgaben bereit.
                              //    123= Makro-Nummer die gerade abgearbeitet wird bzw. abzuarbeiten ist.
    int8_t     MacroTimes;    // 1 Byte Anzahl wie oft das Makro sich selbst wiederholen soll. Normalfall=1 ..126 -1=Endlos
    uint32_t   MacroInterval; // 4 Byte Pause zwischen den Makrowiederholungen, nur relevant wenn MacroTimes ==-1 oder > 1
    uint8_t    StepIdx;       // 1 Byte Positions-Zähler für die gerade geladene Macro-Zeile des Timers/Tasks
                              //   
    uint8_t    objtype;       // 1 Byte s,f  (g)erät  (k)ombination  (p)in  (s)witch  (f)unction() (m)acro 
    int8_t     objnr;         // 1 Byte Nummer der g,k,p,s,f,m   in I_Table   -127 0 128    Bsp: 28 = Power-Switch-LED
                              //   m(akro) ist hier auch die Makro-Nummer die der parallele Task ausführt eingetragen.
                              //   der Index_Zeiger auf einen abzuwartenden parallen Task wird in objswitchto abgelegt.
                              //   2018-06-02 bei objtype='e' eine Status-Nr für das Macro im Bereich 0,1,2,3  
                              //                  wenn > 3 wird m_info[Macro_Nr].LastState1 nicht angerührt/verändert an dieser Stelle
    uint8_t    objopnr;       // 1 Byte 0 undef.  1 =  2 !=   3 <   4 <=   5 >   6 >=  Operator bereits zu Zahl umgewandelt
                              // es folgt ein 3-deutiges Feld 
    int16_t    objswitchto;   // 2 Byte 1=off  2=on  3...255 PWM  /   Schalter_Funktion warte auf Wert  /  Schritt-Nummer bei parallelem Makro-Start
           //  objwaitfor   anderer Name wg. Mehrdeutigkeit dieses int16_t 
                              // der  W A H R -Teil wenn objwaitfor zutrifft.
    uint8_t    objtype2;      // 1 Byte (g),(k),('e')....
    int8_t     objnr2;        // 1 Byte Nummer dazu   // 2018-03-25 wegen relativ-Sprung ist hier Vorzeichen nötig.
    uint8_t    objswitchto2;  // 1 byte 1=off  2=on  3...255 PWM 
                              // der  F A L S C H -Teil bzw. das wenn die Zeit abgelaufen ist, kann auch LEER bleiben.
    uint8_t    objtype3;      // 1 Byte (g),(k),'e',(c),(m),..  p=pin  Auszuführen wenn sich der gewünschte Zustand nicht innerhalb des TimeOut einstellt.
    int8_t     objnr3;        // 1 Byte Nummer des Objektes   // 2018-03-25 wegen relativ-Sprung ist hier Vorzeichen nötig.
    uint8_t    objswitchto3;  // 1 Byte 1=off  2=on  3...255 PWM
} t_var;
#endif
//#ifndef _ERR_TIMESTAMP_
//#define _ERR_TIMESTAMP_
//// Zeitstempel für den letzten Error-Log-Eintrag pro Fehler im EEPROM 2018-03-29 
//typedef struct packed_timestamp_bits {
//  unsigned int JJJJ : 12; // Jahr (reicht bis 4095)
//  unsigned int MO   : 4;  // Monat(0..12) 
//  unsigned int DD   : 5;  // Tag  (0..31) geht sich gerade aus
//  unsigned int HH   : 5;  // Stunde (0..24) 
//  unsigned int MM   : 6;  // Minute (0..60)
//  unsigned int SS   : 6;  // Sekunde(0..60)
//  unsigned int FF   : 2;  // 2 Restbits auf 40 Bit = 5 Byte für den Zeitstempel
//} timestampstruct;
//timestampstruct timestamp; // die Variable timestamp.JJJJ  .MM usw. ist im Programm zu verwenden
//#endif
#ifndef _BLINK_INFO_
#define _BLINK_INFO_
// Stelle um Informationen zum Blinken der Power-LED und LCD-Hintergrundbeleuchtung und 1x Reserveblink RAM-sparsam zu Speichern 
//  blink.xxxps  0...15  Zahl steht für Blinkimpulse pro Sekunde wobei 0 Blinker-AUS bedeutet.
//  blink.xxxis  0,1 Umschaltbit-Merker für Blinker.
typedef struct blink_bits {   // SUMME  5 Byte
  unsigned int LCDps : 4; // kurze Zahl von 0 - 15 mit der man angibt wie oft die LCD-Hintergrundbeleuchtung pro Sekunde blinken soll.
  unsigned int LCDcnt: 4; // Zähler im Timer der bis LCDdiv hochzählen muss bevor der Blinkvorgang ausgelöst wird
  unsigned int LCDdiv: 4; //
  unsigned int LCDis : 1; // Bit Merker ob die LCD gerade AN oder AUS ist 
  unsigned int LEDps : 4; //                      wie oft die LED pro Sekunde blinken soll
  unsigned int LEDcnt: 4;
  unsigned int LEDdiv: 4;
  unsigned int LEDis : 1; // Bit Merker ob die LED gerade AN oder AUS ist
  unsigned int RESps : 4; // 
  unsigned int REScnt: 4; 
  unsigned int RESdiv: 4; 
  unsigned int RESis : 1; // verbraucht bis hierher 39 Bit = 5 Byte 
} blinkstruct;
blinkstruct blink;  // die Variable blink.LCDps      und blink.LEDps
#endif

#ifndef _MACRONR_INFO_
#define _MACRONR_INFO_
typedef struct macro_nr_info_bits {  // SUMME 1 BYTE 
  unsigned int Running1   : 1;  //    0 = Macro läuft nicht 1 = Macro läuft
  unsigned int StepNr1    : 5;  //    0..31  in welchem Step befindet sich das Macro im Moment der Nachfrage, steht hier 11111 = 31 dann ist das Macro bzw. dessen Task sofort zu beenden = Killkommando c6
  unsigned int LastState1 : 2;  //    0..3  Status-Nummer mit der das Macro zuletzt beendet wurde bzw. in dem sich das Macro befindet.
} macro_nr_struct;
#endif

 
#ifndef _STAT_DATA 
#define _STAT_DATA 
typedef struct stat_info_bits_res {  // SUMME 6 BYTE   Statistikdaten zu jedem (g)erät (k)ombinatin ...
  unsigned int RYears   : 5;   //  Jahre   0..31
  unsigned int RDays    : 9;   //  Tage    0..511 (364) ab dieser Zahl wird OnYears   + 1 und OnDays    = 0
  unsigned int RHours   : 5;   //  Stunden 0..31  (23)  ab dieser Zahl wird OnDays    + 1 und OnHours   = 0
  unsigned int RMinutes : 6;   //  Minuten 0..63  (59)  ab dieser Zahl wird OnHours   + 1 und OnMinutes = 0
  unsigned int RSeconds : 6;   //  Sekunde 0..63  (59)  ab dieser Zahl wird OnMinutes + 1 und OnSeconds = 0
  unsigned int Rms      :10;   //  ms      0..1023(999) ab dieser Zahl wird OnSeconds + 1 und Onms      = 0
} stat_gk_struct_res; //  41 Bit entspricht 6 Byte    //  
stat_gk_struct_res     gk_stat_res; // 

typedef struct stat_info_bits_EEPROM {  // SUMME 10 BYTE  Statistikdaten wie diese im EEPROM gespeichert werden
  unsigned int OnYears  : 5; // Jahre 0..31
  unsigned int OnDays   : 9; // Tage  0..511
  unsigned long Onms     :32; // milli Sekunden  (kamit kann man durch die Zahl 4233600000 exakt 49 Tage abspeichern, danach sind diese Tage in OnDays zu addieren und Onms auf 0 zu setzen.)
  unsigned long OnOffCnt :32; // Schaltvorgänge 
  unsigned int freebits : 2; // noch freie Bits auf 80 Bit  = 10 Byte
} stat_gk_struct_EEPROM; 
stat_gk_struct_EEPROM    gk_stat;  // im Programm ist gk_stat.OnYears  gk_stat.OnDays  usw.... zu verwenden

// folgende Daten sind pro Gerät/Kombi im RAM und müssen beim Abschalten der Anlage und beim Erreichen bestimmter Werte(Überlauf) in das EEPROM übertragen werden !!!
typedef struct stat_info_bits_RAM {  //  SUMME 11 Byte pro Gerät oder Kombi im RAM   Statistikdaten wie diese im RAM gespeichert sind 
  unsigned long OnOffCnt:  24;     // Schaltvorgänge aufgelöst auf 10ms also max.100 Schaltvorgänge pro 1s Speichervorgang in den EEPROM 1x pro Tag wenn 100 Vorgänge pro Sekunde passieren 
  unsigned long Lastmillis:32;     // millis() Zeitstempel START bei ON (beim OFF ist dann die Differenz von millis() - Lastmillis die ON-TIME in Einheit [ms] verfügbar
  unsigned long Onms:      27;     // 1 Tag hat 86.400.000 [ms]  in 27 Bit geht 134.217.727 rein      
  unsigned int freebits:   5;     // noch frei verfügbare Bits in dieser Struktur
} stat_gk_struct_RAM;
#endif
