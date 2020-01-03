// 2019-01-05  Nachfolger von H_1  (Steuerung von Anlagen alleine über arduino oder via RasPi mittels arduino)
//  Code ist in Entwicklung, Tbl_heat.h ist im Aufbau 
// Sketchbook: W:\Projekte\Heizung\Arduino\Control   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#define SWName     "3it-control:"   // Name der Software  
#define SWVersion  "2.0"            // Version der Software  
//#include <stdio.h>       
#include "Sonderzeichen.h"          // LCD_SZ_OHM, _ENDLESS, _MUE, usw.
#include "Tbl_heat.h"               // So wird im Sketch-Verzeichnis nach der benannten Datei gesucht  <..> sucht die Datei hingegen im Library-Ordner
#include <avr/wdt.h>                // Watchdog
#include <Wire.h>                   // zur Ansteuerung von I2C Komponenten (auch die werden wie normale E/A in der Tbl.h definiert) 
#include <OneWire.h>                // OneWire DS18S20, DS18B20 Temperaturesensoren
#include <LiquidCrystal_I2C.h>      // zur Ansteuerung von LCD-Displays mit I2C-Schnittstelle
#include <LCDMenuLib2.h>            // die MENUE-Lib von jomelo (Nils Feldkämper)
#include <HX711.h>                  // Bibliothek zum Verwenden des Wiege-Element-Verstaerkermoduls für die Verwendung von Körperwaage-Modulen
#include <EEPROM.h>                 // nötig um Daten vom EEPROM am Chip zu lesen oder zu schreiben
#include <MemoryFree.h>             // NUR um den freien RAM ermitteln zu können (wird im Info-Screen angezeigt)
#include <avr/pgmspace.h>           // damit kann man Daten anstatt im RAM im FLASH ablegen die I_Tbl liegt u.a. dort.
#include "Taks_Type.h"              // diverse Typendefinitionen 
#include <TaskScheduler.h>          // Master-Timer
#include "PinChangeInterrupt.h"     // damit können für die PINs: 10,11,12,13,  50, 51,52,53,  62,63,64,65,66,67,68,69   ISR gemacht werden.

#define _UART_BUFFLEN    50         // Maximale Länge eines einzelnen mit LF abzuschließenden Kommandos das in den buffer UART_buffer[] aufgenommen werden kann/soll
#define _BUFFERLEN1     200         // Maximale Länge die für einen Zugriff auf den Flash-Speicher zur Verfügung stehen soll buffer[]
#define _BUFFERLEN2      21         // Maximale Länge für 'kurze' Zugriffe auf den Flash-Speicher, nur zum Einlesen von kurzen Inhalten wie Zahlen od. LCD-Zeilen
#define _TASKS_COUNT     10         // Anzahl Tasks die gleichzeitig laufen können

char    Buffer1[_BUFFERLEN1];       // Hauptsächlich verwendet zum Einlesen aus dem Flash aber auch für anderes verwendet
char    Buffer2[_BUFFERLEN2];       // ein 2. kurzer BUFFER für Flash-Inhalte zum parallelen Abarbeiten mit Buffer1[]

int16_t Buffer1Len=0;               // Länge der Buffer1[] 
int16_t Buffer2Len=0;               // Länge des Buffer2[] 
         
volatile uint32_t   lastIntTime=0;  // Hilfsvariable zum Entprellen von Schaltern innerhalb einer ISR (Ein/Aus/Notaus/..) // aktuell: Endschalter, Power-Taste, Not-Aus-Taste 
volatile uint32_t   aktIntTime=0;   // weitere Hilfsvariable zum Entprellen gemeinsam mit lastIntTime innerhalb einer ISR

uint8_t  lastAReference=1;          // Merker für die zuletzt am arduino verwendete: analogReference(.)  
                                    // DEFAULT=1 INTERNAL1V1=2 INTERNAL2V56=3 EXTERNAL=0
uint8_t  PCFInterrupt=1;            // Merker wird 1 sobald sich am Eingang des PCF-Moduls etwas geändert hat. 
                                    // Ein Interrupt setzt diese Variable, somit kann beim Zugriff auf einen Endschalter rasch auf den exakten Wert zugegriffen 
                                    // werden indem das PCF-Modul eingelesen wird ODER nur auf die vorhandenen Werte im RAM zugegriffen wird.
                                    // da mehr als 1 PCF-Modul als Eingangsmodul definiert werden könnte ist hier ggf. ein definierbares Array für jede PCF-Adresse anzulegen.
                                    // Ein Task könnte ggf. bei Bedarf alle PCF-Module abfragen und entsprechend reagieren.
volatile uint8_t sStati = B00000000;  // 8 Stück Stati-Merker in Form von BIT's 
                                      //  Bit 7 = POWER-STATUS       ON=1 OFF=0      Gerät ist EINGESCHALTET / AUSGESCHALTET
                                      //  Bit 6 = EMERGENCY          ON=1 OFF=0      NOT-AUS ist gedrückt    / Not-Aus ist nicht gedrückt
                                      //  Bit 5 = LCD-BACKLIGHT      ON=1 OFF=0      LCD-Hintergrundbeleuchtung
                                      //  Bit 4 = DS18Sensoren  1=AUFGEFORDERT (Invited)  0=ABGEHOLT und im RAM (Fetch)  (diese Sensoren werden periodisch aufgefordert Daten zu liefern)
                                      //  Bit 3 =  
                                      //  Bit 2 = 
                                      //  Bit 1 = 
                                      //  Bit 0 = in welchem Status befindet man sich innerhalb der Menüsteuerung 
                                      //          0 (ehem.false = HÜPFMODUS) 1 (ehem true) = Änderung-Modus 
//--------------------------------------------------------------------------------------------------------
// BITARRAY definitionen  
#define _S30_S32_RECORDSBIT  ((_S30_RECORDS + _S32_RECORDS)/8)+1      //  Für jedes Gerät + jede Kombination ein BIT
uint8_t obj_stateBIN[_S30_S32_RECORDSBIT];                            //  0=AUS 1=EIN   (weil ja nur ein BIT zur Verfügung steht.

//--------------------------------------------------------------------------------------------------------
// BYTEARRAY  nun mit je einem BYTE und nicht nur einem BIT pro Gerät + Kombination 
#define _S30_S32_RECORDS    ((_S30_RECORDS + _S32_RECORDS) ) +1       // Für jedes Gerät + jede Kombination ein BYTE
uint8_t obj_stateDEC[_S30_S32_RECORDS];                               // 0=undef  1=AUS  2=EIN 3..255 PWM 

//--------------------------------------------------------------------------------------------------------
// STRUCT ARRAY für STATISTIK-Daten pro (g)erät und pro (k)ombination 
#define _STATI_ARR ((_S30_RECORDS+_S32_RECORDS))+1                    //   Anzahl Geräte   + Kombinationen    
stat_gk_struct_RAM gk_statistik[_STATI_ARR];                          //  gk_statistik. 11 Byte pro Satz  
 
#define _LCDML_DISP_cols              20                              // Zeichen pro Zeile auf LCD
#define _LCDML_DISP_rows              4                               // Zeilenanzahl auf LCD 
#define _LCDML_DISP_cfg_cursor        0x7E                            // cursor Symbol  (-> )
#define _LCDML_DISP_cfg_scrollbar     1                               // enable a scrollbar (Natürlich, sonst weiß man im Menue nicht wieviele Einträge das Menue noch hat)
#define _LCDML_DISP_cnt               30                              // = Anzahl Menüelemente -1

#define _LCD_I2C_ADDR                 0x3F                                      // die I2C-Adresse vom angeschlossenen LCD-Display 
LiquidCrystal_I2C lcd(_LCD_I2C_ADDR, _LCDML_DISP_cols, _LCDML_DISP_rows);       // Set the LCD I2C address  

void lcdml_menu_display(); void lcdml_menu_clear(); void lcdml_menu_control();  // Prototypes für LCD Menuelib2
LCDMenuLib2_menu LCDML_0 (255, 0, 0, NULL, NULL);                               // root menu element (do not change)
LCDMenuLib2 LCDML(LCDML_0, _LCDML_DISP_rows,_LCDML_DISP_cols,lcdml_menu_display,lcdml_menu_clear,lcdml_menu_control);
 
#include "C_Mnu.h"

// +++++++++++++++++++ S E T U P   +++++++++++++++++++++++++++++++++++++++
void setup() {
  SetWatchDog(1);                   // Einstellungen zum WATCHDOG falls mal etwas im System hängt wird ein automatischer REBOOT ausgeführt
  SetUART(115200);                  // Einstellungen und Aktivierung der SERIELLEN Schnittstelle
  Msg_ProgStart();                  // Ausgabe an die Serielle Softwarename, Version usw. wenn das laut Parameter 1 vorgesehen ist.
  LCD_Defaults();                   // allgemeine LCD-Vorbereitungsarbeiten wie lcd.begin(),Backlight,Zeichen laden,..  
  LCD_MNU_Start();                  // Menülib Start-Einstellungen
  
  Init_INPUTS();                    // Alle Pins die in TBL als INPUTS   definiert sind entsprechend der dortigen Definition initialisieren
  Init_MACHINES();                  // Alle Pins die in TBL als AUSGÄNGE definiert sind entsprechend der Definition initialisieren und schalten wie definiert
  Init_COMBINATIONS();              // Alle Kombinationen an Ausgängen schalten wie definiert im Feld _33_AT_PWRON_DO
  Init_HX711();                     // Alle HX711 Module initialisieren die in TBL definiert sind. Ebenfalls werden hier vorhandene Kalibrierungswerte gelesen.
  
  Msg_ProgRdy();                    // Ausgabe an die Serielle Teil 2 Init Done wenn laut Parameter 3 vorgesehen   
}
// +++++++++++++++++++ S E T U P   E N D E  +++++++++++++++++++++++++++++



// +++++++++++++++++++ L O O P   ++++++++++++++++++++++++++++++++++++++++
void loop() {
  wdt_reset();                  // Reset Watchdog, sonst passiert alle 8 Sekunden automatisch ein RESET da dieser glaubt es hängt was  
  
    //  LCDML.loop();           // MENUELIB  wenn man keinerlei Tasten extra abfangen möchte wäre das die einzig nötige Zeile zum Betrieb.
  LCDML.loop_control();         // da ich aber auf Tasten-Ereignisse auch außerhalb der Menuelib reagieren möchte
    CheckBT();                  // Encoder-Ereignisse extra abfragen für Aufgaben wie Bildschirmschoner abstellen, Hintergrundbeleuchtung anschalten usw.
  LCDML.loop_menu();            // ist CheckBT() zwischen LCDML.loop_control()  und LCDML.loop_menu() extra eingefügt 

  Do_TasksA();                  // Task Scheduler A  (Macros und andere Aufgaben)
  Do_TasksB();                  // Task Scheduler B  (Eingänge die ständig abgehört werden sollen) 
  
  UART_hear(10);                // Serielle Eingaben annehmen und ggf. abarbeiten wenn ENTER=10=Linefeed angekommen ist.
  
}
// +++++++++++++++++++ L O O P   E N D E  ++++++++++++++++++++++++++++++
