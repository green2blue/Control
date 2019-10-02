// ==============================================================================
// 2019-03-12  hier wird noch nicht die Bibliothek verwendet bei welcher der Messvorgang ohne CPU-Ressourcen durchgeführt werden kann.
//             Da die Messung erst einige Zeit nach dem Messvorgang abgeholt werden darf ist dafür ein Umbau nötig. (Task, Ram-Array mit den Ergebnissen,...)
//             Die Bibliothek ist aber bereits zu finden in den beiliegenden Bibliotheken. HX711_ADC-1.1.3 
//
// 2016-12-08: Gerhard  Wiegemodul einbinden und Wert einlesen.
// 2019-03-03: ein wenig überarbeitet
// ==============================================================================
// wird bereits im Control gemacht #include "HX711.h"        // Bibliothek zum Verwenden des Wiege-Element-Verstaerkermoduls
// Objekt Waage deklarieren und PIN zuweisen, das PIN zuweisen kann auch spater durch Waage01.begin(A1, A0) erfolgen, muß aber scheinbar direkt erfolgen ?

// der Offset sollte Temperaturkompensiert werden !!!

//  float Waage_CaliFact(int8_t WNr, float NewFact)       liefert oder schreibt Kalibrierungsfaktor
//  int16_t Waage_CaliNr(int8_t WNr, int16_t NewCaliNr)   liefert oder schreibt Offset 

HX711 Waage01(_HX711_DOUT_1,_HX711_CLK_1);   // weiß nicht wie das dynamisch möglich ist, daher 1:1 hardcoded and dieser Stelle
HX711 Waage02(_HX711_DOUT_2,_HX711_CLK_2);
HX711 Waage03(_HX711_DOUT_3,_HX711_CLK_3);


//-----------------------------------------------------------------------------------
// je nachdem wieviele Datensätze in TBL definiert sind werden HX711 Daten angelegt einstweilen hardcoded für maximal 3 Module
//-----------------------------------------------------------------------------------
uint8_t Init_HX711() {
  uint8_t InitCnt=0;
  uint8_t ReadCnt=0;
  int8_t PinDout=0;
  int8_t PinClk=0;

  for (uint8_t i = 1; i < _HX711_CNT; i++) {
    ReadCnt ++;
    PinDout = TBL_Int(T_Idx(_S_HXD,i,_S04_DOUT));
    PinClk  = TBL_Int(T_Idx(_S_HXD,i,_S04_CLK));

    if (PinDout==0 || PinClk==0) { continue; }          // Wenn eines der beiden Angaben fehlt wird der Eintrag als kein Wiegemodul erkannt.
    InitCnt ++;
    if (EE_ParaGet(7)==1) {Msg(19,InitCnt,PinDout,PinClk);}    
// folgender Teil funktioniert leider nicht, siehe oben, da ist das hardcoded

    if (InitCnt==1) {HX711 Waage01(PinDout,PinClk);}    // das geht bestimmt auch viel eleganter, nur fehlt mir dazu das C Knowhow  (dynamisches Erzeugen von Objekten)
    if (InitCnt==2) {HX711 Waage02(PinDout,PinClk);}
    if (InitCnt==3) {HX711 Waage03(PinDout,PinClk);}
     // Die zu jeder Waage gehörenden Werte float HX711_Fact  und int32_t HX711_Offset  sind im EEPROM abzulegen und abzurufen
     // Um im EEPROM erkennen zu können ob eine Kalibrierung bereits erfolgt ist wird ein 3. Wert im EEPROM abgelegt.
     // Dieser, ebenfalls 4 BYTE Wert erhält nach der Kalibrierung den Wert 12345.
     // Pro Wiegemodul werden also 12 BYTE im EEPROM belegt.
     // int32_t HX711_CalMarker 
     // float   HX711_Fact
     // int32_t HX711_Offset
  }
  Waage_arrange();  // Kalibrierungswerte einlesen soweit vorhanden
  if (EE_ParaGet(7)==1) {Msg(18,InitCnt); }           // Ausgabe nur wenn Parameter 7 = BOOT Details auf 1 gesetzt ist
  return InitCnt;                                     // Liefert die Anzahl Waagen die definiert wurden
}

//-----------------------------------------------------------------------------
// Waage-Objekt mit den im Kalibrierungsvorgang festgelegten Daten versorgen einstweilen hardcoded für maximal 3 Module
//-----------------------------------------------------------------------------
uint8_t Waage_arrange() {
  uint8_t InitCnt=0;
  int8_t PinDout=0;
  int8_t PinClk=0;
 
  for (uint8_t i = 1; i < _HX711_CNT; i++) {
    PinDout = TBL_Int(T_Idx(_S_HXD,i,_S04_DOUT));
    PinClk  = TBL_Int(T_Idx(_S_HXD,i,_S04_CLK));
    if (PinDout==0 || PinClk==0) { continue; }          // Wenn eines der beiden Angaben fehlt wird der Eintrag als kein Wiegemodul erkannt.
    InitCnt ++;
    if (Waage_CaliNr(InitCnt,0) != 12345) {
      // Waage ist noch nicht kalibriert worden !!
      if (EE_ParaGet(7)==1) {
        Msg(20,InitCnt,PinDout,PinClk);                 // Meldung daß Waage noch nicht kalibriert wurde
      }
    }
    if (InitCnt==1) {Waage01.set_offset(Waage_Offset(InitCnt,0));  Waage01.set_scale(Waage_CaliFact(InitCnt,0)); }
    if (InitCnt==2) {Waage02.set_offset(Waage_Offset(InitCnt,0));  Waage02.set_scale(Waage_CaliFact(InitCnt,0)); }
    if (InitCnt==3) {Waage03.set_offset(Waage_Offset(InitCnt,0));  Waage03.set_scale(Waage_CaliFact(InitCnt,0)); }  
  }
  return InitCnt;
}

//---------------------------------------------------------------------------------------------------
// 2019-03-09  prüfen ob das Wiegemodul angeschossen ist und arbeitet liefert 1 wenn OK sonst 0
//  Es gibt dafür die Waage01.is_ready() welche ebenfalls 1 liefert wenn die Waage bereit ist
//  oder die Methode ob PulseIn auf den Anschlüssen daherkommen, üblich sind 76 PulsIN auf HIGH 
//  und 86454 PulsIn auf LOW.
//  Var=0 Methode über PulseIn
//  Var=1 Methode .is_ready()
//------------------------------------------------------------------------------------------------
int8_t Waage_ready(int8_t = 1, int8_t = 0);  // Defaultparameter für Waage-Nummer = 1 // Defaultparameter für Methode = 0
int8_t Waage_ready(int8_t WNr, int8_t Var) {
  uint16_t check=0;
  if (Var==0) {
    check=pulseIn(TBL_Int(T_Idx(_S_HXD, 1, _S04_DOUT)), HIGH);    // Zählen ob am Pin Impulse daherkommen, hier wird vorausgesetzt daß bei der Definition der Waagen keine Lücken in TBL vorliegen, sonst müsste man in einer Schleife die 1.sinnvoll definierte Waage aus TBL suchen !
    if (check==0) {                                               // Modul liefert keine Impulse. Normalerweise komme hier ca. 74 Pulse an.
          // while (pulseIn(HX711_01_DOUT, HIGH) ==0) { }  // Warten bis Impulse ankommen !!!!! Endlosschleife !!!!
      check=0;
          //Serial.print(F("HX711-Modul nicht aktiv, arbeitet nicht")); Serial.println();
      return 0;
    } 
  }
  if (Var==1) {
    if (WNr==1) { return Waage01.is_ready(); }
    if (WNr==2) { return Waage02.is_ready(); }
    if (WNr==3) { return Waage03.is_ready(); }
  }
  return 1; // Modul arbeitet
}

//-----------------------------------------------------------------------------
// Waage  Gewicht ablesen und als 1=g  100=dag  1000=kg  liefern
//-----------------------------------------------------------------------------
float Waage_read(int8_t = 1, int8_t = 1);             // Default Waage-Nummer = 1  // Default als Gramm = 1
float Waage_read(int8_t WNr, int8_t Eh) {
  float Weight=0;
  if (Eh==0) {Eh=1;}                      // Damit es zu keiner Div0 kommt
     // WorkULong[0] = micros();          // Merken des Zeitstempels vor dem Wiegevorgang
  if (WNr==1) {
    Weight=Waage01.get_units(1);            // Gewicht lesen in Gramm 1x damit die Messung besser stimmt
  }
  if (WNr==2) {
    Weight=Waage02.get_units(1);            // Gewicht lesen in Gramm 1x damit die Messung besser stimmt
  }
  if (WNr==3) {
    Weight=Waage03.get_units(1);            // Gewicht lesen in Gramm 1x damit die Messung besser stimmt
  }  
  
  if (Eh==2) {Weight = Weight / 100; }      // auf dag
  if (Eh==3) {Weight = Weight / 1000;}      // auf kg
  
    // WorkULong[1] = micros();              // Zeitstempel nach dem Wiegen
    // WorkULong[0]= WorkULong[1]-WorkULong[0]; // benötigte Microsekunden
    // Serial.print(F(" \nWaage01.get_units(5) ENDE  --> Weight:"));Serial.print(Weight);
    // Serial.print(F(" benötigte micros:"));Serial.print(WorkULong[0]);Serial.println();
    // Serial.print(F("Waage01 Weight:"));Serial.print(Weight/Eh,0);
    // switch (Eh) {
    //   case 1: Serial.println(F(" [g]")); break;
    //   case 2: Serial.println(F(" [dag]")); break;
    //   case 3: Serial.println(F(" [kg]")); break;
    // }
  return Weight;
}


//-----------------------------------------------------------------------------------------------------
// 2019-03-09  prüft ob das Gewicht überschritten,unterschritten 
//  Liefert -32000 bei Modulfehler
//  Liefert Var=1    0=innerhalb der Min_g und Max_g    -1 = unter MIN   +1 = über MAX
//          Var=2    0= OK     - Zahl [g] unter Min    + Zahl [g] über Max 
//          Var=3    0= OK     - Zahl [g] unter DS     + Zahl [g] über DS   DS=(Max+Min)/2
//          Var=4    0= OK     -1..-126   unter DS     +1..+127   über DS   (damit könnten 2 LED eine für Untergewicht und eine für Übergewicht betrieben werden)
//-----------------------------------------------------------------------------------------------------
int16_t Waage_check(int8_t WNr, int8_t Var, int16_t Min_g, int16_t Max_g ) {
  int16_t isW=0;
  int16_t Low=0;  int16_t Hig=0;
  int16_t Min= Min_g; // GetpVal(15);         // Minimum-Gewicht Parameter 
  int16_t Max= Max_g; // GetpVal(16);         // Maximum-Gewicht Parameter

  isW = Waage_read(WNr);                      // Gewicht in g lesen. 
  
  Low= isW - Min;                             // Ist-Gewicht  -    Minimum-Gewicht  ergibt eine  -Zahl wenn man unter MINIMUM liegt
  Hig= Max - isW;                             // Maximal-Gewicht - Ist-Gewicht      ergibt eine  -Zahl wenn man über MAXIMUM liegt
  
  if (Low < 0 || Hig < 0)  {                  // Gewicht nicht OK wenn eine der beiden Zahlen < 0 ist 
    if (Var==1) {                             // Rückgabe-Variante 1
      if (Low < 0) { return -1; }             // Gewicht unter MINIMUM
      else         { return +1; }             // Gewicht über  MAX
    }
    if (Var==2) {                             // exakte [g]  unter Min oder  über Max
      if (Low < 0) { return Low; }            // Zahl unter MIN 
      else         { return -Hig; }           // Zahl  über MAX
    }
  } else {                                    // wenn weder Low noch Hig < 0 ist 
    if (Var==1 || Var==2) {return 0;}         // alles gut da man bei diesen Varianten nur innerhalb der Grenzen liegen muß
  }
  
  int16_t DS = (Min+Max)/2;                   // Mittelwert   
  Low= isW - DS; 
  Hig= DS - isW; 
  if (Low < 0 || Hig < 0) {                   // Gewicht liegt vermutlich leicht über oder unter dem Durchschnitt
    if (Var==3) {        
      if (Low < 0) { return Low; } 
      else         { return -Hig; }
    }
    if (Var==4) { return -(127- map(Low, -DS, 0, 0, 127)); }
  } 
  else { return 0; }
  return 0;
}

//------------------------------------------------------------------------------------
// 2019-03-11  TARA für Gewichtsmodul festlegen bzw. 0 stellen
//------------------------------------------------------------------------------------
int8_t HX711_UART_Tara(int8_t WNr, int32_t = 0);
int8_t HX711_UART_Tara(int8_t WNr, int32_t Tara) {
  int32_t wTara = 0;                                            // gewogenes Tara bzw. mitgegebenes Tara
  char    SChars[]= {'1','9'}; 

  if (Tara != 0) {wTara = Tara; } 
   else { wTara = WNr_read_average(WNr,10); }                   // Gewicht auf der Waage ermitteln
  Msg(29 ,WNr,Waage_Offset(WNr,0),wTara );                      // Waage %ld mit eingestelltem  %ld[?] jetzt auf %ld[?] auf stellen 1=ja 9=abbrechen 
  SetWatchDog(0);                                               // arduino wird hier angehalten bis der Anwender eine Antwort gibt. Der Watchdog hier keinen Reset auslösen.

  if (Wait4(SChars,2)=='9') {Msg(1);goto HX711_UART_Tara_END; } // abgebrochen

  WNr_set_offset(WNr, wTara);                                   // Offset einstellen auf das oben ermittelte LEERGEWICHT oder mitgegebene Gewicht
  Waage_Offset  (WNr, wTara);                                   // neuen Offset im EEPROM zur Waage speichern.

  Msg(28);                                                      // Meldung gespeichert
HX711_UART_Tara_END:
  SetWatchDog(1);                                               // Watchdog wieder einschalten
  return 0;
}


//------------------------------------------------------------------------------------
// 2019-03-10  Modul + Messsensor Kalibrierung   arduino ist damit BLOCKIERT !!
//  K A L I B R I E R U N G  
//  über UART Fragen Antwort Spiel, der Arduino wird dabei vollständig blockiert und
//  steht für keine anderen Tätigkeiten zur Verfügung. 
// Bei der Kalibrierung über das LCD-Menü läuft hingegen alles im Hintergrund weiter.
// liefert 0 wenn Kalibrierung ABGEBROCHEN wurde 
//         1 = Kalibrierung wurde durchgeführt und damit die Werte auch im EEPROM gespeichert.
//------------------------------------------------------------------------------------
int8_t HX711_UART_Calibration(int8_t WNr) {                 // Vorgang der Kalibrierung über UART 
  char    SChars[]= {'1','9'};                              // auf diese Zeichen wird von der UART gewartet  1=weiter  9=abbrechen
  float   Scale=0;                                          // errechneter Skalierungsfaktor zum Modul
  int32_t Scalef=0;                                         // für die formatierte Ausgabe 
  int32_t Tara=0;                                           // Tara-Gewicht zum Modul 
  int8_t  RetVal=0;                                         // Rückgabewert 0=Kalibrierung abgebrochen 1=Kalibrierung durchgeführt und gespeichert
  
  SetWatchDog(0);                                           // Watchdog DEAKTIVIEREN da hier der adrduino blockiert wird.

  Msg(21,WNr,EE_ParaGet(8));                                // Start Kalibrierung für Modul Nr Kalibrierungsgewicht von x [g] wird benötigt oder ändere Parameter 8
  Msg(22,WNr);                                              // Schritt 1 v 4  Waage frei machen  1=ok  9=Abbrechen 
  if (Wait4(SChars,2)=='9') {Msg(1);goto HX711_Cali_END;}   // Wartet auf eine Eingabe von 1=weiter  9=abbrechen  und meldet bei 9 abgebrochen
  
  Msg(23,WNr);                                              // Schritt 2 v 4  Leerzustand wird ermittelt
  WNr_set_scale(WNr,0);                                     // Waageskalierung initialisieren
  WNr_read_average(WNr,10);                                 // Leer-Gewicht von Modul lesen nur damit sich das Modul einpendelt.
  Tara = WNr_read_average(WNr,10);                          // Leer-Gewicht von Modul lesen und MERKEN 

  Msg(24,EE_ParaGet(8),WNr);                                // Schritt 3 v 4  Waage mit Kalibrierungsgewicht belegen 

  if (Wait4(SChars,2)=='9') {Msg(1);goto HX711_Cali_END;}   // Wartet erneut auf eine Eingabe 
  
  Msg(25,WNr,EE_ParaGet(8));                                // Schritt 4 v 4  Kalibrierung mit Gewicht läuft ...
  WNr_set_offset(WNr,Tara);                                 // Offset einstellen auf das oben ermittelte LEERGEWICHT
  WNr_get_units(WNr,10);                                    // Wiegevorgang durchführen
  Scale = (WNr_get_units(WNr,10) / EE_ParaGet(8));          // Faktor errechnen
  WNr_set_scale(WNr, Scale);                                // Faktor beim Objekt speichern

  Msg(26,WNr);                                              // Kalibrierung abgeschlossen, Referenzgewicht kann entfernt werden.
  Scalef=Scale;
  Msg(27,WNr,Tara,Scalef);                                  // Speichern der Kalibrierung 1=Ja 9=abbrechen 
  
  if (Wait4(SChars,2)=='9') {Msg(1);goto HX711_Cali_END;}   // Warten auf Eingabe

  Waage_CaliFact(WNr, Scale);                               // Speichern Skalierungsfaktor zum Wiegemodul
  Waage_Offset  (WNr, Tara);                                // Speichern Tara zum Wiegemodul
  Waage_CaliNr  (WNr, 12345);                               // MERKER daß dieses Modul kalibriert wurde.

  Msg(28);                                                  // Meldung gespeichert
    //  while (SIn != '9') {
    //    SIn=Serial.read();
    //    Serial.print(F(" aktuelles Gewicht:"));Serial.print(WNr_get_units(WNr,5));Serial.print(F("[g]"));Serial.println();
    //  } 
    
HX711_Cali_END:  
  SetWatchDog(1);                                           // Watchdog wieder AKTIVIEREN.
  return RetVal;
}

float WNr_get_units(int8_t WNr, int8_t RCnt) {
  float RetVal=0;
  if (WNr == 1) { RetVal= Waage01.get_units(RCnt); }
  if (WNr == 2) { RetVal= Waage02.get_units(RCnt); }
  if (WNr == 3) { RetVal= Waage03.get_units(RCnt); }
  return RetVal;
}

void WNr_set_offset(int8_t WNr, int32_t Tara) {
  if (WNr == 1) { Waage01.set_offset(Tara); }
  if (WNr == 2) { Waage02.set_offset(Tara); }
  if (WNr == 3) { Waage03.set_offset(Tara); }  
}

void WNr_set_scale(int8_t WNr, float ScaleVal) {
  if (WNr == 1) {if (ScaleVal !=0) { Waage01.set_scale(ScaleVal); } else { Waage01.set_scale();} }
  if (WNr == 2) {if (ScaleVal !=0) { Waage02.set_scale(ScaleVal); } else { Waage02.set_scale();} }
  if (WNr == 3) {if (ScaleVal !=0) { Waage03.set_scale(ScaleVal); } else { Waage03.set_scale();} }
}

int32_t WNr_read_average(int8_t WNr, int8_t RCnt) {
  int32_t RetVal=0;
  if (WNr == 1) { RetVal= Waage01.read_average(RCnt); }
  if (WNr == 2) { RetVal= Waage02.read_average(RCnt); }
  if (WNr == 3) { RetVal= Waage03.read_average(RCnt); }
  return RetVal;
}

//------------------------------------------------------------------------------------------------------
// Es wird eine Eingabe an die UART erwartet bis diese einem der Zeichen im Array CharArr[] entspricht
// Bsp: Wait4('1,9', 2)  liest solange von der UART  bis  1 oder 9 angekommen sind.
//------------------------------------------------------------------------------------------------------
char Wait4(char CharArr[], int8_t CharCnt) {
  int8_t  Ci;                                     // Zähler für Schleife
  char    SIn=0x00;                               // eingelesenes Zeichen von UART
  while (SIn != CharArr[0] ) {
    for (Ci=0; Ci <= CharCnt; Ci++) {
      if (SIn == CharArr[Ci]) { Ci=99; break; }   // Sobald ein gültiges Zeichen angekommen ist wird die for-Schleife und in Folge auch die while-Schleife verlassen
    }
    if (Ci == 99) {break; }                       // Es wurde in der for-Schleife ein passendes Zeichen gefunden
    SIn=Serial.read();
  }
  return SIn;
}
