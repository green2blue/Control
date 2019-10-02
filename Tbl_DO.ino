// Funktionen die im Zusammenhant mit TBL etwas ausführen.

// ----------------------------------------------------------------------------------------------------------
// 2017-11-26  Funktion zum  I N I T I A L I S I E R E N   von in Tbl deklarierten   E I N G Ä N G E N, 
// Es werden nur jene EINGÄNGE hier beachtet die in iher Definition  
// geliefert wird die Anzahl INPUT gesetzter Eingänge.
// ----------------------------------------------------------------------------------------------------------
uint8_t Init_INPUTS() {return Init_INPUTS(1, _S28_RECORDS);}
uint8_t Init_INPUTS(uint8_t VNr, uint8_t TNr) {
  uint8_t   InitCnt=0;                                          // Anzahl initialisierter Eingänge 
  uint8_t   ReadCnt=0;                                          // Anzahl gelesener Eingänge-Tabelleneinträge
  int16_t   IDo=0; 
      //---Parametergrenzen prüfen und ggf. setzen---- 
  if (VNr==0)             {VNr= 1;}                             // es geht erst ab  1 los
  if (TNr > _S28_RECORDS) {TNr= _S28_RECORDS;}                  // Maximale Anzahl  aus I_Table Compilervariable holen
  if (TNr < VNr)          {TNr= VNr;}                           // wenn das BIS < VON dann wird BIS=VON es wird also nur EIN Objekt angesprochen 
      //---Ende prüfen Parametergrenzen---------------
  for (uint8_t i = VNr; i < TNr; i++) {                         // für jeden in Tbl definierten Eingang
      ReadCnt ++;  
      IDo= TBL_Int(T_Idx(_S_IND,i,_S29_AT_PWRON_DO));           // 0=ignorieren  1=INPUT_PULLUP  2=INPUT
      if (IDo != 0) {
        InitCnt ++;
        TBL_Numbers(T_Idx(_S_IND,i,_S29_PIN_ADR_BIT));          // sbufferNr[0] PIN ;sbufferNr[1] I2C-Adr  ;sbufferNr[2] I2C-Bit
        
          // Serial.print(F(" Init_INPUTS i:"));Serial.print(i);Serial.print(F(" sbufferNr[0]:"));Serial.print(sbufferNr[0]);Serial.print(F(" IDo:"));Serial.println(IDo);
        if (sbufferNr[0] != 0) {                                // wenn ein direkter arduino-PIN angesprochen ist
          pinMode(sbufferNr[0], INPUT);
          if (IDo==1) {digitalWrite(sbufferNr[0],INPUT_PULLUP);} 
        }
        if (sbufferNr[1] != 0) {                                // wenn ein I2C-Portexpandermodul angesprochen ist
          // ein Eingang wird definiert indem der jeweilige PIN auf HIGH gesetzt wird.
        }
      }
  }
  if (EE_ParaGet(7)==1) {Msg(14,InitCnt);}                      // BOOT Details anzeigen  ? 
   
  return InitCnt; // zurückgeben der Anzahl 
}  // Ende Initialize_Inputs

// ----------------------------------------------------------------------------------------------------------
// 2017-11-26  Funktion zum I N I T I A L I S I E R E N   von Maschinen(AUSGÄNGEN), 
//             das sind in Tbl definierte Einträge im Abschnitt MASCHINEN/Geräte welche letztendlich 
//             AUSGÄNGE/PIN's  und deren Zustände nach dem RESET definieren. 
// geliefert wird die Anzahl der gesetzten PIN's 
// ----------------------------------------------------------------------------------------------------------
uint8_t Init_MACHINES() {return Init_MACHINES(1, _MACHINE_COUNT);}
uint8_t Init_MACHINES(uint8_t VNr, uint8_t TNr) {
  uint8_t  InitCnt=0;                                           // Anzahl initialisierter Ausgänge 
  uint8_t  ReadCnt=0;                                           // Anzahl gelesener Maschinen-Ausgänge
  int16_t  MDo=0;                                               // Befehl für das Gerät  0=nix  1=OFF 2=ON 3..255 PWM, ...
      //---Parametergrenzen prüfen und ggf. setzen---- 
  if (VNr==0)           {VNr= 1;}                               // es geht erst ab Maschine 1 los
  if (TNr > _MACHINE_COUNT) {TNr = _MACHINE_COUNT;}             // Maximale Anzahl Maschinen 
  if (TNr < VNr)     {TNr = VNr;}                               // wenn das BIS < VON dann wird BIS=VON es wird also nur EINE Maschine angesprochen 
      //---Ende prüfen Parametergrenzen---------------
  for (uint8_t i = VNr; i < TNr; i++) {                         // für jede Maschinen-Nummer
      ReadCnt ++;                                               // Zähler für Anzahl gelesener Maschinen erhöhen
      MDo=TBL_Int(T_Idx(_S_GED,i,_S31_AT_PWRON_DO));            // Aktion die für die Maschine beim BOOT auszuführen ist  0=NICHTS  1=OFF  2=ON  3..255 PWM

      if (MDo != 0) {                                           // wenn beim BOOT etwas zu tun ist
        InitCnt ++;
        TBL_Numbers(T_Idx(_S_GED,i,_S31_PIN_I2CADDR_BIT));      //  sbufferNr[0] PIN ;sbufferNr[1] I2C-Adr  ;sbufferNr[2] I2C-Bit
        if (sbufferNr[0] != 0) {                                // ein direkter arduino-PIN ist angesprochen
          pinMode(sbufferNr[0],OUTPUT);                         // damit geht der PIN bereits sofort LOW und schaltet somit voll durch !
        }
        Machine_DO(i,MDo,0,0,0);                                // Maschinen-Nummer i, mache, fq, dauer, statusnichtmerken
      }
  }
  if (EE_ParaGet(7)==1) {Msg(15,InitCnt); }                     // Ausgabe nur wenn Parameter 7 = BOOT Details auf 1 gesetzt ist
  return InitCnt;                                               // zurückgeben der Anzahl gesetzter Ausgänge
}   

//---------------------------------------------------------------------------------------------------------
// 2019-03-02 Initialisieren von KOMBINATIONEN  die aus 1 bis n Maschinenzuständen bestehen
//   
//
//---------------------------------------------------------------------------------------------------------
uint8_t Init_COMBINATIONS() {return Init_COMBINATIONS(1, _KOMBINATION_COUNT);}
uint8_t Init_COMBINATIONS(uint8_t VNr, uint8_t TNr) {
  uint8_t InitCnt=0;      // Anzahl initialisierter Combinationen
  uint8_t ReadCnt=0;      // Anzahl gelesener Datensätze
  uint16_t CToDo=3;
  if (VNr==0)   {VNr=1;}
  if (TNr > _KOMBINATION_COUNT) {TNr = _KOMBINATION_COUNT; }
  if (TNr < VNr)                {TNr = VNr; }

  for (uint8_t i = VNr; i < TNr; i++) {
    ReadCnt ++;
    InitCnt+= Combination_DO(i, CToDo,0,0);   // Ausführen des im AT_PWON_DO Feld definierten Ausdruckes der Combination
  }
  if (EE_ParaGet(7)==1) {Msg(16,InitCnt);}    // Ausgabe wenn Parameter BOOT Details auf 1 

  return InitCnt;
}


//----------------------------------------------------------------------------------------------------------------------------
// 2019-02-17  Schalten einer einzelnen bzw. einer Kombination von Geräten quasi zur gleichen Zeit.
//             Eine Kombination besteht aus einer Zeichenfolge mit Geräten und optionalen Parametern. 
//             Es gibt die Folge a) für Zustand nach BOOT
//                               b) für Zustand EIN
//                               c) für Zustand AUS
//  Bsp einer Folge: g17 p12, g18 2, g20 p11  
//    bedeutet: schalte gerät 17 auf Wert laut Parameter 12, schalte gerät 18 EIN, schalte gerät 20 auf Wert laut Parameter 11
// 2017-12-03  Funktion   S C H A L T E N   einer KOMBINATION 
//  Was passiert hier:
//  Anmerkung: Eine Combination kennt keine zeitlichen Unterschiede, nur eine Reihe von Maschinen die hintereinander in maximaler 
//             Ausführungs-Geschwindigkeit bestimmte Zustände geschaltet werden.
//             Für die zusätzliche zeitliche Komponente gibt es die Macros (siehe RunMacro() )
// Parameter: CIdx =  Combination-Nummer 
//   CToDo = 0 nix  1= 1:1 Umsetzen der im Feld _S33_OFF_DO       OFF  definierten Folge 
//                  2= 1:1 Umsetzen der im Feld _S33_ON_DO        ON   definierten Folge
//                  3= 1:1 Umsetzen der im Feld _S33_AT_PWRON_DO  INIT definierten Folge
//            4=  jedes im S33_OFF_DO       erwähnte Gerät auf OFF schalten, unabhängig was beim Zustand in S33_OFF_DO steht.
//            5=  jedes im S33_ON_DO        erwähnte Gerät auf OFF schalten, unabhängig was beim Zustand beschrieben steht.
//            6=  jedes im S33_AT_PWRON_DO  erwähnte Gerät auf OFF unabhängig vom Beschriebenen.
//
//            7=  jedes im S33_OFF_DO       erwähnte Gerät auf ON  schalten, unabhängig
//            8=  jedes im S33_ON_DO        erwähnte Gerät auf ON  schalten, unabhängig
//            9=  jedes im S33_AT_PWRON_DO  erwähnte Gerät auf ON  schalten, unabhängig
//
//           11=  Invertiere jedes in _S33_OFF_DO       erwähnten Gerät den aktuellen Zustand
//           12=  Invertiere jeden in _S33_ON_DO        erwähnten Gerät den aktuellen Zustand
//           13=  Invertiere jeden in _S33_AT_PWR_ON_DO erwähnten Zustand 
//  SaveNoState = Speichere nicht den Zustand der Kombination. Gerätezustände werden von dieser Einstellung nicht beeinflußt 
//  SaveNoGState= Speichere nicht den Zustand der betroffenen Geräte             GEHT NICHT in Verbindung mit CToDo 11,12,13 da hier jeder Zustand INVERTIERT werden soll
// 
//----------------------------------------------------------------------------------------------------------------------------
uint8_t Combination_DO(uint8_t CIdx,uint16_t CToDo, uint8_t = 0);  // Default Save State 
uint8_t Combination_DO(uint8_t CIdx,uint16_t CToDo, uint8_t SaveNoState, uint8_t = 0); 
uint8_t Combination_DO(uint8_t CIdx,uint16_t CToDo, uint8_t SaveNoState, uint8_t SaveNoGState) {
  uint8_t   DoCnt       = 0;              // Zähler/Merker ob etwas ausführbares vorgefunden wurde, also eine Definition mit Inhalt
  int16_t   bStartPos   = 0;
  int16_t   bEndPos     = Buffer1Len;
  char      bChar1      = ' ';
  char      VZ          = ' ';            // Merker vom Vorzeichen beim Durchsuchen der Befehlskette
  uint8_t   buffI       = 0;
  char *    b_pt        = & Buffer1[0];
  uint8_t FieldNr=250;                    // FieldNr die benötigt wird um damit das Kommando einzulesen  250=ungültig-Merker !!! 
                                        
    if (CIdx==0 || CIdx > _KOMBINATION_COUNT )              {return 0;}                         // es geht erst ab Kombination  1 richtig  los

    // Abhängig vom CToDo sind unterschiedliche Zeilen aus TBL einzulesen / umzusetzen
    if      (CToDo == 1 || CToDo == 4 || CToDo == 7 || CToDo == 11) FieldNr= _S33_OFF_DO;       // AUS-Sequenz-Zeile wird benötigt 
    else if (CToDo == 2 || CToDo == 5 || CToDo == 8 || CToDo == 12) FieldNr= _S33_ON_DO;        // ON-Zeile wird benötigt
    else if (CToDo == 3 || CToDo == 6 || CToDo == 9 || CToDo == 13) FieldNr= _S33_AT_PWRON_DO;  // INIT-Zeile

    if (FieldNr == 250) {                                                                       // Fehlerhafter CToDo Befehl mitgegeben 
      Notify(12,0); Serial.print(CToDo); Notify(13);
      // Serial.print(F("Fehlerhafter Aufruf von Combination_DO Parameter CToDo:"));Serial.print(CToDo);Serial.println(F("unbekannt"));         
      return 0;
    }

    F2X(T_Idx(_S_KOD, CIdx, FieldNr));                                                          // Einlesen des benötigten Feldes in Buffer1[] 
    Buffer1Len = strlen(Buffer1);                                                               // Länge der eingelesenen Zeichenkette

    if (Buffer1Len > 0 ) {DoCnt+=1; ReplacePwithVal(); }                                        // ev. angeführte PARAMETER durch die echten Werte ersetzen


    // BUFFER mit den eingebauten Parameterwerten MERKEN da dieser Buffer1 vom Machine_DO übermalt wird

    
    if (CToDo==11 || CToDo==12 || CToDo==13) SaveNoGState = 0;                                  // Weil diese INVERTIER-Funktionen einen gesicherten Status der Geräte erfordern
    if (SaveNoState == 0) {
      SaveKStatus(CIdx, CToDo);  // STATUS der Kombination setzen wie oben beschrieben    1,2,3,  4,5,6,  7,8,9,  11,12,13   im RAM-Array speichern
    }
    bEndPos = Buffer1Len; 
   // in Buffer1[] etwas wie:  g12 200, g17 1  , g12 2  ,  g19 100   --> das ist nun nach unterschiedlichen Methoden abzuarbeiten 
   //------------------------------------------------------------------------------------------
   // Zerlegen und Abarbeiten der Kette  g Nummer Nummer, g Nummer Nummer , g Nummer Nummer 
   //------------------------------------------------------------------------------------------
   //     Serial.print(F(" C_Do Buffer1:"));Serial.println(Buffer1);
    do {
      bStartPos=GetPosNEQ(Buffer1,bStartPos,bEndPos,bChar1,' ',',');        // nächstes gültiges Zeichen, alles außer LEER
      if (bChar1 != 0x00) {                                                 // es wurde ein gültiges Zeichen gefunden
        if (bChar1 == 'g') {                                                // ein 'g' ist ein Zeichen daß nun eine Gerätenummer folgen muß ( g darf aber auch weggelassen werden siehe ELSE)
          bStartPos ++;
          bStartPos=GetPosNEQ(Buffer1,bStartPos,bEndPos,bChar1,' ');        // Stelle suchen an welcher die Zahl beginnt
          sbufferNr[buffI] = atol(b_pt + bStartPos);                        // Geräte-Nummer merken
          buffI ++;
          bStartPos=GetPosEQ (Buffer1,bStartPos,bEndPos,bChar1,' ');        // wo kommt nach der Gerätenummer das LEER
          if (bChar1 != 0x00) {
            bStartPos ++;                                                   // LEER gefunden
            bStartPos=GetPosNEQ(Buffer1,bStartPos,bEndPos,bChar1,' ');      // wo beginnt nach den LEER  der gewünschte Schaltzustand zur Gerätenummer  
            VZ = ' ';
            if (bChar1=='+' || bChar1=='-') {
              VZ = bChar1;
              sbufferNr[buffI]=1000;
              bStartPos ++;
              bStartPos=GetPosNEQ(Buffer1,bStartPos,bEndPos,bChar1,' ');
            }
            if (VZ== '-') {sbufferNr[buffI]+= -atol(b_pt + bStartPos);}
                else      {sbufferNr[buffI]+=  atol(b_pt + bStartPos);}  
          }
        } else {                                                            // Wenn kein 'g' sondern was anderes kommt wird angenommen es kommt nun sofort die Gerätenummer
          sbufferNr[buffI] = atol(b_pt + bStartPos);                        // Geräte-Nummer merken
          buffI ++;
          bStartPos=GetPosEQ (Buffer1,bStartPos,bEndPos,bChar1,' ');        // nach dem Beginn der Gerätenummer das nächste LEER suchen
          if (bChar1 != 0x00) {
            bStartPos ++;
            bStartPos=GetPosNEQ(Buffer1,bStartPos,bEndPos,bChar1,' ');      // wo beginnt nach dem LEER  der gewünschte Schaltzustand bzw. + - zur Gerätenummer
            VZ = ' ';
            if (bChar1=='+' || bChar1=='-') {
              VZ = bChar1; 
              sbufferNr[buffI]=1000;   
              bStartPos ++;                                     
              bStartPos=GetPosNEQ(Buffer1,bStartPos,bEndPos,bChar1,' ');    // wo beginnt nach dem Vorzeichen die echte Zahl 
            }
            if (VZ== '-') {sbufferNr[buffI]+= -atol(b_pt + bStartPos);} 
               else       {sbufferNr[buffI]+=  atol(b_pt + bStartPos);}     // Schaltzustand merken  
          }
        }
        if (buffI == 1) {                                                   // nun ist in buffI[0] die Gerätenummer und in sbufferNr[1] der Schaltzustand laut Tbl_ gefunden
          //----------------------------------------------------
          //  Maschine wie gewünscht SCHALTEN  !!!!
          //----------------------------------------------------
          switch (CToDo ) {
            case  4:                              // jedes erwähnte Gerät auf OFF
            case  5:                              // jedes erwähnte Gerät auf OFF
            case  6: sbufferNr[1] =  1; break;    // jedes erwähnte Gerät auf OFF
            
            case  7:
            case  8:
            case  9: sbufferNr[1] =  2; break;    // jedes erwähnte Gerät auf ON

            case 11:
            case 12:
            case 13: sbufferNr[1] = 258; break;   // invertiere von jedem erwähnten Gerät den Zustand
          }
              //Serial.print(F(" C_Do: "));Serial.print(sbufferNr[0]);Serial.print(F("  "));Serial.println(sbufferNr[1]); 
              
          Machine_DO(sbufferNr[0], sbufferNr[1] , 0, 0, SaveNoGState);    //  Machine_DO verwendet den kleinen Buffer2 damit der hier benötigte Buffer1 nicht zerstört wird
  
        }
        sbufferNr[0]=0; sbufferNr[1]=0;  buffI = 0;                       // Aufräumen 
        bStartPos=GetPosEQ(Buffer1,bStartPos,bEndPos,bChar1,',',' ');     // nun sollte ein ',' oder ein ' ' folgen  der erste welcher vorkommt zählt
      }
    } while (bChar1 != 0x00); 
 
  return DoCnt;
}



//-----------------------------------------------------------------
// STATUS der Kombination setzen wie oben beschrieben
//  1,2,3,  4,5,6,  7,8,9,  11,12,13   im RAM-Array speichern
//-----------------------------------------------------------------
void SaveKStatus(uint8_t CIdx, uint16_t CToDo) {
  // if (CIdx == 0 || CIdx > _KOMBINATION_COUNT) return;  // Prüfung der Grenzen kann entfallen wenn die Funktion nur mit gültigen Parametern aufgerufen wird
  obj_Status_BIT(UART_KOM,CIdx,CToDo);                    // BINÄREN Zustand interpretieren und speichern  bei 1 wird AUS   bei >=2 wird EIN interpretiert 
  obj_Status_DEC(UART_KOM,CIdx,CToDo);                    // DETAIL Zustand speichern 
}

// -----------------------------------------------------------------------------------------------------
// 2019-02-10  Schalten eines laut TBL definierten Gerätes/Ausganges. !!!!  HIER WIRD BUFFER2 verwendet !!!!!
//  Mit dem Schalten wird der Schalt-Zustand, OPTIONAL NICHT, in den Arrays obj_stateBIN[] und obj_stateDEC[] gespeichert 
//    damit man den Schaltzustand jederzeit abfragen kann oder zum Vorzustand zurückschalten kann.
// Eine in der TBL_ definierte Maschine/Gerät  1=AUS/2=EIN/3..255 PWM schalten  
//             ebenfalls kann eine Frequenz für eine bestimmte Dauer ausgegeben werden.
// Der Funktion wird die Nummer der Maschine mitgegeben und ein 
//    ToDo: 0=tu nix   1 = OFF  2 = ON  3..255 = PWM bzw. ON falls PWM nicht möglich ist.
//          256=(eigentlich außerhalb des möglichen Schaltzustandes) stellt den Zustand her der im RAM gespeichert ist.
//          257=invertiert den aktuell im RAM gespeicherten Zustand
//    ToDo: ...1000... --> erhöhe/verkleinere aktuellen Wert um  den Mittelwert von 1000
//          Bsp: 900  --> verkleinere den aktuellen Wert um 100
//          Bsp: 1010 --> vergrößere  den aktuellen Wert um 10
//  Fq=auszugebende Frequenz in [Hz]    FqDur=dauer in [ms]
//  SaveNoState = 1 --> Stati werden NICHT in den Arrays gespeichert.
// -----------------------------------------------------------------------------------------------------

//int16_t Machine_DO(uint8_t MIdx,uint16_t MToDo,uint16_t =0);
//int16_t Machine_DO(uint8_t MIdx,uint16_t MToDo,uint16_t Fq,uint32_t =0);
//int16_t Machine_DO(uint8_t MIdx,uint16_t MToDo,uint16_t Fq,uint32_t FqDur,uint8_t = 0);
int16_t Machine_DO(uint16_t MIdx, uint16_t MToDo, uint16_t Fq, uint32_t FqDur, int8_t SaveNoState) {
  uint8_t  SwitchTo=0;   // Hilfsvar für den logischen Zustand in den geschaltet werden soll. 0, 1, oder 3..255 PWM wenn möglich
  uint8_t  g_owpis;      // Hilfsvariable On when Pin Is  
  int16_t  DoDiff = 0;   // Differenzbetrag um den der aktuelle Wert verändert werden soll.
    // ----Eingabeparameter---prüfen-----
    if (MIdx == 0)             {return 0; };                  // es geht erst ab Maschine 1 los  also EXIT
    if (MIdx > _MACHINE_COUNT) {return 0; };                  // Anzahl definierte Maschinen überschritten also EXIT 
    if (MToDo == 0 && Fq == 0) {return 0; };                  // bei 0 ist nix zu tun
 
    // ----wie die Maschine angesprochen wird ist aus TBL abzuholen-----
    TBL_Numbers(T_Idx(_S_GED,MIdx,_S31_PIN_I2CADDR_BIT));     // Einlesen Pin, pcfadr, pcfbit  --> sbufferNr[0.1.2]
      // sbufferNr[0]= PIN   1..69    // sbufferNr[1]= PCF_ADDR 33...63     // sbufferNr[2]= PCF_BIT  0..7
 
    g_owpis = TBL_Int(T_Idx(_S_GED,MIdx, _S31_OWP_IS));   // Einlesen des Zustandes für ON      
    
    if (g_owpis != 0 && g_owpis != 1) {g_owpis = 0;}          // wenn ON_WHEN_PIN_IS nicht definiert ist in der I_Table dann wird ON-Zustand ein LOW angenommen. Ein LOW-Ausgang kann mehr Leistung ausgeben und sollte deshalb wo immer möglich so verwendet werden.

    if (MToDo == 256) MToDo = obj_Status_DEC(UART_GER,MIdx,0); // Zustand wie im RAM-Array definiert  
    if (MToDo == 257) {
      MToDo = obj_Status_BIT(UART_GER,MIdx,0);                // Liefert 1 = AUS bzw. 2 für EIN und das ist nun zu invertieren aus 1 --> 2   und aus 2 --> 1
      if (MToDo == 1) {MToDo = 2;} else {MToDo = 1;}          // das geht bestimmt einfacher um aus 1 eine 2 zu machen und aus 2 eine 1
    }

    if (MToDo > 0 && MToDo < 257 ) {
      switch (MToDo) { 
        case   1: SwitchTo = !g_owpis;   break;               // AUS Invertiere was für den Pin für EIN steht
        case   2: SwitchTo =  g_owpis;   break;               // EIN direkt was für den Pin für EIN steht
        default:  SwitchTo = MToDo;      break;               // 3...255 PWM 
      }
      if (SaveNoState !=1) {
        obj_Status_BIT(UART_GER,MIdx,MToDo);                  // BINÄRER Zustand für das Gerät festhalten 1=AUS 2..255=EIN 
        obj_Status_DEC(UART_GER,MIdx,MToDo);                  // DETAIL  Zustand für das Gerät festhalten 1...255
      }
    } else {
      if (MToDo < 1000) {       // Wert ist zu senken um 1000 - MToDo 
        DoDiff = MToDo - 1000;  // ergibt eine NEGATIVE Zahl
      } else {                  // Wert ist zu erhöhen um MToDo - 1000
        DoDiff = MToDo - 1000;  // ergibt eine POSITIVE Zahl oder 0
      }
      // ------------------------------------------------------------------------------------
      // Überlauf Unterlauf Check Nach dem Hinzurechnen der Differenz zum aktuellen Zustand darf der Wert nicht > 255  und nicht < 0 werden 
      // ------------------------------------------------------------------------------------
      if (MToDo > 999) {        // PWM soll vergrößtert werden
        if (obj_Status_DEC(UART_GER,MIdx,0) + DoDiff > 255) DoDiff = 255 - obj_Status_DEC(UART_GER,MIdx,0);
      } else {                  // PWM soll verkleinert werden
        if (obj_Status_DEC(UART_GER,MIdx,0) + DoDiff < 0 ) DoDiff= -obj_Status_DEC(UART_GER,MIdx,0) +1 ;  // damit wird maximal der aktuelle Stand abgezogen auf 1 bedeutet AUS
      }
      
      SwitchTo = obj_Status_DEC(UART_GER,MIdx,0) + DoDiff;
      obj_Status_DEC(UART_GER,MIdx,SwitchTo);
      if (SwitchTo == 1) obj_Status_BIT(UART_GER,MIdx,1);  // bei 1 ist exakt AUS gemeint 
    }

    
    if (sbufferNr[1] != 0) {                                  // wenn eine PCFAdresse angegeben ist, dann muß dieser Ausgang über ein PCF-Modul angesteuert werden. 
                // Pcf-Adr,     BitNr,        Wert
        PCFSetPin(sbufferNr[1], sbufferNr[2], SwitchTo); }    // Gerät hangt an einem Porterweiterungsmodul
     else {                                                   // direkt über den ARDUINO-PIN  pinMode(g_pinnr, OUTPUT);  //  das muss bereits beim INITIALISIEREN erfolgt sein !   ARDUINO-PIN auf AUSGANG einstellen
        // ------------------------------------------
        // -- Ausgabe direkt an arduino-PIN
        // ------------------------------------------
      if (Fq > 30) {                                          // Frequenzen sind erst ab 31 Hz möglich
          // -------------------------------------------------------------------
          // Frequenz an PIN ausgeben im Bereich von 31 Hz bis x-Kiloherz KHz
          // -------------------------------------------------------------------
          noTone(sbufferNr[0]);                               // sonst wird einfach die PWM-Frequenz ausgegeben !!
          tone(sbufferNr[0],Fq,FqDur);                        // ACHTUNG nach dem TON bleibt der PIN auf LOW, ich weiß noch nicht wie ich das flexibel machen kann.
          } 
       else {
          // -----------------------------------------
          // DIGITAL oder PWM an arduino-PIN ausgeben
          // -----------------------------------------
        if (SwitchTo <= 2 )                             
          { digitalWrite(sbufferNr[0],SwitchTo);}             //  Digitale Ausgabe 0 und 1  (funktioniert NUR wenn der Ausgang vorher auch als OUTPUT deklariert wurde !)
          else                                              
          { if (g_owpis == 0) {SwitchTo=255-SwitchTo;}        // Wenn g_owpis = 1  bleibt der PWM-Wert 1:1   bei g_owpis = 0 wird der PWM-Wert umgerechnet auf: 255-MToDo
            analogWrite(sbufferNr[0],SwitchTo);}              // hier werden Werte von 3...255 per PWM ausgegeben wobei 255 DAUERAUSGANG bedeutet und 3 eine sehr kleine Pulsweite  (funktioniert auch ohne vorheriger Deklaration als OUTPUT)
      } // Ende else wenn Fq > 30
     }  // Ende else Zweig direkt über ARDUINO-PIN  
  return 0;
}
