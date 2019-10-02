
int32_t scrollval[4]={0,0,0,0}; // nur zum Test 4 unterschiedliche Variablen die mittels srollkontext angesprungen werden können
 // uint8_t scrollvarIDX=0;        // hier weiter nachdenken wie man die Adresse einer beliebigen Variable als Kontext einstellen kann.
int32_t * scrollvarPT = & scrollval[0];
//-------------------------------------------------------------------------------------------
// Aktionen die zusätzlich zu jeder Menüaktion auszuführen sind sobald sich da etwas tut.
// Am DREH-DRÜCK Knopf ENCODER kann man AUF-AB-SCROLLEN, DRÜCKEN, DRÜCKEN+AUF/AB-SCROLLEN.
// Bei jedem Tastenereignis soll ein eventuell abgeschalteter Hintergrund sofort eingeschaltet werden.
// Diese Funktion wird vom MAIN-LOOP aufgerufen. Es wird jene Variable hoch bzw. heruntergezählt
// auf die der Kontext zeigt. Damit können unterschiedliche scroller - Variablen behandelt werden.
//
//-------------------------------------------------------------------------------------------
int8_t CheckBT() {
    if(LCDML.BT_checkAny()) {   // wenn irgendein Dreh-Drück Ereignis eingetreten ist
      // Blink_LCDBack(0);LCD_BackLight(1); // ein eventuelles Blinken abstellen, Hintergrundlicht AN
      
        // Serial.print(F("Action:"));   
      if(LCDML.BT_checkUp())     {Serial.print(F("UP   "));  
                                  *scrollvarPT = *scrollvarPT + 1;  // sollte den ZAHLEN-INHALT an der Stelle wo der Zeiger hinzeigt um 1 erhöhen
      }
      if(LCDML.BT_checkDown())   {Serial.print(F("DOWN "));  
                                  *scrollvarPT = *scrollvarPT - 1;  // ZAHLEN-INHALT verkleinern
      }
      if(LCDML.BT_checkRight())  {Serial.print(F("RIGHT"));  
                                  scrollvarPT ++;                   // ADRESSE um 1 erhöhen (4 Byte weil scrollvarPT auf eine uint32_t Variable in der Deklaration zeigt.
                                  //Serial.print(F(" scrollvarIDX:"));Serial.print(scrollvarIDX); 
      }
      if(LCDML.BT_checkLeft())   {Serial.print(F("LEFT "));
                                  scrollvarPT --;                   // ADRESSE um 1 verkleinern
                                  //Serial.print(F(" scrollvarIDX:"));Serial.print(scrollvarIDX);
      }
      if(LCDML.BT_checkEnter())  {Serial.print(F("ENTER"));}
      if(LCDML.BT_checkQuit())   {Serial.print(F("QUIT "));}
      
        //Serial.print(F(" Scroller scrollval["));Serial.print(scrollvarIDX);Serial.print(F("]:")); Serial.println(scrollval[scrollvarIDX]);
      Serial.print(F(" Adr: ")); 
       // uint16_t adr;
       // adr = (uint16_t) scrollvarPT; // in adr befindet sich nun die ADRESSE als Wert mit dem weitergearbeitet werden kann.
      Serial.print( (uint16_t) scrollvarPT,HEX); Serial.print(F("[HEX] "));
      Serial.print( (uint16_t) scrollvarPT); Serial.print(F("[DEC] "));
      Serial.print(F(" Wert:"));  
      Serial.println(*scrollvarPT);

    } 
  return 0;
}

// zum kurzen Aufruf des Ausdruckes der aus dem Buffer1 extrahierten Zahlen
void PNr() {
  Serial.print(F(" sbufferNr[0 1 2 3 4):"));
  for (int i=0; i<5; i++) {
    Serial.print(sbufferNr[i]); PBLANKS();
  }
  Serial.println();
}

void PB() {PBLANKS(1);}
void PBLANKS(int8_t BlankCount) {
  for (int8_t i=0; i<BlankCount; i++) {Serial.print(F(" ")); }
}
void PLF() {PLFS(1);}
void PLFS(int8_t LfCount) {
  for (int8_t i=0; i<LfCount; i++) {Serial.println(); }
}

//----------------------------------------------------------------------------------------------
// Meldungen aller Art in der aktuell einstellten Sprache auf UART ausgeben.
//  Ähnlich Notify() mit dem Unterschied daß hier Zahlen in einen Text eingebaut werden können.
//  Bsp. Msg(11, 2, 4, 5)  baut in den Meldungstext 11 die Zahlen 2,4 und 5 ein und gibt das formatiert aus.
//----------------------------------------------------------------------------------------------
//void Notify(int16_t MsgNr, uint8_t = 10);  // per Default wird ein ENTER an die Meldung angefügt
//int8_t Msg(int16_t MsgNr, int32_t = 0);
//int8_t Msg(int16_t MsgNr, int32_t Nr1, int32_t = 0);
//int8_t Msg(int16_t MsgNr, int32_t Nr1, int32_t Nr2, int32_t = 0);
//int8_t Msg(int16_t MsgNr, int32_t Nr1, int32_t Nr2, int32_t Nr3, uint8_t = 10);
int8_t Msg(int16_t MsgNr)                                         {return Msg(MsgNr,0,  0,  0,  10);}
int8_t Msg(int16_t MsgNr, int32_t Nr1)                            {return Msg(MsgNr,Nr1,0  ,0,  10);}
int8_t Msg(int16_t MsgNr, int32_t Nr1, int32_t Nr2)               {return Msg(MsgNr,Nr1,Nr2,0,  10);}
int8_t Msg(int16_t MsgNr, int32_t Nr1, int32_t Nr2, int32_t Nr3)  {return Msg(MsgNr,Nr1,Nr2,Nr3,10);}
int8_t Msg(int16_t MsgNr, int32_t Nr1, int32_t Nr2, int32_t Nr3, uint8_t CharAtEnd) {
  char   tmpBuffer[_BUFFERLEN1+20];                       // temporärer Buffer für die Ausgabe
  int8_t FLen = 0;                                        // geliefert von sprintf = Länge des formatierten Textes
  if (EE_ParaGet(6) == 1) {                               // Wenn Parameter für Meldungs-Nummer vor die Meldung stellen aktiv ist.
    Serial.print(F2X(T_Idx(_S_MSL,0,_S02_MSG)));
    Serial.print(MsgNr);
    Serial.print(F(": "));
  }
  F2X(T_Idx(_S_MSL, MsgNr, _S02_MSG),Buffer1);            // Meldungstext in den Buffer1 laden.
  FLen=sprintf(tmpBuffer, Buffer1, Nr1, Nr2, Nr3);        // in tmpBuffer wird der formatierte Text abgelegt
  Serial.print(tmpBuffer);                                // formatierten Text ausgeben
   // PLF();                                                  // Linefeed ausgeben
  if (CharAtEnd != 0x00) {if (CharAtEnd==10) {Serial.println();} }
  return FLen;
}

//-----------------------------------------------------------------------------------
// Meldungen aller Art in der aktuell einstellten Sprache auf UART ausgeben 
//-----------------------------------------------------------------------------------
//void Notify(int16_t MsgNr, uint8_t = 10);  // per Default wird ein ENTER an die Meldung angefügt
void Notify(int16_t MsgNr, uint8_t CharAtEnd) {
  if (EE_ParaGet(6) == 1) {
    Serial.print(F2X(T_Idx(_S_MSL,0,_S02_MSG)));
    Serial.print(MsgNr);
    Serial.print(F(": "));
  }
  Serial.print(F2X(T_Idx(_S_MSL, MsgNr, _S02_MSG)));
  if (CharAtEnd != 0x00) {
    if (CharAtEnd == 10) {Serial.println();}
  }
}

//-------------------------------------------------------------------------------------------------------------
// Programm-Start Meldung an SERIELLE (soll nur erfolgen wenn das im EEPROM so als Parameter abgelegt ist)
// DEFAULT Einstellungen mancher Variablen !!!
//-------------------------------------------------------------------------------------------------------------
void Msg_ProgStart() {                  // Ausgabe an die Serielle Softwarename,Version usw.
  if (EE_ParaGet(0) != 12345) {         // ist der Wert von Parameter 0 <> 12345 dann werden alle Parameter auf DEFAULT eingestellt
    Serial.println(F("Start all Parameters set to Factory Defaults"));    
    FactoryParameters();                // Alle Parameter im EEPROM mit den in TBL definierten DEFAULT-Werten füllen.
    Serial.println(F("Done all Parameters set to Factory Defaults"));
    EE_ParaLet(0, 12345);              // Damit wird ab jetzt nicht mehr automatisch beim BOOT FactoryParameters() durchgeführt
  }
  
  if (EE_ParaGet(1) == 1) {            // Ausgabe an UART nur wenn Parameter 1 eine 1 hat.
    Serial.print(F(SWName));Serial.print(F(SWVersion)); Serial.print(F(" "));Serial.println(F(_LCDML_VERSION));  
  } 
         // Now2UART();   // Realtime-Clock Daten einlesen und ausgeben auf UART  
  CurrLanguage(EE_ParaGet(2));         // Standardsprache einstellen wie im Parameter 2 gespeichert 0=en  1=de,..  
}

//-------------------------------------------------------------------------------------------------------------------
// Programm hat beim Start die SETUP-Routine beendet  (nur Meldung wenn im EEPROM dieser Parameter gesetzt ist
//-------------------------------------------------------------------------------------------------------------------
void Msg_ProgRdy() {                    // Ausgabe an die Serielle nachdem bestimmte Initialisierungen abgeschlossen wurden.
  if (EE_ParaGet(3)==1) {Notify(11);}   // Ausgabe nur wenn Parameter 3 = 1
    
  // Now2UART();
}

//--------------------------------------------------------------------
//  Defaults beim Start des Programmes für LCD einstellen (todo  nur wenn im EEPROM die Parameter so gesetzt sind)
//--------------------------------------------------------------------
void LCD_Defaults() {     
    lcd.begin();                    // allgemeine  LCD Vorbereitungsarbeiten 
    LCD_Additional_LCDMENULIB();    // MUSS sein für die benötigten Sonderzeichen Speicherstelle 0..4 der LCDMENULIB  (Scrollbalken)
    LCD_Additional_GradCelsius();   // Grad-Zeichen auf Sonderzeichenspeicherstelle 5 
    LCD_BackLight(EE_ParaGet(4));   // Hintergrundbeleuchtung LCD je nach Parameterwert 1=AUS 2=EIN 
}

//-----------------------------------------------------------------------------------------
// 2018-03-17  schaltet/liefert  die Hintergrundbeleuchtung des LCD-Displays 
//             0=LIEFERT Zustand  bzw. SETZT Zustand bei 2 EIN  bei 1 AUS   
//             und vermerkt den Zustand in der Variablen sStati BitNr 5 
//-----------------------------------------------------------------------------------------
int8_t LCD_BackLight(int8_t OnOff) {
    if (OnOff == 1) {lcd.noBacklight(); } 
    if (OnOff == 2) {lcd.backlight();   }
    if (OnOff > 0) {bitWrite(sStati,5,OnOff-1); }  // Status merken  Hintergrundbeleuchtung des LCD-Displays einschalten
    return bitRead(sStati, 5) +1;  // damit wird 1 für AUS  und 2 für EIN geliefert
}

//-------------------------------------------------------------------------------------------------------
// 2019-01-20 Onboard LED ein/aus/pwm  Achtung diese Funktion vermerkt NICHT den hier gesetzten Zustand 
//            wie sonst üblich für jeden IO-Pin, sondern setzt hier einmalig den Ausgang.
//-------------------------------------------------------------------------------------------------------
#define ONBOARDLEDPIN 13
uint8_t LED_Onbord(uint8_t OnOff) {
  static uint8_t Ret=0;  // Rückgabe 0=undef.  1=AUS  2=EIN  3..255 PWM
  switch (OnOff) {
    case 0:                             // Zustand liefern
      return Ret;
      break; 
    case 1:                             // AUS schalten
      pinMode(ONBOARDLEDPIN, OUTPUT);   // Sicherstellen daß dieser ONBOARD-LED-PIN als OUTPUT eingestellt ist
      digitalWrite(ONBOARDLEDPIN,0);    // LED AUS an dieser Stelle wird fix von einer HIGH aktiven Onboad-Led ausgegangen
      Ret = 1;
      break;
    case 2:   
      pinMode(ONBOARDLEDPIN, OUTPUT);  // EIN schalten 
      digitalWrite(ONBOARDLEDPIN,1);
      Ret = 2; 
      break;
    default:  
      pinMode(ONBOARDLEDPIN, OUTPUT);  // PWM schalten 
      analogWrite(ONBOARDLEDPIN,OnOff); // aus 3 soll auch ein PWM-Verhältnis von 3x 1 und 252x 0 bleiben 
      Ret = OnOff;
      break;
  }
  return Ret;  // ohne Bedeutung
}

//------------------------------------------------------------------------------------
// 2019-01-20 Setzt einen angegeben PIN direkt auf logisch 0 bzw. 1 bzw. PWM ab > 1
//------------------------------------------------------------------------------------
uint8_t PIN_Do(uint8_t PinNr, uint8_t Logic) {
  pinMode(PinNr, OUTPUT);  // Sicherstellen daß der angesprochene PIN OUTPUT wird/ist
  switch (Logic) {
    case 0:   // oder
    case 1:   digitalWrite(PinNr, Logic);   break;
    default:  analogWrite (PinNr, Logic);   break;
  }
  return 0;
}
//--------------------------------------------------------------------------------------
// 2019-01-20 Stellt einen Pin wenn 
//    SetTo:  1=EINGANG, 2=EINGANG mit internem PULLUP, 3=AUSGANG
//--------------------------------------------------------------------------------------
uint8_t PIN_Set(uint8_t PinNr, uint8_t SetTo) {
  switch (SetTo) {
    case 1:  pinMode(PinNr, INPUT       ); break;
    case 2:  pinMode(PinNr, INPUT_PULLUP); break;
    case 3:  pinMode(PinNr, OUTPUT      ); break;
  }
  return 0;
}

//-------------------------------------------------------------------------------
// 2018-03-18  zum auffinden/oder SETZEN des gewünschten BIT aus obj_stateBIN[] 
//  Je nachdem wieviele BIT ein Element in obj_stateBIN[] hat wird ein bestimmtes
//  Element errechnet in dem sich dann das gewünschte BIT befindet.
//  Parameter  NewBitVal = 0 = es wird nur  GELESEN 
//                         1 = es wird LOW  gesetzt
//                         2 = es wird HIGH gesetzt
//-------------------------------------------------------------------------------
int8_t obj_BIT(uint16_t BitNr, uint8_t NewBitVal) {
  uint8_t BitsPE=0;         // Anzahl Bit per     obj_stateBIN[0]        Bsp:  64
  uint8_t NElement=0;       // das Element in dem die gesuchte BitNr zu finden ist.
  int8_t  BitVal=0;         // der Wert des gelesenen Bit's 
  BitsPE   = sizeof(obj_stateBIN[0]) * 8;   // Bsp:  64
  NElement = BitNr / BitsPE;                // Bsp: BitNr=2  2/64=0  Bsp: BitNr=66  66/64=1
  if (NewBitVal == 0) {
    BitVal= bitRead(obj_stateBIN[NElement], BitNr-(NElement*BitsPE)); // nun wird aus dem richtigen Feld das entsprechende BIT gelesenen
  } else {
           bitWrite(obj_stateBIN[NElement], BitNr-(NElement*BitsPE), NewBitVal -1 );
  }
  return BitVal + 1; // damit wird 1 = LOW  und 2 = HIGH geliefert wie hier üblich 
}



//---------------------------------------------------------------------------------------------------------
// 2019-02-05  überarbeitet
// 2018-03-17  Verarbeitung/Auswertung vom Bit-Array obj_stateBIN[], Merker  
//  in welchem Zustand sich ein (g)Gerät (k)Kombination befindet
// 0= undefiniert oder 1=AUS 2=EIN
// beginnend bei Bit 0.._S30_RECORDS .. bis _S32_RECORDS  
// Type = 'g' für Gerät
//        'k' für Kombination
// Nr = Gerätenummer  bzw.  Gerätekombinationsnummer 
// Val  0 = LESE BIT     
//      1 = SCHALTE BIT  auf 0  = LOW  (PWM wird an dieser Stelle nicht unterschieden siehe dazu obj_Status_DEC() 
//   >= 2 = SCHALTE BIT  auf 1  = HIGH 
//---------------------------------------------------------------------------------------------------
int8_t obj_Status_BIT(uint8_t Type, uint8_t Nr, uint8_t Val) {
  uint16_t Idx=0;         // absoluter Index 0...70.. im Bitmuster
  int8_t   Ret=2;         // Rückgabe
  if (Val > 2) Val=2;     // damit wird aus allen Werten > 2 der Zustand für EIN = 2 gesetzt
  switch (Type) {
    case UART_GER:
      if (Nr > _S30_RECORDS) {Nr = _S30_RECORDS; }      // wenn eine zu hohe Gerätenummer angefargt wird dann setzen auf das maximal mögliche.
      Idx = Nr;
      break;
    case UART_KOM:
      if (Nr > _S32_RECORDS) {Nr = _S32_RECORDS; }
      Idx = _S30_RECORDS + Nr;                          // nach den Geräten kommen die Kombinationen im Bitmuster
      break;
  }
  Ret = obj_BIT(Idx,Val);                               // je nach Val wird Bit nur gelesen = 0  oder bei 1=LOW 2=HIGH gesetzt
  return Ret;
}

//--------------------------------------------------------------------------------------------------
// 2019-02-05 überarbeitet
// 2018-03-19  Verarbeitung/Auswertung vom Byte-Array obj_stateDEC[], Merker  
//  in welchem Zustand sich ein (g)Gerät (k)Kombination befindet  
// 0=unberührt 1=AUS 2=EIN 3...255 PWM 
// beginnend bei Bit 0.._S30_RECORDS .. bis _S32_RECORDS  
// Type = 'g' für Gerät
//        'k' für Kombination
// Nr = Gerätenummer  bzw.  Gerätekombinationsnummer 
// Val  0 = Abfragen des Zustandes 
//  (g) 1..255 = setze n des Zustandes     
//  (g) 1 = AUS  digital  
//  (g) 2 = EIN  digital       
//  (g) 3...255  PWM                  
//                                         
// (k)  1= 1:1 Umsetzen der im Feld _S33_OFF_DO       OFF  definierten Folge 
// (k)  2= 1:1 Umsetzen der im Feld _S33_ON_DO        ON   definierten Folge
// (k)  3= 1:1 Umsetzen der im Feld _S33_AT_PWRON_DO  INIT definierten Folge
// (k)  4=  jedes im S33_OFF_DO       erwähnte Gerät auf OFF schalten, unabhängig was beim Zustand in S33_OFF_DO steht.
// (k)  5=  jedes im S33_ON_DO        erwähnte Gerät auf OFF schalten, unabhängig was beim Zustand beschrieben steht.
// (k)  6=  jedes im S33_AT_PWRON_DO  erwähnte Gerät auf OFF unabhängig vom Beschriebenen.
// (k)
// (k)  7=  jedes im S33_OFF_DO       erwähnte Gerät auf ON  schalten, unabhängig
// (k)  8=  jedes im S33_ON_DO        erwähnte Gerät auf ON  schalten, unabhängig
// (k)  9=  jedes im S33_AT_PWRON_DO  erwähnte Gerät auf ON  schalten, unabhängig
// (k)
// (k) 11=  Invertiere jedes in _S33_OFF_DO       erwähnten Gerät den aktuellen Zustand
// (k) 12=  Invertiere jeden in _S33_ON_DO        erwähnten Gerät den aktuellen Zustand
// (k) 13=  Invertiere jeden in _S33_AT_PWR_ON_DO erwähnten Zustand 
// siehe auch obj_Status_BIT() für die rein binäre Form der Zustands-Speicherung 0=off 1=on 
//---------------------------------------------------------------------------------------------------
uint8_t obj_Status_DEC(uint8_t Type, uint8_t Nr, uint8_t Val) {    
  uint16_t Idx=0;         // absoluter Index 0... im Status-Array obj_stateDEC[Idx] 
  uint8_t  Ret=2;         // Rückgabewert  0=unangetastet  (g)  1=AUS  2=EIN  3...255 PWM   (k) siehe oben
  switch (Type) {
    case UART_GER:
      if (Nr > _S30_RECORDS) {Nr = _S30_RECORDS; }    // wenn eine zu hohe Gerätenummer angefargt wird dann setzen auf das maximal mögliche.
      Idx = Nr;
      break;
    case UART_KOM:
      if (Nr > _S32_RECORDS) {Nr = _S32_RECORDS; }
      Idx = _S30_RECORDS + Nr;                        // nach den Geräten kommen die Kombinationen im Bitmuster
      break;
  }
  if (Val > 0 ) {  // Zustand im Byte speichern 
    obj_stateDEC[Idx] = Val;                          // den Wert direkt ins Byte schreiben
    Ret=Val;                                          // und auch liefern
  } else {                                            // Zustand aus Byte lesen
    Ret = obj_stateDEC[Idx];
  }
   
  return Ret;
}

 
//---------------------------------------------------------------------------------------------------------
// 2019-02-18  im Buffer1 sollen mehrere vorkommende (p) durch die entsprechenden Werte aus dem EEPROM
//             ersetzt werden.
//             Als Wunsch sollen diese Werte dann noch miteinander mittels Rechenoperatoren verarbeitet werden können, siehe Algorithmus Shuntingyard ist noch offen.
//  Bsp: p17*p18-p12*2  mal sehen ob das mit vernünftigem Aufwand hier lösbar ist.
//
//  Ersetzte 'p ##' Stellen im buffer durch den entsprechenden Parameter-Wert
//  Bsp: 'p 17*1000 '   --> wird im buffer ersetzt mit '23000 '  (wenn der Parameter 17 den Wert 23 hat)
//  Bsp: 'p1-12 g27 p13'--> wird zu '8 g27 30'  (wenn Parameter 1 den Wert 20 und Parameter 13 den Wert 30 hat) 
//--------------------------------------------------------------------------------------------
void ReplacePwithVal() {
  uint8_t   found=0;      // anzahl gefundener 'p' - Vorkommen
  uint8_t   ParaNr=0;     // dem 'p' folgende Parameter-Nummer
  uint8_t   DigCnt=0;     // Länge der gefundenen Zahl.
  uint8_t   DigCnt2=0;    // Läne des gefundenen Operanden
  int32_t   ParaVal=0;    // Wert des Parameters
  int16_t  ibuffer;
  int16_t  jbuffer;
  int16_t  ibuffB=0;
  char      buffB[200];   // zwischen-Speicher (Hilfs-buffer) zum Einfügen der zu ersetzenden Zeichen/Stellen
  char      ParaStr[9];   // zwischen-Speicher für die Parameter-Wert-Zahl als Zeichenkette 
  char      CharAfterPnr; // Zeichen das der Parameter-Nummer folgt (gesucht ist hierbei *, /, + - ^ % als von Interesse
  char      CharAfterOp;  // 1. sinnvolles Zeichen das dem Operator folgt 
  int16_t   Operand;      // Zahl die dem Rechenzeichen ggf. folgt
  int8_t    ToCalc=0;     // Kennzeichen ob zu rechnen ist
  
  for (ibuffer=0; ibuffer < Buffer1Len; ibuffer++) {               // für jedes ZEICHEN im buffer
    if (Buffer1[ibuffer] == 'p') {                                 // wenn ein  'p' =Parameter ZEICHEN gefunden wird  
      if (found==0) {                                               // wenn der 1.Parameter gefunden wird ist die bis dahin durchsuchte Zeichenkette in buffB[] zu übertragen
        buffB[0]=0x00; buffB[1]=0x00;                               // das ist ausreichend um den Hilfs-buffer buffB[] als LEER zu definieren 
        for (ibuffB=0; ibuffB < ibuffer; ibuffB++) {
          buffB[ibuffB]=Buffer1[ibuffB];
        }
        buffB[ibuffB]=0x00;buffB[ibuffB+1]=0x00;                    // die beiden folgenden Zeichen sind mit HEX 00 zu füllen damit wird das Ende definiert
      }
      
      // nachdem ein 'p'  gefunden wurde ist dieses nun durch den WERT und einer ev. Rechnung zu ersetzen.
      found++; 
      ibuffer++;                                                    // ab der Stelle nach 'p' wird die nächstgelegene Zahl für 'Parameter-Nummer' gesucht
      GetPosNEQ(Buffer1, ibuffer, Buffer1Len,CharAfterPnr,' ');   // Sucht nach der nächsten NICHT LEEREN Stelle

      ParaNr= bufferGetLong(Buffer1,ibuffer,Buffer1Len,DigCnt);   // Parameter-Nummer aus dem buffer[] ab Stelle ibuffer holen.
      if (DigCnt==0) {ibuffer--;}                                   // wenn dem 'p' keine Nummer gefogt ist wird p nicht ersetzt.
      
      jbuffer=ibuffer;                                              // Stelle an der die Parameter-Nummer endet, also das NÄCHSTE Zeichen beginnt 
                                                                    // dieses Zeichen kann ein beliebiges Zeichen sein, auch ein LEER-Zeichen.
      // Feststellen ob der Parameter-Nummer ein Operand folgt.     // Dazu Suchen eines nächsten Zeichens (ausgenommen LEER)
      GetPosNEQ(Buffer1, jbuffer, Buffer1Len,CharAfterPnr,' ');   // Kommt nach der Parameter-Nummer ein Operand * + - / ^ % # 

       //---------------------------------------------------------------
       //  * MAL   + PLUS   - MINUS   / DIVIDIERT    ^ HOCH    % MODULO   
       //---------------------------------------------------------------
      if (CharIsOperator(CharAfterPnr) > 0) {                       // wenn der Parameter-Nummer ein *+-/ folgt
        jbuffer++;                                                  // Zeiger auf nächstes Zeichen setzen (dem Operanden folgend)
        GetPosNEQ(Buffer1, jbuffer,Buffer1Len,CharAfterOp,' ');   // Sucht nach der nächsten Stelle wo der Operator beginnen soll     
                                                                    // jbuffer zeigt auf das Zeichen nach einem Operator.
        Operand= bufferGetLong(Buffer1,jbuffer,Buffer1Len,DigCnt2); 
        ToCalc=1;
      }

       // Wenn Anzahl Stellen von Parameternummer > 0 UND Parameternummer > 0 UND Parameternummer <= mögliche gültige Parameter-Nummern
      if (DigCnt > 0 && ParaNr > 0 && ParaNr <= _S20_RECORDS) {     // wenn 'p' eine gültige Parameter-Nummer folgt
        ParaVal=EE_ParaGet(ParaNr);                                 // eingestellten Parameterwert abholen
        if (ToCalc == 1) {                                          // wenn der Parameter-Nummer eine Rechnung folgte
          ToCalc=0;
          ibuffer=jbuffer;                                          // Position nach Operand-Ende 
          ParaVal = Calc(ParaVal,CharAfterPnr,Operand);
        }          
        ltoa(ParaVal, ParaStr, 10);                                 // Konvertiert die Zahl ParaVal zu einer Zeichenkette 
            // nun ist ab inkl. [i]  der Stelle 'p ##'  der Wert einzubauen
        for (uint8_t i=0;i<strlen(ParaStr);i++) {buffB[ibuffB]=ParaStr[i];ibuffB++;}
      }    // ibuffer steht nun an der Stelle an der Parameter-Nummer endet, ab dieser ist nun weiterzuarbeiten.
    }
    
    if (found >0)  {                                                // sobald ein 'p' ersetzt wurde,
      buffB[ibuffB]=Buffer1[ibuffer]; 
      buffB[ibuffB+1]=0x00;                                         // damit die Zeichenkette ihr Ende kennt, danach habe ich lange gesucht an dieser Stelle
      ibuffB++;
    }
  } 
       
  if (found>0) {
      for(ibuffer=0;ibuffer < ibuffB; ibuffer++) {Buffer1[ibuffer]=buffB[ibuffer];}      
      Buffer1[ibuffer]=0x00;
      Buffer1Len=ibuffer;                                          // neue Buffer1länge bekannt geben
  }
  
}

//-----------------------------------------------------------------------------
// 2018-04-26  berechnet einen Ausdruck im Format 'Zahl1 Operator Zahl2'
//  Interpreter-Teil für Grundrechnungsart zwischen 2 Werten
//-----------------------------------------------------------------------------
int32_t Calc(int32_t ParaVal, uint8_t Operator, int32_t Operand) {
    // Serial.print(F("Calc ParaVal:"));Serial.print(ParaVal);Serial.print(char(Operator));Serial.println(Operand);
  switch (Operator) {
      case '+': return ParaVal + Operand; break;
      case '-': return ParaVal - Operand; break;            
      case '*': return ParaVal * Operand; break;
      case '/': return ParaVal / Operand; break;
      case '^': return powint(ParaVal,Operand); break;
      case '%': return ParaVal % Operand; break;
  }
  return 0;
}

//----------------------------------------------------------------------------
// 2018-04-26  liefert eine Zahl 1,2,3,4,5 wenn das Zeichen ein Operator ist
//   0=kein OP  1=+  2=- 3=*  4=/  5=^  6=%
//----------------------------------------------------------------------------
int8_t CharIsOperator(char &CharA) {
    switch (CharA) {
      case '+': return 1; break;
      case '-': return 2; break;            
      case '*': return 3; break;
      case '/': return 4; break;
      case '^': return 5; break;
      case '%': return 6; break;
    }
    return 0;
}

//---------------------------------------------------------------------------------------------
// 2018-05-26  POW Funktion für Ganzzahlen da die Original pow() nur für Kommarechnen gut ist
//  pow(40,3) liefert 63999.99  deshalb gibt es diese Funktion posint()
//---------------------------------------------------------------------------------------------
int32_t powint(int32_t factor, int32_t exponent) {
   int32_t product = 1;
   while (exponent--) { product *= factor; }
   return product;
}

//--------------------------------------------------------------------------------------------------------
// 2019-02-18  liefert eine LONG Zahl die AB der Stelle bStartPos im buffer zu finden ist. 
// Liegt keine Zahl an dieser Stelle vor, dann wird -1 geliefert.
// 4294967295 wäre UL  -2147483648(1000 0000 ..)   bis +2147483647(0111 1111 ..)
// ACHTUNG  bStartPos wird der Funktion als Zeiger übergeben und wird von der Funktion an die Stelle gesetzt wo die gefundene Zahl endet.
//--------------------------------------------------------------------------------------------------------
int32_t bufferGetLong(char xbuffer[], int16_t &bStartPos, int16_t &bEndPos, uint8_t& DigCnt ) {
  char    bufferpart[10];   // nötig für die Verwendung der atol() Funktion
  int32_t buffNr = -1;      // der Rückgabewert -1 ist wenn nichts gefunden wird
  int8_t  Nr;               // das einzelne Zeichen aus dem buffer welches zu einer Zahl umgewandelt wird
  uint8_t bui;              // Schleifenzähler und Zeiger für bStartPos
  uint8_t VzFound = 0;      // Merker ob der Zahl ein Vorzeichen vorangestellt ist. Bsp: -123 dann wird VzFound = 1
  uint8_t bp;               // Positionsmerker für bufferpart[bp]
  uint8_t bl;               // Positionsmerker für buffer[bl]  
  DigCnt = 0;               // Anzahl gefundener Stellen für die gefundene Zahl 
    // 2018-06-06 folgende Schleife soll ein führendes '-' auch als Zahl zulassen.
    //   '-' '+' darf nur als erstes Zeichen stehen, danach müssen Zahlen folgen
  if (xbuffer[bStartPos] == '-' || xbuffer[bStartPos] == '+') { VzFound=1; }
  
  for (bui= bStartPos + VzFound; bui< bEndPos; bui++) {       // folgende Zeilen könnte man verm. auch mit isDigit() lösen. kannte ich noch nicht als diese Fkt geschrieben wurde. Laufzeit ?
      Nr = xbuffer[bui] -'0';                                 // das ergibt die Zahl welche an der Position steht.
      if (Nr < 0 || Nr > 9) {break;} else {DigCnt++;}         // es ist keine Ziffer mehr also haben fertig.
  }                                                           // solange es sich um eine Ziffer handelt Zählen der Stellen und weiter in der Schleife.

  if (DigCnt > 0) {                                           // DigCnt ist der Zähler für gefundene Stellen von Ziffern.
    bp = 0;                                                   // Positionszeiger für Hilfsbuffer bufferpart[]
    for (bl=bStartPos;bl <(bStartPos+DigCnt+VzFound ); bl++) {
      bufferpart[bp] = xbuffer[bl];                           // Übertragen der Ziffern in ein eigenes Datenfeld damit atol verwendet werden kann.
      bp++;
    }
    if (bp < 10) bufferpart[bp] = 0;                          // wichtig damit eventuelle RESTE aus vorherigen Aufrufen im RAM nicht zu einer falschen Zahl führen sondern der Zusammenhang hier unterbrochen wird.
    buffNr = atol(bufferpart);                                // aus einer Zeichenkette mit Ziffern wird eine Zahl
    bStartPos = bui; }                                          // Zeiger im buffer setzen damit die nächste Parse-Funktion an dieser Stelle weitermachen kann.
  return buffNr;      // Anhand der zurückgegebenen Zahl kann auch die Position im buffer erkannt werden. 
}





//--------------------------------------------------------------------------------
// 2019-01-21  Konvertieren einer Zahl zu einem STRING bzw. einer Zeichenkette
//  liefert einen Zeiger auf eine 11-stellige Zeichenkette 
//--------------------------------------------------------------------------------
char * NrToStr(int32_t Nr) {
  char buf[12];             // '-2147483646' ist 11 Stellen lange, mit dem 0x00 zum Abschluss sind 12 Stellen nötig
  char * bufpt = & buf[0];  // Zeiger auf die Zeichenkette buf[12]
  sprintf(buf, "%ld", Nr);  // formatiert buf mit Nr und man hat damit eine Zeichenkette aus einer Zahl gemacht.
  return bufpt;
}

// ungetestete Funktion !!!!
void StrAdd(char ToBuffer[], uint8_t AtPos, char ToAdd[] ) {
  strcpy (&ToBuffer[AtPos], ToAdd);
}
// *******************************************************************************
// RESET vom arduino per Befehl soft_reset();  oder resetFunc(); reboot commando 2
// *******************************************************************************
void soft_reset() {do { wdt_enable(WDTO_15MS); for(;;) {;}  } while (0); }
void(* resetFunc) (void) = 0;   // declare reset function at address 0


//-------------------------------------------------------------------------------------------------
// LCD  Sonderzeichensatz, NÖTIG für die LCDMENULIB  (Scrollbalken) 
// Anm: Es können NUR 8 Sonderzeichen auf dem Display gespeichert werden.
//      Sollten mehr Sonderzeichen nötig werden dann ist das NUR möglich durch ein "UMSPEICHERN" bzw. Hin- und HerSpeichern mit neuen Zeichen.
//      Zum Schluss müssen wieder die Sonderzeichen für die LCDMENULIB gespeichert werden, zwischendruch kann herumgespeichert werden.
//-------------------------------------------------------------------------------------------------
void LCD_Additional_LCDMENULIB() {
  const uint8_t scroll_bar[5][8] = {  // 5 individuelle Zeichen je 8 bit für das LCD
                    {B10001, B10001, B10001, B10001, B10001, B10001, B10001, B10001},   // scrollbar top
                    {B11111, B11111, B10001, B10001, B10001, B10001, B10001, B10001},   // scroll state 1 
                    {B10001, B10001, B11111, B11111, B10001, B10001, B10001, B10001},   // scroll state 2
                    {B10001, B10001, B10001, B10001, B11111, B11111, B10001, B10001},   // scroll state 3
                    {B10001, B10001, B10001, B10001, B10001, B10001, B11111, B11111}};  // scrollbar bottom
    lcd.createChar(0, (uint8_t*)scroll_bar[0]);    // Sonderzeichen auf dem LCD-Display erstellen.
    lcd.createChar(1, (uint8_t*)scroll_bar[1]);
    lcd.createChar(2, (uint8_t*)scroll_bar[2]);
    lcd.createChar(3, (uint8_t*)scroll_bar[3]);
    lcd.createChar(4, (uint8_t*)scroll_bar[4]);
}

//--------------------------------------------------------------------------------------------------------
//  LCD  Sonderzeichen für Grad Celsius  auf Stelle 5, das behindert die Zeichen für die LCDMENULIB nicht
//--------------------------------------------------------------------------------------------------------
void LCD_Additional_GradCelsius() {
  uint8_t Celsius[8] = {B11100, B10100, B11100, B00000, B00000, B00000, B00000, B00000};  // für die Anzeige eines Grad Celsius Zeichens am LCD-Display: lcd.createChar(0, Celsius);  und  lcd.write((uint8_t)0)
    lcd.createChar(5, Celsius);   // noch ein Selbstgemachtes Zeichen für Grad ° 
}
