//-------------------------------------------------------------------------------------
// 2019-01-23  die Reihenfolge dieses REITERS muß an 2.Stelle liegen da der Compiler 
//   sonst Fehler liefert von wegen Variable nicht gefunden, zu wenig Parameter bei einer Funktion angegeben.
// Hier sind Prototypen von Funktionen mit Standardparametern bestens aufgehoben da 
// der Compiler hier vor allen anderen Reitern hinkommt und somit weiß daß manche Funktionen 
// Standardwerte besitzen. Grundsätzlich ist für jeden Parameter der einen Defaultwert haben soll
// von RECHTS nach LINKS vorzugehen. Siehe Beispiele.
//-------------------------------------------------------------------------------------
// zu jedem Macro von 0...Max eine Daten-Struktur im RAM anlegen.
// ACHTUNG in dieser Version darf ein Macro dann NUR 1x laufen sonst würden die Macro-Daten vermischt.
//  Workarround wenn ein Macro mehrfach gleichzeitig laufen darf: Kopieren des Macros (ist natürlich etwas redundant aber es ginge)
//  Alternative: nicht pro Macro diese Datenstruktur sondern pro Task+Macro. 
macro_nr_struct  m_info[_MACRO_COUNT+1];        // Info zu jeder Macro-Nummer  .Running1  .StepNr1  .LastState1 (Element 0 unverwendet da Macros bei Nr1 beginnen nur der Einfachheit wegen)

#include "Taks_Type.h"          // gehört zum TaskScheduler, eine zentrale Angelegenheit dieser Software gemeinsam mit der LCDMenuLib2
#include <TaskScheduler.h>      // Master-Timer

#define _TASK_SLEEP_ON_IDLE_RUN // Master-Timer ein Parameter
#define _TASK_STATUS_REQUEST    // Master-Timer Ein Task kann Ereignisgesteuert gestartet werden
#define _TASK_WDT_IDS           // Master-Timer damit kann jeder Task mit seiner ID benannt werden
#define _TASK_LTS_POINTER       // Master-Timer damit kann eine Taskproc auf den Inhalt einer Struktur Zugriff bekommen
#define _TASK_TIMEOUT           // Master-Timer kann mit Timeout arbeiten (.getTimeout()  .setTimeout(Long)  .untilTimeout()  .timedOut() ?

//StatusRequest stB;              // um einem Task das Ereignis per  .siganlComplete(); mitteilen zu können

Scheduler     ts;               // Task Scheduler A
Scheduler     tsB;              // Task Scheduler B

void    sfunc();                // Call-Back Proc (Prototype) die von stask() angestoßen wird sobald EIN/AUS gedrückt wurde.
uint8_t sfuncMacroNr=0;         // Macro-Nummer die in sfunc zu starten ist. (wird von der jeweiligen ISR gesetzt)
//Task    stask(& sfunc, & tsB);  // Task der vom StatusRequest angestoßen wird und von sfunc()  das sfuncMacroNr startet.

void tf_Macro();  bool tEn_Macro();  void tDis_Macro(); bool WrapperOn(); void WrapperOff(); 

//      Interval,    wie oft,      Proc-Zeiger,     enabled, OnEnable-Zeiger, OnDisable-Zeiger
Task t01(TASK_SECOND, TASK_FOREVER, &tf_Macro,     &ts, false,  &tEn_Macro,      &tDis_Macro); 
Task t02(TASK_SECOND, 1,            &tf_Macro,     &ts, false,  &tEn_Macro,      &tDis_Macro); 
Task t03(TASK_SECOND, 1,            &tf_Macro,     &ts, false,  &tEn_Macro,      &tDis_Macro);
Task t04(TASK_SECOND, 1,            &tf_Macro,     &ts, false,  &tEn_Macro,      &tDis_Macro);
Task t05(TASK_SECOND, 1,            &tf_Macro,     &ts, false,  &tEn_Macro,      &tDis_Macro);
Task t06(TASK_SECOND, 1,            &tf_Macro,     &ts, false,  &tEn_Macro,      &tDis_Macro);
Task t07(TASK_SECOND, 1,            &tf_Macro,     &ts, false,  &tEn_Macro,      &tDis_Macro);
Task t08(TASK_SECOND, 1,            &tf_Macro,     &ts, false,  &tEn_Macro,      &tDis_Macro);
Task t09(TASK_SECOND, 1,            &tf_Macro,     &ts, false,  &tEn_Macro,      &tDis_Macro);
Task t10(TASK_SECOND, 1,            &tf_Macro,     &ts, false,  &tEn_Macro,      &tDis_Macro);

void tf_Temp();           // DS18 Temperatursensoren anfordern/auslesen
void tf_Press();          // Soll-System-Druck aufrecht halten 
void tf_H2t();            // Sicherheitscheck falls H2-Gehalt zu hoch wird (Parameter p3) + Parameter p6 Temperatur
void tf_blink();          // Blink-Task gesteuert über die Funktion getfVal() 
bool tf_Press_enable();   // sobald diese Proc zugewiesen wird funktioniert tf_Press nicht mehr
void tf_Press_disable();  //
Task t11(1000, TASK_FOREVER, &tf_Temp,      &ts, false,  NULL, NULL);               // Task der die Temperatursensoren einliest.
Task t12(300,  TASK_FOREVER, &tf_Press,     &ts, false,  NULL, &tf_Press_disable);  // Task der den Systemdruck prüft 
Task t13(3000, TASK_FOREVER, &tf_H2t,       &ts, false,  NULL, NULL);               // Task der den H2-Gehalt prüft
Task t14(1000, TASK_FOREVER, &tf_blink,     &ts, false,  NULL, NULL);               // Task der die z.B. LCD-Anzeige blinken läßt.


t_var   vt[_TASKS_COUNT];    // jedem Task t01..t10 seine Struct(siehe Tasks_Type.h) in der er seine Variablen ablegen/abholen/bearbeiten kann.
// Array erlaubt indizierten Zugriff zu den Tasks und deren Variablen-Struktur 
Task* tasks[] = {&t01,&t02,&t03,&t04,&t05,&t06,&t07,&t08,&t09,&t10}; // die Adressen zu den Timerobjekten
// *tvars[x] beinhaltet Zeiger auf die zu den Tasks gehörenden Variablen. Mit diesen verstehe ich allerdings nicht umzugehen deshalb unverwendet.
t_var* tvars[] = {&vt[0],&vt[1],&vt[2],&vt[3],&vt[4],&vt[5],&vt[6],&vt[7],&vt[8],&vt[9],&vt[10] };

//----------------------------------------------------------------
//  Task Scheduler A , B
//----------------------------------------------------------------
void Do_TasksA() { ts.execute(); }
void Do_TasksB() { tsB.execute();}

void SetWathcDog(int8_t = 1); // per Default soll der Watchdog bei Aufruf ohne Parameter aktiviert werden.  0=deaktivieren
void SetWatchDog(int8_t ON_OFF) {
  wdt_disable();                  // immer gut den Watchdog zu deaktivieren, falls dieser zuletzt versehentlich aktiviert verblieb. siehe auch wdt_enable(WDT0_8S)
                                  // Watchdog muß deaktiviert werden falls man kurz eine Seriell-do-While benötigt wird.
  if (ON_OFF == 1) {
    wdt_enable(WDTO_8S);          // Watchdog aktivieren mit 8 Sekunden Meldezeit bis zum Auslösen   
  }
}

//---------------------------------------------
// Prototypen für Funktions-DEFAULT Werte 
//---------------------------------------------
//int8_t Msg(int16_t MsgNr, int32_t = 0);
//int8_t Msg(int16_t MsgNr, int32_t Nr1, int32_t = 0);
//int8_t Msg(int16_t MsgNr, int32_t Nr1, int32_t Nr2, int32_t = 0);
//int8_t Msg(int16_t MsgNr, int32_t Nr1, int32_t Nr2, int32_t Nr3, uint8_t = 10);

void Notify(int16_t MsgNr, uint8_t = 10);   // per Default wird ein ENTER an die Meldung angefügt
void PLFS(int8_t = 1);                      // per Default 1x Serial.println()
void PBLANKS(int8_t = 1);                   // per Default 1x BLANK

int16_t GetPosNEQ(char xbuffer[], int16_t & StartPos, int16_t & EndPos, char & foundChar, char noSChar, char = 0xfe );
 //                                                                                                         HideEmptys 1 = LEERE auslassen 
int8_t a_AUS(int32_t AdrFrom, int32_t AdrTo, int32_t ToDevice, uint8_t Cursor_Spalte, uint8_t Cursor_Zeile, int8_t = 1);

uint16_t T_Idx(uint16_t okz, uint16_t NR, uint8_t = 0, int8_t = 0);       // Feldnummer 0 und Sprache 0

int16_t Machine_DO(uint16_t MIdx, uint16_t MToDo, uint16_t = 0);
int16_t Machine_DO(uint16_t MIdx, uint16_t MToDo, uint16_t Fq, uint32_t = 0);
int16_t Machine_DO(uint16_t MIdx, uint16_t MToDo, uint16_t Fq, uint32_t FqDur, int8_t = 0); 


//uint8_t Combination_DO(uint8_t CIdx,uint16_t CToDo, uint8_t = 0);  // Default Save State 
//uint8_t Combination_DO(uint8_t CIdx,uint16_t CToDo, uint8_t SaveNoState, uint8_t = 0); 

//----------------------------------------------------------------------------
// Kommunikationseinstellungen bezüglich UART BAUD usw.
// Standardwert ist 115200, das kann durch Aufrufen der Funktion mit einem anderen Wert gesetzt werden.
// Bei Aufruf mit -1 wird nur der gerade eingestellt Wert geliefert
// Das Umstellen während des Betriebes und erneutes Verbinden funktioniert nicht wie gewünscht, da fehlt noch Arbeit ?
//----------------------------------------------------------------------------
int32_t SetUART(int32_t = 115200);  // Defaultwert für Speed der UART
int32_t SetUART(int32_t SetBaudrate) {
  static int32_t AktBaudrate;
  static uint8_t Serial_IsOpen=0;
  if (SetBaudrate >= 0) {
    AktBaudrate = SetBaudrate;  
    if (Serial_IsOpen == 1) { Notify(2); Serial.end(); }  // eine womöglich offene Verbindung schließen
    while(!Serial);                           // Warten bis SERIELL bereit ist
    Serial.begin(SetBaudrate);  // Serial.println(F("Serial is Open"));
    Serial_IsOpen=1;
  }
  return AktBaudrate;
}

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

//-----------------------------------------------------------------------
// Setup Einstellungen für das Menü der Menülib
//-----------------------------------------------------------------------
void LCD_MNU_Start() {
  LCDML_setup(_LCDML_DISP_cnt);       // LCDMenuLib Setup
  if (EE_ParaGet(5) == 1) {
    LCDML.MENU_enRollover();          // Menü-Sprung vom letzten zum ersten und vom ersten zum letzten bei Überdrehen
  } else {
    LCDML.MENU_disRollover();         // Menü-Stop am Ende und Stop am Beginn
  }
  
    // LCDML.SCREEN_enable(mFunc_screensaver, 10000); // set to 10 seconds  Enable Screensaver (screensaver menu function, time to activate in ms)
    // LCDML.SCREEN_disable();
    // LCDML.OTHER_jumpToFunc(mFunc_p2); // the parameter is the function name // You can jump to a menu function from anywhere with  
}

char * F2buffer(uint16_t LfdNr, uint8_t = 0);  // Default

//-----------------------------------------------------------------------------------
// Einstellen/Abfragen ob auf jede Eingabe ein Echo der Eingabe erfolgen soll.
// Bsp: Echo() od. (-1)  es wird nur der aktuell eingestellte Zustand geliefert
//          (0) Echo wird AUSGESCHALTET
//          (1) Echo wird EINGESCHALTET
//-----------------------------------------------------------------------------------
int8_t Echo(int8_t = -1); // Defaultwert für ECHO bedeutet nur Abfragen, nicht setzen
int8_t Echo(int8_t On_Off) {
  static int8_t AktEcho;
  if (On_Off >= 0) {AktEcho = On_Off; }
  if (AktEcho > 1) {AktEcho = 0; }
  return AktEcho;
}
