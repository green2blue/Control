//---------------------------------------------------------------------------------------------------------------------
// Daten, Standardwerte, Sprachen, Auswahlmöglichkeiten, Texte, Namen   die im FLASH des Microcontrollers 
//  abgelegt werden und per Funktion F2X() in Kombination mit T_Idx(Bereich,SatzNr,FeldNr)
//  in den  F_bufferX[_FLASH_BUFFxLEN]  geholt und dann verwarbeitet oder ausgegeben werden können. 
//  Die Groeße von F_bufferX[] ist mit der maximalen Anzahl Zeichen die definiert wird anzupassen.
// Um die Daten aus dieser strukturierten Textdatei im FLASH gezielt auszulesen gibt es mehrere Funktionen.
// Grund dieser Struktur ist um möglichst ohne Source-Code-Aenderungen für diverse Anpassungen auszukommen und
// möglichst wenige hardcodierte Stellen im Source-Code zu haben.
//---------------------------------------------------------------------------------------------------------------------
// Struktur: Die Startpositionen der einzelnen Bereiche(Tabellen) werden über #define _  fixe und 
//           vom Compiler berechnete Compilervariablen gesteuert.
//           Pro Zeile   'const char ..[] PROGMEM = "";    wächst die Gesamtstruktur.
//           Die Position einer Zeile ergibt sich über eine einfache Formel.
//           Hier wird also eine Art einfachste Datenbank abgebildet die nur einen INDEX (siehe ganz unten) hat.
//---------------------------------------------------------------------------------------------------------------------
// Jeder Bereich beginnt mit seinem vom Compiler errechneten OFFSET, hat eine bestimmte Anzahl FIELDS und hat 
// eine definierte Anzahl RECORDS und eine Anzahl LANGUAGES. Nur der 1.Bereich _S00_OFFS ist FIX bei 0 und nicht errechnet.
// Die Bereiche können als Paket innerhalb der gesamten Struktur verschoben werden. (Indextabelle dabei beachten und ebenfalls Bereich verschieben)
// Die Bereiche können einfach erweitert werden (Indextabelle ebenfalls beachten und an der entsprechenden Stelle erweitern)
//---------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------
// BEREICHE / SECTIONEN  BEREICH 
//  hier werden alle BEREICHE die in dieser Tabelle vorkommen genannt
//  Die Bereiche werden verwendet um den jeweiligen OFFSET zu benennen
//--------------------------------------------------------------------------
#define _S_LNG        _S00_OFFS         // Sprachen-Kurzform  die im System vorkommen dürfen  en, de, fr, ...
#define _S_LNL        _S01_OFFS         // Sprachen-Langform  die im System vorkommen dürfen  english, german, french, ...
#define _S_MSL        _S02_OFFS         // Message Sprachabhängig 
#define _S_HXL        _S03_OFFS         // HX711 Module Sprachabhängiger Teil
#define _S_HXD        _S04_OFFS         // HX711 Daten
#define _S_FMD        _S07_OFFS 		    // Formatierungsangaben                               für sprintf( buf[], FORMATSTR, Wert1,2,...);
#define _S_CMD        _S09_OFFS         // Kommandos  Funktionen Abfragen                     c ommando Beschreibungen
#define _S_TMP        _S10_OFFS         // DS18-Temperatur Sensoren Sprachbereich             t emperaturfühler DS18 Namen, Adressen
#define _S_TMD 		    _S11_OFFS 		    //      Temperatur Datenbereich                       t Daten
#define _S_MNU        _S12_OFFS         // Menues teilw. in verschiedenen Sprachen            Menü Experiment
#define _S_MNS        _S13_OFFS         // Menües Sprungpunkte in/für die versch. Sprachen     
#define _S_MEH        _S14_OFFS         // Mengeneinheiten in Sprachen                        Mengeneinheiten 
#define _S_DAY        _S15_OFFS         // Wochentagnamen in Sprachen Montag,Dienstag,..      Montag, Dienstag, Mittwoch, ...
#define _S_DAK        _S16_OFFS         // Wochentagnamen Kurzform Mo,Di,..                   Mo, Di, Mi, ...
#define _S_MON        _S17_OFFS         // Monatsnamen in Sprachen Jänner,Februar,..          Jänner, Februar, ...
#define _S_MOK        _S18_OFFS         // Monatsnamen Kurzform Jan,Feb,Mär,...               Jan, Feb, ...
#define _S_PAL        _S20_OFFS         // Parameter Namen Beschreibung Sprachabhängig        PARAMETER Beschreibung
#define _S_PAD        _S21_OFFS         // Parameter Daten                                     
#define _S_ERL        _S22_OFFS         // Error Sprachen                                     ERROR s
#define _S_ERD        _S23_OFFS         // Error Daten                                
#define _S_INL        _S28_OFFS         // Eingänge Sprachabhängig                            EINGÄNGE
#define _S_IND        _S29_OFFS         // Eingänge Daten
#define _S_GEL        _S30_OFFS         // Geräte Namen Sprachabhängig                        g EERÄTE
#define _S_GED        _S31_OFFS         // Geräte DATEN 
#define _S_KOL        _S32_OFFS         // Kombinationen Namen Sprachabhängig                 k OMBINATIONEN
#define _S_KOD        _S33_OFFS         // Kombinationen DATEN
#define _S_ISL        _S34_OFFS         // Spezial-Eingänge Namen Sprachabhängig              Eingänge spezial
#define _S_ISD        _S35_OFFS         // Spezial-Eingänge DATEN 
#define _S_MCL        _S40_OFFS         // Macros Kopf Sprachabhängig                         m AKRO s Namen
#define _S_MCD        _S41_OFFS         // Macros Kopf Daten                                           Kopf
#define _S_MCS	      _S43_OFFS 		    // Macros Steps Daten 0...49                          m AKRO Schritte

//------------------------------------------------------------------------------------
// BEREICH  LNG = SPRACHEN 
//------------------------------------------------------------------------------------
#define _S00_OFFS       0                         // Beginn einer Section  (ergibt sich aus OFFSET der Section zuvor + FIELDS*RECORDS der Section zuvor)
#define _S00_FIELDS     1                         // Anzahl Felder pro Datensatz (gilt für ALLE Datensätze)
#define _S00_RECORDS    3                         // Anzahl Datensätze
#define _S00_LNGCOUNT   0                         // Anzahl zusätzlicher Sprachen innerhalb der Section
#define _LNGCOUNT  _S00_RECORDS 
  const char S00_0001[] PROGMEM = "en";           // Sprachkürzel Index 0
  const char S00_0002[] PROGMEM = "ge";           // Sprachkürzel Index 1  
  const char S00_0003[] PROGMEM = "fr";		        // Sprachkürzel Index 2 
#define _S01_OFFS		    _S00_OFFS + _S00_RECORDS*_S00_FIELDS + _S00_RECORDS*_S00_FIELDS*_S00_LNGCOUNT  // = 3 
#define _S01_FIELDS     1
#define _S01_RECORDS	  3
#define _S01_LNGCOUNT	  1
  const char S01_0001[] PROGMEM = "english";      // Sprachbezeichnungen in Sprache 0
  const char S01_0002[] PROGMEM = "german";       //   
  const char S01_0003[] PROGMEM = "french";  
  const char S01_0004[] PROGMEM = "Englisch";     // Sprachebezeichnung in Sprache 1 
  const char S01_0005[] PROGMEM = "Deutsch";      //
  const char S01_0006[] PROGMEM = "Französisch";
//------------------------------------------------------------------------
// BEREICH  Meldungen, Meldungszeilen,  auch für sprintf als Formatierungszeichenkette 
// normale Anwendung    mit Notify(1...n)  hierbei wird 1:1 der hier definierte ausgegeben.
// erweitere Anwendung  mit Msg(1..n)  für sprintf Fälle mit einzubauenden Zahlen in den Text
//------------------------------------------------------------------------
#define _S02_OFFS       _S01_OFFS + _S01_RECORDS*_S01_FIELDS + _S01_RECORDS*_S01_FIELDS*_S01_LNGCOUNT 
#define _S02_FIELDS     1
#define _S02_RECORDS   41
#define _S02_LNGCOUNT   1
// Felder
#define _S02_MSG        0
  const char S02_0000[] PROGMEM = "Msg ";                                                     // 00 Meldung engl. Notify()
  const char S02_0001[] PROGMEM = "canceled";                                                 //  1          
  const char S02_0002[] PROGMEM = "CurrentLanguage:";                                         //  2  
  const char S02_0003[] PROGMEM = "Close Serial Port";                                        //  3          
  const char S02_0004[] PROGMEM = "Functions are not used in this Version, see (c)ommandos";  //  4
  const char S02_0005[] PROGMEM = "Language Set to:";                                         //  5
  const char S02_0006[] PROGMEM = "LCD IS SET TO:";                                           //  6
  const char S02_0007[] PROGMEM = "Onboard-Led is set to:";                                   //  7
  const char S02_0008[] PROGMEM = "BAUDRATE:";                                                //  8
  const char S02_0009[] PROGMEM = "Macro-Number:";                                            //  9
  const char S02_0010[] PROGMEM = "Status:";                                                  // 10
  const char S02_0011[] PROGMEM = "Setup Done";                                               // 11
  const char S02_0012[] PROGMEM = "wrong call Combination_DO Parameter CToDo:";               // 12
  const char S02_0013[] PROGMEM = "unknown";                                                  // 13
  const char S02_0014[] PROGMEM = " %ld Input Pins inited";                                   // 14
  const char S02_0015[] PROGMEM = " %ld Output Pins inited and set";                          // 15  
  const char S02_0016[] PROGMEM = " %ld Combinations inited and set";                         // 16
  const char S02_0017[] PROGMEM = "HX711 Modul is not calibrated at the moment";              // 17
  const char S02_0018[] PROGMEM = " %ld HX711 Moduls initialized";                            // 18
  const char S02_0019[] PROGMEM = " %ld .HX711 Modul Pin Dout:%ld Pin Clk:%ld defined";       // 19
  const char S02_0020[] PROGMEM = " %ld .HX711 Modul with Pin Dout:%ld and Pin Clk:%ld is not calibrated. Please calibrate before using data.";   // 20
  const char S02_0021[] PROGMEM = "Start Calibration for Weight-Modul %ld a Referenceweight from %ld [g] is required or pls change Parameter 8";  // 21
  const char S02_0022[] PROGMEM = "Step 1 f 4  empty Weight-Modul %ld press  1=ok  9=cancel";                                                     // 22
  const char S02_0023[] PROGMEM = "Step 2 f 4  Weight %ld Calibration empty Situation is taken ...";                                              // 23
  const char S02_0024[] PROGMEM = "Step 3 f 4  give Calibrationweight %ld[g] to Weight %ld, then   1=ok  9=cancel";                               // 24  
  const char S02_0025[] PROGMEM = "Step 4 f 4  Calibration for Weight %ld with referenceweight %ld is running ...";                               // 25
  const char S02_0026[] PROGMEM = "End Calibration for Weight-Modul %ld, Referenzweight can take off.";                                           // 26
  const char S02_0027[] PROGMEM = "Save Calibrationresults for Weight %ld Tara:%ld Factor:%ld   1=yes 9=cancel";                                  // 27
  const char S02_0028[] PROGMEM = "saved";                                                                                                        // 28
  const char S02_0029[] PROGMEM = "Weight %ld with Tara %ld[?] at moment set to %ld[?]         1=yes 9=cancel";                                   // 29 
  
  const char S02_0030[] PROGMEM = "Msg30:";       // 30 
  const char S02_0031[] PROGMEM = "Msg31:";       // 31
  const char S02_0032[] PROGMEM = "Msg32:";       // 32
  const char S02_0033[] PROGMEM = "Msg33:";       // 33
  const char S02_0034[] PROGMEM = "Msg34:";       // 34
  const char S02_0035[] PROGMEM = "Msg35:";       // 35
  const char S02_0036[] PROGMEM = "Msg36:";       // 36
  const char S02_0037[] PROGMEM = "Msg37:";       // 37
  const char S02_0038[] PROGMEM = "Msg38:";       // 38
  const char S02_0039[] PROGMEM = "Msg39:";       // 39
  const char S02_0040[] PROGMEM = "Msg40:";       // 40  
  
  const char S02_1000[] PROGMEM = "Mldg ";
  const char S02_1001[] PROGMEM = "abgebrochen";                                                            //  1
  const char S02_1002[] PROGMEM = "aktuelle Sprache:";                                                      //  2
  const char S02_1003[] PROGMEM = "Schliesse Serielle Schnittstelle";                                       //  3
  const char S02_1004[] PROGMEM = "Funktionen werden in dieser Version nicht verwendet. Siehe (c)ommandos"; //  4
  const char S02_1005[] PROGMEM = "Sprache eingestellt auf:";                                               //  5
  const char S02_1006[] PROGMEM = "LCD ist geschaltet zu:";                                                 //  6
  const char S02_1007[] PROGMEM = "Onboard LED ist geschaltet:";                                            //  7
  const char S02_1008[] PROGMEM = "Baudrate:";                                                              //  8
  const char S02_1009[] PROGMEM = "Makro-Nummer:";                                                          //  9
  const char S02_1010[] PROGMEM = "Status:";                                                 // 10
  const char S02_1011[] PROGMEM = "Setup abgeschlossen";                                     // 11
  const char S02_1012[] PROGMEM = "Fehlerhafter Aufruf von Combination_DO Parameter CToDo:"; // 12
  const char S02_1013[] PROGMEM = "unbekannt";                                               // 13
  const char S02_1014[] PROGMEM = "Es wurden %ld Eingänge initialisiert";                    // 14
  const char S02_1015[] PROGMEM = "Es wurden %ld Ausgänge initialisiert";                    // 15
  const char S02_1016[] PROGMEM = "Es wurden %ld Kombinationen initialisiert";               // 16
  const char S02_1017[] PROGMEM = "Das HX711 Modul ist zur Zeit noch nicht kalibriert";      // 17
  const char S02_1018[] PROGMEM = "Es wurden %ld HX711 Module initialisiert.";               // 18
  const char S02_1019[] PROGMEM = " %ld .HX711 Modul Pin Dout:%ld Pin Clk:%ld definiert";    // 19
  const char S02_1020[] PROGMEM = " %ld .Hx711 Modui mit Pin Dout:%ld und Pin Clk:%ld ist noch nicht kalibriert. Vor Verwendung muß das Modul kalibriert werden.";  // 20
  const char S02_1021[] PROGMEM = "Start Kalibrierung für Gewichtsmessmodul %ld ein Referenzgewicht von %ld[g] wird benötigt oder ändere Parameter 8";              // 21 
  const char S02_1022[] PROGMEM = "Schritt 1 v 4  Waage %ld frei machen  1=ok  9=Abbrechen ";                                                                       // 22 
  const char S02_1023[] PROGMEM = "Schritt 2 v 4  Waage %ld Kalibrierung Leerzustand wird ermittelt ..";                                                            // 23
  const char S02_1024[] PROGMEM = "Schritt 3 v 4  lege das Kalibrierungsgewicht von %ld[g] auf die Waage %ld, dann 1=ok  9=Abbrechen";                              // 24
  const char S02_1025[] PROGMEM = "Schritt 4 v 4  Kalibrierung von Modul %ld mit Referenzgewicht %ld läuft ...";                                                    // 25
  const char S02_1026[] PROGMEM = "Ende der Kalibrierung von Gewichtsmodul %ld, das Referenzgewicht kann entfernt werden.";                                         // 26
  const char S02_1027[] PROGMEM = "Speichern der Kalibrierung für Waage %ld Tara:%ld Fakt:%ld   1=Ja 9=abbrechen";                                                  // 27 
  const char S02_1028[] PROGMEM = "gespeichert";                                                                                                                    // 28
  const char S02_1029[] PROGMEM = "Waage %ld mit momentan eingestelltem ? von %ld[?] auf einen Wert von %ld[?] stellen    1=ja 9=abbrechen";                        // 29
  
  const char S02_1030[] PROGMEM = "Msg30:"; // 30
  const char S02_1031[] PROGMEM = "Msg21:"; // 31
  const char S02_1032[] PROGMEM = "Msg22:"; // 32
  const char S02_1033[] PROGMEM = "Msg23:"; // 33
  const char S02_1034[] PROGMEM = "Msg24:"; // 34
  const char S02_1035[] PROGMEM = "Msg25:"; // 35
  const char S02_1036[] PROGMEM = "Msg26:"; // 36
  const char S02_1037[] PROGMEM = "Msg27:"; // 37
  const char S02_1038[] PROGMEM = "Msg28:"; // 38
  const char S02_1039[] PROGMEM = "Msg29:"; // 39
  const char S02_1040[] PROGMEM = "Msg30:"; // 40

//--------------------------------------------------------------------------------------
// HX711 Module  Wiegemodule  pro Modul werden 2 Pins benötigt DOUT und CLK 
//  Sprachabhängiger Teil
//--------------------------------------------------------------------------------------
#define _HX711_CNT      _S03_RECORDS-1
#define _S03_OFFS       _S02_OFFS + _S02_RECORDS*_S02_FIELDS + _S02_RECORDS*_S02_FIELDS*_S02_LNGCOUNT 
#define _S03_FIELDS     2
#define _S03_RECORDS    4
#define _S03_LNGCOUNT   1
#define _S03_NAME       0  // Feld 0
#define _S03_DESCR      1  // Feld 1
  const char S03_0000[] PROGMEM = "Weight Module Name";                     // 00
  const char S03_0001[] PROGMEM = "Weightmoduls HX711 detail description";  // 

  const char S03_0002[] PROGMEM = "Weight Modul 01";                        // 01
  const char S03_0003[] PROGMEM = "Weightmodul  01 HX711";

  const char S03_0004[] PROGMEM = "Weight Modul 02";                        // 02
  const char S03_0005[] PROGMEM = "Weightmodul  02 HX711";
  
  const char S03_0006[] PROGMEM = "Weight Modul 03";                        // 03
  const char S03_0007[] PROGMEM = "Weightmodul  03 HX711";
    
// nächste Sprache
  const char S03_1000[] PROGMEM = "Gewichtsmess Modul Name";                // 00
  const char S03_1001[] PROGMEM = "Gewichtsmessmodul Beschreibung";   
  
  const char S03_1002[] PROGMEM = "Gewichtsmodul 01";                       // 01 
  const char S03_1003[] PROGMEM = "Gewichtsmessmodul 01 für ...";

  const char S03_1004[] PROGMEM = "Wiege Modul 02";                         // 02
  const char S03_1005[] PROGMEM = "Wiegemodul 02 HX711 für nächsten Zweck";
  
  const char S03_1006[] PROGMEM = "Wiege Modul 03";                         // 03
  const char S03_1007[] PROGMEM = "Wiegemodul 03 HX711 für nächsten Zweck";
  
//------------------------------------------------------------------------------------
// HX711 Module Sprachunabhängiger Teil  DATEN
//------------------------------------------------------------------------------------
#define _S04_OFFS       _S03_OFFS + _S03_RECORDS*_S03_FIELDS + _S03_RECORDS*_S03_FIELDS*_S03_LNGCOUNT 
#define _S04_FIELDS     3
#define _S04_RECORDS    _S03_RECORDS
#define _S04_LNGCOUNT   0

#define _S04_DOUT       0  // Feld 0
#define _S04_CLK        1  // Feld 1
  const char S04_0000[] PROGMEM = "PIN Nr for DOUT of HX711";               // 00
  const char S04_0001[] PROGMEM = "PIN Nr for CLK  of HX711";   
  const char S04_0002[] PROGMEM = "Reservedaten falls noch etwas pro Modul hinzukommt";
#define _HX711_DOUT_1 55    // A1 Pin
#define _HX711_CLK_1  56    // A2 Pin 
  const char S04_0003[] PROGMEM = "55";                                     // 01 Modul  PIN für DOUT
  const char S04_0004[] PROGMEM = "56";                                     //           PIN für CLK
  const char S04_0005[] PROGMEM = "";
#define _HX711_DOUT_2 55
#define _HX711_CLK_2  56 
  const char S04_0006[] PROGMEM = "55";                                     // 02 Modul  PIN für DOUT
  const char S04_0007[] PROGMEM = "56";                                     //           PIN für CLK
  const char S04_0008[] PROGMEM = "";
#define _HX711_DOUT_3 55
#define _HX711_CLK_3  56
  const char S04_0009[] PROGMEM = "55";                                     // 03 Modul  PIN für DOUT
  const char S04_0010[] PROGMEM = "56";                                     //           PIN für CLK
  const char S04_0011[] PROGMEM = "";
  
  // noch freie Bereiche ------------------------------
#define _S05_OFFS       _S04_OFFS + _S04_RECORDS*_S04_FIELDS + _S04_RECORDS*_S04_FIELDS*_S04_LNGCOUNT 
#define _S05_FIELDS     1
#define _S05_RECORDS    0
#define _S05_LNGCOUNT   0
// noch freie Bereiche ------------------------------
#define _S06_OFFS       _S05_OFFS + _S05_RECORDS*_S05_FIELDS + _S05_RECORDS*_S05_FIELDS*_S05_LNGCOUNT 
#define _S06_FIELDS     1
#define _S06_RECORDS    0
#define _S06_LNGCOUNT   0

//----------------------------------------------------------------------------------------
// BEREICH  Datum/Zeit  Formatierungsangaben
//----------------------------------------------------------------------------------------
#define _S07_OFFS       _S06_OFFS +_S06_RECORDS *_S06_FIELDS + _S06_RECORDS*_S06_FIELDS*_S06_LNGCOUNT   //  
#define _S07_FIELDS     1
#define _S07_RECORDS    3
#define _S07_LNGCOUNT   2
  const char S07_0001[] PROGMEM = "date:%04d-%02d-%02d time %02d:%02d:%02d";  // Datumsformat Sprache 0 Format 1
  const char S07_0002[] PROGMEM = "%04d-%02d-%02d %02d:%02d:%02d";            // Datumsformat Sprache 0 Format 2
  const char S07_0003[] PROGMEM = "%04d-%02d-%02d %02d:%02d";                 // Datumsformat Sprache 0 Format 3
      
  const char S07_0004[] PROGMEM = "Datum:%04d-%02d-%02d Zeit %02d:%02d:%02d"; //              Sprache 1 Format 1
  const char S07_0005[] PROGMEM = "%04d-%02d-%02d %02d:%02d:%02d";
  const char S07_0006[] PROGMEM = "%04d-%02d-%02d %02d";
    
  const char S07_0007[] PROGMEM = "date:%04d-%02d-%02d temps %02d:%02d:%02d"; //              Sprache 2 Format 1
  const char S07_0008[] PROGMEM = "%04d-%02d-%02d %02d:%02d:%02d";
  const char S07_0009[] PROGMEM = "%04d-%02d-%02d %02d:%02d:%02d";
    
//----------------------------------------------------------------------------------------
// BEREICH  frei
//----------------------------------------------------------------------------------------
#define _S08_OFFS       _S07_OFFS +_S07_RECORDS *_S07_FIELDS + _S07_RECORDS*_S07_FIELDS*_S07_LNGCOUNT   // = 6 + 3*1 + 3*0 = 9
#define _S08_FIELDS     12
#define _S08_RECORDS    1
#define _S08_LNGCOUNT   0
  const char S08_0001[] PROGMEM = "";
  const char S08_0002[] PROGMEM = "";
  const char S08_0003[] PROGMEM = "";
  const char S08_0004[] PROGMEM = "";
  const char S08_0005[] PROGMEM = "";
  const char S08_0006[] PROGMEM = "";
  const char S08_0007[] PROGMEM = "";
  const char S08_0008[] PROGMEM = "";
  const char S08_0009[] PROGMEM = "";
  const char S08_0010[] PROGMEM = "";
  const char S08_0011[] PROGMEM = "";
  const char S08_0012[] PROGMEM = "";
  
//-------------------------------------------------------------------------------------------------------
// BEREICH  Commando Namen Syntax Bezeichnungen für c(commando)  C O M M A N D O S / F U N K T I O N E N
//-------------------------------------------------------------------------------------------------------
#define _S09_OFFS		    _S08_OFFS +_S08_RECORDS *_S08_FIELDS + _S08_RECORDS*_S08_FIELDS*_S08_LNGCOUNT   // = 6 + 3*1 + 3*0 = 9
#define _S09_FIELDS     7
#define _S09_RECORDS   24
#define _S09_LNGCOUNT   0
// S p r a c h e   0   3 Datensätze je 7 Felder 
  const char S09_0001[] PROGMEM = "00 empty cmd";    			      // Kommandobezeichnung Sprache 0 für LCD-Ausgabe
  const char S09_0002[] PROGMEM = "empty command do nothing"; 	// Kommandobezeichnung Sprache 0 für UART-Ausgabe
  const char S09_0003[] PROGMEM = "has 0 Parameter";        	  // Anzahl Parameter 
  const char S09_0004[] PROGMEM = "P1:";       					        // Bedeutung zu Parameter 1
  const char S09_0005[] PROGMEM = "P2:";						            // Bedeutung zu Parameter 2
  const char S09_0006[] PROGMEM = "P3:";						            // Bedeutung zu Parameter 3
  const char S09_0007[] PROGMEM = "P4:";						            // Bedeutung zu Parameter 4 
  
  const char S09_0008[] PROGMEM = "01 LCD Backlight";    		    // Kommandobezeichnung Sprache 0 für LCD-Ausgabe
  const char S09_0009[] PROGMEM = "LCD Background Light";   	  // Kommandobezeichnung Sprache 0 für UART-Ausgabe
  const char S09_0010[] PROGMEM = " 1 Parameter";
  const char S09_0011[] PROGMEM = "P1 0=IST  1=OFF  2=ON"; 
  const char S09_0012[] PROGMEM = "";
  const char S09_0013[] PROGMEM = "";
  const char S09_0014[] PROGMEM = "";

  const char S09_0015[] PROGMEM = "02 reboot";    				      // Kommandobezeichnung Sprache 0 für LCD-Ausgabe            
  const char S09_0016[] PROGMEM = "reboot the arduino now";   	// Kommandobezeichnung Sprache 0 für UART-Ausgabe 
  const char S09_0017[] PROGMEM = " 1 Parameter for safety"; // Anzahl Parameter 
  const char S09_0018[] PROGMEM = "P1  2=really else=not really"; 
  const char S09_0019[] PROGMEM = "";
  const char S09_0020[] PROGMEM = "";
  const char S09_0021[] PROGMEM = "";

  const char S09_0022[] PROGMEM = "03 OnBd LED";
  const char S09_0023[] PROGMEM = "switch Onboard LED to";
  const char S09_0024[] PROGMEM = " 1 Parameter";
  const char S09_0025[] PROGMEM = "P1  0=IST 1=OFF 2=ON 3..PWM";
  const char S09_0026[] PROGMEM = "";
  const char S09_0027[] PROGMEM = "";
  const char S09_0028[] PROGMEM = "";

  const char S09_0029[] PROGMEM = "04 Set PIN-Nr";
  const char S09_0030[] PROGMEM = "sets a defined Pin direct to";
  const char S09_0031[] PROGMEM = " 2 Parameter";
  const char S09_0032[] PROGMEM = "P1  Pin-Nr";
  const char S09_0033[] PROGMEM = "P2  0=Low 1=High 2..255 PWM";
  const char S09_0034[] PROGMEM = "";
  const char S09_0035[] PROGMEM = "";

  const char S09_0036[] PROGMEM = "05 Set Pin-Mode";
  const char S09_0037[] PROGMEM = "sets a defined Pin to MODE";
  const char S09_0038[] PROGMEM = " 2 Parameter";
  const char S09_0039[] PROGMEM = "P1  Pin-Nr";
  const char S09_0040[] PROGMEM = "P2  1=INPUT  2=INPUT_PULLUP  3=OUTPUT";
  const char S09_0041[] PROGMEM = "";
  const char S09_0042[] PROGMEM = "";
 
  const char S09_0043[] PROGMEM = "06 Baudrate";
  const char S09_0044[] PROGMEM = " ";
  const char S09_0045[] PROGMEM = "2  Parameter";
  const char S09_0046[] PROGMEM = "P1  0= ? Serial.BAUDRATE  ";
  const char S09_0047[] PROGMEM = "P2  2= Set Serial.BAUDRATE ";
  const char S09_0048[] PROGMEM = "P3  BAUDRATE";
  const char S09_0049[] PROGMEM = "   300 1200 2400 4800 ... 115200"; 
 
  const char S09_0050[] PROGMEM = "07 Print TBL Sections to UART";
  const char S09_0051[] PROGMEM = " ";
  const char S09_0052[] PROGMEM = "2 Parameter";
  const char S09_0053[] PROGMEM = "P1 Section-Number";
  const char S09_0054[] PROGMEM = "P2 Field-Number";
  const char S09_0055[] PROGMEM = "";
  const char S09_0056[] PROGMEM = ""; 
 
  const char S09_0057[] PROGMEM = "08 Print TBL Adresses to UART";
  const char S09_0058[] PROGMEM = " ";
  const char S09_0059[] PROGMEM = "2 Parameter";
  const char S09_0060[] PROGMEM = "P1  From Adress";
  const char S09_0061[] PROGMEM = "P2  To   Adress";
  const char S09_0062[] PROGMEM = "";
  const char S09_0063[] PROGMEM = ""; 
 
  const char S09_0064[] PROGMEM = "09 Print TBL Adresses to LCD";
  const char S09_0065[] PROGMEM = " ";
  const char S09_0066[] PROGMEM = "4 Parameter";
  const char S09_0067[] PROGMEM = "P1  From Adress";
  const char S09_0068[] PROGMEM = "P2    To Adress";
  const char S09_0069[] PROGMEM = "P3  LCD-X-Position";
  const char S09_0070[] PROGMEM = "P4  LCD-Line 0..3";    

  const char S09_0071[] PROGMEM = "10 Set/Ask MacroStatus ";
  const char S09_0072[] PROGMEM = "Ask or Set the Status from the Macro to a specified Number.";
  const char S09_0073[] PROGMEM = "3 Parameter";
  const char S09_0074[] PROGMEM = "P1 Macro-Nr";
  const char S09_0075[] PROGMEM = "P2 0=Ask  1=Let";
  const char S09_0076[] PROGMEM = "P3 Status-Number 0=OK, 1...3 at Work";
  const char S09_0077[] PROGMEM = "";    
  
  const char S09_0078[] PROGMEM = "11 Kill one or FROM - TO running Macro/s";
  const char S09_0079[] PROGMEM = "stops one or more running Macro/s at next Step";
  const char S09_0080[] PROGMEM = "2 Parameter";
  const char S09_0081[] PROGMEM = "P1 FROM Macro-Number";
  const char S09_0082[] PROGMEM = "P2 TO   Macro-Number";
  const char S09_0083[] PROGMEM = "";
  const char S09_0084[] PROGMEM = ""; 
  
  const char S09_0085[] PROGMEM = "12 macro reserve";
  const char S09_0086[] PROGMEM = "";
  const char S09_0087[] PROGMEM = "  Parameter";
  const char S09_0088[] PROGMEM = "P1";
  const char S09_0089[] PROGMEM = "P2";
  const char S09_0090[] PROGMEM = "";
  const char S09_0091[] PROGMEM = "";

  const char S09_0092[] PROGMEM = "13 Add 1 Occurrence to Error-LOG";
  const char S09_0093[] PROGMEM = "Writes Data to the EEPROM Section Error-Log ";
  const char S09_0094[] PROGMEM = "2 Parameter";
  const char S09_0095[] PROGMEM = "P1 Error-Number ";
  const char S09_0096[] PROGMEM = "P2 Section 1=unconfirmed 2=total(readed) ";
  const char S09_0097[] PROGMEM = "";
  const char S09_0098[] PROGMEM = "";

  const char S09_0099[] PROGMEM = "14 Read Occurrence or Timestamp from Error-LOG";
  const char S09_0100[] PROGMEM = "Reads Counters from the EEPROM Section Error-Log ";
  const char S09_0101[] PROGMEM = "2 Parameter";
  const char S09_0102[] PROGMEM = "P1 Error-Number ";
  const char S09_0103[] PROGMEM = "P2 Section 1=unconfirmed 2=total(readed) 3=last Timestamp ";
  const char S09_0104[] PROGMEM = "";
  const char S09_0105[] PROGMEM = "";
                                  
  const char S09_0106[] PROGMEM = "15 RESET Err-Nr 0=ALL Occurrence Error-LOG";
  const char S09_0107[] PROGMEM = "RESET Counter1, Counter2 or Time Data in the EEPROM Section Error-Log ";
  const char S09_0108[] PROGMEM = "2 Parameter";
  const char S09_0109[] PROGMEM = "P1 Error-Number 0=ALL";
  const char S09_0110[] PROGMEM = "P2 Section 1=unconfirmed 2=total(readed) 3=both  4=both + Timestamp ZERO";
  const char S09_0111[] PROGMEM = "";
  const char S09_0112[] PROGMEM = ""; 
                                  
  const char S09_0113[] PROGMEM = "16 error reserve";
  const char S09_0114[] PROGMEM = " ";
  const char S09_0115[] PROGMEM = "   Parameter";
  const char S09_0116[] PROGMEM = "P1";
  const char S09_0117[] PROGMEM = "P2";
  const char S09_0118[] PROGMEM = "P3";
  const char S09_0119[] PROGMEM = "P4";
                                  
  const char S09_0120[] PROGMEM = "17 error reserve";
  const char S09_0121[] PROGMEM = " ";
  const char S09_0122[] PROGMEM = "   Parameter";
  const char S09_0123[] PROGMEM = "P1";
  const char S09_0124[] PROGMEM = "P2";
  const char S09_0125[] PROGMEM = "P3";
  const char S09_0126[] PROGMEM = "P4";
                                  
  const char S09_0127[] PROGMEM = "18 Let/Ask Indicator Bit-Array manually ";
  const char S09_0128[] PROGMEM = "Set/Ask single BIT in Indicator-Bit-Array ";
  const char S09_0129[] PROGMEM = "3 Parameter returns 1 =LOW 2 =HIGH";
  const char S09_0130[] PROGMEM = "P1 Bit-Nr 1..8 ";
  const char S09_0131[] PROGMEM = "P2 0=Ask  1=Let";
  const char S09_0132[] PROGMEM = "P3 New Bit-Value 1=Low 2=HIGH";
  const char S09_0133[] PROGMEM = "";
                                  
  const char S09_0134[] PROGMEM = "19 Let/Ask g-Indicator Bit-Array manually";
  const char S09_0135[] PROGMEM = "set/Ask singe g-Bit in g-Bit-Array";
  const char S09_0136[] PROGMEM = "2 Parameter returns 1 =OFF  2 =ON";
  const char S09_0137[] PROGMEM = "P1 g-Number 1...gmax ";
  const char S09_0138[] PROGMEM = "P2 0=Ask  1=Let OFF  2=Let ON";
  const char S09_0139[] PROGMEM = "";
  const char S09_0140[] PROGMEM = "";
                                  
  const char S09_0141[] PROGMEM = "20 Let/Ask g-Indicator BYTE-Array manually";
  const char S09_0142[] PROGMEM = "set/Ask singe g-BYTE in g-BYTE-Array";
  const char S09_0143[] PROGMEM = "2 Parameter returns 1=OFF  2=ON  3..255 PWM";
  const char S09_0144[] PROGMEM = "P1 g-Number 1...gmax ";
  const char S09_0145[] PROGMEM = "P2 0=Ask  1=Let OFF  2=Let ON  3..255 Let PWM";
  const char S09_0146[] PROGMEM = "";
  const char S09_0147[] PROGMEM = "";
                                  
  const char S09_0148[] PROGMEM = "21 Let/Ask k-Indicator Bit Array manually";
  const char S09_0149[] PROGMEM = "Set/Ask singe k-bit in k-bit-Array";
  const char S09_0150[] PROGMEM = "2 Parameter returns 1=OFF  2=ON";
  const char S09_0151[] PROGMEM = "P1 k-Nr 1..kmax";
  const char S09_0152[] PROGMEM = "P2 0=Ask  1=Let OFF  2=Let ON";
  const char S09_0153[] PROGMEM = "";
  const char S09_0154[] PROGMEM = "";
     
  const char S09_0155[] PROGMEM = "22 Let/Ask k-Indicator BYTE-Array manually";
  const char S09_0156[] PROGMEM = "Set/Ask singe k-bit in k-BYTE-Array";
  const char S09_0157[] PROGMEM = "2 Parameter";
  const char S09_0158[] PROGMEM = "P1 k-Nr 1..kmax";
  const char S09_0159[] PROGMEM = "P2 0=Ask  1=Let OFF  2=Let ON  3=all (g) OFF  4=all (g) ON  5=all ON inverted";
  const char S09_0160[] PROGMEM = "";
  const char S09_0161[] PROGMEM = "";
     
  const char S09_0162[] PROGMEM = "23 ";  // free reserve
  const char S09_0163[] PROGMEM = " ";
  const char S09_0164[] PROGMEM = "has   Parameter";
  const char S09_0165[] PROGMEM = "P1";
  const char S09_0166[] PROGMEM = "P2";
  const char S09_0167[] PROGMEM = "P3";
  const char S09_0168[] PROGMEM = "P4";      
     	 
//--------------------------------------------------------------------------------------------------------------
// BEREICH  TMP = Temperaturfühler  DS18    Ein Sprachabhängiger Datensatz hat 2 FELDER  1 Zusatz-SPRACHE (zum Test)
//     1.Fühlerkürzel  2.Fühler-Name Sprache 1 (en)
//--------------------------------------------------------------------------------------------------------------
#define _S10_OFFS  		_S09_OFFS +_S09_RECORDS *_S09_FIELDS + _S09_RECORDS*_S09_FIELDS*_S09_LNGCOUNT
#define _S10_FIELDS     2
#define _S10_RECORDS    6
#define _S10_LNGCOUNT   1
// S p r a c h e   0  6 Datensätze je 2 Felder  = 12 Einträge
  const char S10_0001[] PROGMEM = "DS18 01";              // Fühler Nummer 1
  const char S10_0002[] PROGMEM = "buffer store top";     // Name Sprache 1 = Englisch
  const char S10_0003[] PROGMEM = "DS18 02";
  const char S10_0004[] PROGMEM = "buffer store down";
  const char S10_0005[] PROGMEM = "DS18 03";
  const char S10_0006[] PROGMEM = "heat pump VL";
  const char S10_0007[] PROGMEM = "DS18 04";
  const char S10_0008[] PROGMEM = "sensor 4";
  const char S10_0009[] PROGMEM = "DS18 05";
  const char S10_0010[] PROGMEM = "sensor 5";
  const char S10_0011[] PROGMEM = "DS18 06";
  const char S10_0012[] PROGMEM = "sensor 6";
// nächste S p r a c h e  1  6 Datensätze je 2 Felder = 12 Einträge 
  const char S10_0013[] PROGMEM = "DS18 01";              // Fühler Nummer 1  
  const char S10_0014[] PROGMEM = "Pufferspeicher oben";  // Name Sprache 2 = Deutsch 
  const char S10_0015[] PROGMEM = "DS18 02";
  const char S10_0016[] PROGMEM = "Pufferspeicher unten";
  const char S10_0017[] PROGMEM = "DS18 03";
  const char S10_0018[] PROGMEM = "Wärmepumpe VL";
  const char S10_0019[] PROGMEM = "DS18 04";
  const char S10_0020[] PROGMEM = "Sensor Nr 4";
  const char S10_0021[] PROGMEM = "DS18 05";
  const char S10_0022[] PROGMEM = "Sensor Nr 5";
  const char S10_0023[] PROGMEM = "DS18 06";
  const char S10_0024[] PROGMEM = "Sensor Nr 6";
//-------------------------------------------------------------------------
// Temperaturfühler DS18 Sprachenunabhängiger TEIL wie PIN, ADRESSE
//-------------------------------------------------------------------------
#define _S11_OFFS      _S10_OFFS +_S10_RECORDS *_S10_FIELDS + _S10_RECORDS*_S10_FIELDS*_S10_LNGCOUNT   
#define _S11_FIELDS     2     // Anzahl Felder pro Datensatz
#define _S11_RECORDS    6     // Anzahl Datensätze (muß Synchron mit _S10_ Abschnitt sein !!)
#define _S11_LNGCOUNT   0     // Anzahl zusätzlicher Sprachen für diesen Bereich
  const char S11_0001[] PROGMEM = "50";                     // Pin-Nummer für Sensor 01
  const char S11_0002[] PROGMEM = "Adresse für Sensor 01";  // HEX-Adresse des Sensors 
  const char S11_0003[] PROGMEM = "50";                     // Pin-Nummer für Sensor 02
  const char S11_0004[] PROGMEM = "Adresse für Sensor 02";  // HEX-Adresse des Sensors 
  const char S11_0005[] PROGMEM = "50";                     // Pin-Nummer für Sensor 03
  const char S11_0006[] PROGMEM = "Adresse für Sensor 03";  // HEX-Adresse des Sensors 
  const char S11_0007[] PROGMEM = "50";                     // Pin-Nummer für Sensor 04
  const char S11_0008[] PROGMEM = "Adresse für Sensor 04";  // HEX-Adresse des Sensors 
  const char S11_0009[] PROGMEM = "50";                     // Pin-Nummer für Sensor 05
  const char S11_0010[] PROGMEM = "Adresse für Sensor 05";  // HEX-Adresse des Sensors 
  const char S11_0011[] PROGMEM = "50";                     // Pin-Nummer für Sensor 06
  const char S11_0012[] PROGMEM = "Adresse für Sensor 06";  // HEX-Adresse des Sensors 


//--------------------------------------------------------------------------------------------------------------
// BEREICH  MNU = MENÜ    Ein Datensatz hat 4 ZEILEN=FELDER    
//--------------------------------------------------------------------------------------------------------------
#define _S12_OFFS     _S11_OFFS + _S11_RECORDS*_S11_FIELDS +  _S11_RECORDS*_S11_FIELDS*_S11_LNGCOUNT  //  9 + 6*2 + 6*1 = 9 + 12 + 12 = 9 + 24 = 33
#define _S12_FIELDS       4   // Anzahl normaler Felder pro Datensatz
#define _S12_RECORDS      2   // Anzahl Datensätze (2 Menüs in diesem Fall also)
#define _S12_LNGCOUNT     2   // Anzahl zusätzlicher Sprachen 
//-------------------M E N Ü  1 // 0123456789012456789   Z E I L E  0    TEXTZEILEN 0...3  Sprache 0 ------------------------		
  const char S12_0001[] PROGMEM = "ON:.   SSR:...     ";  //   Zeile 0 des Info-Schirms A  in Sprache 0  E N G L I S H      Feld 1
  const char S12_0002[] PROGMEM = "psi:... set:...    ";  //   Zeile 1 des Info-Schirms A  in Sprache 0  E N G L I S H      Feld 2  
  const char S12_0003[] PROGMEM = "H2:.... max:....   ";  //   Zeile 2 des Info-Schirms A  in Sprache 0  E N G L I S H      Feld 3  
  const char S12_0004[] PROGMEM = "DCA:... max:...    ";  //   Zeile 3 des Info-Schirms A  in Sprache 0  E N G L I S H      Feld 4  
//-------------------M E N Ü  2 // 0123456789012456789   Z E I L E  0    TEXTZEILEN 0...3  Sprache 0 ------------------------
  const char S12_0005[] PROGMEM = "BON:.  BBR:...     ";  //   Zeile 0 des Info-Schirms A  in Sprache 0  E N G L I S H
  const char S12_0006[] PROGMEM = "fsi:... set:...    ";  //   Zeile 1 des Info-Schirms A  in Sprache 0  E N G L I S H  
  const char S12_0007[] PROGMEM = "H2:.v.  max:....   ";  //   Zeile 2 des Info-Schirms A  in Sprache 0  E N G L I S H  
  const char S12_0008[] PROGMEM = "DCA:.k. max:...    ";  //   Zeile 3 des Info-Schirms A  in Sprache 0  E N G L I S H    
  
  
//-------------------M E N Ü  1 // 0123456789012456789   Z E I L E  0    TEXTZEILEN 0...3  Sprache 1 ------------------------
  const char S12_0009[] PROGMEM = "EIN:.  PWM-SSR:... ";  //   Zeile 0 des Info-Schirms A  in Sprache 1  D E U T S C H 
  const char S12_0010[] PROGMEM = "druck:... soll:... ";  //   Zeile 1 des Info-Schirms A  in Sprache 1  D E U T S C H 
  const char S12_0011[] PROGMEM = "H2:.... Grenze:....";  //   Zeile 2 des Info-Schirms A  in Sprache 1  D E U T S C H 
  const char S12_0012[] PROGMEM = "DCA:... Grenze:... ";  //   Zeile 3 des Info-Schirms A  in Sprache 1  D E U T S C H   
//-------------------M E N Ü  2 // 0123456789012456789   Z E I L E  0    TEXTZEILEN 0...3  Sprache 1 ------------------------  
  const char S12_0013[] PROGMEM = "CCN:.  PcccSSR:... ";  //   Zeile 0 des Info-Schirms A  in Sprache 1  D E U T S C H
  const char S12_0014[] PROGMEM = "xruck:... soll:... ";  //   Zeile 1 des Info-Schirms A  in Sprache 1  D E U T S C H  
  const char S12_0015[] PROGMEM = "H2:.v.. Grenze:....";  //   Zeile 2 des Info-Schirms A  in Sprache 1  D E U T S C H  
  const char S12_0016[] PROGMEM = "DCA:.k. Grenze:... ";  //   Zeile 3 des Info-Schirms A  in Sprache 1  D E U T S C H    
  
  
//-------------------M E N Ü  1 // 0123456789012456789   Z E I L E  0    TEXTZEILEN 0...3  Sprache 2 ------------------------						    
  const char S12_0017[] PROGMEM = "ISON:. PWMNr:...   ";  //   Zeile 0 des Info-Schirms A  in Sprache 2  F R E N C H        
  const char S12_0018[] PROGMEM = "p:... découvert:...";  //   Zeile 1 des Info-Schirms A  in Sprache 2  F R E N C H   
  const char S12_0019[] PROGMEM = "H2:.... limite:....";  //   Zeile 2 des Info-Schirms A  in Sprache 2  F R E N C H   
  const char S12_0020[] PROGMEM = "STR:... llll:......";  //   Zeile 3                     in Sprache 2  F R E N C H      

//-------------------M E N Ü  2 // 0123456789012456789   Z E I L E  0    TEXTZEILEN 0...3  Sprache 2 ------------------------
  const char S12_0021[] PROGMEM = "OOON:. POONr:...   ";  //   Zeile 0 des Info-Schirms A  in Sprache 2  F R E N C H 
  const char S12_0022[] PROGMEM = "v:... découvert:...";  //   Zeile 1 des Info-Schirms A  in Sprache 2  F R E N C H   
  const char S12_0023[] PROGMEM = "H2:.v.. limite:....";  //   Zeile 2 des Info-Schirms A  in Sprache 2  F R E N C H   
  const char S12_0024[] PROGMEM = "DCA:.k. limite:... ";  //   Zeile 3 des Info-Schirms A  in Sprache 2  F R E N C H    
  
//--------------------------------------------------------------------------------------------------------------
// BEREICH  MNS = MENÜ-Sprungpunkte   Ein Datensatz hat 10 Felder = 10 SPRUNGPUNKTE   pro Menü 
//--------------------------------------------------------------------------------------------------------------
#define _S13_OFFS     _S12_OFFS + _S12_RECORDS*_S12_FIELDS +  _S12_RECORDS*_S12_FIELDS*_S12_LNGCOUNT  //  33 + 2*4 + 2*4*2 = 33 + 8 + 16 = 33 + 24 = 57
#define _S13_FIELDS      10   // Anzahl normaler Felder pro Datensatz
#define _S13_RECORDS      2   // Anzahl Datensätze (für 2 Menüs die Sprungpunkte in diesem Fall also)
#define _S13_LNGCOUNT     2   // Anzahl zusätzlicher Sprachen  (wie auch bei den Menüs )

   //-------------------------//  10 S P R U N G P U N K T E  für Menü 1  Sprache 0 = Standard Englisch 
  const char S13_0001[] PROGMEM = " 3, 0";
  const char S13_0002[] PROGMEM = " 4, 0";
  const char S13_0003[] PROGMEM = " 5, 0";
  const char S13_0004[] PROGMEM = "11, 0";
  const char S13_0005[] PROGMEM = "16, 0";
  const char S13_0006[] PROGMEM = "14, 0";
  const char S13_0007[] PROGMEM = " 4, 1";
  const char S13_0008[] PROGMEM = " 6, 1";
  const char S13_0009[] PROGMEM = " 2, 1";
  const char S13_0010[] PROGMEM = "12, 1";
   //-------------------------//  10 S P R U N G P U N K T E  für Menü 2  Sprache 0 = Standard Englisch   
  const char S13_0011[] PROGMEM = "16, 1";
  const char S13_0012[] PROGMEM = "17, 1";
  const char S13_0013[] PROGMEM = " 3, 2";
  const char S13_0014[] PROGMEM = " 3, 2";
  const char S13_0015[] PROGMEM = " 3, 2";
  const char S13_0016[] PROGMEM = "12, 2";
  const char S13_0017[] PROGMEM = "16, 2";
  const char S13_0018[] PROGMEM = "16, 2";
  const char S13_0019[] PROGMEM = " 3, 3";
  const char S13_0020[] PROGMEM = " 3, 3";
                              //  10 S P R U N G P U N K T E  für Menü 1  Sprache 1 = de
  const char S13_0021[] PROGMEM = " 4, 3";
  const char S13_0022[] PROGMEM = "12, 3";
  const char S13_0023[] PROGMEM = "16, 3";
  const char S13_0024[] PROGMEM = "16, 3";
  const char S13_0025[] PROGMEM = " 4, a0";
  const char S13_0026[] PROGMEM = " 4, b0";
  const char S13_0027[] PROGMEM = " 5, c0";
  const char S13_0028[] PROGMEM = "v11, 0";
  const char S13_0029[] PROGMEM = "v16, 0";
  const char S13_0030[] PROGMEM = "v14, 0";
  
  const char S13_0031[] PROGMEM = " 4b, 1";
  const char S13_0032[] PROGMEM = " 6c, 1";
  const char S13_0033[] PROGMEM = " 2d, 1";
  const char S13_0034[] PROGMEM = "1d2, 1";
  const char S13_0035[] PROGMEM = "1d6, 1";
  const char S13_0036[] PROGMEM = "1d7, 1";
  const char S13_0037[] PROGMEM = " j3, 2";
  const char S13_0038[] PROGMEM = " o3, 2";
  const char S13_0039[] PROGMEM = " p3, 2";
  const char S13_0040[] PROGMEM = "1t2, 2";
  
  const char S13_0041[] PROGMEM = "1t6, 2";
  const char S13_0042[] PROGMEM = "1t6, 2";
  const char S13_0043[] PROGMEM = "k 3, 3";
  const char S13_0044[] PROGMEM = "k 3, 3";
  const char S13_0045[] PROGMEM = "k 4, 3";
  const char S13_0046[] PROGMEM = "u12, 3";
  const char S13_0047[] PROGMEM = "u16, 3";
  const char S13_0048[] PROGMEM = "u16, 3";
  const char S13_0049[] PROGMEM = " p3, 2";
  const char S13_0050[] PROGMEM = "1t2, 2";

  const char S13_0051[] PROGMEM = "1t6, 2";
  const char S13_0052[] PROGMEM = "1t6, 2";
  const char S13_0053[] PROGMEM = "k 3, 3";
  const char S13_0054[] PROGMEM = "k 3, 3";
  const char S13_0055[] PROGMEM = "k 4, 3";
  const char S13_0056[] PROGMEM = "u12, 3";
  const char S13_0057[] PROGMEM = "u16, 3";
  const char S13_0058[] PROGMEM = "u16, 3";
  const char S13_0059[] PROGMEM = " p3, 2";
  const char S13_0060[] PROGMEM = "1t2, 2";
  
//--------------------------------------------------------------------------------------------------------------
// BEREICH  MEH = MENGEN-EINHEITEN     Ein Datensatz hat 3 ZEILEN=FELDER + 2 SPRACHEN
//--------------------------------------------------------------------------------------------------------------
#define _S14_OFFS           _S13_OFFS + _S13_RECORDS * _S13_FIELDS  + _S13_RECORDS*_S13_FIELDS*_S13_LNGCOUNT
//                               57   +        2     *      10      +       2       *      10 *  2 
//                               57   +        2     *      10      +       2       *      10 *  2
//                               57   +        20                   +       40                     = 117
#define _S14_FIELDS        2
#define _S14_RECORDS       10
#define _S14_LNGCOUNT      0
  const char S14_0001[] PROGMEM = "[g]";          // 1
  const char S14_0002[] PROGMEM = "gram";
               
  const char S14_0003[] PROGMEM = "[kg]";         // 2
  const char S14_0004[] PROGMEM = "kilo";
               
  const char S14_0005[] PROGMEM = "[ml]";         // 3
  const char S14_0006[] PROGMEM = "milli";
               
  const char S14_0007[] PROGMEM = "[L]";          // 4
  const char S14_0008[] PROGMEM = "Liter";
               
  const char S14_0009[] PROGMEM = "[ms]";         // 5
  const char S14_0010[] PROGMEM = "milli sec";
               
  const char S14_0011[] PROGMEM = "[s]";          // 6
  const char S14_0012[] PROGMEM = "Sekunden"; 
               
  const char S14_0013[] PROGMEM = "mbar";         // 7
  const char S14_0014[] PROGMEM = "millibar";
               
  const char S14_0015[] PROGMEM = "[" "\xDF" "]C";  // 8
  const char S14_0016[] PROGMEM = "Grad Celsius";   
               
  const char S14_0017[] PROGMEM = "[pcs]";        // 9
  const char S14_0018[] PROGMEM = "piece";
               
  const char S14_0019[] PROGMEM = "[cnt]";        // 10
  const char S14_0020[] PROGMEM = "count";

//-------------------------------------------------------------------------
// BEREICH DAY = WOCHENTAGNAMEN  Montag, Dienstag, Mittwoch,...
//-------------------------------------------------------------------------
#define _S15_OFFS           _S14_OFFS + _S14_RECORDS * _S14_FIELDS  + _S14_RECORDS*_S14_FIELDS*_S14_LNGCOUNT  
//                               117  +       10     *      2       +       10    *      2    *  0           =    137
#define _WEEKDAYLONG  _S15_OFFS   // einfach ein ALIAS Name
#define _S15_FIELDS     1
#define _S15_RECORDS    7
#define _S15_LNGCOUNT   1
  const char S15_0001[] PROGMEM = "Sunday";
  const char S15_0002[] PROGMEM = "Monday";
  const char S15_0003[] PROGMEM = "Tuesday";
  const char S15_0004[] PROGMEM = "Wednesday";
  const char S15_0005[] PROGMEM = "Thursday";
  const char S15_0006[] PROGMEM = "Friday";
  const char S15_0007[] PROGMEM = "Saturday";
  
  const char S15_0008[] PROGMEM = "Sonntag";
  const char S15_0009[] PROGMEM = "Montag";
  const char S15_0010[] PROGMEM = "Dienstag";
  const char S15_0011[] PROGMEM = "Mittwoch";
  const char S15_0012[] PROGMEM = "Donnerstag";
  const char S15_0013[] PROGMEM = "Freitag";
  const char S15_0014[] PROGMEM = "Samstag";
  
//-------------------------------------------------------------------------
// BEREICH DAS = WOCHENTAGNAMEN Kurz   Mo, Di, Mi, 
//-------------------------------------------------------------------------
#define _S16_OFFS           _S15_OFFS + _S15_RECORDS * _S15_FIELDS  + _S15_RECORDS*_S15_FIELDS*_S15_LNGCOUNT
//                               137  +       7      *      1       +       7     *      1    *  1           =    151
#define _WEEKDAYSHORT  _S16_OFFS   // ein ALIAS
#define _S16_FIELDS     1
#define _S16_RECORDS    7
#define _S16_LNGCOUNT   1
  const char S16_0001[] PROGMEM = "sun";
  const char S16_0002[] PROGMEM = "mon";
  const char S16_0003[] PROGMEM = "tue";
  const char S16_0004[] PROGMEM = "wed";
  const char S16_0005[] PROGMEM = "thu";
  const char S16_0006[] PROGMEM = "fri";
  const char S16_0007[] PROGMEM = "sat";
  
  const char S16_0008[] PROGMEM = "Son";
  const char S16_0009[] PROGMEM = "Mon";
  const char S16_0010[] PROGMEM = "Die";
  const char S16_0011[] PROGMEM = "Mit";
  const char S16_0012[] PROGMEM = "Don";
  const char S16_0013[] PROGMEM = "Fre";
  const char S16_0014[] PROGMEM = "Sam";

//-------------------------------------------------------------------------------
// BEREICH MON = Monate 
//-------------------------------------------------------------------------------
#define _S17_OFFS           _S16_OFFS + _S16_RECORDS * _S16_FIELDS  + _S16_RECORDS*_S16_FIELDS*_S16_LNGCOUNT  
//                               151  +       7      *      1       +        7      *      1      *        1      =      165
#define _MONTHNAMELONG   _S17_OFFS
#define _S17_FIELDS     1
#define _S17_RECORDS   13
#define _S17_LNGCOUNT   1
  const char S17_0001[] PROGMEM = "emptymonth";
  const char S17_0002[] PROGMEM = "January";  //01
  const char S17_0003[] PROGMEM = "February"; //02
  const char S17_0004[] PROGMEM = "March";    //03
  const char S17_0005[] PROGMEM = "April";    //04
  const char S17_0006[] PROGMEM = "May";      //05
  const char S17_0007[] PROGMEM = "June";     //06
  const char S17_0008[] PROGMEM = "July";     //07
  const char S17_0009[] PROGMEM = "August";   //08
  const char S17_0010[] PROGMEM = "September";//09
  const char S17_0011[] PROGMEM = "October";  //10
  const char S17_0012[] PROGMEM = "November"; //11
  const char S17_0013[] PROGMEM = "December"; //12
  
  const char S17_0014[] PROGMEM = "Leermonat";
  const char S17_0015[] PROGMEM = "Januar";
  const char S17_0016[] PROGMEM = "Februar";
  const char S17_0017[] PROGMEM = "Maerz";
  const char S17_0018[] PROGMEM = "April";
  const char S17_0019[] PROGMEM = "Mai";
  const char S17_0020[] PROGMEM = "Juni";
  const char S17_0021[] PROGMEM = "Juli";
  const char S17_0022[] PROGMEM = "August";
  const char S17_0023[] PROGMEM = "September";
  const char S17_0024[] PROGMEM = "Oktober";
  const char S17_0025[] PROGMEM = "November";
  const char S17_0026[] PROGMEM = "Dezember";

//-------------------------------------------------------------------------------
// BEREICH MOK = Monate  Kurz 
//-------------------------------------------------------------------------------
#define _S18_OFFS           _S17_OFFS + _S17_RECORDS * _S17_FIELDS  + _S17_RECORDS*_S17_FIELDS*_S17_LNGCOUNT  
//                               165  +      13      *      1       +       13      *      1      *        1      =      191
#define _MONTHNAMESHORT   _S18_OFFS
#define _S18_FIELDS     1
#define _S18_RECORDS   13
#define _S18_LNGCOUNT   1
  const char S18_0001[] PROGMEM = "emp";
  const char S18_0002[] PROGMEM = "Jan";
  const char S18_0003[] PROGMEM = "Feb";
  const char S18_0004[] PROGMEM = "Mar";
  const char S18_0005[] PROGMEM = "Apr";
  const char S18_0006[] PROGMEM = "May";
  const char S18_0007[] PROGMEM = "Jun";
  const char S18_0008[] PROGMEM = "Jul";
  const char S18_0009[] PROGMEM = "Aug";
  const char S18_0010[] PROGMEM = "Sep";
  const char S18_0011[] PROGMEM = "Oct";
  const char S18_0012[] PROGMEM = "Nov";
  const char S18_0013[] PROGMEM = "Dec";

  const char S18_0014[] PROGMEM = "Leerm";
  const char S18_0015[] PROGMEM = "Jan";
  const char S18_0016[] PROGMEM = "Feb";
  const char S18_0017[] PROGMEM = "Mae";
  const char S18_0018[] PROGMEM = "Apr";
  const char S18_0019[] PROGMEM = "Mai";
  const char S18_0020[] PROGMEM = "Jun";
  const char S18_0021[] PROGMEM = "Jul";
  const char S18_0022[] PROGMEM = "Aug";
  const char S18_0023[] PROGMEM = "Sep";
  const char S18_0024[] PROGMEM = "Okt";
  const char S18_0025[] PROGMEM = "Nov";
  const char S18_0026[] PROGMEM = "Dez";

//-------------------------------------------------------------------------------
// BEREICH RE4  noch frei frei 
//-------------------------------------------------------------------------------
#define _S19_OFFS           _S18_OFFS + _S18_RECORDS * _S18_FIELDS  + _S18_RECORDS*_S18_FIELDS*_S18_LNGCOUNT
#define _S19_FIELDS     1
#define _S19_RECORDS   0
#define _S19_LNGCOUNT   0

//-------------------------------------------------------------------------------------------------
// BEREICH  PARAMETER    P a r a m e t e r   P A R A M E T E R   NAMEN Beschreibungen
//    SPRACHABHÄNGIGER TEIL   (über den Wert von Parameter Nr 0 wird erkannt gesteuert ob die Steuerung 
// das ERSTE mal in Betrieb genommen wird. Wenn der Wert nicht 12345 ist dann werden beim BOOT-Vorgang
// automatisch alle Parameter mit den Default-Werten befüllt und Parameter 0 wird auf 12345 gesetzt.
// Wird der Parameterwert 0 händisch von 12345 auf einen anderen Wert geändert werden automatisch
// beim nächsten Boot-Vorgang alle Standardwerte in die Parameter übertragen !!!!!!
//-------------------------------------------------------------------------------------------------
#define _PARAMETER_COUNT  _S20_RECORDS
#define _S20_OFFS 			_S19_OFFS + _S19_RECORDS * _S19_FIELDS  + _S19_RECORDS*_S19_FIELDS*_S19_LNGCOUNT
#define _S20_FIELDS 	  2 
#define _S20_RECORDS   21
#define _S20_LNGCOUNT   2   // zusätzliche Sprachen 
// Felder
#define _S20_NAME       0   // ParameterName
#define _S20_DESCR      1   // Beschreibung
 // ENGLISCHE Parameternamen, Berschreibung 
  const char S20_0000[] PROGMEM = "Parameter-Name____:"; 	                              // 00 Erkennen ob die Steuerung das ERSTE mal bootet
  const char S20_0001[] PROGMEM = "Description:______:";                                //    ist der Wert von Parameter 0 <> 12345 dann werden alle Parameter auf DEFAULT eingestellt
  
  const char S20_0002[] PROGMEM = "Msg BOOT to UART";				                            // 01
  const char S20_0003[] PROGMEM = "should arduino send a Message to the UART when booting ? 0/1";
  
  const char S20_0004[] PROGMEM = "set Language";		                                    // 02
  const char S20_0005[] PROGMEM = "witch Language for the Userinterface.";
  
  const char S20_0006[] PROGMEM = "Msg BOOT done UART";		                              // 03
  const char S20_0007[] PROGMEM = "send a message to UART when boot is done.";
  
  const char S20_0008[] PROGMEM = "LCD Backl.on BOOT";						                      // 04
  const char S20_0009[] PROGMEM = "turn ON the LCD Backlight after BOOT ? 1/2";
  
  const char S20_0010[] PROGMEM = "LCD MnuRollover";						                        // 05
  const char S20_0011[] PROGMEM = "turn ON the LCD Menue-rollover ? 0/1";
  
  const char S20_0012[] PROGMEM = "Message with Nr";						                        // 06
  const char S20_0013[] PROGMEM = "Print each Message with its Number at the beginning 0/1";
  
  const char S20_0014[] PROGMEM = "Msg BOOT details";						                        // 07
  const char S20_0015[] PROGMEM = "send BOOT Details to UART ? 0/1"; 
  
  const char S20_0016[] PROGMEM = "Calibration Weight";						                      // 08
  const char S20_0017[] PROGMEM = "Weight for Weight-Modul Calibration [g]";
  
  const char S20_0018[] PROGMEM = "";						// 09
  const char S20_0019[] PROGMEM = "";
  const char S20_0020[] PROGMEM = "";						// 10
  const char S20_0021[] PROGMEM = "";
  const char S20_0022[] PROGMEM = "";						// 11
  const char S20_0023[] PROGMEM = "";
  const char S20_0024[] PROGMEM = "";						// 12
  const char S20_0025[] PROGMEM = "";
  const char S20_0026[] PROGMEM = "";						// 13
  const char S20_0027[] PROGMEM = "";
  const char S20_0028[] PROGMEM = "";						// 14
  const char S20_0029[] PROGMEM = "";
  const char S20_0030[] PROGMEM = "";						// 15
  const char S20_0031[] PROGMEM = "";
  const char S20_0032[] PROGMEM = "";						// 16
  const char S20_0033[] PROGMEM = "";
  const char S20_0034[] PROGMEM = "";						// 17
  const char S20_0035[] PROGMEM = "";
  const char S20_0036[] PROGMEM = "";						// 18
  const char S20_0037[] PROGMEM = "";
  const char S20_0038[] PROGMEM = "";						// 19
  const char S20_0039[] PROGMEM = "";
  const char S20_0040[] PROGMEM = "";						// 20
  const char S20_0041[] PROGMEM = "";
 // DEUTSCH 
  const char S20_0042[] PROGMEM = "Parameter-Name____:"; 	                              // 00
  const char S20_0043[] PROGMEM = "Beschreibung:_____:"; 
  
  const char S20_0044[] PROGMEM = "Mldg BOOT an UART";	                                // 01
  const char S20_0045[] PROGMEM = "Soll arduino eine Meldung zur Seriellen senden beim booten ? 0/1";
  
  const char S20_0046[] PROGMEM = "Sprache";		                                        // 02
  const char S20_0047[] PROGMEM = "In welcher Sprache sollen die Ausgaben erfolgen.";
  
  const char S20_0048[] PROGMEM = "Mldg BOOT done";		                                  // 03
  const char S20_0049[] PROGMEM = "Meldung an die Serielle wenn Bootvorgang abgeschlossen ist.";
  
  const char S20_0050[] PROGMEM = "LCD Backl.nach BOOT";	                              // 04
  const char S20_0051[] PROGMEM = "LCD Hintergrundbeleuchtung nach dem Bootvorgang ? 1/2";
  
  const char S20_0052[] PROGMEM = "LCD Menuerollover";						                      // 05
  const char S20_0053[] PROGMEM = "Am Menue-Ende automatisch zum Menueanfang springen ? 0/1";
  
  const char S20_0054[] PROGMEM = "Meldung mit Nummer";						                      // 06
  const char S20_0055[] PROGMEM = "Ausgabe der Meldungen mit deren Meldungsnummer zu Beginn 0/1";
  
  const char S20_0056[] PROGMEM = "Mldg BOOT details";						                      // 07
  const char S20_0057[] PROGMEM = "Detaillierte Meldungen beim BOOTEN ? 0/1";
  
  const char S20_0058[] PROGMEM = "Kalibrierungsgewicht";	                    					// 08
  const char S20_0059[] PROGMEM = "bekanntes Gewicht in Gramm für die Kalibrierung von HX711 Wiegemodulen";
  
  const char S20_0060[] PROGMEM = "";						// 09
  const char S20_0061[] PROGMEM = "";
  const char S20_0062[] PROGMEM = "";						// 10
  const char S20_0063[] PROGMEM = "";
  const char S20_0064[] PROGMEM = "";						// 11
  const char S20_0065[] PROGMEM = "";
  const char S20_0066[] PROGMEM = "";						// 12
  const char S20_0067[] PROGMEM = "";
  const char S20_0068[] PROGMEM = "";						// 13
  const char S20_0069[] PROGMEM = "";
  const char S20_0070[] PROGMEM = "";						// 14
  const char S20_0071[] PROGMEM = "";
  const char S20_0072[] PROGMEM = "";						// 15
  const char S20_0073[] PROGMEM = "";
  const char S20_0074[] PROGMEM = "";						// 16
  const char S20_0075[] PROGMEM = "";
  const char S20_0076[] PROGMEM = "";						// 17
  const char S20_0077[] PROGMEM = "";
  const char S20_0078[] PROGMEM = "";						// 18
  const char S20_0079[] PROGMEM = "";
  const char S20_0080[] PROGMEM = "";						// 19
  const char S20_0081[] PROGMEM = "";
  const char S20_0082[] PROGMEM = "";						// 20
  const char S20_0083[] PROGMEM = "";
 // FRANZ
  const char S20_0084[] PROGMEM = "Fr Parametername__:"; 	// 00
  const char S20_0085[] PROGMEM = "Fr Description:___:"; 
  const char S20_0086[] PROGMEM = "keine Ahnung2";				// 01
  const char S20_0087[] PROGMEM = "keine Ahnung3";
  const char S20_0088[] PROGMEM = "keine Ahnung4";		// 02
  const char S20_0089[] PROGMEM = "keine Ahnung5";
  const char S20_0090[] PROGMEM = "keine Ahnung6";		// 03
  const char S20_0091[] PROGMEM = "keine Ahnung7";
  const char S20_0092[] PROGMEM = "";						// 04
  const char S20_0093[] PROGMEM = "";
  const char S20_0094[] PROGMEM = "";						// 05
  const char S20_0095[] PROGMEM = "";
  const char S20_0096[] PROGMEM = "";						// 06
  const char S20_0097[] PROGMEM = "";
  const char S20_0098[] PROGMEM = "";						// 07
  const char S20_0099[] PROGMEM = "";
  const char S20_0100[] PROGMEM = "";						// 08
  const char S20_0101[] PROGMEM = "";
  const char S20_0102[] PROGMEM = "";						// 09
  const char S20_0103[] PROGMEM = "";
  const char S20_0104[] PROGMEM = "";						// 10
  const char S20_0105[] PROGMEM = "";
  const char S20_0106[] PROGMEM = "";						// 11
  const char S20_0107[] PROGMEM = "";
  const char S20_0108[] PROGMEM = "";						// 12
  const char S20_0109[] PROGMEM = "";
  const char S20_0110[] PROGMEM = "";						// 13
  const char S20_0111[] PROGMEM = "";
  const char S20_0112[] PROGMEM = "";						// 14
  const char S20_0113[] PROGMEM = "";
  const char S20_0114[] PROGMEM = "";						// 15
  const char S20_0115[] PROGMEM = "";
  const char S20_0116[] PROGMEM = "";						// 16
  const char S20_0117[] PROGMEM = "";
  const char S20_0118[] PROGMEM = "";						// 17
  const char S20_0119[] PROGMEM = "";
  const char S20_0120[] PROGMEM = "";						// 18
  const char S20_0121[] PROGMEM = "";
  const char S20_0122[] PROGMEM = "";						// 19
  const char S20_0123[] PROGMEM = "";
  const char S20_0124[] PROGMEM = "";						// 20
  const char S20_0125[] PROGMEM = "";

//-------------------------------------------------------------------------------------
// BEREICH  PARAMETER    P a r a m e t e r   P A R A M E T E R   WERTE, GRENZEN  SPRACHENUNABHÄNGIG
//   Teil der Parameter  Default = Werkseinstellung  FactoryParameterValues() 
//--------------------------------------------------------------------------------
#define _S21_OFFS 			_S20_OFFS + _S20_RECORDS * _S20_FIELDS  + _S20_RECORDS*_S20_FIELDS*_S20_LNGCOUNT
#define _S21_FIELDS 	  4 
#define _S21_RECORDS   21
#define _S21_LNGCOUNT   0   
// Felder
#define _S21_UNIT       0
#define _S21_MIN        1
#define _S21_MAX        2
#define _S21_DEF        3
  const char S21_0700[] PROGMEM = "unity:__________";		// 00
  const char S21_0701[] PROGMEM = "minimum:________";		// minimal erlaubter Wert für die Einstellung  	
  const char S21_0702[] PROGMEM = "maximum:________";		// maximal erlaubter Wert 
  const char S21_0703[] PROGMEM = "factoryset:_____";		// Standardwert / Fabrikseinstellung  Erkennung für 'wurde' gesetzt = 12345 

  const char S21_0704[] PROGMEM = "[0/1]";					// 01 Meldung an Serielle beim BOOTVORGANG
  const char S21_0705[] PROGMEM = "0";						  //    Minimaler Wert 
  const char S21_0706[] PROGMEM = "1";					    //    Maximaler Wert  
  const char S21_0707[] PROGMEM = "1";					    //    Standarad-Wert   

  const char S21_0708[] PROGMEM = "[Nr]";	  				// 02 Standard-SPRACHE  
  const char S21_0709[] PROGMEM = "0";              //    minimal Wert für Sprache
  const char S21_0710[] PROGMEM = "2";              //    Language Maximale Zahl zur Zeit 
  const char S21_0711[] PROGMEM = "1";              //    Standardsprache 1=de  0=en  2=fr  usw.

  const char S21_0712[] PROGMEM = "[0/1]";					// 03 Meldung an Serielle wenn Boot beendet  
  const char S21_0713[] PROGMEM = "0";
  const char S21_0714[] PROGMEM = "1";
  const char S21_0715[] PROGMEM = "1";

  const char S21_0716[] PROGMEM = "[1/2]";          // 04 LCD-Hintergrundbeleuchtung nach BOOT   
  const char S21_0717[] PROGMEM = "1";
  const char S21_0718[] PROGMEM = "2";
  const char S21_0719[] PROGMEM = "1";
  
  const char S21_0720[] PROGMEM = "[0/1]";          // 05 LCD-Menue - Rollover 
  const char S21_0721[] PROGMEM = "0";
  const char S21_0722[] PROGMEM = "1";
  const char S21_0723[] PROGMEM = "0";
  
  const char S21_0724[] PROGMEM = "[0/1]";          // 06 Meldungsausgabe mit Nummer
  const char S21_0725[] PROGMEM = "0";
  const char S21_0726[] PROGMEM = "1";
  const char S21_0727[] PROGMEM = "1";     //  
  
  const char S21_0728[] PROGMEM = "[0/1]";          // 07  Boot Details ausgeben an UART
  const char S21_0729[] PROGMEM = "0";
  const char S21_0730[] PROGMEM = "1";
  const char S21_0731[] PROGMEM = "0";
  
  const char S21_0732[] PROGMEM = "[g]";            // 08  Kalibrierungsgewicht
  const char S21_0733[] PROGMEM = "500";            // minimales Kalibrierungsgewicht 0,5 Kg
  const char S21_0734[] PROGMEM = "10000";          // maximales Kalibrierungsgewicht 10 Kg
  const char S21_0735[] PROGMEM = "2000";           // Default sind 2 Kg
  
  const char S21_0736[] PROGMEM = "[ ]";  // 09
  const char S21_0737[] PROGMEM = "min";
  const char S21_0738[] PROGMEM = "max";
  const char S21_0739[] PROGMEM = "def";
  const char S21_0740[] PROGMEM = "[ ]";  // 10
  const char S21_0741[] PROGMEM = "min";
  const char S21_0742[] PROGMEM = "max";
  const char S21_0743[] PROGMEM = "def";
  const char S21_0744[] PROGMEM = "[ ]";  // 11
  const char S21_0745[] PROGMEM = "min";
  const char S21_0746[] PROGMEM = "max";
  const char S21_0747[] PROGMEM = "def";
  const char S21_0748[] PROGMEM = "[ ]";  // 12
  const char S21_0749[] PROGMEM = "min";
  const char S21_0750[] PROGMEM = "max";
  const char S21_0751[] PROGMEM = "def";
  const char S21_0752[] PROGMEM = "[ ]";  // 13
  const char S21_0753[] PROGMEM = "min";
  const char S21_0754[] PROGMEM = "max";
  const char S21_0755[] PROGMEM = "def"; 
  const char S21_0756[] PROGMEM = "[ ]";  // 14
  const char S21_0757[] PROGMEM = "min";
  const char S21_0758[] PROGMEM = "max";
  const char S21_0759[] PROGMEM = "def"; 
  const char S21_0760[] PROGMEM = "[ ]";  // 15
  const char S21_0761[] PROGMEM = "min";
  const char S21_0762[] PROGMEM = "max";
  const char S21_0763[] PROGMEM = "def"; 
  const char S21_0764[] PROGMEM = "[ ]";  // 16
  const char S21_0765[] PROGMEM = "min";
  const char S21_0766[] PROGMEM = "max";
  const char S21_0767[] PROGMEM = "def"; 
  const char S21_0768[] PROGMEM = "[ ]";  // 17
  const char S21_0769[] PROGMEM = "min";
  const char S21_0770[] PROGMEM = "max";
  const char S21_0771[] PROGMEM = "def"; 
  const char S21_0772[] PROGMEM = "[ ]";  // 18
  const char S21_0773[] PROGMEM = "min";
  const char S21_0774[] PROGMEM = "max";
  const char S21_0775[] PROGMEM = "def"; 
  const char S21_0776[] PROGMEM = "[ ]";  // 19
  const char S21_0777[] PROGMEM = "min";
  const char S21_0778[] PROGMEM = "max";
  const char S21_0779[] PROGMEM = "def"; 
  const char S21_0780[] PROGMEM = "[ ]";  // 20
  const char S21_0781[] PROGMEM = "min";
  const char S21_0782[] PROGMEM = "max";
  const char S21_0783[] PROGMEM = "def"; 
  
//----------------------------------------------------------------
// BEREICH  ERROR - LOG  Sprachabhängig
//----------------------------------------------------------------
#define _S22_OFFS       _S21_OFFS + _S21_RECORDS * _S21_FIELDS  + _S21_RECORDS*_S21_FIELDS*_S21_LNGCOUNT   
#define _S22_FIELDS     4
#define _S22_RECORDS    2
#define _S22_LNGCOUNT   1
  const char S22_0000[] PROGMEM = "Error-Name";                   // 00 Kurzname
  const char S22_0001[] PROGMEM = "Error-Description";            // Beschreibung
  const char S22_0002[] PROGMEM = "LCD-Text when Error arises";   // "" for no Info to LCD
  const char S22_0003[] PROGMEM = "UART-Text when Error arises";  // "" for no Info to UART

  const char S22_0004[] PROGMEM = "Compr Temp";                   // 01
  const char S22_0005[] PROGMEM = "Compressor temperatur to high";  
  const char S22_0006[] PROGMEM = "Compr.Temp OVERHEAT";          
  const char S22_0007[] PROGMEM = "Err: 01 Compressor Temperature to High";  
// nächste Sprache 
  const char S22_0008[] PROGMEM = "Fehler-Name";                  // 00 Kurzname
  const char S22_0009[] PROGMEM = "Fehler-Beschreibung";          // Beschreibung
  const char S22_0010[] PROGMEM = "LCD-Text wenn der Fehler auftritt";   // "" for no Info to LCD
  const char S22_0011[] PROGMEM = "UART-Text wenn Fehler auftritt";  // "" for no Info to UART

  const char S22_0012[] PROGMEM = "Kompr.Temp";                   // 01
  const char S22_0013[] PROGMEM = "Kompressor überhitzt";  
  const char S22_0014[] PROGMEM = "Kompressor zu heiss";          
  const char S22_0015[] PROGMEM = "Err: 01 Kompressor Temperatur zu hoch siehe Parameter xx für ev. Einstellungskorrekturen";    
//----------------------------------------------------------------
// BEREICH  ERROR - LOG  DATEN  Sprachunabhängig
//----------------------------------------------------------------
#define _S23_OFFS       _S22_OFFS + _S22_RECORDS * _S22_FIELDS  + _S22_RECORDS*_S22_FIELDS*_S22_LNGCOUNT   
#define _S23_FIELDS     1 
#define _S23_CATEGORY   0 // Feldnummer der Fehlerkategorie
#define _S23_RECORDS    _S22_RECORDS
#define _S23_LNGCOUNT   0   // zusätzliche Sprachen   
  const char S23_0000[] PROGMEM = "Error-Category";               // 00 Fehler Kategorie als Zahl (wozu die auch immer verwendet wird)
  const char S23_0001[] PROGMEM = "3";                            // 01 Wenn der Kompressor zu heiß wird bedeutet das die Kategorie 3 (vielleicht für Gefahr stehend)

//----------------------------------------------------------------
// BEREICH   
//----------------------------------------------------------------
#define _S24_OFFS       _S23_OFFS + _S23_RECORDS * _S23_FIELDS  + _S23_RECORDS*_S23_FIELDS*_S23_LNGCOUNT   
#define _S24_FIELDS     0 
#define _S24_RECORDS    0
#define _S24_LNGCOUNT   0   // zusätzliche Sprachen   
//----------------------------------------------------------------
// BEREICH   
//----------------------------------------------------------------
#define _S25_OFFS       _S24_OFFS + _S24_RECORDS * _S24_FIELDS  + _S24_RECORDS*_S24_FIELDS*_S24_LNGCOUNT   
#define _S25_FIELDS     0 
#define _S25_RECORDS    0
#define _S25_LNGCOUNT   0   // zusätzliche Sprachen   
//----------------------------------------------------------------
// BEREICH   
//----------------------------------------------------------------
#define _S26_OFFS       _S25_OFFS + _S25_RECORDS * _S25_FIELDS  + _S25_RECORDS*_S25_FIELDS*_S25_LNGCOUNT   
#define _S26_FIELDS     0 
#define _S26_RECORDS    0
#define _S26_LNGCOUNT   0   // zusätzliche Sprachen  
//----------------------------------------------------------------
// BEREICH   
//----------------------------------------------------------------
#define _S27_OFFS       _S26_OFFS + _S26_RECORDS * _S26_FIELDS  + _S26_RECORDS*_S26_FIELDS*_S26_LNGCOUNT   
#define _S27_FIELDS     0 
#define _S27_RECORDS    0
#define _S27_LNGCOUNT   0   // zusätzliche Sprachen  

//----------------------------------------------------------------
// BEREICH   Eingänge / SCHALTER  SPRACHABHÄNGIGER TEIL
//----------------------------------------------------------------
#define _S28_OFFS       _S27_OFFS + _S27_RECORDS * _S27_FIELDS  + _S27_RECORDS*_S27_FIELDS*_S27_LNGCOUNT   
#define _S28_FIELDS     2 
#define _S28_RECORDS    9
#define _S28_LNGCOUNT   0   // zusätzliche Sprachen  
  const char S28_0000[] PROGMEM = "Input/Switch Name";            // 00 Name 
  const char S28_0001[] PROGMEM = "Input/Switch Description";     //    Beschreibung 
  
  const char S28_0002[] PROGMEM = "Push Button 1";                // 01 
  const char S28_0003[] PROGMEM = "switches machine 1 on/off";
  
  const char S28_0004[] PROGMEM = "Push Button 2";                // 02 
  const char S28_0005[] PROGMEM = "switches machine 2 on";

  const char S28_0006[] PROGMEM = "Push Button 3";                // 03 
  const char S28_0007[] PROGMEM = "switches machine 3 on";

  const char S28_0008[] PROGMEM = "Push Button 4";                // 04 
  const char S28_0009[] PROGMEM = "switches machine 4 on";

  const char S28_0010[] PROGMEM = "Push Button 5";                // 05 
  const char S28_0011[] PROGMEM = "switches machine 5 on";

  const char S28_0012[] PROGMEM = "Push Button 6";                // 06 
  const char S28_0013[] PROGMEM = "switches machine 6 on";

  const char S28_0014[] PROGMEM = "Push Button 7";                // 07 
  const char S28_0015[] PROGMEM = "switches machine 7 on";

  const char S28_0016[] PROGMEM = "Push Button 8";                // 08 
  const char S28_0017[] PROGMEM = "switches machine 8 on";

//-----------------------------------------------------
// Eingänge / Schalter DATEN
//-----------------------------------------------------
#define _S29_OFFS 			_S28_OFFS + _S28_RECORDS * _S28_FIELDS  + _S28_RECORDS*_S28_FIELDS*_S28_LNGCOUNT   
#define _S29_FIELDS 	  7
#define _S29_RECORDS    _S28_RECORDS
#define _S29_LNGCOUNT   0   // zusätzliche Sprachen  
#define _S29_PIN_ADR_BIT 0  // PIN I2C-ADR I2C-BIT
#define _S29_AT_PWRON_DO 1  // 0=ignore  1=INPUT_PULLUP  2=INPUT 
#define _S29_AREF_DIG    2  // 0=AREF Ext  1=Default   2=1.1V   3=2.56V  9=BINÄR 0/1
#define _S29_OWP_IS      3  // 0=Ground  1=High   2...1023
#define _S29_CALC        4  // Calc Formel
#define _S29_RES1        5  // reserviert für Neues
#define _S29_EH          6  // Einheit [mV]...
  const char S29_0000[] PROGMEM = "Pin[0-60],I2C-Address[dec],Bit[0-7]";        // 00     Arduino-PIN  .  I2C-Addr[decimal]  . PCF-Bit[1-8]  Bsp: 0,56,2  > bedeutet PCF-Modul mit Adresse 56 Bit 2
  const char S29_0001[] PROGMEM = "at PWR-ON 0=ignore 1=INPUT_PULLUP 2=INPUT";  //        Beim Einschalten der Anlage ist dieser Eingang in den hier angegebenen Zustand zu stellen. 
  const char S29_0002[] PROGMEM = "AREF 1=Default 2=1.1[V] 3=2.56[V] 0=AREF Ext 9=DIGITAL"; 
  const char S29_0003[] PROGMEM = "ON when PIN is 0=Ground  1=High 2..1023";    //        Welchen Zustand muß der PIN einnehmen damit der Eingang als EIN erkannt wird.
  const char S29_0004[] PROGMEM = "Calc Formula";                               //        reserviert für Formel
  const char S29_0005[] PROGMEM = "res";                                        //        reserviert
  const char S29_0006[] PROGMEM = "Einheit Bsp:[mV]";                           //        Einheit für Anzeigezwecke
                    
  const char S29_0007[] PROGMEM = "n 57 0";                 // 01   n, ein Buchstabe bzw. etwas nicht nummerisches bedeutet daß kein arduino-PIN gemeint ist.
  const char S29_0008[] PROGMEM = "1";                      //      INPUT_PULLUP nach Reset
  const char S29_0009[] PROGMEM = "9";                      //      Ist ein DIGITAL-Pin 
  const char S29_0010[] PROGMEM = "0";                      //      ist EIN wenn Wert LOW
  const char S29_0011[] PROGMEM = "1";                      //      Formel
  const char S29_0012[] PROGMEM = "";
  const char S29_0013[] PROGMEM = "[.]";                    //      Einheit
                    
  const char S29_0014[] PROGMEM = "n 57 1";                 // 02   n, ein Buchstabe bzw. etwas nicht nummerisches bedeutet daß kein arduino-PIN gemeint ist.
  const char S29_0015[] PROGMEM = "1";                      //      INPUT_PULLUP nach Reset
  const char S29_0016[] PROGMEM = "9";                      //      Ist ein DIGITAL-Pin 
  const char S29_0017[] PROGMEM = "0";                      //      ist EIN wenn Wert LOW
  const char S29_0018[] PROGMEM = "1";                      //      Formel
  const char S29_0019[] PROGMEM = "";
  const char S29_0020[] PROGMEM = "[.]";                    //      Einheit

  const char S29_0021[] PROGMEM = "n 57 2";                 // 03   n, ein Buchstabe bzw. etwas nicht nummerisches bedeutet daß kein arduino-PIN gemeint ist.
  const char S29_0022[] PROGMEM = "1";                      //      INPUT_PULLUP nach Reset
  const char S29_0023[] PROGMEM = "9";                      //      Ist ein DIGITAL-Pin 
  const char S29_0024[] PROGMEM = "0";                      //      ist EIN wenn Wert LOW
  const char S29_0025[] PROGMEM = "1";                      //      Formel
  const char S29_0026[] PROGMEM = "";
  const char S29_0027[] PROGMEM = "[.]";                    //      Einheit

  const char S29_0028[] PROGMEM = "n 57 3";                 // 04   n, ein Buchstabe bzw. etwas nicht nummerisches bedeutet daß kein arduino-PIN gemeint ist.
  const char S29_0029[] PROGMEM = "1";                      //      INPUT_PULLUP nach Reset
  const char S29_0030[] PROGMEM = "9";                      //      Ist ein DIGITAL-Pin 
  const char S29_0031[] PROGMEM = "0";                      //      ist EIN wenn Wert LOW
  const char S29_0032[] PROGMEM = "1";                      //      Formel
  const char S29_0033[] PROGMEM = "";
  const char S29_0034[] PROGMEM = "[.]";                    //      Einheit

  const char S29_0035[] PROGMEM = "n 57 4";                 // 05   n, ein Buchstabe bzw. etwas nicht nummerisches bedeutet daß kein arduino-PIN gemeint ist.
  const char S29_0036[] PROGMEM = "1";                      //      INPUT_PULLUP nach Reset
  const char S29_0037[] PROGMEM = "9";                      //      Ist ein DIGITAL-Pin 
  const char S29_0038[] PROGMEM = "0";                      //      ist EIN wenn Wert LOW
  const char S29_0039[] PROGMEM = "1";                      //      Formel
  const char S29_0040[] PROGMEM = "";
  const char S29_0041[] PROGMEM = "[.]";                    //      Einheit

  const char S29_0042[] PROGMEM = "n 57 5";                 // 06   n, ein Buchstabe bzw. etwas nicht nummerisches bedeutet daß kein arduino-PIN gemeint ist.
  const char S29_0043[] PROGMEM = "1";                      //      INPUT_PULLUP nach Reset
  const char S29_0044[] PROGMEM = "9";                      //      Ist ein DIGITAL-Pin 
  const char S29_0045[] PROGMEM = "0";                      //      ist EIN wenn Wert LOW
  const char S29_0046[] PROGMEM = "1";                      //      Formel
  const char S29_0047[] PROGMEM = "";
  const char S29_0048[] PROGMEM = "[.]";                    //      Einheit

  const char S29_0049[] PROGMEM = "n 57 6";                 // 07   n, ein Buchstabe bzw. etwas nicht nummerisches bedeutet daß kein arduino-PIN gemeint ist.
  const char S29_0050[] PROGMEM = "1";                      //      INPUT_PULLUP nach Reset
  const char S29_0051[] PROGMEM = "9";                      //      Ist ein DIGITAL-Pin 
  const char S29_0052[] PROGMEM = "0";                      //      ist EIN wenn Wert LOW
  const char S29_0053[] PROGMEM = "1";                      //      Formel
  const char S29_0054[] PROGMEM = "";
  const char S29_0055[] PROGMEM = "[.]";                    //      Einheit

  const char S29_0056[] PROGMEM = "n 57 7";                 // 08   n, ein Buchstabe bzw. etwas nicht nummerisches bedeutet daß kein arduino-PIN gemeint ist.
  const char S29_0057[] PROGMEM = "1";                      //      INPUT_PULLUP nach Reset
  const char S29_0058[] PROGMEM = "9";                      //      Ist ein DIGITAL-Pin 
  const char S29_0059[] PROGMEM = "0";                      //      ist EIN wenn Wert LOW
  const char S29_0060[] PROGMEM = "1";                      //      Formel
  const char S29_0061[] PROGMEM = "";
  const char S29_0062[] PROGMEM = "[.]";                    //      Einheit

  

//--------------------------------------------------------------------------------
// BEREICH  GERÄTE   NAMEN Beschreibungen  Sprachabhängiger Teil 
//    SPRACHABHÄNGIGER TEIL 
//--------------------------------------------------------------------------------
#define _MACHINE_COUNT  _S30_RECORDS 
#define _S30_OFFS 			_S29_OFFS + _S29_RECORDS * _S29_FIELDS  + _S29_RECORDS*_S29_FIELDS*_S29_LNGCOUNT   
#define _S30_FIELDS 	  2 
#define _S30_RECORDS    8
#define _S30_LNGCOUNT   1
// Feldnamen / FeldPositionen
#define _S30_NAME       0   // Maschinen-Name
#define _S30_DESCR      1   // Berschreibung der Maschine 
 // ENGLISCHE Geräte Benennungen Beschreibungen 
  const char S30_0000[] PROGMEM = "Machine_Name________:";			// 00 Name 
  const char S30_0001[] PROGMEM = "Machine_Description_:";  		//    Beschreibung 
  const char S30_0002[] PROGMEM = "ONBOARD LED";							  // 01
  const char S30_0003[] PROGMEM = "LED fix mounted on the arduino mega"; 
  const char S30_0004[] PROGMEM = "swivel motor 1 R";						// 02
  const char S30_0005[] PROGMEM = "swivel Motor to rotage direction RIGHT";   
  const char S30_0006[] PROGMEM = "Machine 03";									// 03
  const char S30_0007[] PROGMEM = "description for Machine 03";   
  const char S30_0008[] PROGMEM = "Machine 04";									// 04
  const char S30_0009[] PROGMEM = "description for Machine 04";   
  const char S30_0010[] PROGMEM = "Machine 05";									// 05
  const char S30_0011[] PROGMEM = "description for Machine 05";   
  const char S30_0012[] PROGMEM = "Machine 06";									// 06
  const char S30_0013[] PROGMEM = "description for Machine 06";   
  const char S30_0014[] PROGMEM = "Machine 07";									// 07
  const char S30_0015[] PROGMEM = "description for Machine 07";     
 // DEUTSCHE Geräte Benennung 
  const char S30_0016[] PROGMEM = "Maschine_Name________:";			// 00 Name 
  const char S30_0017[] PROGMEM = "Maschine_Beschreibung:"; 		//    Beschreibung 
  const char S30_0018[] PROGMEM = "FIXE arduino LED";						// 01
  const char S30_0019[] PROGMEM = "fest auf dem arduino Board gelötete LED"; 
  const char S30_0020[] PROGMEM = "Schwenkmotor 1 L";								// 02
  const char S30_0021[] PROGMEM = "Beschreibung für Maschine 02";   
  const char S30_0022[] PROGMEM = "Schwenkmotor 1 R";									// 03
  const char S30_0023[] PROGMEM = "Beschreibung für Maschine 03";   
  const char S30_0024[] PROGMEM = "Schwenkmotor 2 L";									// 04
  const char S30_0025[] PROGMEM = "Beschreibung für Maschine 04";   
  const char S30_0026[] PROGMEM = "Schwenkmotor 2 R";									// 05
  const char S30_0027[] PROGMEM = "Beschreibung für Maschine 05";   
  
  const char S30_0028[] PROGMEM = "Haltemagnet 1";									// 06
  const char S30_0029[] PROGMEM = "Beschreibung für Maschine 06";   
  
  const char S30_0030[] PROGMEM = "Wasserpumpe 07";									// 07
  const char S30_0031[] PROGMEM = "Beschreibung für Maschine 07";     
//-------------------------------------------------------------------------------------
// BEREICH  GERÄTE   technisches  Sprachunabhängig 
// Initialize_Machines(byte FromMachNr, byte ToMachNr) 
//--------------------------------------------------------------------------------
#define _S31_OFFS 			_S30_OFFS + _S30_RECORDS * _S30_FIELDS  + _S30_RECORDS*_S30_FIELDS*_S30_LNGCOUNT   
#define _S31_FIELDS   	3 
#define _S31_RECORDS    _S30_RECORDS
#define _S31_LNGCOUNT   0
// So werden quasi Feldnamen zu einer Tabelle definiert 
#define _S31_PIN_I2CADDR_BIT 0    // PIN I2C-Adr I2C-BIT 
#define _S31_AT_PWRON_DO     1    // 0=nichts  1=OFF  2=ON  3...255=PWM  
#define _S31_OWP_IS          2
  
  const char S31_0000[] PROGMEM = "Pin[0-60],I2C-Address[dec],Bit[0-7]_____:";	// 00   	Arduino-PIN  .  I2C-Addr[decimal]  . PCF-Bit[1-8]  Bsp: 0,56,2  > bedeutet PCF-Modul mit Adresse 56 Bit 2
  const char S31_0001[] PROGMEM = "at PWR-ON 0=NC 1=off 2=ON 3..255=PWM____:"; //          Beim Einschalten der Anlage ist dieses Gerät in den hier angegebenen Zustand zu stellen. 
  const char S31_0002[] PROGMEM = "ON when PIN is  0=Ground  1=High________:"; //          Welchen Zustand muß der PIN einnehmen damit das Gerät EINGESCHALTET ist.

  const char S31_0003[] PROGMEM = "13";									    // 01		n, ein Buchstabe bzw. etwas nicht nummerisches bedeutet daß kein arduino-PIN gemeint ist.
  const char S31_0004[] PROGMEM = "1"; 										  //          OFF nach BOOT / RESET / PWR-ON 
  const char S31_0005[] PROGMEM = "1";   										//          ist AN wenn Pin HIGH ist 

  const char S31_0006[] PROGMEM = "n 56 0";									// 02       56 ist die Adresse vom PCF-Modul  1 ist der Pin am PCF-Modul  Sch
  const char S31_0007[] PROGMEM = "1";    // ON nach Reset
  const char S31_0008[] PROGMEM = "0";    // ON when PIN is LOW
  
  const char S31_0009[] PROGMEM = "n 56 1";									// 03        
  const char S31_0010[] PROGMEM = "1";    // OFF nach Reset
  const char S31_0011[] PROGMEM = "0";    // LOW = ON 
  
  const char S31_0012[] PROGMEM = "n 56 2";									// 04        
  const char S31_0013[] PROGMEM = "1";   
  const char S31_0014[] PROGMEM = "0";   
  
  const char S31_0015[] PROGMEM = "n 56 3";									// 05        
  const char S31_0016[] PROGMEM = "1";   
  const char S31_0017[] PROGMEM = "0";   
  
  const char S31_0018[] PROGMEM = "2 ";									// 06        Pin 2 am arduino direkt  Haltemagnet über Mosfet Modul
  const char S31_0019[] PROGMEM = "1";   
  const char S31_0020[] PROGMEM = "0";   
  
  const char S31_0021[] PROGMEM = "  6 ";									// 07        Pin 6 am arduino direkt  Wasserpumpe  
  const char S31_0022[] PROGMEM = "1";    // PWM nach Reset            
  const char S31_0023[] PROGMEM = "0";   
  
//-------------------------------------------------------------------------------------------
// BEREICH  KOMBINATIONEN = Gerätekombinationen  NAMEN Beschreibungen  Sprachabhängiger Teil 
//    SPRACHABHÄNGIGER TEIL 
//-------------------------------------------------------------------------------------------
#define _KOMBINATION_COUNT _S32_RECORDS
#define _S32_OFFS 			_S31_OFFS + _S31_RECORDS * _S31_FIELDS  + _S31_RECORDS*_S31_FIELDS*_S31_LNGCOUNT   
#define _S32_FIELDS 	  2 
#define _S32_RECORDS    8
#define _S32_LNGCOUNT   1   // zusätzliche Sprachen 
 // ENGLISCHE Kombinationen Benennungen Beschreibungen 
  const char S32_0000[] PROGMEM = "Combination Name________:";		// 00 Name 
  const char S32_0001[] PROGMEM = "Combination Description_:";  	//    Beschreibung 
  const char S32_0002[] PROGMEM = " " ;						// 01  
  const char S32_0003[] PROGMEM = " ";				//     
  const char S32_0004[] PROGMEM = "";						 						      // 02
  const char S32_0005[] PROGMEM = "";   
  const char S32_0006[] PROGMEM = "";												      // 03
  const char S32_0007[] PROGMEM = "";   
  const char S32_0008[] PROGMEM = "";												      // 04
  const char S32_0009[] PROGMEM = "";   
  const char S32_0010[] PROGMEM = "";												      // 05
  const char S32_0011[] PROGMEM = "";   
  const char S32_0012[] PROGMEM = "";												      // 06
  const char S32_0013[] PROGMEM = "";   
  const char S32_0014[] PROGMEM = "";												      // 07
  const char S32_0015[] PROGMEM = "";     
 // DEUTSCHE Geräte Benennung 
  const char S32_0016[] PROGMEM = "Kombi Name________:";		 		  // 00 Name 
  const char S32_0017[] PROGMEM = "Kombi Beschreibung:";  			 	//    Beschreibung 
  const char S32_0018[] PROGMEM = "Power-LED Test ";			 		// 01
  const char S32_0019[] PROGMEM = "Testdaten mit Power-LED"; 
  const char S32_0020[] PROGMEM = "Schwenkmotor 1 u 2 Richtung A";												      // 02
  const char S32_0021[] PROGMEM = "";   
  const char S32_0022[] PROGMEM = "Schwenkmotor 1 u 2 Richtung B";												      // 03
  const char S32_0023[] PROGMEM = "";   
  const char S32_0024[] PROGMEM = "Schwenkmotor 1 u 2 STOP";												      // 04
  const char S32_0025[] PROGMEM = "";   
  const char S32_0026[] PROGMEM = "Testsequenz ";												      // 05
  const char S32_0027[] PROGMEM = "";   
  
  const char S32_0028[] PROGMEM = "";												      // 06
  const char S32_0029[] PROGMEM = "";   
  const char S32_0030[] PROGMEM = "";												      // 07
  const char S32_0031[] PROGMEM = "";     
//-------------------------------------------------------------------------------------
// BEREICH  Kombinationen   technisches  Sprachunabhängig 
// Initialize_Combinations(byte FromCombNr, byte ToCombNr) 
//-------------------------------------------------------------------------------------
#define _S33_OFFS 			_S32_OFFS + _S32_RECORDS * _S32_FIELDS  + _S32_RECORDS*_S32_FIELDS*_S32_LNGCOUNT   
#define _S33_FIELDS 	  3 
#define _S33_RECORDS    _S32_RECORDS
#define _S33_LNGCOUNT   0   

#define _S33_AT_PWRON_DO  0 
#define _S33_ON_DO        1
#define _S33_OFF_DO       2

  const char S33_0000[] PROGMEM = "machineNr Value, machineNr Value,_______:";  // 00  Auflistung für 'Kombination INITIALISIEREN'
  const char S33_0001[] PROGMEM = "machineNr Value, machineNr Value,_______:";  //     Auflistung von GeräteNr Wert, GeräteNr Wert,.... für  'Kombination EINSCHALTEN'
  const char S33_0002[] PROGMEM = "g1 1 g2 1                       ________:";  //     Auflistung für 'Kombination AUSSCHALTEN'
  
  const char S33_0003[] PROGMEM = "g1  1, g1 4, g1 10";         // 01   
  const char S33_0004[] PROGMEM = "g1 2";                       //     
  const char S33_0005[] PROGMEM = "g1 1";                       //     
  
  const char S33_0006[] PROGMEM = "g2 1  g3 1";                 // 02 INIT   Gerät 2 AUS, Gerät 3 AUS  = STOP                 k2 3    INIT
  const char S33_0007[] PROGMEM = "g2 2, g3 1";                 //           Gerät 2 EIN, Gerät 3 AUS  = Lauf Richtung A      k2 2    ON
  const char S33_0008[] PROGMEM = "g2 1, g3 1";                 //           Kombi ausschalten = wie INIT                     k2 1    OFF
  
  const char S33_0009[] PROGMEM = "g4 1, g5 1";                 // 03 INIT   k3 3  INIT
  const char S33_0010[] PROGMEM = "g4 2, g5 1";                 //           k3 2  ON
  const char S33_0011[] PROGMEM = "g4 1, g5 1";                 //           k3 1  OFF
  
  const char S33_0012[] PROGMEM = "g1 5 ";           // 04        OnboardLED PWM 5, Haltemagnet PWM 5, Pumpe PWM 4   k4 3
  const char S33_0013[] PROGMEM = "g1 2, g6 1, g7 2";           //           LED ON, Haltemagnet OFF, Pumpe Vollgas             k4 2
  const char S33_0014[] PROGMEM = "g1 1, g6 1, g7 1";           //           LED OFF, Manget OFF, Pumpe OFF                     k4 1 
  
  const char S33_0015[] PROGMEM = "g1 4,    ";   // 05      Alle Geräte ansprechen     k5 3
  const char S33_0016[] PROGMEM = "g1 2,   g2 1,g3 2,   g4 1,g5 2,  g6 30,  g7 2   ";   //         Alle Geräte beinahe VOLL   k5 2
  const char S33_0017[] PROGMEM = "g1 1,   g2 1,g3 1,   g4 1,g5 1,  g6 1,   g7 1 ";     //         Alle Geräte AUS            k5 1
  
  const char S33_0018[] PROGMEM = "g1 p1, ";                       // 06        k6 3 
  const char S33_0019[] PROGMEM = "g7 +10    7 -3 7 +5   7 -1   7 -2  7 +  1 ";                                          //           k6 2
  const char S33_0020[] PROGMEM = "7 +5 7 + 7   7 -2 g7 +12 6 5  g6 + 5  g6  -  10 "; //           k6 1
  
  const char S33_0021[] PROGMEM = "1 4  ";                  // 07        
  const char S33_0022[] PROGMEM = "1 1 2 1 3 1 4 1 5 1 6 1  7 1 ";   
  const char S33_0023[] PROGMEM = "1   1    2  1   3   1   4   1   5  1,6 1  , 7 1 ";   
 
//-------------------------------------------------------------------------------------------
// SPEZIAL-EINGÄNGE für automatische Behandlung langsamer Vorgänge !!!!!
//   NAMEN Beschreibungen  Sprachabhängiger Teil 
//    SPRACHABHÄNGIGER TEIL  die direkt an einem PIN hängen und ohne spezielle Funktion auswertbar sind.
//-------------------------------------------------------------------------------------------
#define _S34_OFFS 			_S33_OFFS + _S33_RECORDS * _S33_FIELDS  + _S33_RECORDS*_S33_FIELDS*_S33_LNGCOUNT   
#define _S34_FIELDS     2 
#define _S34_RECORDS    8
#define _S34_LNGCOUNT   1   // zusätzliche Sprachen 
 // ENGLISCHE Eingänge Benennungen Beschreibungen 
  const char S34_0000[] PROGMEM = "Switch Name________:";						// 00 Name 
  const char S34_0001[] PROGMEM = "Switch Description_:";  					//    Beschreibung 
  const char S34_0002[] PROGMEM = "gas preasure analog";						// 01 gas druck sensor Analogwert 
  const char S34_0003[] PROGMEM = "gas preasure has a analog Value to arduino PIN ";						//
  const char S34_0004[] PROGMEM = "endswitch 1";			 						  // 02
  const char S34_0005[] PROGMEM = "switch at the top of machine";
  const char S34_0006[] PROGMEM = "";												// 03
  const char S34_0007[] PROGMEM = "";   
  const char S34_0008[] PROGMEM = "";												// 04
  const char S34_0009[] PROGMEM = "";   
  const char S34_0010[] PROGMEM = "";												// 05
  const char S34_0011[] PROGMEM = "";   
  const char S34_0012[] PROGMEM = "";												// 06
  const char S34_0013[] PROGMEM = "";   
  const char S34_0014[] PROGMEM = "";												// 07
  const char S34_0015[] PROGMEM = "";     
 // DEUTSCHE Geräte Benennung 
  const char S34_0016[] PROGMEM = "Schalter Name________:";						// 00 Name 
  const char S34_0017[] PROGMEM = "Schalter Beschreibung"; 						//    Beschreibung 
  const char S34_0018[] PROGMEM = "Gas Druck Analogwert";							// 01
  const char S34_0019[] PROGMEM = "Gas Drucksensor liefert analogen Wert"; 
  const char S34_0020[] PROGMEM = "Endschalter 1";									  // 02
  const char S34_0021[] PROGMEM = "Endschalter bei dem Deckel der Maschine";   
  const char S34_0022[] PROGMEM = "";												// 03
  const char S34_0023[] PROGMEM = "";   
  const char S34_0024[] PROGMEM = "";												// 04
  const char S34_0025[] PROGMEM = "";   
  const char S34_0026[] PROGMEM = "";												// 05
  const char S34_0027[] PROGMEM = "";   
  const char S34_0028[] PROGMEM = "";												// 06
  const char S34_0029[] PROGMEM = "";   
  const char S34_0030[] PROGMEM = "";												// 07
  const char S34_0031[] PROGMEM = "";     

//--------------------------------------------------------------------------------
// SPEZIAL-EINGÄNGE für automatische Behandlung langsamer Vorgänge !!!!
//    technisches  Sprachunabhängig 
// Initialize_SpezialInputs(byte FromNr, byte ToNr) 
//--------------------------------------------------------------------------------
#define _S35_OFFS 			_S34_OFFS + _S34_RECORDS * _S34_FIELDS  + _S34_RECORDS*_S34_FIELDS*_S34_LNGCOUNT   
#define _S35_FIELDS 	10 
#define _S35_RECORDS    8
#define _S35_LNGCOUNT   0   // zusätzliche Sprachen 
  const char S35_0000[] PROGMEM = "Pin[0-60],I2C-Address[dec],Bit[0-7]________:";	// 00  
  const char S35_0001[] PROGMEM = "PULLUP 0=NO  1=YES_________________________:"; 	//     
  const char S35_0002[] PROGMEM = "0=AREF Ext. 1=def 2=1.1[V] 3=2.56 9=DIGITAL:"; 	//     
  const char S35_0003[] PROGMEM = "ON when PIN is  0=Ground  1=High___________:";	// 
  const char S35_0004[] PROGMEM = "OFF..ON -VALUES 0  32000___________________:"; 	// Wert ab dem der Sensor als LOGISCH 0 zu sehen ist und wann als LOGISCH 1
  const char S35_0005[] PROGMEM = "1000[ms]_____";  // alle wieviel [ms] ist dieser Eingang zu prüfen/messen
  const char S35_0006[] PROGMEM = "g 1 2 oder k 1 2 oder m 1 0____"; // Gerät/Kombi/Macro    wenn PIN ist ON 
  const char S35_0007[] PROGMEM = "g 1 1 oder k 1 1 oder m 1 stop "; // Gerät/Kombi/Macro    wenn PIN ist OFF  
  const char S35_0008[] PROGMEM = "";
  const char S35_0009[] PROGMEM = "";
  
  const char S35_0010[] PROGMEM = ""; // 02
  const char S35_0011[] PROGMEM = ""; 
  const char S35_0012[] PROGMEM = "";
  const char S35_0013[] PROGMEM = ""; 
  const char S35_0014[] PROGMEM = ""; 
  const char S35_0015[] PROGMEM = "";
  const char S35_0016[] PROGMEM = ""; 
  const char S35_0017[] PROGMEM = ""; 
  const char S35_0018[] PROGMEM = "";
  const char S35_0019[] PROGMEM = ""; 
  const char S35_0020[] PROGMEM = ""; // 03
  const char S35_0021[] PROGMEM = ""; 
  const char S35_0022[] PROGMEM = "";
  const char S35_0023[] PROGMEM = ""; 
  const char S35_0024[] PROGMEM = ""; 
  const char S35_0025[] PROGMEM = "";
  const char S35_0026[] PROGMEM = ""; 
  const char S35_0027[] PROGMEM = ""; 
  const char S35_0028[] PROGMEM = "";
  const char S35_0029[] PROGMEM = ""; 
  const char S35_0030[] PROGMEM = ""; // 04
  const char S35_0031[] PROGMEM = ""; 
  const char S35_0032[] PROGMEM = "";
  const char S35_0033[] PROGMEM = ""; 
  const char S35_0034[] PROGMEM = ""; 
  const char S35_0035[] PROGMEM = "";
  const char S35_0036[] PROGMEM = ""; 
  const char S35_0037[] PROGMEM = ""; 
  const char S35_0038[] PROGMEM = "";
  const char S35_0039[] PROGMEM = ""; 
  const char S35_0040[] PROGMEM = ""; // 05
  const char S35_0041[] PROGMEM = ""; 
  const char S35_0042[] PROGMEM = "";
  const char S35_0043[] PROGMEM = ""; 
  const char S35_0044[] PROGMEM = ""; 
  const char S35_0045[] PROGMEM = "";
  const char S35_0046[] PROGMEM = ""; 
  const char S35_0047[] PROGMEM = ""; 
  const char S35_0048[] PROGMEM = "";
  const char S35_0049[] PROGMEM = ""; 
  const char S35_0050[] PROGMEM = ""; // 06
  const char S35_0051[] PROGMEM = ""; 
  const char S35_0052[] PROGMEM = "";
  const char S35_0053[] PROGMEM = ""; 
  const char S35_0054[] PROGMEM = ""; 
  const char S35_0055[] PROGMEM = "";
  const char S35_0056[] PROGMEM = ""; 
  const char S35_0057[] PROGMEM = ""; 
  const char S35_0058[] PROGMEM = "";
  const char S35_0059[] PROGMEM = ""; 
  const char S35_0060[] PROGMEM = ""; // 07
  const char S35_0061[] PROGMEM = ""; 
  const char S35_0062[] PROGMEM = "";
  const char S35_0063[] PROGMEM = ""; 
  const char S35_0064[] PROGMEM = ""; 
  const char S35_0065[] PROGMEM = "";
  const char S35_0066[] PROGMEM = ""; 
  const char S35_0067[] PROGMEM = ""; 
  const char S35_0068[] PROGMEM = "";
  const char S35_0069[] PROGMEM = ""; 
  const char S35_0070[] PROGMEM = ""; // 08
  const char S35_0071[] PROGMEM = ""; 
  const char S35_0072[] PROGMEM = "";
  const char S35_0073[] PROGMEM = ""; 
  const char S35_0074[] PROGMEM = ""; 
  const char S35_0075[] PROGMEM = "";
  const char S35_0076[] PROGMEM = ""; 
  const char S35_0077[] PROGMEM = ""; 
  const char S35_0078[] PROGMEM = "";
  const char S35_0079[] PROGMEM = ""; 
//---------------------------------------------------------
// BEREICH  Macros  KOPF Sprachabhängig 
//---------------------------------------------------------
#define _MACRO_COUNT _S40_RECORDS   // Anzahl Macros damit wird ein Struct-Array richtig dimensioniert
#define _S40_OFFS 			_S35_OFFS + _S35_RECORDS * _S35_FIELDS  + _S35_RECORDS*_S35_FIELDS*_S35_LNGCOUNT   
#define _S40_FIELDS 	7 
#define _S40_RECORDS    3
#define _S40_LNGCOUNT   0   
 
  const char S40_0000[] PROGMEM = "Macro-Name_________________:";	      // Name des Macros  00
  const char S40_0001[] PROGMEM = "UART Message when started__:";  	    // Meldung an die UART sobald das macro gestartet ist.  
  const char S40_0002[] PROGMEM = "UART Message when done_____:";  	    // Meldung an die UART wenn erledigt. 
  const char S40_0003[] PROGMEM = "LCD Message when started___:";  	    // Meldung auf LCD sobald das macro gestartet ist.  
  const char S40_0004[] PROGMEM = "LCD Message when done______:";  	    // Meldung auf LCD wenn erledigt.
  const char S40_0005[] PROGMEM = "description________________:";  	    // ausführliche Beschreibung des Macros als Langtext.
  const char S40_0006[] PROGMEM = "reserve____________________:";       // frei 
  
  const char S40_0007[] PROGMEM = "start machine";					            // Name des Macros  01
  const char S40_0008[] PROGMEM = "machine is starting now";		        // Meldung an die UART sobald das macro gestartet ist.  
  const char S40_0009[] PROGMEM = "machine start macro done";		        // Meldung an die UART wenn erledigt. 
  const char S40_0010[] PROGMEM = "starting pls wait";  			          // Meldung auf LCD sobald das macro gestartet ist. 
  const char S40_0011[] PROGMEM = "start macro done";  				          // Meldung auf LCD wenn erledigt.
  const char S40_0012[] PROGMEM = "machine is turned to a ready state"; // ausführliche Beschreibung des Macros als Langtext.
  const char S40_0013[] PROGMEM = "";
  
  const char S40_0014[] PROGMEM = "stop machine";					              // Name des Macros  02
  const char S40_0015[] PROGMEM = "machine shutting down now";		      // Meldung an die UART sobald das macro gestartet ist.  
  const char S40_0016[] PROGMEM = "machine down macro done";		        // Meldung an die UART wenn erledigt. 
  const char S40_0017[] PROGMEM = "stopping pls wait";  			          // Meldung auf LCD sobald das macro gestartet ist. 
  const char S40_0018[] PROGMEM = "stopping macro done";  			        // Meldung auf LCD wenn erledigt.
  const char S40_0019[] PROGMEM = "machine is turned to a STOP state";  // ausführliche Beschreibung des Macros als Langtext.
  const char S40_0020[] PROGMEM = "";
 
  
//---------------------------------------------------------
// Macro KOPF Daten 
//---------------------------------------------------------
#define _S41_OFFS       _S40_OFFS + _S40_RECORDS * _S40_FIELDS  + _S40_RECORDS*_S40_FIELDS*_S40_LNGCOUNT   
#define _S41_FIELDS     10 
#define _S41_RECORDS    _S40_RECORDS  
#define _S41_LNGCOUNT   0   

  const char S41_0000[] PROGMEM = "delay before start[ms]_____:";  	// 0 wie lange ist nach dem Aufruf zum Makro-Start zu warten bis das Makro tatsächlich gestartet wird. Der Macro-Arbeits-Task wird mit entsprechendem Delay gestartet.
  const char S41_0001[] PROGMEM = "Macro repeat times_________:";  	//   wie oft ist das Macro als ganzes hintereinander auszuführen. Üblich ist hier 1. -1 = endlos 
  const char S41_0002[] PROGMEM = "Macro interval[ms]_________:";   //   ist nur relevant wenn repeat times <> 1, dann bedeutet diese Zahl die [ms] wie lange es dauern soll bis die Task-Enable-Procedur aufgerufen wird. 
  const char S41_0003[] PROGMEM = "default Step Pause[ms]_____:";  	//   Default-Pause zwischen Makro-Schritten [ms] (wird normalerweise in den Schritten mittels w(ait) n festgelegt, kann hier jedoch GENERELL festgelegt werden.
  const char S41_0004[] PROGMEM = "run parallel allowed Cnt___:";  	//   wie oft darf dieses Makro parallel ausgeführt werden (üblich = 1 0 gleichbedeutend mit darf überhaupt nicht gestartet werden)
  const char S41_0005[] PROGMEM = "kill possible till step____:";  	//   bis zu welchem Macro-Schritt kann/darf das Macro gestoppt werden 
  const char S41_0006[] PROGMEM = "run Macro after kill_______:";  	//   welche Macro-Nummer soll nach einem Stop automatisch gestartet werden. 
  const char S41_0007[] PROGMEM = "other Macros must run______:";  	//   welche anderen Macro-Nummern müssen laufen damit dieses gestartet werden darf 
  const char S41_0008[] PROGMEM = "other Macros may not run___:";  	//   welche anderen Macro-Nummern dürfen nicht laufen damit dieses gestartet werden darf 
  const char S41_0009[] PROGMEM = "reserve ___________________:";  	//   reserve 

  const char S41_0010[] PROGMEM = "0";  	// 01 wie lange ist nach dem Aufruf zum Makro-Start zu warten bis das Makro tatsächlich gestartet wird. Der Macro-Arbeits-Task wird mit entsprechendem Delay gestartet.  
  const char S41_0011[] PROGMEM = "1";  	//    wie oft ist das Macro als ganzes hintereinander auszuführen. Üblich ist hier 1. -1 = endlos 
  const char S41_0012[] PROGMEM = "0";   	//    ist nur relevant wenn repeat times <> 1, dann bedeutet diese Zahl die [ms] wie lange es dauern soll bis die Task-Enable-Procedur aufgerufen wird. 
  const char S41_0013[] PROGMEM = "1000";	//    Default-Pause zwischen Makro-Schritten [ms] (wird normalerweise in den Schritten mittels w(ait) n festgelegt, kann hier jedoch GENERELL festgelegt werden.
  const char S41_0014[] PROGMEM = "1";  	//    wie oft darf dieses Makro parallel ausgeführt werden (üblich = 1 0 gleichbedeutend mit darf überhaupt nicht gestartet werden)
  const char S41_0015[] PROGMEM = "99";  	//    bis zu welchem Macro-Schritt kann/darf das Macro gestoppt werden 
  const char S41_0016[] PROGMEM = "";  		//    welche Macro-Nummer soll nach einem Stop automatisch gestartet werden. 
  const char S41_0017[] PROGMEM = "";  		//    welche anderen Macro-Nummern müssen laufen damit dieses gestartet werden darf 
  const char S41_0018[] PROGMEM = "2";  	//    welche anderen Macro-Nummern dürfen nicht laufen damit dieses gestartet werden darf 
  const char S41_0019[] PROGMEM = "";  		//    reserve 

  const char S41_0020[] PROGMEM = "0";  	// 02  wie lange ist nach dem Aufruf zum Makro-Start zu warten bis das Makro tatsächlich gestartet wird. Der Macro-Arbeits-Task wird mit entsprechendem Delay gestartet.
  const char S41_0021[] PROGMEM = "1";  	//     wie oft ist das Macro als ganzes hintereinander auszuführen. Üblich ist hier 1. -1 = endlos 
  const char S41_0022[] PROGMEM = "";   	//     ist nur relevant wenn repeat times <> 1, dann bedeutet diese Zahl die [ms] wie lange es dauern soll bis die Task-Enable-Procedur aufgerufen wird. 
  const char S41_0023[] PROGMEM = "500"; 	//     Default-Pause zwischen Makro-Schritten [ms] (wird normalerweise in den Schritten mittels w(ait) n festgelegt, kann hier jedoch GENERELL festgelegt werden.
  const char S41_0024[] PROGMEM = "1";  	//     wie oft darf dieses Makro parallel ausgeführt werden (üblich = 1 0 gleichbedeutend mit darf überhaupt nicht gestartet werden)
  const char S41_0025[] PROGMEM = "99";  	//     bis zu welchem Macro-Schritt kann/darf das Macro gestoppt werden 
  const char S41_0026[] PROGMEM = "";  		//     welche Macro-Nummer soll nach einem Stop automatisch gestartet werden. 
  const char S41_0027[] PROGMEM = "";  		//     welche anderen Macro-Nummern müssen laufen damit dieses gestartet werden darf 
  const char S41_0028[] PROGMEM = "";  		//     welche anderen Macro-Nummern dürfen nicht laufen damit dieses gestartet werden darf 
  const char S41_0029[] PROGMEM = "";  		//     reserve 


//-----------------------------------------------------------------
// Macro POSITIONEN SCHRITTE Daten z.zt max 50 Schritte pro Macro 
//-----------------------------------------------------------------
#define _S43_OFFS       _S41_OFFS + _S41_RECORDS * _S41_FIELDS  + _S41_RECORDS*_S41_FIELDS*_S41_LNGCOUNT   
#define _S43_FIELDS    50 
#define _S43_RECORDS    3
#define _S43_LNGCOUNT   0  

  const char S43_0000[] PROGMEM = "Leerbereich da Macro 0 nur zu Dokuzwecken existiert";
  const char S43_0001[] PROGMEM = "";
  const char S43_0002[] PROGMEM = "";
  const char S43_0003[] PROGMEM = "";
  const char S43_0004[] PROGMEM = "";
  const char S43_0005[] PROGMEM = "";
  const char S43_0006[] PROGMEM = "";
  const char S43_0007[] PROGMEM = "";
  const char S43_0008[] PROGMEM = "";
  const char S43_0009[] PROGMEM = "";
  const char S43_0010[] PROGMEM = "";
  const char S43_0011[] PROGMEM = "";
  const char S43_0012[] PROGMEM = "";
  const char S43_0013[] PROGMEM = "";
  const char S43_0014[] PROGMEM = "";
  const char S43_0015[] PROGMEM = "";
  const char S43_0016[] PROGMEM = "";
  const char S43_0017[] PROGMEM = "";
  const char S43_0018[] PROGMEM = "";
  const char S43_0019[] PROGMEM = "";
  const char S43_0020[] PROGMEM = "";
  const char S43_0021[] PROGMEM = "";
  const char S43_0022[] PROGMEM = ""; 
  const char S43_0023[] PROGMEM = "";
  const char S43_0024[] PROGMEM = "";
  const char S43_0025[] PROGMEM = "";
  const char S43_0026[] PROGMEM = "";
  const char S43_0027[] PROGMEM = "";
  const char S43_0028[] PROGMEM = "";
  const char S43_0029[] PROGMEM = "";
  const char S43_0030[] PROGMEM = "";
  const char S43_0031[] PROGMEM = "";
  const char S43_0032[] PROGMEM = "";
  const char S43_0033[] PROGMEM = "";
  const char S43_0034[] PROGMEM = "";
  const char S43_0035[] PROGMEM = "";
  const char S43_0036[] PROGMEM = "";
  const char S43_0037[] PROGMEM = "";
  const char S43_0038[] PROGMEM = "";
  const char S43_0039[] PROGMEM = "";
  const char S43_0040[] PROGMEM = "";
  const char S43_0041[] PROGMEM = "";
  const char S43_0042[] PROGMEM = "";
  const char S43_0043[] PROGMEM = "";
  const char S43_0044[] PROGMEM = "";
  const char S43_0045[] PROGMEM = "";
  const char S43_0046[] PROGMEM = "";
  const char S43_0047[] PROGMEM = "";
  const char S43_0048[] PROGMEM = "";
  const char S43_0049[] PROGMEM = "";

  const char S43_0100[] PROGMEM = "";	// Schritt 0  des Macros 01 
  const char S43_0101[] PROGMEM = "";	// Schritt 1  des Macros 01 
  const char S43_0102[] PROGMEM = "";
  const char S43_0103[] PROGMEM = "";
  const char S43_0104[] PROGMEM = "";
  const char S43_0105[] PROGMEM = "";
  const char S43_0106[] PROGMEM = "";
  const char S43_0107[] PROGMEM = "";
  const char S43_0108[] PROGMEM = "";
  const char S43_0109[] PROGMEM = "";
  const char S43_0110[] PROGMEM = "";
  const char S43_0111[] PROGMEM = "";
  const char S43_0112[] PROGMEM = "";
  const char S43_0113[] PROGMEM = "";
  const char S43_0114[] PROGMEM = "";
  const char S43_0115[] PROGMEM = "";
  const char S43_0116[] PROGMEM = "";
  const char S43_0117[] PROGMEM = "";
  const char S43_0118[] PROGMEM = "";
  const char S43_0119[] PROGMEM = "";
  const char S43_0120[] PROGMEM = "";
  const char S43_0121[] PROGMEM = "";
  const char S43_0122[] PROGMEM = ""; 
  const char S43_0123[] PROGMEM = "";
  const char S43_0124[] PROGMEM = "";
  const char S43_0125[] PROGMEM = "";
  const char S43_0126[] PROGMEM = "";
  const char S43_0127[] PROGMEM = "";
  const char S43_0128[] PROGMEM = "";
  const char S43_0129[] PROGMEM = "";
  const char S43_0130[] PROGMEM = "";
  const char S43_0131[] PROGMEM = "";
  const char S43_0132[] PROGMEM = "";
  const char S43_0133[] PROGMEM = "";
  const char S43_0134[] PROGMEM = "";
  const char S43_0135[] PROGMEM = "";
  const char S43_0136[] PROGMEM = "";
  const char S43_0137[] PROGMEM = "";
  const char S43_0138[] PROGMEM = "";
  const char S43_0139[] PROGMEM = "";
  const char S43_0140[] PROGMEM = "";
  const char S43_0141[] PROGMEM = "";
  const char S43_0142[] PROGMEM = "";
  const char S43_0143[] PROGMEM = "";
  const char S43_0144[] PROGMEM = "";
  const char S43_0145[] PROGMEM = "";
  const char S43_0146[] PROGMEM = "";
  const char S43_0147[] PROGMEM = "";
  const char S43_0148[] PROGMEM = "";
  const char S43_0149[] PROGMEM = "";

  const char S43_0200[] PROGMEM = "";	// Schritt 0  des Macros 02 
  const char S43_0201[] PROGMEM = "";	// Schritt 1  des Macros 02 
  const char S43_0202[] PROGMEM = "";
  const char S43_0203[] PROGMEM = "";
  const char S43_0204[] PROGMEM = "";
  const char S43_0205[] PROGMEM = "";
  const char S43_0206[] PROGMEM = "";
  const char S43_0207[] PROGMEM = "";
  const char S43_0208[] PROGMEM = "";
  const char S43_0209[] PROGMEM = "";
  const char S43_0210[] PROGMEM = "";
  const char S43_0211[] PROGMEM = "";
  const char S43_0212[] PROGMEM = "";
  const char S43_0213[] PROGMEM = "";
  const char S43_0214[] PROGMEM = "";
  const char S43_0215[] PROGMEM = "";
  const char S43_0216[] PROGMEM = "";
  const char S43_0217[] PROGMEM = "";
  const char S43_0218[] PROGMEM = "";
  const char S43_0219[] PROGMEM = "";
  const char S43_0220[] PROGMEM = "";
  const char S43_0221[] PROGMEM = "";
  const char S43_0222[] PROGMEM = ""; 
  const char S43_0223[] PROGMEM = "";
  const char S43_0224[] PROGMEM = "";
  const char S43_0225[] PROGMEM = "";
  const char S43_0226[] PROGMEM = "";
  const char S43_0227[] PROGMEM = "";
  const char S43_0228[] PROGMEM = "";
  const char S43_0229[] PROGMEM = "";
  const char S43_0230[] PROGMEM = "";
  const char S43_0231[] PROGMEM = "";
  const char S43_0232[] PROGMEM = "";
  const char S43_0233[] PROGMEM = "";
  const char S43_0234[] PROGMEM = "";
  const char S43_0235[] PROGMEM = "";
  const char S43_0236[] PROGMEM = "";
  const char S43_0237[] PROGMEM = "";
  const char S43_0238[] PROGMEM = "";
  const char S43_0239[] PROGMEM = "";
  const char S43_0240[] PROGMEM = "";
  const char S43_0241[] PROGMEM = "";
  const char S43_0242[] PROGMEM = "";
  const char S43_0243[] PROGMEM = "";
  const char S43_0244[] PROGMEM = "";
  const char S43_0245[] PROGMEM = "";
  const char S43_0246[] PROGMEM = "";
  const char S43_0247[] PROGMEM = "";
  const char S43_0248[] PROGMEM = "";
  const char S43_0249[] PROGMEM = "";

//-----------------------------------------------------------------
// ENDE der Tabellen  
//-----------------------------------------------------------------
#define _S99_OFFS       _S43_OFFS + _S43_RECORDS * _S43_FIELDS  + _S43_RECORDS*_S43_FIELDS*_S43_LNGCOUNT
#define _S99_FIELDS     0
#define _S99_RECORDS    0
#define _S99_LNGCOUNT   0
  const char S99_0000[] PROGMEM = "THE END IN TBL IS HERE";

// -----------------------------------------------------------------------------------------------------------
// Z e i g e r t a b e l l e    auf die einzelnen Stellen. Hier müssen einfach ALLE oben angeführten 
//                              Variablen-Namen der Reihe nach eingetragen werden.
// -----------------------------------------------------------------------------------------------------------
const char* const F_TBL[] PROGMEM = {
  
  // Anzahl generelle Sprachen  Kürzel en,de,fr,...
S00_0001,S00_0002,S00_0003,

  // Sprachen in Langnamen  english,german,french  Englisch,Deutsch,Französisch
S01_0001,S01_0002,S01_0003,S01_0004,S01_0005,S01_0006,

  // Meldungen Notify() Msg() Sprache 0
S02_0000,S02_0001,S02_0002,S02_0003,S02_0004,S02_0005,S02_0006,S02_0007,S02_0008,S02_0009,
S02_0010,S02_0011,S02_0012,S02_0013,S02_0014,S02_0015,S02_0016,S02_0017,S02_0018,S02_0019,
S02_0020,S02_0021,S02_0022,S02_0023,S02_0024,S02_0025,S02_0026,S02_0027,S02_0028,S02_0029,
S02_0030,S02_0031,S02_0032,S02_0033,S02_0034,S02_0035,S02_0036,S02_0037,S02_0038,S02_0039,
S02_0040,
  // Notify() Msg() Sprache 1
S02_1000,S02_1001,S02_1002,S02_1003,S02_1004,S02_1005,S02_1006,S02_1007,S02_1008,S02_1009,
S02_1010,S02_1011,S02_1012,S02_1013,S02_1014,S02_1015,S02_1016,S02_1017,S02_1018,S02_1019,
S02_1020,S02_1021,S02_1022,S02_1023,S02_1024,S02_1025,S02_1026,S02_1027,S02_1028,S02_1029,
S02_1030,S02_1031,S02_1032,S02_1033,S02_1034,S02_1035,S02_1036,S02_1037,S02_1038,S02_1039,
S02_1040,

  // HX711 Module  Sprachabhängig
S03_0000,S03_0001,S03_0002,S03_0003,S03_0004,S03_0005,S03_0006,S03_0007,
S03_1000,S03_1001,S03_1002,S03_1003,S03_1004,S03_1005,S03_1006,S03_1007,
  // HX711 Module  Daten
S04_0000,S04_0001,S04_0002,S04_0003,S04_0004,S04_0005,S04_0006,S04_0007,S04_0008,S04_0009,
S04_0010,S04_0011, 

  // Datumsformatierungsausdrücke in versch. Sprachen 
S07_0001,S07_0002,S07_0003,S07_0004,S07_0005,S07_0006,S07_0007,S07_0008,S07_0009,

  // freie Bereiche
S08_0001,S08_0002,S08_0003,S08_0004,S08_0005,S08_0006,S08_0007,S08_0008,S08_0009,S08_0010,
S08_0011,S08_0012,

  // c ommandos 
S09_0001,S09_0002,S09_0003,S09_0004,S09_0005,S09_0006,S09_0007,S09_0008,S09_0009,S09_0010,
S09_0011,S09_0012,S09_0013,S09_0014,S09_0015,S09_0016,S09_0017,S09_0018,S09_0019,S09_0020,
S09_0021,S09_0022,S09_0023,S09_0024,S09_0025,S09_0026,S09_0027,S09_0028,S09_0029,S09_0030,
S09_0031,S09_0032,S09_0033,S09_0034,S09_0035,S09_0036,S09_0037,S09_0038,S09_0039,S09_0040,
S09_0041,S09_0042,S09_0043,S09_0044,S09_0045,S09_0046,S09_0047,S09_0048,S09_0049,S09_0050,
S09_0051,S09_0052,S09_0053,S09_0054,S09_0055,S09_0056,S09_0057,S09_0058,S09_0059,S09_0060,
S09_0061,S09_0062,S09_0063,S09_0064,S09_0065,S09_0066,S09_0067,S09_0068,S09_0069,S09_0070,
S09_0071,S09_0072,S09_0073,S09_0074,S09_0075,S09_0076,S09_0077,S09_0078,S09_0079,S09_0080,
S09_0081,S09_0082,S09_0083,S09_0084,S09_0085,S09_0086,S09_0087,S09_0088,S09_0089,S09_0090,
S09_0091,S09_0092,S09_0093,S09_0094,S09_0095,S09_0096,S09_0097,S09_0098,S09_0099,S09_0100,
S09_0101,S09_0102,S09_0103,S09_0104,S09_0105,S09_0106,S09_0107,S09_0108,S09_0109,S09_0110,
S09_0111,S09_0112,S09_0113,S09_0114,S09_0115,S09_0116,S09_0117,S09_0118,S09_0119,S09_0120,
S09_0121,S09_0122,S09_0123,S09_0124,S09_0125,S09_0126,S09_0127,S09_0128,S09_0129,S09_0130,
S09_0131,S09_0132,S09_0133,S09_0134,S09_0135,S09_0136,S09_0137,S09_0138,S09_0139,S09_0140,
S09_0141,S09_0142,S09_0143,S09_0144,S09_0145,S09_0146,S09_0147,S09_0148,S09_0149,S09_0150,
S09_0151,S09_0152,S09_0153,S09_0154,S09_0155,S09_0156,S09_0157,S09_0159,S09_0159,S09_0160,
S09_0161,S09_0162,S09_0163,S09_0164,S09_0165,S09_0166,S09_0167,S09_0168,

  // Temperaturfühler DS1820
S10_0001,S10_0002,S10_0003,S10_0004,S10_0005,S10_0006,S10_0007,S10_0008,S10_0009,S10_0010,
S10_0011,S10_0012,S10_0013,S10_0014,S10_0015,S10_0016,S10_0017,S10_0018,S10_0019,S10_0020,
S10_0021,S10_0022,S10_0023,S10_0024,
  // Temperaturfühler DS1820 Sprachunabhäng DATEN
S11_0001,S11_0002,S11_0003,S11_0004,S11_0005,S11_0006,S11_0007,S11_0008,S11_0009,S11_0010,
S11_0011,S11_0012,

 // Menüzeilen 
S12_0001,S12_0002,S12_0003,S12_0004,S12_0005,S12_0006,S12_0007,S12_0008,S12_0009,S12_0010,
S12_0011,S12_0012,S12_0013,S12_0014,S12_0015,S12_0016,S12_0017,S12_0018,S12_0019,S12_0020,
S12_0021,S12_0022,S12_0023,S12_0024,
 // Menüsprungpunkte
S13_0001,S13_0002,S13_0003,S13_0004,S13_0005,S13_0006,S13_0007,S13_0008,S13_0009,S13_0010,
S13_0011,S13_0012,S13_0013,S13_0014,S13_0015,S13_0016,S13_0017,S13_0018,S13_0019,S13_0020,
S13_0021,S13_0022,S13_0023,S13_0024,S13_0025,S13_0026,S13_0027,S13_0028,S13_0029,S13_0030,
S13_0031,S13_0032,S13_0033,S13_0034,S13_0035,S13_0036,S13_0037,S13_0038,S13_0039,S13_0040,
S13_0041,S13_0042,S13_0043,S13_0044,S13_0045,S13_0046,S13_0047,S13_0048,S13_0049,S13_0050,
S13_0051,S13_0052,S13_0053,S13_0054,S13_0055,S13_0056,S13_0057,S13_0058,S13_0059,S13_0060,

 // Mengeneinheiten
S14_0001,S14_0002,S14_0003,S14_0004,S14_0005,S14_0006,S14_0007,S14_0008,S14_0009,S14_0010,
S14_0011,S14_0012,S14_0013,S14_0014,S14_0015,S14_0016,S14_0017,S14_0018,S14_0019,S14_0020,

   // Tagnamen LANGE 
S15_0001,S15_0002,S15_0003,S15_0004,S15_0005,S15_0006,S15_0007,S15_0008,S15_0009,S15_0010,
S15_0011,S15_0012,S15_0013,S15_0014,
   // Tagnamen KURZ
S16_0001,S16_0002,S16_0003,S16_0004,S16_0005,S16_0006,S16_0007,S16_0008,S16_0009,S16_0010,
S16_0011,S16_0012,S16_0013,S16_0014,

   // Monatsnamen LANGE
S17_0001,S17_0002,S17_0003,S17_0004,S17_0005,S17_0006,S17_0007,S17_0008,S17_0009,S17_0010,
S17_0011,S17_0012,S17_0013,S17_0014,S17_0015,S17_0016,S17_0017,S17_0018,S17_0019,S17_0020,
S17_0021,S17_0022,S17_0023,S17_0024,S17_0025,S17_0026,
   // Monatsnamen KURZ
S18_0001,S18_0002,S18_0003,S18_0004,S18_0005,S18_0006,S18_0007,S18_0008,S18_0009,S18_0010,
S18_0011,S18_0012,S18_0013,S18_0014,S18_0015,S18_0016,S18_0017,S18_0018,S18_0019,S18_0020,
S18_0021,S18_0022,S18_0023,S18_0024,S18_0025,S18_0026,

   // S19_ ist noch frei -------------------------------------
   
   // PARAMETER  Sprachabhängiger Teil 
S20_0000,S20_0001,S20_0002,S20_0003,S20_0004,S20_0005,S20_0006,S20_0007,S20_0008,S20_0009,
S20_0010,S20_0011,S20_0012,S20_0013,S20_0014,S20_0015,S20_0016,S20_0017,S20_0018,S20_0019,
S20_0020,S20_0021,S20_0022,S20_0023,S20_0024,S20_0025,S20_0026,S20_0027,S20_0028,S20_0029,
S20_0030,S20_0031,S20_0032,S20_0033,S20_0034,S20_0035,S20_0036,S20_0037,S20_0038,S20_0039,
S20_0040,S20_0041,S20_0042,S20_0043,S20_0044,S20_0045,S20_0046,S20_0047,S20_0048,S20_0049,
S20_0050,S20_0051,S20_0052,S20_0053,S20_0054,S20_0055,S20_0056,S20_0057,S20_0058,S20_0059,
S20_0060,S20_0061,S20_0062,S20_0063,S20_0064,S20_0065,S20_0066,S20_0067,S20_0068,S20_0069,
S20_0070,S20_0071,S20_0072,S20_0073,S20_0074,S20_0075,S20_0076,S20_0077,S20_0078,S20_0079,
S20_0080,S20_0081,S20_0082,S20_0083,S20_0084,S20_0085,S20_0086,S20_0087,S20_0088,S20_0089,
S20_0090,S20_0091,S20_0092,S20_0093,S20_0094,S20_0095,S20_0096,S20_0097,S20_0098,S20_0099,
S20_0100,S20_0101,S20_0102,S20_0103,S20_0104,S20_0105,S20_0106,S20_0107,S20_0108,S20_0109,
S20_0110,S20_0111,S20_0112,S20_0113,S20_0114,S20_0115,S20_0116,S20_0117,S20_0118,S20_0119,
S20_0120,S20_0121,S20_0122,S20_0123,S20_0124,S20_0125,
   // PARAMETER  sprachunabhängiger Teil 'nur Daten'
S21_0700,S21_0701,S21_0702,S21_0703,S21_0704,S21_0705,S21_0706,S21_0707,S21_0708,S21_0709,
S21_0710,S21_0711,S21_0712,S21_0713,S21_0714,S21_0715,S21_0716,S21_0717,S21_0718,S21_0719,
S21_0720,S21_0721,S21_0722,S21_0723,S21_0724,S21_0725,S21_0726,S21_0727,S21_0728,S21_0729,
S21_0730,S21_0731,S21_0732,S21_0733,S21_0734,S21_0735,S21_0736,S21_0737,S21_0738,S21_0739,
S21_0740,S21_0741,S21_0742,S21_0743,S21_0744,S21_0745,S21_0746,S21_0747,S21_0748,S21_0749,
S21_0750,S21_0751,S21_0752,S21_0753,S21_0754,S21_0755,S21_0756,S21_0757,S21_0758,S21_0759,
S21_0760,S21_0761,S21_0762,S21_0763,S21_0764,S21_0765,S21_0766,S21_0767,S21_0768,S21_0769,
S21_0770,S21_0771,S21_0772,S21_0773,S21_0774,S21_0775,S21_0776,S21_0777,S21_0778,S21_0779,
S21_0780,S21_0781,S21_0782,S21_0783,

   // ERROR Sprachabhängig 
S22_0000,S22_0001,S22_0002,S22_0003,S22_0004,S22_0005,S22_0006,S22_0007,S22_0008,S22_0009,
S22_0010,S22_0011,S22_0012,S22_0013,S22_0014,S22_0015,
   // ERROR DAten 
S23_0000,S23_0001,

   // Eingänge / SCHALTER Sprachabhängig 
S28_0000,S28_0001,S28_0002,S28_0003,S28_0004,S28_0005,S28_0006,S28_0007,S28_0008,S28_0009,
S28_0010,S28_0011,S28_0012,S28_0013,S28_0014,S28_0015,S28_0016,S28_0017,

   // Eingänge / SCHALTER Daten 
S29_0000,S29_0001,S29_0002,S29_0003,S29_0004,S29_0005,S29_0006,S29_0007,S29_0008,S29_0009,
S29_0010,S29_0011,S29_0012,S29_0013,S29_0014,S29_0015,S29_0016,S29_0017,S29_0018,S29_0019,
S29_0020,S29_0021,S29_0022,S29_0023,S29_0024,S29_0025,S29_0026,S29_0027,S29_0028,S29_0029,
S29_0030,S29_0031,S29_0032,S29_0033,S29_0034,S29_0035,S29_0036,S29_0037,S29_0038,S29_0039,
S29_0040,S29_0041,S29_0042,S29_0043,S29_0044,S29_0045,S29_0046,S29_0047,S29_0048,S29_0049,
S29_0050,S29_0051,S29_0052,S29_0053,S29_0054,S29_0055,S29_0056,S29_0057,S29_0058,S29_0059,
S29_0060,S29_0061,S29_0062,

  // S19 frei
  // MASCHINEN Sprachabhängiger Teil 
S30_0000,S30_0001,S30_0002,S30_0003,S30_0004,S30_0005,S30_0006,S30_0007,S30_0008,S30_0009,
S30_0010,S30_0011,S30_0012,S30_0013,S30_0014,S30_0015,S30_0016,S30_0017,S30_0018,S30_0019,
S30_0020,S30_0021,S30_0022,S30_0023,S30_0024,S30_0025,S30_0026,S30_0027,S30_0028,S30_0029,
S30_0030,S30_0031,
  // MASCHINEN nur Daten sprachunabhängig
S31_0000,S31_0001,S31_0002,S31_0003,S31_0004,S31_0005,S31_0006,S31_0007,S31_0008,S31_0009,
S31_0010,S31_0011,S31_0012,S31_0013,S31_0014,S31_0015,S31_0016,S31_0017,S31_0018,S31_0019,
S31_0020,S31_0021,S31_0022,S31_0023,
  // KOMBINATIONEN Sprachabhängig
S32_0000,S32_0001,S32_0002,S32_0003,S32_0004,S32_0005,S32_0006,S32_0007,S32_0008,S32_0009,
S32_0010,S32_0011,S32_0012,S32_0013,S32_0014,S32_0015,S32_0016,S32_0017,S32_0018,S32_0019,
S32_0020,S32_0021,S32_0022,S32_0023,S32_0024,S32_0025,S32_0026,S32_0027,S32_0028,S32_0029,
S32_0030,S32_0031,
  // KOMBINATIONEN Daten
S33_0000,S33_0001,S33_0002,S33_0003,S33_0004,S33_0005,S33_0006,S33_0007,S33_0008,S33_0009,
S33_0010,S33_0011,S33_0012,S33_0013,S33_0014,S33_0015,S33_0016,S33_0017,S33_0018,S33_0019,
S33_0020,S33_0021,S33_0022,S33_0023,
  // SCHALTER / SENSOREN Sprachabhängig
S34_0000,S34_0001,S34_0002,S34_0003,S34_0004,S34_0005,S34_0006,S34_0007,S34_0008,S34_0009,
S34_0010,S34_0011,S34_0012,S34_0013,S34_0014,S34_0015,S34_0016,S34_0017,S34_0018,S34_0019,
S34_0020,S34_0021,S34_0022,S34_0023,S34_0024,S34_0025,S34_0026,S34_0027,S34_0028,S34_0029,
S34_0030,S34_0031,
  // SCHALTER / SENSOREN DATEN
S35_0000,S35_0001,S35_0002,S35_0003,S35_0004,S35_0005,S35_0006,S35_0007,S35_0008,S35_0009,
S35_0010,S35_0011,S35_0012,S35_0013,S35_0014,S35_0015,S35_0016,S35_0017,S35_0018,S35_0019,
S35_0020,S35_0021,S35_0022,S35_0023,S35_0024,S35_0025,S35_0026,S35_0027,S35_0028,S35_0029,
S35_0030,S35_0031,S35_0032,S35_0033,S35_0034,S35_0035,S35_0036,S35_0037,S35_0038,S35_0039,
S35_0040,S35_0041,S35_0042,S35_0043,S35_0044,S35_0045,S35_0046,S35_0047,S35_0048,S35_0049,
S35_0050,S35_0051,S35_0052,S35_0053,S35_0054,S35_0055,S35_0056,S35_0057,S35_0058,S35_0059,
S35_0060,S35_0061,S35_0062,S35_0063,S35_0064,S35_0065,S35_0066,S35_0067,S35_0068,S35_0069,
S35_0070,S35_0071,S35_0072,S35_0073,S35_0074,S35_0075,S35_0076,S35_0077,S35_0078,S35_0079,

  // Macro KOPF Sprachabhängig 
S40_0000,S40_0001,S40_0002,S40_0003,S40_0004,S40_0005,S40_0006,S40_0007,S40_0008,S40_0009,
S40_0010,S40_0011,S40_0012,S40_0013,S40_0014,S40_0015,S40_0016,S40_0017,S40_0018,S40_0019,
S40_0020,

  // Macro Kopf Daten 00
S41_0000,S41_0001,S41_0002,S41_0003,S41_0004,S41_0005,S41_0006,S41_0007,S41_0008,S41_0009,
  // Macro Kopf Daten 01
S41_0010,S41_0011,S41_0012,S41_0013,S41_0014,S41_0015,S41_0016,S41_0017,S41_0018,S41_0019,
  // Macro Kopf DAten 02
S41_0020,S41_0021,S41_0022,S41_0023,S41_0024,S41_0025,S41_0026,S41_0027,S41_0028,S41_0029,

  // Macro-Schritte 00
S43_0000,S43_0001,S43_0002,S43_0003,S43_0004,S43_0005,S43_0006,S43_0007,S43_0008,S43_0009,
S43_0010,S43_0011,S43_0012,S43_0013,S43_0014,S43_0015,S43_0016,S43_0017,S43_0018,S43_0019,
S43_0020,S43_0021,S43_0022,S43_0023,S43_0024,S43_0025,S43_0026,S43_0027,S43_0028,S43_0029,
S43_0030,S43_0031,S43_0032,S43_0033,S43_0034,S43_0035,S43_0036,S43_0037,S43_0038,S43_0039,
S43_0040,S43_0041,S43_0042,S43_0043,S43_0044,S43_0045,S43_0046,S43_0047,S43_0048,S43_0049,
 // Macro-Schritte 01 
S43_0100,S43_0101,S43_0102,S43_0103,S43_0104,S43_0105,S43_0106,S43_0107,S43_0108,S43_0109,
S43_0110,S43_0111,S43_0112,S43_0113,S43_0114,S43_0115,S43_0116,S43_0117,S43_0118,S43_0119,
S43_0120,S43_0121,S43_0122,S43_0123,S43_0124,S43_0125,S43_0126,S43_0127,S43_0128,S43_0129,
S43_0130,S43_0131,S43_0132,S43_0133,S43_0134,S43_0135,S43_0136,S43_0137,S43_0138,S43_0139,
S43_0140,S43_0141,S43_0142,S43_0143,S43_0144,S43_0145,S43_0146,S43_0147,S43_0148,S43_0149,
 // Macro-Schritte 02
S43_0200,S43_0201,S43_0202,S43_0203,S43_0204,S43_0205,S43_0206,S43_0207,S43_0208,S43_0209,
S43_0210,S43_0211,S43_0212,S43_0213,S43_0214,S43_0215,S43_0216,S43_0217,S43_0218,S43_0219,
S43_0220,S43_0221,S43_0222,S43_0223,S43_0224,S43_0225,S43_0226,S43_0227,S43_0228,S43_0229,
S43_0230,S43_0231,S43_0232,S43_0233,S43_0234,S43_0235,S43_0236,S43_0237,S43_0238,S43_0239,
S43_0240,S43_0241,S43_0242,S43_0243,S43_0244,S43_0245,S43_0246,S43_0247,S43_0248,S43_0249,



S99_0000};
