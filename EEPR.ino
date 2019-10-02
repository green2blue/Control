/* 2016-12-11  2019-02-02 überarbeitet 
Datenstruktur bzw. Daten die im EEPROM des Prozessors gespeichert werden.
    - Arduino Uno:        1024 BYTE EEPROM storage.
    - Arduino Mega:       4096 BYTE EEPROM storage.     */
// -------------------------------------------------------------------------------------------------------------------------------------------------
// 2017-10-26  das Chip-interne EEPROM kann laut Datenblatt 100.000 mal sicher beschrieben werden. 
//             Diese Software ist bemüht die Schreibzugriffe auf das EEPROM niedrig zu halten.
//      Deshalb folgen jetzt einige Zeilen als Idee was man machen könnte wenn einmal ein FRAM-Modul hinzukommt.
//      Pro Gerät/Kombination könnte man noch Interessantes wie Zeitpunkt des letzen EIN, Zeitpunkt des letzten AUS usw. speichern.
//   Eine machbare und sinnvolle Speicherung dieser Daten wäre möglich mit einem FRAM-Modul, die gibt es winzigst bei
//   Adafruit I2C Non-Volatile FRAM Breakout - 256Kbit 32.000Byte 32KByte um 12 Euro auf ebay
//   Um die Zustände, Zeiten, Häufigkeiten der Komponenten zu speichern wäre ein FRAM-Modul genau das Richtige.
//
// ----------------------------------------------------------------------------------------------------
// 2017-08-13  PARAMETER die in einem eigenen Menü eingestellt und gespeichert werden können.
//             Die (statischen) Parameter-Daten wie: Name,Min,Max,Einheit und falls mal noch
//             mehr nötig wird, einfach erweiterbar,..  liegen im Prgramm-FLASH, hier in der Tbl_..
//             Zu jedem dieser Parameter gibt es nun den eigentlichen WERT, gespeichert im EEPROM.
//             Dieser Wert ist nun ein LONG INTEGER, früher ein INT  also 4 Byte.
//             Jeder dieser Parameter verbraucht nun im EEPROM 4 Byte  (APW_INT ab hier)
//             Wir wissen nicht wieviele Parameter zukünftig noch anfallen, aktuell schätzen wir 
//             50 Stück APW_INT, das sind 100 BYTE im EEPROM
//   Festlegung: Es werden im EEPROM für die APW 512 Byte reserviert, das entspricht 256 Stück APW_INT. 
// -----------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------------
// 2017-08-13   S p e i c h e r    O r g a n i s a t i o n   im EEPROM  nur ein momentanes BILD davon 
//                      OFFSET                    OFFSET
//   OFFSET 0           _EE_OFFS_PARA             _EE_OFFS_ERR          _EE_SEG_STA_OFFS         _EE_SEG08_OFFS
//   -----------        ----------------------    -------------------   ----------------
//   D I V E R S E S    P A R A M E T E R         E R R O R S           S T A T I S T I K
//                      je 4 Byte int32_t         je 8 Byte = 64 Stk    je 10 Byte = 150 Stk
//   512 Byte           512 Byte                  512 Byte              1500 Byte 
//   0..511             512..1023                 1024..1535            1536...3035               3036...   4096
//   diverses           APW_INT          
//                                              
// --------------------------------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------
//  O F F S E T    Compilervariablen  
// --------------------------------------------------------------------------------
#define _EE_OFFS_HX711    312   // Offset ab der HX711 Daten gespeichert werden pro Modul 10 Byte (2 Byte Calibrated:12345, 4 Byte float Fact, 4 Byte int32_t Offset)
#define _EE_HX711_SIZE     10   // benötigte BYTE pro HX711 Modul    bis 512 können also 20 Stück gespeichert werden.
                                
#define _EE_OFFS_PARA     512   // Offset ab der die  PARAMETER -Werte LONG INTEGER(also je zu 4 Byte)  zu finden sind
#define _EE_PARA_SIZE       4   // Anzahl Byte die ein Parameter einnimt

#define _EE_SEG_STA_OFFS 1536   // Offset für die Statistikdaten pro Gerät/Kombination  2018-06-10 begonnen einzubauen
#define _EE_STA_SIZE       10   // Anzahl Byte pro Statistiksatz .OnYears .OnDays .OnHours .OnMinutes .OnSeconds .Onms .OnCnt .free 5+9+5+6+6+10+32+7=80Bit=10Byte

#define _EE_OFFS_ERR 1024   // Offset für die Error-Häufigkeit pro Err-Nr (1 Byte(Cnt1) + 2 Byte(Cnt2) + 5 Byte(timestamp)) 
#define _EE_ERR_SIZE        8   // Anzahl Byte pro Errordatensatz 

#define _EE_DS18_areasize  30   // für wieviele DS18-Sensor Adressen ist im EEPROM Platz reserviert

//---------------------------------------------------------------------
// Alle in TBL definierten Parameter auf DEFAULT-WERT im EEPROM setzen
//---------------------------------------------------------------------
void FactoryParameters() {
  int32_t ParaVal=0; 
  for (uint8_t PNr=1; PNr < _PARAMETER_COUNT; PNr++) {
    ParaVal= TBL_Long( T_Idx(_S_PAD, PNr, _S21_DEF));     // Parameter Defaultwert aus TBL Parameter-DATEN lesen und als LONG liefern
    Serial.print(F("Parameter:"));Serial.print(PNr);Serial.print(F(" set to:"));Serial.println(ParaVal);
    EE_ParaLet(PNr, ParaVal);
  }
}

//---------------------------------------------------------------------------------------------------------------
//             E R R O R - L O G   Bearbeiten
//  2018-03-28 Error-Log 'löschen' bzw. Zähler/Datum rücksetzen in verschiedenen Ausführungen je nach Parameter.
//   
// Ein Error hat 2 Zähler. 1. den vom Anwender noch 'unbestätigten' Zähler 
//                         2. den gesamt Zähler in den die bestätigten addiert werden.
//              und einen ZEITSTEMPEL wann zuletzt dieser Fehler aufgetreten ist.
// Aufruf-Parameter der Funktion:
//   ErrNr  = 0 es werden ALLE Fehlerlogs geleert, sonst nur die Daten des Errors mit der Fehlernummer= ErrNr
//   ErrVar = 1 es wird der nicht bestätigte Teil der Fehler in den bestätigten Teil 
//              addiert und der unbestätigte Teil auf 0 gesetzt.
//          = 2 es wird die Anzahl der gesamten Fehlervorkommen auf 0 gesetzt
//          = 3 es werden beide Zähler auf 0 gesetzt
//          = 4 wie 3 jedoch wird auch das Datum des letzten Vorkommens auf 0 gestellt
// timestamp.
//  byte JJJJ : 12; // Jahr (reicht bis 4095)
//  byte MO   : 4;  // Monat(0..12) 
//  byte DD   : 5;  // Tag  (0..31) geht sich gerade aus
//  byte HH   : 5;  // Stunde (0..24) 
//  byte MM   : 6;  // Minute (0..60)
//  byte SS   : 6;  // Sekunde(0..60)
//  byte FF   : 2;  // Restbit auf 40 Bit = 5 Byte für den Zeitstempel
//----------------------------------------------------------------------------------------------------------------
uint8_t Error_Clear(int8_t ErrNr, int8_t ErrVar) {
  uint16_t ErrCnt1;                       // Anzahl der im EEPROM liegenden Fehlerhäufigkeit
  uint16_t ErrCnt2 = 0; 
  uint8_t  ErrCnt0 = 0;
  int8_t   ErrFrom = ErrNr;
  int8_t   ErrTo   = _S22_RECORDS;        // maximale, im Moment vergebene Error-Nummer laut I_Tbl
  if (ErrNr > _S22_RECORDS) {return 0; }
  if (ErrNr != 0) {ErrTo = ErrNr; } else {ErrFrom = 1; }
  for (uint8_t ENr=ErrFrom; ENr <= ErrTo; ENr++) {
    if (ErrVar == 1) {                  // unbestätigte Anzahl Fehler in die Gesamtanzahl hinzufügen, danach unbestätigte Anzahl auf 0 stellen
      ErrCnt2 = Error_Read(ENr, 2);                                   // aktuelle gesamtanzahl einlesen
      ErrCnt1 = Error_Read(ENr, 1);                                   // unbestätigte Anzahl einlesen
      ErrCnt2 = ErrCnt2 + ErrCnt1;
      EEPROM.put(_EE_OFFS_ERR+(ENr-1)*_EE_ERR_SIZE    ,ErrCnt0);      // die unbestätigte Anzahl auf 0      1 Byte
      EEPROM.put(_EE_OFFS_ERR+(ENr-1)*_EE_ERR_SIZE +1 ,ErrCnt2);      // Schreiben der neuen Gesamtzahl     2 Byte
    }
    if (ErrVar == 2) {                                                // es wird die Anzahl der gesamten Fehlervorkommen auf 0 gesetzt
      ErrCnt2=0;
      EEPROM.put(_EE_OFFS_ERR+(ENr-1)*_EE_ERR_SIZE +1, ErrCnt2);
    }
    if (ErrVar == 3) {                                                // es werden einfach beide Zähler auf 0 gestellt
      ErrCnt2=0;
      EEPROM.put(_EE_OFFS_ERR+(ENr-1)*_EE_ERR_SIZE    ,ErrCnt0);      // die unbestätigte Anzahl auf 0        
      EEPROM.put(_EE_OFFS_ERR+(ENr-1)*_EE_ERR_SIZE +1 ,ErrCnt2);      // Schreiben der neuen Gesamtzahl
    }
    if (ErrVar == 4) {                                                // wie 3 jedoch MIT leeren des Datums 
      EEPROM.put(_EE_OFFS_ERR+(ENr-1)*_EE_ERR_SIZE    ,ErrCnt0);      // die unbestätigte Anzahl auf 0        
      EEPROM.put(_EE_OFFS_ERR+(ENr-1)*_EE_ERR_SIZE +1 ,ErrCnt2);      // Schreiben der neuen Gesamtzahl
      timestamp.JJJJ=0; timestamp.MO=0; timestamp.DD=0; timestamp.HH=0; timestamp.MM=0; timestamp.SS=0; timestamp.FF=0; // (JJJJ MO DD HH MM SS FF)
      EEPROM.put(_EE_OFFS_ERR+(ENr-1)*_EE_ERR_SIZE +3 ,timestamp);    // Schreiben eines leeren Zeitstempels 5 Byte     (0000 00 00 00 00 00 00)      
    }
  }     // Ende Schleife für jeden bzw. den Error
  return 0;
}


// ----------------------------------------------------------------------------------------
//  Lesen   E R R O R - Log   Format 1 +  2 Byte
//  liest die Anzahl Fehlervorkommen zur gefragten ErrNr.
//  ErrVar=1 liefert die Anzahl die noch nicht bestätigt wurde, 1 Byte 0.....250
//        =2 liefert die Gesamte Anzahl des Vorkommens          2 Byte 0...65536
//        =3 liefert timestamp.JJJJ .MO .DD .HH. MM. SS         5 Byte struct
// ----------------------------------------------------------------------------------------
uint16_t Error_Read(int8_t ErrNr,int8_t ErrVar) {
  uint8_t   Ret0;
  uint16_t  Ret;  // je nach ErrVar die noch unbestätigte Anzahl oder die gesamte Anzahl Error-Vorkommen.
  if (ErrVar<=1) {EEPROM.get(_EE_OFFS_ERR+(ErrNr-1)*_EE_ERR_SIZE,   Ret0); return Ret0;} 
  if (ErrVar==2) {EEPROM.get(_EE_OFFS_ERR+(ErrNr-1)*_EE_ERR_SIZE+1, Ret ); return Ret;}
  if (ErrVar==3) {EEPROM.get(_EE_OFFS_ERR+(ErrNr-1)*_EE_ERR_SIZE+3, timestamp);} // der Aufrufer muß sich nun das Datum aus der Struktur timestamp abholen.
  return 0;
}  

// --------------------------------------------------------------------------------------------------
//  Erhöhen der Häufigkeit  eines  ERROR  mit aktuellem Zeitstempel  
//    ErrVar=1  erhöht die Anzahl noch nicht bestätigter Fehleranzahl  + 1
//          =2  erhöht die Gesamt-Anzahl Fehlervorkommen               + 1
//  sollte die noch nicht bestätigte Fehleranzahl bereits >= 250 sein, wird diese in die Gesamt-Anzahl addiert.
//             in der nicht bestätigten Anzahl verbleibt dann 1 
// der ZEITSTEMPEL wird ebenfalls gesetzt !!
// --------------------------------------------------------------------------------------------------
uint16_t Error_Add_Cnt(int8_t ErrNr,int8_t ErrVar) {
  uint16_t ErrCnt1;                         // Anzahl der im EEPROM liegenden Fehlerhäufigkeit
  uint16_t ErrCnt2; 
  ErrCnt1= Error_Read(ErrNr, ErrVar);       // liest die aktuelle Anzahl 1 oder 2 aus dem EEPROM
  if (ErrVar == 1) {                        // unbestätigte Anzahl  
       
    if (ErrCnt1 >= 250) {                   // wenn die unbestätigte Anzahl bereits >= 250 ist dann wird diese Anzahl jetzt zur Gesamtanzahl hinzugefügt.
      ErrCnt2= Error_Read(ErrNr, 2);        // liest die gesamte Anzahl 
      ErrCnt2= ErrCnt2 + ErrCnt1;           // Zusammenzählen der beiden, ist ja klar
      EEPROM.put(_EE_OFFS_ERR+(ErrNr-1)*_EE_ERR_SIZE +1 ,ErrCnt2);    // Schreiben der Gesamtzahl
      EEPROM.put(_EE_OFFS_ERR+(ErrNr-1)*_EE_ERR_SIZE    ,1);          // die unbestätigte Anzahl wird wieder zu 1   
      RTC2timestamp(); 
      EEPROM.put(_EE_OFFS_ERR+(ErrNr-1)*_EE_ERR_SIZE +3 ,timestamp);  // Zeitstempel 
     } else { // solange die unbestägite Anzahl noch < 250 ist wird zu dieser addiert
      RTC2timestamp();  //Printtimestamp();
      EEPROM.put(_EE_OFFS_ERR+(ErrNr-1)*_EE_ERR_SIZE, ErrCnt1 + 1);   // die unbestätigte Anzahl um 1 erhöhen und in den EEPROM schreiben.  
      EEPROM.put(_EE_OFFS_ERR+(ErrNr-1)*_EE_ERR_SIZE +3 ,timestamp);  // Zeitstempel 
     }
  } else { // Gesamt Anzahl soll erhöht werden. ACHTUNG diese wird als vom Anwender bereits bestätigt angesehen und dient nur zur Dokumentation wie oft welcher Fehler vorgekommen ist.
    EEPROM.put(_EE_OFFS_ERR+(ErrNr-1)*_EE_ERR_SIZE +1, ErrCnt1 + 1);  // damit wird die bestätigte Anzahl um 1 erhöht und im EEPROM gespeichert. 
  }
  return ErrCnt1;
}


// ----------------------------------------------------------------------------------------
//  Lesen  P A R A M E T E R - N U M M E R - W E R T E  Format LONG INTEGER, also 4 Byte
//  Die Stelle(adresse) an der nach dem Wert zu greifen ist errechnet sich aus:
//  Bsp: ParaNr=1   addr= (ParaNr-1) * AnzahlByteEinesWertes  
//       das ergibt die Reihe 0, 4, 8, 12 und das sind für die Parameter-Nummern die Startadressen im EEPROM (+Offset) 
// ----------------------------------------------------------------------------------------
int32_t EE_ParaGet(uint8_t ParaNr) {
  int32_t ParaVal;
  EEPROM.get(_EE_OFFS_PARA + (ParaNr-1) * _EE_PARA_SIZE, ParaVal);  // Einlesen der 4 Byte in den Integer RetVal
  return ParaVal;                           
}
// ---------------------------------------------------------------------------------------
//  Speichern  von  P A R A M E T E R - N U M M E R - W E R T E  Format LONG INTEGER
//  SPEICHERT einen Integer-Wert zum Parameter mit der Nummer ParaNr.
//  siehe auch EE_ParaGet()
// ---------------------------------------------------------------------------------------
void EE_ParaLet(uint8_t ParaNr,int32_t ParaVal) {
  EEPROM.put(_EE_OFFS_PARA + (ParaNr-1) * _EE_PARA_SIZE , ParaVal);    // Schreiben der 4 Byte ins EEPROM (nur wenn sich etwas geändert hat)
}
//---------------------------------------------------------------------------------------
//  ADDIEREN SUBTRAHIEREN von Werte zu einem gespeicherten Parameterwert 
//---------------------------------------------------------------------------------------
int32_t EE_ParaWork(uint8_t ParaNr, int32_t ParaWorkVal) {
  int32_t ParaVal;
  EEPROM.get(_EE_OFFS_PARA + (ParaNr-1) * _EE_PARA_SIZE , ParaVal);   // Einlesen aktuell gespeicherter Wert (4 Byte)
  ParaVal = ParaVal + ParaWorkVal;                                    // hinzurechnen/abziehen  des mitgegebenen Wertes 
  EEPROM.put(_EE_OFFS_PARA + (ParaNr-1) * _EE_PARA_SIZE , ParaVal);   // Speichern des neuen Wertes
  return ParaVal;                                                     // Liefert den neuen gespeicherten Wert im EEPROM
}




// ab hier noch nicht verwendete Teile



// ----------------------------------------------------------------------------------------
//  Lesen   Geräte-Kombination-Statistik-Satz zu einer Geräte/Kombination Nummer.
//  liest die Daten eines Objektes in die Struktur gk_stat 
//  Als Feature wird gk_stat.OnOffCnt  geliefert
//           gk_stat.OnYears  : 5; // Jahre 0..31
//                  .OnDays   : 9; // Tage  0..511
//                  .Onms     :32; // milli Sekunden  (kamit kann man durch die Zahl 4233600000 exakt 49 Tage abspeichern, danach sind diese Tage in OnDays zu addieren und Onms auf 0 zu setzen.)
//                  .OnOffCnt :32; // Schaltvorgänge (Anzahl wie oft das Objekt geschaltet wurde wobei ON/OFF nur die ON gezählt werden.)
//                  .freebits : 2; // noch freie Bits auf 80 Bit  = 10 Byte pro (g) (k)
//  Aufrufparameter t_Nr  ist die (g)eräte-Nummer  oder im Falle einer (k) das Maximum an Geräten + die (k)ombinations-Nummer
// Anm:  mittels Funktion:  ms2YDHMSM(uint32_t msCnt)  UND entsprechender Vorbelegung von gk_stat_res  können die  Werte
//       aus der Struktur gk_stat in DETAIL-Werte für eine lesbare Darstellung in JAHRE, TAGE, STUNDEN, MINUTEN, SEKUNDEN, MILLIS 
//       aus der Struktur gk_stat_res abgeholt werden.
// ----------------------------------------------------------------------------------------
uint32_t gk_stat_Read(uint8_t t_Nr) {
   EEPROM.get(_EE_SEG_STA_OFFS+(t_Nr-1)*_EE_STA_SIZE,   gk_stat); 
   return gk_stat.OnOffCnt ;
}  


//--------------------------------------------------------------------------------------------------------
//       S T A T I S T I K   Daten zu den (g)eräten  (k)ombinationen  Verändern/Speichern
//  Zweck und Möglichkeiten: Mit diesen Daten kann man zum Beispiel die erzeugte Gasmenge errechnen, die Lebensdauer von Aktoren bewerten,
//                           die viel benützten Komponenten herausfinden, nötige Wartung erkennen,...
//   Zu jedem Gerät, jeder Kombination  werden im EEPROM und im RAM Statistikdaten geführt. 
//   Dafür sind 150 Positionen je 10 Byte im EEPROM    und aktuell 71 Positionen je 11 Byte im RAM reserviert.
//   Der Platzbedarf im RAM ergibt sich aus der aktuellen Anzahl definierter (g)eräte + (k)ombinationen 
//   Zur Zeit werden von den 150 reservierten Positionen im EEPROM nur 71 Positionen verwendet da jetzt nicht mehr Geräte und/oder Kombinationen 
//   in der I_Tbl definiert sind und selbst die definierten Dummy's also Leerstellen beinhalten. 
//  Im RAM werden Statistikdaten geführt damit die Schreibzugriffe in das EEPROM, auch bei höchster Frequenz an Geräte-Schaltaktionen
//  oder höchsten Laufzeiten der Geräte/Kombinationen für Jahrzehnte kein Problem machen sollte, also die garantierten 100.000 Schreibzyklen sollten
//  bei Weitem unterschritten bleiben. Da die Daten aus dem RAM mindestens 1x pro TAG in das EEPROM geschrieben werden wird im RAM nur der Platz
//  für 'kürzere' Variablen benötigt.
//  Die Statistikdaten vom RAM werden spätestens vor einem Zahlen-Überlauf und beim Abschaltvorgang in den EEPROM geschrieben. 
//  Solange wir keine Stützung des arduino über einen Supercap gebaut haben,
//  würden die Daten bei einem Stromausfall oder einer harten Abschaltung verloren gehen. 
//  Der Aufwand einer Supercap-Stützung wäre relativ gering und würde ein Sichern der Daten auch bei einem Stromausfall ermöglichen.
//  Der arduino müsste an einem freien Interruptfähigen PIN den Stromausfall mitbekommen und dadurch die Schreibvorgänge auslösen. 
//  Vermutlich brauchen die Schreibvorgänge nur einige 100 Milli-Sekunden, sodaß die Not-Spannungserhaltung für den Arduino mit kleinen Kapazitäten auskommen würde.
//
// Aufruf-Parameter der Funktion:
//   type  'g' 'k'   'x' = ALLE  (im EEPROM liegen zuerst die 'g'eräte  dann folgend die 'k'ombinationen
//   t_Nr  = 0 es werden ALLE Statistikdaten der type geleert/gelöscht, sonst werden nur die Daten mit der t_Nr im EEPROM aktualisiert
//   Var   = 1 es wird  .OnOffCnt   mit Val befüllt   
// Vorgehensweise der Fkt:
//   Für ein Update muß zuerst die struct vom EEPROM geholt werden, dann wird das betreffende Feld wie z.B.: .OnOffCnt aktualisiert 
//   und dann die gesamte struct neu geschrieben.
//
//           2          .Onms       
//           3          .OnSeconds  
//           4          .OnMinutes  
//           5          .OnHours   
//           6          .OnDays
//           7          .OnYears
//
//          10          .freebits
// gk_stat.OnYears   : 5;   //  Jahre   0..31
// gk_stat.OnDays    : 9;   //  Tage    0..511 (364) ab dieser Zahl wird OnYears   + 1 und OnDays    = 0
// gk_stat.OnHours   : 5;   //  Stunden 0..31  (23)  ab dieser Zahl wird OnDays    + 1 und OnHours   = 0
// gk_stat.OnMinutes : 6;   //  Minuten 0..63  (59)  ab dieser Zahl wird OnHours   + 1 und OnMinutes = 0
// gk_stat.OnSeconds : 6;   //  Sekunde 0..63  (59)  ab dieser Zahl wird OnMinutes + 1 und OnSeconds = 0
// gk_stat.Onms      :10;   //  ms      0..1023(999) ab dieser Zahl wird OnSeconds + 1 und Onms      = 0
// gk_stat.freebits  : 7;   // bis hier 48 Bit = 6 Bytes die per Struktur in/vom EEPROM geschrieben/gelesen werden
// gk_stat.OnOffCnt  :32;   //  Schaltvorgänge Zähler  0..  reicht für 68 Jahre wenn jede 1s ein Schaltvorgang dann wäre ein Zählerüberlauf 
//                           // hier noch die restlichen 4 Bytes 
//                 Da in/vom EEPROM selten gelesen/geschrieben wird habe ich vorerst die relativ große struct belassen mit den 10 Byte
//                 Falls es zu Zeitproblemen kommt kann diese struct in kleinere Teile zerlegt werden, dafür ist dann etwas mehr Code fällig.
//---------------------------------------------------------------------------------------------------------------------------------------
//                   'g','k','x'      0. 1,2,3   0. 1=Cnt 2=Onms      Wert der hinzuaddiert werden soll wenn=0 dann wird EEPROM-Pos bzw. Var auf 0 gestellt
uint8_t gk_stat_Set(uint8_t type, uint8_t t_Nr, uint8_t Var, uint32_t Val) {
  (void) type;
  uint32_t ValResult=0;       // Zwischenergebnis in ms
  uint8_t  t_From=t_Nr;       // Von beginnt bei mitgegebener t_Nr z.B.: 0 bedeutet ALLE
  uint8_t  t_To=_STATI_ARR;   // maximale, im Moment definierte Geräte+Kombi+1
  if (t_Nr > _STATI_ARR) {return 0; }
    // wenn t_Nr mitgegebene wurde wird Bis gleich Von  sonst beginne bei 1 und ende bei Max
  if (t_Nr != 0) {t_To = t_Nr; } else {t_From = 1; }

  
  for (uint8_t ENr=t_From; ENr <= t_To; ENr++) {  // Schleife entwender nur für Zahl_1 bis Zahl_2   oder nur für eine Zahl
    gk_stat_Read(ENr);      // Einlesen eines Satzes in die Struktur gk_stat.  aus dem EEPROM
    switch (Var) {
      case 1: //  gk_stat.OnOffCnt   ( gk_stat2.OnOffCnt )
        if (Val==0) {gk_stat.OnOffCnt=0;} else {gk_stat.OnOffCnt = gk_stat.OnOffCnt + Val;}
        break;
      case 2: //  gk_stat.Onms
        ValResult=Val;
        break;
      case 3: //  gk_stat.OnSeconds
        ValResult=Val*1000UL;       // Umrechnen auf ms
        break;
      case 4: //  gk_stat.OnMinutes
        ValResult=Val*60000UL;      // Umrechnen auf ms
        break;
      case 5: //  gk_stat.OnHours
        ValResult=Val*3600000UL;    // Umrechnen auf ms
        break;
      case 6: //  gk_stat.OnDays
        if (Val==0) {gk_stat.OnDays=0;} else {gk_stat.OnDays = gk_stat.OnDays + Val;} 
        break;
      case 7: //  gk_stat.OnYears
        if (Val==0) {gk_stat.OnYears=0;} else {gk_stat.OnYears = gk_stat.OnYears + Val; }
        break;
      case 10: // gk_stat.freebits
        break;
    }

      // if (ErrVar == 2) { // es wird die Anzahl der gesamten Fehlervorkommen auf 0 gesetzt
      //  ErrCnt2=0;
      //   EEPROM.put(_EE_OFFS_ERR+(ENr-1)*_EE_ERR_SIZE +1, ErrCnt2);
      //   }
  } // Ende Schleife für jeden bzw. den Error
  Serial.println(ValResult);
  return 0;
}


//----------------------------------------------------------------------------------------------------
// 2018-06-27, 28 Weiterarbeiten an dieser Fkt.
// 2018-06-14 begonnen.
//   1. Umrechnen von der dieser Funktion mitgegebenen msCnt  auf Jahre, Tage, Stunden, Minuten, Sekunden, Millis 
//      mittels einfacher Addition aller millis würde ein mögl. Überlauf der 32-bit Variablen provoziert.
//      Falls auch (Year)(Day) exakt benötigt werden, dann sind diese Werte bereits vor Aufruf in gk_stat_res vorzubelegen.
//      msCnt wird in die gk_stat_res ADDIERT  falls in gk_stat_res Werte > 0 vorliegen.
// 
//  gk_stat_res.  wurde gebaut für Anzeigezwecke wegen der besseren Lesbarkeit, nicht für eine Datenspeicherung 
//---------------------------------------------------------------------------------------------------- 
void ms2YDHMSM(uint32_t msCnt) {  // milliseconds to gk_stat_res. (Year)(Day)(Hour)(Minute)(Second)(Millis)
  //uint32_t ValHelp=msCnt;   // Zwischenergebnis  (Millis u.a.)
  //uint32_t Ams = 0;         // Ausgangs Millis aus gk_stat_res - Daten (ohne Year, ohne Day da diese nicht in 32-bit als millis abbildbar sind.
                            // Für eine Addition wird erst ALLES auf Millis umgewandelt, dann addiert, dann wieder zu Jahr,Tag,Stunde,Minute,Sekunde,Millis konvertiert

  stat_gk_struct_res     gk_stat_res_help;  // Hilfsstruktur JAHRE,TAGE,STUNDEN,MINUTEN,SEKUNDEN,MILLIS  
  uint32_t Div = 0; // 1000*60*60*24UL; =Tage-Divisor   1000*60*60UL =Stunden-Divisor  usw. // Divisor für die jeweilige Einheit 
  uint32_t Rest= 0; // Rest nach jeder Einheitsumrechnungs-Division 
  // 0 1 2 3 4 5 6 7 8 910111213141516171819  Beispiel für eine Anzeige am LCD:
  //----------------------------------------
  // 0 1   H o l d m a g n e t   1    
  //   3 1 [ Y ]   1 8 1 [ d ]   1 1 [ h ] 
  //   1 2 [ m ]   9 8 2 [ m s ]     U S E D
  // 1 2 3 4 5 6 7 8 9 [ S w i t c h e d ] _                 
  //----------------------------------------
  gk_stat_res_help.RYears   =0; //   2 Jahre    
  gk_stat_res_help.RDays    =0; // 181 Tage
  gk_stat_res_help.RHours   =0; //  11 Stunden
  gk_stat_res_help.RMinutes =0; //  12 Minuten
  gk_stat_res_help.RSeconds =0; //  43 Sekunden
  gk_stat_res_help.Rms      =0; // 982 Millis      

  
  //----------------------------------------------------------------------------------------------
  // 1. UMRECHNEN des Eingabeparameters  msCnt  auf  TAGE, STUNDEN, MINUTEN, SEKUNDEN, MILLIS 
  //    aus 1000ms wird +1s  aus 60min wird +1h   usw.
  //----------------------------------------------------------------------------------------------
  Rest                      = msCnt;                                  // nur damit folgende Zeilen immer im gleichen Schema sind
      //
  Div                       = 1000UL*60*60*24;                        // Divisor für höchste Einheit = TAGE
  gk_stat_res_help.RDays    = Rest  /Div;                             // Ganzzahl  T a g e 
  Rest                      = Rest  -(Div*gk_stat_res_help.RDays);    // Rest nachdem die Tage ermittelt wurden.
  Div                       = 1000UL*60*60;                           // Divisor für nächste Einheit = Stunden
  gk_stat_res_help.RHours   = Rest  /Div;                             // Ganzzahl  S t u n d e n  
  Rest                      = Rest  -(Div*gk_stat_res_help.RHours);   // neuer Rest nachdem die Stunden ermittelt wurden.
  Div                       = 1000UL*60;                              // Divisor für nächste Einheit = Minuten
  gk_stat_res_help.RMinutes = Rest  /Div;                             // Ganzzahl  M i n u t e n
  Rest                      = Rest  -(Div*gk_stat_res_help.RMinutes); // neuer Rest nachdem die Minuten ermittelt wurden.
  Div                       = 1000UL;                                 // Divisor für nächste Einheit = Sekunden
  gk_stat_res_help.RSeconds = Rest  /Div;                             // Ganzzahl  S e k u n d e n
  Rest                      = Rest  -(Div*gk_stat_res_help.RSeconds); // neuer Rest nachdem die Sekunden ermittelt wurden.
  Div                       = 1UL;                                    // Divisor für nächste Einheit = ms
  gk_stat_res_help.Rms      = Rest  /Div;                             // Ganzzahl  M i l l i s
  
  //------------------------------------------------------------------------------------
  // 2. das Zwischenergebnis gk_stat_res_help  ist nun zu  gk_stat_res  zu addieren
  //    Div wird ab hier als Zwischenrechnungsvariable verwendet
  //    Rest wird ab hier als Variable für die jeweils nächst höhere Einheit verwendet
  //------------------------------------------------------------------------------------
  // 2 a) Betrachtung der Millis 
  //----------------------------------
  Div = gk_stat_res.Rms    + gk_stat_res_help.Rms;        // Ergebnis Div  darf nach Addition nicht > 999 sein da hier sonst Rest-1000 einzustellen ist + 1 Sekumde Übertrag  Bsp: 1998 Bsp2: 343
  if (Div > 999) {                                        // Wenn die Addition der Millis > 999 wäre
    gk_stat_res.RSeconds += 1;                            // nächste Einheit erhöt sich damit um 1                      Bsp: 51
    gk_stat_res.Rms      = Div - 1000;                    // kleinere Einheit verkleinert sich damit um 1000            Bsp: 998 
  } else {
    gk_stat_res.Rms      = Div;                           // kein Übertrag erforderlich, die Summe der Millis kann 1:1 übernommen werden Bsp2: 343
  }
  //----------------------------------
  // 2 b) Betrachtung der Sekunden
  //----------------------------------
  Div = gk_stat_res.RSeconds + gk_stat_res_help.RSeconds; // Ergebnis darf nicht > 59 sein da sonst Rest-60 einzustellen ist + 1 Minute Übertrag
  if (Div > 59) {
    gk_stat_res.RMinutes += 1; 
    gk_stat_res.RSeconds = Div - 60;
  } else {
    gk_stat_res.RSeconds = Div;
  }
  //----------------------------------
  // 2 c) Betrachtung der Minuten
  //----------------------------------
  Div   = gk_stat_res.RMinutes + gk_stat_res_help.RMinutes;
  if (Div > 59) {
    gk_stat_res.RHours   += 1;
    gk_stat_res.RMinutes = Div - 60;
  } else {
    gk_stat_res.RMinutes = Div;
  }
  //----------------------------------
  // 2 d) Betrachtung der Stunden
  //----------------------------------
  Div   = gk_stat_res.RHours + gk_stat_res_help.RHours;
  if (Div > 23) {
    gk_stat_res.RDays  += 1;
    gk_stat_res.RHours = Div -24;
  } else {
    gk_stat_res.RHours = Div;
  }
  
  //--------------------------------
  // 2 e) Betrachtung der Tage
  //--------------------------------
  Div   = gk_stat_res.RDays + gk_stat_res_help.RDays;
  if (Div > 364) {  // für diese Statistik hat jedes Jahr FIX 365 Tage
    gk_stat_res.RYears += 1;    // Achtung hier ist wegen der 5 Bit's nur Platz bis 31, danach erfolgt ein Überlauf auf 0 !! ich lasse das einfach mal unberücksichtigt da sehr unwahrscheinlich während der Lebensdauer der Hardware.
    gk_stat_res.RDays  = Div -365;
  } else {
    gk_stat_res.RDays = Div;
  }      
  
  //------------------------------------------------------------------
  // 3. nun steht in gk_stat_res die um  msCnt erhöhte Zeitdauer  als  .JAHR .TAGE .STUNDEN .MINUTEN  .SEKUNDEN  .MILLIS  zur Verfügung
  //------------------------------------------------------------------

  
}




//----------------------------------------------------------------------------
// 2018-04-01 Nächste bzw. vorgehende noch unbestätigte Fehlermeldungen
//            aus Error-Log ermitteln
//            Var = 1 nur Fehler die noch nicht bestätigt wurden
//            Var = 2 alle Fehler mit einer Gesamtanzahl > 0
//  FromErrNr = Ausgehend    Rückwärts <--- Fehler-Nr --->  Vorwärts
//  Direction = 1 = Vorwärts  2 = Rückwärts
//----------------------------------------------------------------------------
uint8_t Error_Next(uint8_t Var, uint8_t ErrNr, uint8_t Direction) {
  uint8_t  ErrCnt0=0;
  //uint8_t  ErrNrRet=0;
  if (Direction == 1) {
    for (uint8_t ErrI=1; ErrI <=_S22_RECORDS; ErrI++) {  // vorwärts solange bis die ErrNr überschritten ist
      switch (Var) {
        case 1: 
          ErrCnt0=Error_Read(ErrI,1);         // unbestätigte Fehleranzahl für Fehler-Nr ErrI
          if (ErrCnt0 > 0) { // ErrI ist also ein Treffer
            if (ErrI > ErrNr) {return ErrI; } // gefundener Error ist nun nach der angegebenen ErrNr 
            }   
          break;
        case 2:
          ErrCnt0=Error_Read(ErrI,1) + Error_Read(ErrI,2); // Summe aller Fehler für die FehlerNr 
          if (ErrCnt0 > 0) {
            if (ErrI > ErrNr) {return ErrI; }
          }
          break;
      }
    }
    // hier landet man wenn es keine nachfolgende Nummer mehr gibt
    return 0;  // Stillstand
  }
  if (Direction == 2) {
    for (uint8_t ErrI=_S22_RECORDS; ErrI >=1; ErrI--) {  // Rückwärts solange bis die ErrNr unterschritten ist
      switch (Var) {
        case 1: 
          ErrCnt0=Error_Read(ErrI,1);         // unbestätigte Fehleranzahl für Fehler-Nr ErrI
          if (ErrCnt0 > 0) { // ErrI ist also ein Treffer
            if (ErrI < ErrNr) {return ErrI; } // gefundener Error ist direkt vor der angegebenen ErrNr 
            }   
          break;
        case 2:
          ErrCnt0=Error_Read(ErrI,1) + Error_Read(ErrI,2); // Summe aller Fehler für die FehlerNr 
          if (ErrCnt0 > 0) {
            if (ErrI < ErrNr) {return ErrI; }
          }
          break;
      }
    }
    // hier landet man wenn es keine davorstehende Nummer mehr gibt
    return 0;  // Stillstand  
  }
  return 0;
}

//----------------------------------------------------------------------
// 2018-03-29 Anzahl gefüllter Fehlermeldungen in Error-Log ermitteln
//            Var = 1 nur Fehler die noch nicht bestätigt wurden
//            Var = 2 alle Fehler mit einer Gesamtanzahl > 0
//            Var = 3 alle im System möglichen Fehler 
//----------------------------------------------------------------------
uint8_t Error_Cnt(uint8_t Var) {
  uint8_t  ErrCnt0=0;
  uint8_t  ErrCntRet=0;

  for (uint8_t ErrI=1; ErrI <=_S22_RECORDS; ErrI++) {
    switch (Var) {
      case 1:
        ErrCnt0=Error_Read(ErrI,1);         // unbestätigte Fehleranzahl für Fehler-Nr ErrI
        if (ErrCnt0 > 0) {ErrCntRet ++; }   // Error hat unbestätigte Fehlermeldungen und wird nun gezählt
        break;
      case 2:
        ErrCnt0=Error_Read(ErrI,1) + Error_Read(ErrI,2); // Summe aller Fehler für die FehlerNr 
        if (ErrCnt0 > 0) {ErrCntRet ++; }  // Error hat Fehlermeldungen und wird nun gezählt
        break;
      case 3:
        ErrCntRet++;
        break;
    }
  }
  return ErrCntRet; // Anzahl der Fehlernummern die den Kriterien entsprechen
}


//-----------------------------------------------------------------------------------------------------------------
// 2019-03-03  liefert oder setzt zu einem HX711 Wiegemodul die Zahl welche aussagt ob das Modul kalibriert ist
//   eine gute Idee ist es 12345 als Wert zu wählen der aussagt daß das Modul fertig kalibriert ist.
//  WNr = Wiegemodul-Nummer 1...n 
//  NewCaliNr == 0  bedeutet es wird die aktuelle Zahl aus dem EEPROM gelesen und geliefert.
//  NewCaliNr != 1  bedeutet es wird diese Zahl in den EEPROM geschrieben.
// Jedes HX711 Modul benötigt im EEPROM 10 Byte   2 Byte für CaliNr  4 float für Fact  4 int32_t für Offset
//-----------------------------------------------------------------------------------------------------------------
int16_t Waage_CaliNr(int8_t WNr, int16_t = 0);                        // Per Default soll abgefragt werden
int16_t Waage_CaliNr(int8_t WNr, int16_t NewCaliNr) {
  int16_t Ret=0;
  if (WNr < 1) {WNr = 1;} 
  if (WNr > _HX711_CNT) { WNr = _HX711_CNT;}
  if (NewCaliNr == 0) {                                               // Abfragen 
    EEPROM.get(_EE_OFFS_HX711 +(WNr-1)*_EE_HX711_SIZE + 0,Ret);       // im EEPROM wird kein Platz verschenkt deshalb beginnt die Speicherung bei Stelle 0 
  } else {                                                            // Speichern
    EEPROM.put(_EE_OFFS_HX711 +(WNr-1)*_EE_HX711_SIZE + 0,NewCaliNr); // 
  }
  return Ret;   // geliefert wird der aktuelle EEPROM-Wert oder 0 wenn NewCaliNr geschrieben wird
}

float Waage_CaliFact(int8_t WNr, float = 0);                          // per Default soll abgefragt werden
float Waage_CaliFact(int8_t WNr, float NewFact) {
  float Ret=0;
  if (WNr < 1) {WNr = 1;} 
  if (WNr > _HX711_CNT) { WNr = _HX711_CNT;}
  if (NewFact == 0) {                                                 // Abfragen 
    EEPROM.get(_EE_OFFS_HX711 +(WNr-1)*_EE_HX711_SIZE + 2,Ret);       // im EEPROM wird kein Platz verschenkt deshalb beginnt die Speicherung bei Stelle 0 
  } else {                                                            // Speichern
    EEPROM.put(_EE_OFFS_HX711 +(WNr-1)*_EE_HX711_SIZE + 2,NewFact);   // 
  }
  return Ret;   // geliefert wird der aktuelle EEPROM-Wert oder 0 wenn NewFact geschrieben wird
}

int32_t Waage_Offset(int8_t WNr, int32_t = 0);
int32_t Waage_Offset(int8_t WNr, int32_t NewOffset) {
  int32_t Ret=0;
  if (WNr < 1) {WNr = 1;} 
  if (WNr > _HX711_CNT) { WNr = _HX711_CNT;}
  if (NewOffset == 0) {                                               // Abfragen 
    EEPROM.get(_EE_OFFS_HX711 +(WNr-1)*_EE_HX711_SIZE + 6,Ret);       // im EEPROM wird kein Platz verschenkt deshalb beginnt die Speicherung bei Stelle 0 
  } else {                                                            // Speichern
    EEPROM.put(_EE_OFFS_HX711 +(WNr-1)*_EE_HX711_SIZE + 6,NewOffset); // 
  }
  return Ret;   // geliefert wird der aktuelle EEPROM-Wert oder 0 wenn NewOffset geschrieben wird  
}


//-----------------------------------------------------------------------
// Lesen und liefern des Kalibrierungsfaktors für Waage01 float Pos: 0:4
//-----------------------------------------------------------------------
float Waage01_CaliFact() { 
  float CaliFakt=0;
  EEPROM.get(0,CaliFakt); // 
  return CaliFakt;
}
//------------------------------------------------------------
// Abspeichern des Kalibrierungs-Faktors zur Waage01  Pos 0:4
//------------------------------------------------------------
void Waage01_CaliFact_Save(float CaliFakt) {
  EEPROM.put(0,CaliFakt);
}
//---------------------------------------------------
// Lesen des Offset-Gewichtes  zur Wage01 Pos 4:4
//---------------------------------------------------
int32_t Waage01_Offset() {
  int32_t Offset=0;
  EEPROM.get(4,Offset);
  return Offset;
}
//-----------------------------------------------------
// Schreiben des Offset-Gewichtes zur Wage01 Pos 4:4
//-----------------------------------------------------
void Waage01_Offset_Save(int32_t Offset) {
  EEPROM.put(4,Offset);  
}




//----------------------------------------------------------------------------------------
// 2018-03-12  INITIALISIEREN EEPROM DATEN   Idee: hier sollten alle Aufrufe zum
//             grundsätzlichen Initialisieren der benützen Stellen im EEPROM erfolgen.
//             ACHTUNG bereits eingestellt Parameterwerte werden hier rückgesetzt !!!
//----------------------------------------------------------------------------------------
void EE_INIT() {
  Error_Clear(0, 4);    // Rücksetzen aller Fehlereinträge auf 0 und aller Fehlerzeitstempel auf 0
  // Para_Clear();         // Rückstellen aller Parameterwerte auf die Default-Werte laut TBL
 
  
}
