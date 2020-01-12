#define UART_AUS    'a'   // Buchstabe für Auszug aus Tbl               FERTIG
#define UART_CMD    'c'   // Buchstabe für Kommando                     FERTIG soweit die Kommandos vorhanden sind
#define UART_DAS    'd'   // Buchstabe für Date/Zeitstempel             FERTIG
#define UART_MES    'e'   // Buchstabe für Macro Status, meist Ende-Status, aber auch Zwischendurch-Status    OFFEN !!!!
#define UART_FKT    'f'   // Buchstabe für Funktion
#define UART_GER    'g'   // Buchstabe für Gerät
#define UART_INF    'i'   // Buchstabe für Info an LCD-Display ausgeben
#define UART_KOM    'k'   // Buchstabe für Kombination von Geräten
#define UART_LNG    'l'   // Buchstabe für Sprache 0=Frage was eingestellt ist 1=Engl. 2=Deutsch 
#define UART_MCR    'm'   // Buchstabe für Makro
#define UART_NFY    'n'   // Buchstabe für Notify 
#define UART_NF2    'N'   // Buchstabe für Msg wie Notify jedoch mit sprintf Funktionalität zum einbauen von Zahlen in den Text hinein.
#define UART_OUT    'o'   // Buchstabe für Output Meldungsnummer
#define UART_PAR    'p'   // Buchstabe für Parameter 
#define UART_ERR    'r'   // Buchstabe für Error 
#define UART_SWI    's'   // Buchstabe für Schalter Zustand
#define UART_TEM    't'   // Buchstabe für Temperatur 
#define UART_WEI    'w'   // Buchstabe für Weight Gewicht
#define UART_HLP    '?'   // Buchstabe für HELP danach sollte der Buchstabe folgen für den HILFE gewünscht ist  Bsp: ?c --> Auflistung aller commandos  
// frei: a,b,h,j,n,q,u,v,x,y,z  ,alle GROSSBUCHSTABEN, Sonderzeichen, Zahlen 

#include "Global.h"
#include "Func_Help.h"

//char     UART_buffer[_UART_BUFFLEN];      // eine Zeichenkette einzeln vom UART eingelesen bis zum ENTER = 10
//uint8_t  UAIdx=0;                         // Zechenanzahl bzw. Hilfszeiger für die Bearbeitung der Zeichenkette in UART_buffer[]
//uint8_t  UART_bufferlen=0;                // Länge der Zeichenkette

//----------------------------------------------------------------------------------------------------------------------------------
//  Funktionen im Zusammenhang mit dem Hören / Senden auf der SERIELLEN Schnittstelle (universal asyncronous receiver transmitter)
//----------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------
// Abhören von dem was an der UART daherkommt und ggf. reagieren/UART_buffer ausführen wenn das ENDE-Zeichen angekommen ist.
//----------------------------------------------------------------------------------------------------------------------------
void UART_hear(uint8_t EndCmd) {        // normalerweise wird EndCmd = 10 = ENTER beim Aufruf gesetzt
  uint8_t UAInp = UART_Check(EndCmd,0); // Hören was an der seriellen Schnittstelle ankommt und einlesen. 
                                        // Falls mit 10=Enter abgeschlossen wird eine Anzahl gesammelter Zeichen geliefert.
  if (UAInp > 0 ) {                     // Wenn eine Anzahl Zeichen gesammelt wurde und mit ENTER abschließt 
    UART_bufferlen = UAInp;             // Merken der Zeichenketten-Länge 
    UART_Parse( );                      // Abarbeiten der vorliegenden Zeichen im UART_buffer[], danach wird UART_bufferlen=0 gesetzt
  }  
}

//-----------------------------------------------------------------------------------------------------------------
// sammelt die an der seriellen Schnittstelle ankommenden Zeichen im buffer UART_buffer[] und liefert bei 
// einem ankommenden ENTER die Länge der Zeichenkette als Indiz daß die Zeichenkette nun vollständig ist und ausgewertet werden kann.
// Wenn mehr als sizeof(UART_buffer) Zeichen bereits vorliegen und noch immer kein ENTER angekommen ist, wird ein ENTER eingefügt,
// damit der buffer dann abgearbeitet und geleert werden kann.
// CNr_Ignore  = zu ignorierendes Zeichen (im Ascii-Code) 
// CNr_End     = Zeichen (im Ascii-Code welches das Ende einer Zeichenkette bestimmt) Bsp: 10 = ENTER
//-----------------------------------------------------------------------------------------------------------------
uint8_t UART_Check(uint8_t CNr_End, uint8_t CNr_Ignore) {  
  uint8_t UART_Char=0;      // ein einzelnes ankommendes Zeichen
  uint8_t UART_Done=0;      // wird gleich Anzahl Zeichen gesetzt und geliefert, sobald das ENTER angekommen ist.
  if (Serial.available()) {  
      UART_Char     = Serial.read();
      if (UART_Char != CNr_Ignore) {      // wenn das Zeichen nicht zum ignorieren ist
        if (UART_Char == CNr_End) {       // wenn ENTER Zeichen kommt
                // Serial.print(F("Eingabe:'"));Serial.print(UART_buffer);Serial.print(F("' Laenge:"));Serial.println(strlen(UART_buffer));
          UART_Done=UAIdx;                // Merken der bisher angekommenen Zeichenanzahl 
          UART_buffer[UAIdx]=0x00;        // damit wird das ENDE der Zeichenkette bestimmt
          UAIdx=0;                        // Rückstellen des Zeichenzählers auf 0
          } else {                      
          UART_buffer[UAIdx]=UART_Char;   // solange das Endezeichen nicht ankommt werden die Zeichen gesammelt
          UAIdx++;                        // und der Zeichenzähler um 1 erhöht.
        }
      }
  }
  return UART_Done;                       // UART_Done bleibt auf 0 bis ein Ende-Zeichen ankommt, dann wird es auf die Anzahl Zeichen im Buffer gesetzt.
}


//--------------------------------------------------------------------------------------------------------
// Abarbeiten der im UART_buffer[] vorliegenden Zeichenkette, Interpretieren der angekommenen Zeichen
// 2019-01-13  es wird hier fix auf bestimmte Zeichen abgefragt. Siehe #define
// Nach dem 1.Zeichen, meist ein Buchstabe soll direkt ein nächster Wert folgen dürfen, bei allem Folgenden
//   wird ein Trennzeichen wie BLANK erwartet.
// Bsp: l(Language)1  soll die Sprache auf 1 umstellen
// Eine NICHTEINGABE von Zahlen nach dem 1.Zeichen kann erkannt werden indem  bStartPos abgefragt wird. (0=keine Zahl wurde eingegeben, 1 EINE Zahl, usw.)
//--------------------------------------------------------------------------------------------------------    
void UART_Parse()  {
    int16_t  bStartPos        = 0;                  // Startposition im UART_buffer
    int16_t  bEndPos          = UART_bufferlen;     // EndePosition im UART_buffer
    char     sbufferChar;                           // gefundenens Zeichen 
    char     sbufferChar1     = 0x00;               // 1. Zeichen wird gemerkt
    uint8_t  sbufferNrI       = 0;                  // Zähler von 0...4 für die 5 Nummern
    char     sbufferCh[2]     = {' ',0x00};         // wenn anstatt Zahlen weitere Buchstaben dem ERSTEN folgen Bsp: ?c  
    char *   UART_pt          = & UART_buffer[0];   // Zeiger auf UART_buffer[0]      

    if (Echo()==1) {Print_UART_buffer2UART(0,1);} // Ausgabe nach Methode 0=auf UART , mit 1 Stk Cr

    bStartPos = GetPosNEQ( UART_buffer, bStartPos, bEndPos, sbufferChar, ' ' );        // 1. gültiges Zeichen und Position bestimmen ab Stelle 0

    if (sbufferChar != 0x00) {                      // Wenn etwas gefunden wurde
      sbufferNr[0]=0;sbufferNr[1]=0;sbufferNr[2]=0;
      sbufferNr[3]=0;sbufferNr[4]=0;
      sbufferChar1 = sbufferChar;                   // merken des 1. Zeichens  
      bStartPos ++;                                 // Zeiger um 1 erhöhen
    }
    //---------------------------------------------------------------------------------------------------------------------------
    // in folgender Schleife werden die nachfolgenden NUMMERN +ev. ZEICHEN ermittelt die meist je durch mindesten EIN TRENNZEICHEN ' ' vereinzelt sind.
    // nach dem ersten Zeichen muß kein, kann aber ein Trennzeichen folgen Bsp: c3 1 = c 3 1 --> Commando 3 Befehl 1
    // Sonderfall: ? c 3   hier muß nach dem c mind. ein Trennzeichen folgen.
    //---------------------------------------------------------------------------------------------------------------------------
    do {

      bStartPos = GetPosNEQ(UART_buffer,bStartPos,bEndPos,sbufferChar,' ');     // nächstes gültiges ZEICHEN
      if (sbufferChar != 0x00) {                                        // es wurde ein nächstes ZEICHEN gefunden
        sbufferNr[sbufferNrI] = atol(UART_pt + bStartPos);              // n-te Zahl beginnend bei 0 ...4  merken
        if (sbufferNrI < 2) {                                           // nur maximal die ersten beiden Buchstaben ermitteln die dem Hauptzeichen folgen
          sbufferCh[sbufferNrI] = *(UART_pt + bStartPos);               // Hier sollte/könnte ein Zeichen vorliegen         
        }
        if (sbufferNrI == 2 && sbufferChar1 == UART_INF) {              // für die Spezialaufgabe 'i'nfo auf LCD: i Spalte Zeile Text
          sbufferNr[sbufferNrI] = bStartPos;                            // Position ab welcher die Zeichenkette für die Ausgabe auf LCD starten soll.
        }                   
        sbufferNrI ++;                                                  // Index erhöhen
        bStartPos = GetPosEQ(UART_buffer,bStartPos,bEndPos,sbufferChar,' ',' ');    // wo kommt nach der Zahl der nächste TRENNER
        if (sbufferChar != 0x00) { bStartPos ++; }                      // TRENNER gefunden       
      }
    } while (sbufferChar != 0x00);       // solange noch eine passende Position im buffer gefunden wird
        //----------------------------------------------------------------------------------------------------
        // temporäre Ausgabe der sauber eingelesenen Zeichen und Zahlen Nur für die Entwicklung nötig, kann also auskommentiert werden.
        //----------------------------------------------------------------------------------------------------
        if (Echo()== 1) {
          Serial.print(F(" INPUTS:'"));       Serial.print(sbufferChar1); 
          Serial.print(F("' sbufferCh[0]:'"));Serial.print(sbufferCh[0]);
          Serial.print(F("'[1]:'"));          Serial.print(sbufferCh[1]);
          Serial.print(F(" Cnt:"));           Serial.print(sbufferNrI);  // Anzahl eingegebener Zeichen/Zahlen NACH dem 1.Startzeichen 
          Serial.print(F("' z0-4:"));
          for (uint8_t i=0; i < 5; i++) { PBLANKS(); Serial.print(sbufferNr[i]); } 
          Serial.println();Serial.println();
        }

    // -----------------------------------------------------------------------------------------------------
    //  Abhängig vom 1. Buchstaben werden unterschiedliche Zahlen benötigt um die Funktionen auszuführen
    // -----------------------------------------------------------------------------------------------------
    //     Kürzel                        was ist das                        was tut das           Programmierzustand
    //     ------                       -------------                       -------------         ------------------
    switch (sbufferChar1) {
      case UART_AUS:                  // Auszug Tbl_                        AUSGEBEN UART         a (2019-01-15)  + + + F E R T I G + + +  (2019-01-19)
        a_AUS  (sbufferNr[0],sbufferNr[1],sbufferNr[2],sbufferNr[3],sbufferNr[4] ); //              Ausgabe beliebiger Zeilen aus TBL auf UART/LCD
        break;
      case UART_CMD:                  // Kommando                           AUSFÜHREN/LIEFERN     c (2019-01-20) Kommandos können noch wachsen daher nicht fertig
        c_DoCmd(sbufferNr[0],sbufferNr[1],sbufferNr[2],sbufferNr[3],sbufferNr[4] ); //            
        break;
      case UART_DAS:                  // Date Time Zeitstempel              LIEFERN               d (2019-01-20) Syntax 'd0 FormatNr'  od. d1..8 1=JJJJ 2=MO 3=DD usw. d10=Wochentag d11=Monat
        d_TIME (sbufferNr[0],sbufferNr[1] );  //                                                    ZEIT 
        break;
      case UART_MES:                  // Macro Status                       LIEFERN               e (2019-02-08) Wert aus Array m_info[sbufferNr[0]].LastState1
        e_MStat (sbufferNr[0],sbufferNr[1] ); //                                                     Ein Wert von 0...3  für den aktuellen/letzten Status des Macros
        break;
      case UART_FKT:                  // Funktion  (NICHT VERWENDET z.Zt. !!!!!!!!!!!!!!!)
        Notify(4);                    // Meldung Nr 4 auf UART ausgeben mit ENTER = 10 hinten dran.
        break;
      case UART_GER:                  // Gerät                              AUSFÜHREN / LIEFERN   g Geräte per  1,2, 3..255 schalten  0 Liefert Zustand oder andere Größen des Gerätes
        //      (g)erät Nrx,  0=Fragen 1..255, Frequ=0,    Duration=0, SaveNoState=1
        //              Nrx,                  Para3= (5=Gerätename, 6=Gerätebeschreibung  10=PIN  11=PWRON_DO  12=OWP_IS)
        g_SetGet(sbufferNr[0],sbufferNr[1],  sbufferNr[2], sbufferNr[3], sbufferNr[4] ); 
        break;
      case UART_INF:                  // Info an LCD ausgeben               AUSGEBEN              i (nfo) auf LCD  Spalte, Zeile, Text
            // Spalte,        Zeile,     buffer,      bufferStartPosition 
        i_LCD(sbufferNr[0],sbufferNr[1], UART_buffer, sbufferNr[2]);
        break;
      case UART_KOM:                  // Kombination von Geräten ansteuern  AUSFÜHREN / LIEFERN
        k_SetGet(sbufferNr[0], sbufferNr[1],  sbufferNr[2] );                            //       k Kombination per  1,2,3.. schalten 0 Liefert Zustand siehe Beschr.


      
        break;
      case UART_LNG:                  // Sprache                            AUSFÜHREN / LIEFERN   l Sprache
        if (sbufferNrI==0 || sbufferNr[0]== -1) {Notify(3,0); Serial.println(CurrLanguage()); }
          else { 
            CurrLanguage(sbufferNr[0]);                                     // Sprache setzen
            Notify(5,0); Serial.println(CurrLanguage());}                   // und nochmal ausgeben
        break;
      case UART_MCR:                  // Macro ausführen                    AUSFÜHREN
      
        break;
      case UART_NFY:                  // Notify 
        Notify(sbufferNr[0]);
        break;
      
      case UART_NF2:                  // Msg zum Testen 
        Msg(sbufferNr[0],sbufferNr[1], sbufferNr[2], sbufferNr[3]);  // , sbufferNr[4]);      // Bis zu 3 Stück Zahlen können zur Zeit in einen Text eingebaut werden
          // Msg(sbufferNr[0],sbufferNr[1], sbufferNr[2]);
        break; 
      
      case UART_OUT:                  // Ausgabe einer Meldungsnummer       AUSGEBEN
      
        break;
      case UART_PAR:                  // Parameter                          SETZEN / LIEFERN      p Parameter   p_SetGet(ParameterNummer, 0..9=Fragen  10=setzen, ParameterWert)
         //      ParameterNr, 0=Fragen Wert           7=ParameterName:Wert
         //                   1=Fragen Name          
         //                   2=Fragen Beschreibung  10=Parameterwert SCHREIBEN  
         //                   3=Fragen Einheit          mit dem Wert sbufferNr[2] 
         //                   4=Minimum Wert  
         //                   5=Maximum Wert
         //                   6=Standard Wert  
         
        p_SetGet(sbufferNr[0], sbufferNr[1], sbufferNr[2]);
        break;
        
      case UART_ERR:                  // Error                              LIEFERN
      
        break;
      case UART_SWI:                  // Schalter Zustand                   LIEFERN
      
        break;
      case UART_TEM:                  // Temperatur                         LIEFERN
      
        break;
      case UART_WEI:                  // Weight Gewicht abfragen            LIEFERN / Cali     w  WNr, 0=Fragen ob aktiv  1=Fragen Gramm  2=Fragen dag 3=Fragen Kg  10=Calibrieren
        //        Modul-Nr,     0,1,2,3,.. Fragen  10=Calibrieren 11=Tara
        w_CaliGet(sbufferNr[0], sbufferNr[1], sbufferNr[2]);
        break;

      case UART_HLP:                  // HELP zu x                          LIEFERN             + + + F E R T I G + + +  (2019-01-20) jedoch sind die Bereich noch fertig zu stellen
        // Serial.print(F(" Help_about("));Serial.print(sbufferCh[0]);Serial.print(F(","));Serial.print(sbufferNr[1]),Serial.print(F(","));Serial.print(sbufferNr[2]);Serial.println(F(")"));
        //                 c      -1 od.0..End  0=UART 1=LCD        
        Help_about(sbufferCh[0], sbufferNr[1],  sbufferNr[2] );  
        //  Gibt aus der TBL_ zum angegebenen Kürzel und Nummer den Datensatz aus bzw. den ganzen Bereich aus bei Nummer -1
        break;
      default: 
        Serial.print(sbufferChar1);Serial.print(F(":"));Serial.print(F("!"));Serial.println();  // ANTWORT auf unbekannte Anfrage
        break;  
    }
    // folgende Zeile muß detailliert in den jeweiligen case eingebaut werden nachdem die Aktion ausgeführt wurde.
    // Serial.print(sbufferChar1);Serial.print(sbufferNr[0]);Serial.print(F(":"));Serial.print(F("Name")); Serial.println(F(" OK"));
      
    UART_buffer[0] = 0x00;               // UART_buffer[1] = 0x00;  // das ist nötig damit strlen() = 0 ergibt.
    UART_bufferlen=0;                     // zuletzt noch Rückstellen des Zeigers für die Zeichenkettenlänge im buffer
}

//-------------------------------------------------------------------------------------------
// HX711 Wiegemodul 1..n abfragen  / Calibrieren  
//       0=Fragen ob aktiv  1= Gramm  2= dag  3= Kg   
//       4=Name  5=Beschreibung
//       6=Kalibrierungsfaktor  7=Offset 
//       10=Calibrieren starten
//       11=Tara mit aktuell anliegendem Gewicht auf 0 stellen.
//       12=Tara neu setzen.  (Wenn Para3 <> 0 wird fix das als Tara gewählt sonst das aktuell anliegende Gewicht.)
//-------------------------------------------------------------------------------------------
int32_t w_CaliGet(int32_t Para1, int32_t Para2, int32_t Para3) {
  if (Para1 > _HX711_CNT) {Para1 = _HX711_CNT; }
  if (Para1 < 1 )         {Para1 = 1; }
  switch (Para2) { 
    case 0:                                                                                   // Ist das Modul Aktiv 
      // Serial.print(F(" Modul Nr:"));Serial.print(Para1);Serial.print(F(" liefert:"));
      Serial.print(Waage_ready(Para1,0));Serial.println();
      break;
    case 1:                                                                                   // Gewicht in GRAMM
      // Serial.print(F(" Modul Nr:"));Serial.print(Para1);Serial.print(F(" Gramm:"));
      Serial.print(Waage_read(Para1,1));Serial.println();
      break;
    case 2:                                                                                   // Gewicht in dag
      // Serial.print(F(" Modul Nr:"));Serial.print(Para1);Serial.print(F(" dag:"));
      Serial.print(Waage_read(Para1,2));Serial.println();
      break;
    case 3:                                                                                   // Gewicht in Kg 
      // Serial.print(F(" Modul Nr:"));Serial.print(Para1);Serial.print(F(" Kg:"));
      Serial.print(Waage_read(Para1,3));Serial.println();      
      break;
    case 4:                                                                                   // Wiegemodul-NAME aus TBL 
      Serial.println(F2X(T_Idx(_S_HXL,Para1, _S03_NAME)));    
      break;
    case 5:                                                                                   // Wiegemodul-BESCHREIBUNG aus TBL
      Serial.println(F2X(T_Idx(_S_HXL,Para1, _S03_DESCR)));
      break;
    case 6:                                                                                   // FAKTOR aus EEPROM
      // Serial.print(F(" Calibr.Fakt for ModulNr:"));Serial.print(Para1);Serial.print(F(" Fact:"));
      Serial.print(Waage_CaliFact(Para1,0));Serial.println();
      break;
    case 7:                                                                                   // TARA aus EEPROM
      // Serial.print(F(" Offset for ModulNr:"));Serial.print(Para1);Serial.print(F(" Offset:"));
      Serial.print(Waage_CaliNr(Para1,0));Serial.println();
      break;
      
    case 10:                                                                                  // KALIBRIERUNG  
      HX711_UART_Calibration(Para1);                                                          // Vorgang der Kalibrierung über UART
      break;
    case 11:                                                                                  // TARA Wiegemodul auf 0 stellen mit dem aktuell darauf lastenden Gewicht
      HX711_UART_Tara(Para1,0); 
      break;
    case 12:                                                                                  // TARA Wiegemodul mit EINGABEGEWICHT versehen (händisches 0 stellen)
      HX711_UART_Tara(Para1,Para3); 
      break;
    default:
      Serial.print(F("w Commando "));Serial.print(Para2);Serial.print(F(" is not programmed !"));Serial.println();
      break;
  }
  return 0;
}

//--------------------------------------------------------------------------------------------------------
// PARAMETER lesen / schreiben / Informationen zum Parameter liefern (Beschreibung, Min, Max, usw.)
//    Para1 = Parameter-Nr   0... _PARAMETER_COUNT-1  
//    Para2 = 0..9 WERT FRAGEN  10= Wert schreiben   0=Wert 1=Name 2=Beschr. 3=Einheit 4=Min 5=Max 6=Default 
//    Para3 = NEUER WERT
//--------------------------------------------------------------------------------------------------------
uint32_t p_SetGet(int32_t Para1, int32_t Para2, int32_t Para3){
  if (Para1 > _PARAMETER_COUNT -1 || Para1 < 0) {Para1 = 0;}    // Absicherung des möglichen Parameter-Nummern-Bereiches
 
  if (Para2 >= 0 && Para2 < 10) {                                   // Parameter Fragen 
    switch (Para2) {
      case  0:                                                      // Wert aus EEPROM und sonst nichts liefern
        Serial.println(EE_ParaGet(Para1));              
        break;
      case  1:                                                      // nur Name aus TBL liefern
        Serial.println(F2X(T_Idx(_S_PAL,Para1, _S20_NAME)));
        break;
      case  2:                                                      // nur Beschreibung aus TBL
        Serial.println(F2X(T_Idx(_S_PAL,Para1, _S20_DESCR)));
        break;
      case  3:                                                      // nur Einheit aus TBL
        Serial.println(F2X(T_Idx(_S_PAD,Para1, _S21_UNIT)));
        break;
      case  4:                                                      // nur Minum-Wert aus TBL
        Serial.println(F2X(T_Idx(_S_PAD,Para1, _S21_MIN)));
        break;
      case  5:                                                      // nur Maximum-Wert aus TBL
        Serial.println(F2X(T_Idx(_S_PAD,Para1, _S21_MAX)));
        break;
      case  6:                                                      // nur Default-Wert aus TBL
        Serial.println(F2X(T_Idx(_S_PAD,Para1, _S21_DEF)));      
        break;
      case  7:                                                      // Name aus TBL:Wert aus EEPROM
        Serial.print(F2X(T_Idx(_S_PAL,Para1, _S20_NAME)));
        Serial.print(F(":"));
        Serial.println(EE_ParaGet(Para1));
        break;
      case  8:                                                      // Name:Wert[Einheit] 
        Serial.print(F2X(T_Idx(_S_PAL,Para1, _S20_NAME)));          // Name
        Serial.print(F(":"));                                       // : 
        Serial.print(EE_ParaGet(Para1));                            // Wert
        PBLANKS();                                                  // LEERZEICHEN
        Serial.println(F2X(T_Idx(_S_PAD,Para1, _S21_UNIT)));        // Einheit
        break;
    }
  } else {                // wenn Para2 >= 10                       // Parameter SETZEN / SCHREIBEN
    switch (Para2) {
      case 10:
          EE_ParaLet(Para1, Para3);                                 // Parameterwert 1:1 schreiben
          EE_ParaLetCheck(Para1, Para3);                            // Abhängig vom Parameter ist die Software ggf. SOFORT anzupassen, bzw. sind SOFORT Befehle auszuführen. Bsp. Menü-Rollover
          break;
      case 11:
          EE_ParaWork(Para1, Para3);                                // Wert addieren    Parameter + - 
          break;
      case 12:
          EE_ParaLet(Para1,TBL_Long(T_Idx(_S_PAD,Para1,_S21_MIN))); // Wert auf MINIMUM setzen 
          break;
      case 13:
          EE_ParaLet(Para1,TBL_Long(T_Idx(_S_PAD,Para1,_S21_MAX))); // Wert auf MAXIMUM setzen
          break;
      case 14: 
          EE_ParaLet(Para1,TBL_Long(T_Idx(_S_PAD,Para1,_S21_DEF))); // Wert auf DEFAULT setzen
          break;
      case 99:  //++++++ Alle Parameter auf DEFAULT setzen +++++++++
          FactoryParameters();
          break;
    }
  }
  return 0;
}

//------------------------------------------------------------------------------
// Parameteränderung ggf. SOFORT umsetzen nachdem ein Parameter verändert wurde
//  Bsp: Parameter 5  LCD-Menue-Rollover 
//------------------------------------------------------------------------------
void EE_ParaLetCheck(int32_t Para1, int32_t Para3) {
  switch (Para1) {
    case 5:   
      if (Para3 == 1) {
        LCDML.MENU_enRollover();          // Menü-Sprung vom letzten zum ersten und vom ersten zum letzten bei Überdrehen
      } else {
        LCDML.MENU_disRollover();         // Menü-Stop am Ende und Stop am Beginn
      }      
      break;
    
  }
}


//----------------------------------------------------------------------------------------------------------
// Kombination von Geräten oder auch nur eines Gerätes  schalten/abfragen 
// Bsp: k1 0 --> liefert nur den Zustand 
//      k1 1 --> schaltet Kombinatin auf Zustand wie defniert auf AUS 
//  (k)erät Nrx,  0=Fragen,      Welche Info,       SaveNoState=1
//          Nrx,  0=Fragen       Para3=(0=DEC-Status, 1=BIT-Status, 5=KombinationName, 6=Beschreibung  
//                         direct       10=DoAtPwrON      11=DoAtPwrON  12=ON Kommands   13=OFF Kommands 
//                         Bsp:         g1 p1, g1 p2
//                         translated   20=g1 100, g1 200 21=...        22=...           23=...
//          Para2 1,2,3  5,6,7 
//                1=Do Off-Kommand          2=Do On-Kommand         3=Do Initialization Kommand 
//                5=switch every g-Nr OFF   6=switch every g-Nr ON  7=switch every g-Nr to Invert
//----------------------------------------------------------------------------------------------------------
//                 KombiNr,       0..7         0..22          SaveNoState=0 Default Status wird mit dem Setzen gespeichert

uint8_t k_SetGet(int32_t Para1, int32_t Para2, int32_t Para3) {   
  if (Para1 > _KOMBINATION_COUNT || Para1 < 0) {Para1 = 0;} 
  if (Para2 == 0) {     // nur FRAGEN 
    switch (Para3) {    // WAS genau Fragen
      case 0:  Serial.println(obj_Status_DEC(UART_KOM,Para1,Para2));                break;  // den exakten Kombinations-Status liefern
      case 1:  Serial.println(obj_Status_BIT(UART_KOM,Para1,Para2));                break;  //  
      
      case 5:  Serial.println(F2X(T_Idx(_S_KOL,Para1,_S30_NAME )));                 break;  // Gerätename
      case 6:  Serial.println(F2X(T_Idx(_S_KOL,Para1,_S30_DESCR)));                 break;  // Gerätebeschreibung
      case 7:  break; // frei
      case 8:  break; // frei
      case 9:  break; // frei
      
      case 10: Serial.println(F2X(T_Idx(_S_KOD,Para1,_S33_AT_PWRON_DO)));           break;  // Todo Kommands after Reset  
      case 11: Serial.println(F2X(T_Idx(_S_KOD,Para1,_S33_AT_PWRON_DO)));           break;  // Todo Kommands after Reset 
      case 12: Serial.println(F2X(T_Idx(_S_KOD,Para1,_S33_ON_DO)));                 break;  // ON  Kommandos
      case 13: Serial.println(F2X(T_Idx(_S_KOD,Para1,_S33_OFF_DO)));                break;  // OFF Kommandos
      
      case 20: // ---- Translated Kommandos mit ersetzten Parameterwerten ----------------------------------------------------
      case 21: Serial.println(F2X(T_Idx(_S_KOD,Para1,_S33_AT_PWRON_DO)));           break;  // Todo Kommands after Reset 
      case 22: Serial.println(F2X(T_Idx(_S_KOD,Para1,_S33_ON_DO)));                 break;  // ON  Kommandos
      case 23: Serial.println(F2X(T_Idx(_S_KOD,Para1,_S33_OFF_DO)));                break;  // OFF Kommandos
      
      default: break;
    }
   }
   else {
    // Kombination AUSFÜHREN --------------------------------
    //             k-Nr   1,2,3, 5,6,7  SaveNoState, SaveNoGState
    Combination_DO(Para1, Para2,        Para3,      0);
    
   }
  return 99; 
}

//------------------------------------------------------------------------------------
// Gerät  schalten/abfragen 
// Bsp: g1 0 --> liefert nur den Zustand 
//      g1 1 --> setzt Gerät auf AUS 
//  (g)erät Nrx,  0=Fragen 1..255, Frequ=0,    Duration=0, SaveNoState=1
//          Nrx,  0,               Para3= (0=DEC-Status,  1=BIT-Status, 5=Gerätename, 6=Gerätebeschreibung  10=PIN  11=PWRON_DO  12=OWP_IS)
//------------------------------------------------------------------------------------
//                 GerätNr,       0..255         Frequ,         Dauer,         SaveNoState=0 Default Status wird mit dem Setzen gespeichert
uint8_t g_SetGet(int32_t Para1, int32_t Para2, int32_t Para3, int32_t Para4, int32_t = 0);
uint8_t g_SetGet(int32_t Para1, int32_t Para2, int32_t Para3, int32_t Para4, int32_t Para5 ) {
  if (Para1 > _MACHINE_COUNT || Para1 < 0) {Para1 = 0;} 
  
  if (Para2 == 0 ) {  // && Para3 == 0) {     // nur FRAGEN 
    switch (Para3) {    // WAS genau Fragen
      case 0:  Serial.println(obj_Status_DEC(UART_GER,Para1,Para2));                    break;  // den exakten Ausgangs-Status liefern
      case 1:  Serial.println(obj_Status_BIT(UART_GER,Para1,Para2));                    break;  //  
      
      case 5:  Serial.println(F2X(T_Idx(_S_GEL,Para1,_S30_NAME )));                     break;  // Gerätename
      case 6:  Serial.println(F2X(T_Idx(_S_GEL,Para1,_S30_DESCR)));                     break;  // Gerätebeschreibung
      case 7:  break; // frei
      case 8:  break; // frei
      case 9:  break; // frei
      case 10: Serial.println(F2X(T_Idx(_S_GED,Para1,_S31_PIN_I2CADDR_BIT)));           break;  // PIN_I2C_ADDR_und BIT
      case 11: Serial.println(F2X(T_Idx(_S_GED,Para1,_S31_AT_PWRON_DO)));               break;  // Power on after Reset ? 0=nothing do  1=OFF  bzw. 2=ON  3...255 PWM
      case 12: Serial.println(F2X(T_Idx(_S_GED,Para1,_S31_OWP_IS)));                    break;  // is on WHEN Pin is 0 / 1  
      default: break;
    }
   }
   else {
      // int16_t Machine_DO(uint8_t MIdx,uint8_t MToDo,uint16_t Fq,uint32_t FqDur,uint8_t SaveNoState)
      //        g-Nr  ToDo   Fq     Dura   SaveNo
    Machine_DO(Para1, Para2, Para3, Para4, Para5);
   }
  return 99; 
}

//--------------------------------------------------------------------------------
// Macro-Status  liefern auf UART 
// Bsp: e1  --> liefert 0...3
//      e1 10 --> setzt den Status des Macros 1 auf 0
//      e1 11 --> setzt Status fu 1   e1 13 --> setzt auf 3
// ACHTUNG innerhalb eines Macros bedeutet e2 --> setzte den Status vom eigenen Macro auf 2
//--------------------------------------------------------------------------------
uint8_t e_MStat (int32_t Para1, int32_t Para2 ) {
  if (Para1 > _MACRO_COUNT || Para1 < 0) {Para1 = 0;}
  if (Para2 == 0) {  Serial.print(m_info[Para1].LastState1); Serial.println(); }
  if (Para2 >=10 && Para2 <= 13) {m_info[Para1].LastState1 = Para2 - 10;   }
  return m_info[Para1].LastState1;
}

//-------------------------------------------------------------------------------------------------
// Zeitstempel, Datum, Jahr, Monat, Tag, Stunde, Minute, Sekunde, ..... auf LCD bzw. UART ausgeben
// LIEFERN    (SETZEN mit einem c...? )
// Datum und Zeitformate können und werden pro Sprache in TBL_ definiert.
//-------------------------------------------------------------------------------------------------
uint8_t d_TIME(int32_t CmdNr, int32_t Para1) { 
  RTC2timestamp();                                                        // Zeit vom RTC-Modul frisch abholen und in der timestamp. Struktur ablegen
  switch (CmdNr) {
    case 0: 
      F2X(T_Idx(_S_FMD,Para1));                                           // Formatierungs-Zeichenkette für die aktuell eingestellte Sprache in Buffer1[] holen
      Serial.println(TimeInfo(Buffer1));
      break;
    case 1:                                                               // JJJJ
    case 2:                                                               // MO
    case 3:                                                               // DD
    case 4:                                                               // HH
    case 5:                                                               // MM
    case 6:                                                               // SS
    case 7:                                                               // millis()
    case 8:                                                               // micros()
    case 9:                                                               // Tagnummer 0....6 
    case 10:                                                              // Kalender-Wochen-Nummer 1...53 
    case 11:                                                              // nix
      Serial.println(TimePartNr(CmdNr));        break;
    case 12:                                                              // Tagesname   vom aktuellen Datum
    case 13:                                                              // Monatsname  vom aktuellen Datum
      Serial.println(TimePartTx(CmdNr));        break;
    case 20:                                                              // _WEEKDAYLONG  laut Para1
      Serial.println(F2X(T_Idx(_S_DAY,Para1))); break;
    case 21:                                                              // _WEEKDAYSHORT laut Para1 
      Serial.println(F2X(T_Idx(_S_DAK,Para1))); break;
    case 22:                                                              // _MONTHNAMELONG
      Serial.println(F2X(T_Idx(_S_MON,Para1))); break;
    case 23:                                                              // _MONTHNAMESHORT
      Serial.println(F2X(T_Idx(_S_MOK,Para1))); break;
    default:
      break;
  }
  return 0;
}

//-----------------------------------------------------------------------------------------------------
// 2019-01-20  HILFE zu einem Buchstaben, Nr_Detail 
//  durch die Eingabe von ? c 1 wird als Beispiel der Bereich in der TBL aufgelistet der zum Commando 1 gehört
//  Durch ? c -1(-EINS) wird der gesamte Bereich zu den Kommandos aufgelistet die Zahl -1 bedeutet also immer ALLES zum Bereich des Buchstaben
//  Para2: die Zahl für das Element im Bereich od. -1 für ALLE
//  Para3: die Schnittstelle an die ausgegeben werden soll 0=Standard = UART  1=LCD Spalte 0, Zeile 0 FIX
//-----------------------------------------------------------------------------------------------------
//                      c,a,...              0..            0=UART
uint8_t Help_about(char sbufferCh,  int32_t Para2, int32_t Para3 ) {
  int32_t AdrFr;  // Von Adresse
  int32_t AdrTo;  // Bis Adresse 
  switch (sbufferCh) {
      case UART_AUS:                                                    // Auszug Tbl_                         

        break;
      case UART_CMD:                                                    // Kommando                 SYNTAX-HILFE                    
        if (Para2 > _S09_RECORDS-1) {Para2 = _S09_RECORDS -1 ; } 
        if (Para2 < 0) {                                                // Ganzen Kommando-Bereich ausgeben in Sprache 0
          AdrFr = _S09_OFFS;
          AdrTo = _S09_OFFS + _S09_RECORDS * _S09_FIELDS -1; 
          } else {                                                      // nur ein Kommando ausgeben
          AdrFr = _S09_OFFS + Para2 * _S09_FIELDS;
          AdrTo = _S09_OFFS + Para2 * _S09_FIELDS  + _S09_FIELDS -1; 
        }
         //  Serial.print(F("a_AUS(")); Serial.print(AdrFr); Serial.print(F(",")); Serial.print(AdrTo); Serial.print(F(","));Serial.print(Para3);Serial.println(F(")"));
        a_AUS  (AdrFr , AdrTo ,  Para3,  1,  10   );  
        break;
        
      case UART_DAS:                  // Date Time Zeitstempel               
         
        break;
      case UART_MES:                  // Macro Status                        

        break;
      case UART_FKT:                  // Funktion
      
        break;
      case UART_GER:                  // Gerät                               
         
        break;
        
      case UART_INF:                  // Info an LCD ausgeben                
 
        break;
      case UART_KOM:                  // Kombination von Geräten ansteuern   
      
        break;
      case UART_LNG:                  // Sprache 
      
        break;
      case UART_MCR:                  // Macro ausführen 
      
        break;
      case UART_OUT:                  // Ausgabe einer Meldungsnummer 
      
        break;
      case UART_PAR:                  // Parameter 
      
        break;
      case UART_ERR:                  // Error 
      
        break;
      case UART_SWI:                  // Schalter Zustand 
      
        break;
      case UART_TEM:                  // Temperatur  
      
        break;
      case UART_WEI:                  // Weight Gewicht abfragen 
      
        break;

      case UART_HLP:                  // HELP zu x  
                                            
        break;
  }
   // Serial.print(F("Help_about sbufferCh[0]"));Serial.println(sbufferCh[0]);
  return 0;
}

//------------------------------------------------------------------------------------------------------------------
// 2019-01-29  c(ommandos) können jetzt auch Werte liefern und sollen damit f(unktionen) ablösen.
//             Anlass war da beides mehr oder weniger FEST codiert ist.
//             mit 'c6 = 115200' kann man also in einem Macro abfragen ob die aktuelle BAUDRATE 115200 ist.
// 2018-03-25  Ausführen eines fest codierten Kommandos (c), identifiziert über eine feste Nummer. 
// (c) Kürzel für Command gefolgt von einer Kommando-Nummer und Parametern zum jeweiligen Kommando.
//     Auch die c(ommandos) werden in der TBL_ geführt um Details (Kurzname, Bedeutung) parat zu haben.
// Nr=Commando-Nummer,  Para=Parameter,  DIdx= optional Task-Index (z.Zt. für c 4 nötig)
//------------------------------------------------------------------------------------------------------------------
int32_t c_DoCmd(int32_t CmdNr, int32_t Para1, int32_t Para2, int32_t Para3, int32_t Para4) {
      //Serial.print(F("c_DoCmd CmdNr:"));Serial.print(CmdNr);Serial.print(F(" Para1:"));Serial.println(Para1);
  int32_t RetVal = 0;  // Rückgabewert OPTIONAL, bei manchen Commandos relevant
  switch (CmdNr) {
    case 0:                                                             // Ausgabe Commando 0 =Infos auf UART 
      a_AUS  (_S09_OFFS, _S09_OFFS+_S09_FIELDS-1, 0, 1, 10 ); 
      break;
    case 1:                                                             // LCD-Back    H I N T E R G R U N D - L I C H T  LCD  
      if (Para1<1) {
          RetVal = LCD_BackLight(Para1);
          Notify(6,0);Serial.println(RetVal);  
          }
      LCD_BackLight(Para1);                                             // Para 0=noop 1=AUS  2=EIN
      break;
    case 2:                                                             // REBOOT 
      if (Para1 == 2) soft_reset();
      break;
    case 3:                                                             // Onboard-LED  1=AUS 2=EIN  3..255 PWM
      RetVal = LED_Onbord(Para1);
      if (Para1 == 0) {
        Notify(7,0);Serial.println(RetVal); 
      }
      break;
    case 4:                                                             // PIN_DO(Pin, 0=LOW 1=HIGH  or PWM)
      PIN_Do (Para1, Para2);                                            // Ausgabe von logisch Para2 0=LOW 1=HIGH 2..255 PWM auf PIN direkt Es wird ohne jegliche Prüfung der angegeben PIN geschaltet !!!
      break;
    case 5:                                                             // PIN_SET(Pin, INPUT  OUTPUT  INPUT_PULLUP)
      PIN_Set(Para1, Para2);                                            // Para1=PinNr   Para2: 1=INPUT  2=INPUT_PULLUP  3=OUTPUT 
      break;
    case 6:                                                             // Abfragen, Einstellen BAUDRATE
      if (Para1<2) {
        RetVal=SetUART(-1);
        Notify(8,0);Serial.println(RetVal);}
      if (Para1 == 2 && (Para2 >=300 || Para2<= 115200)) {SetUART(Para2);}
      break;
    case 7:                                                             // Ausgabe TBL Punkte/ Abschnitte STARTS OFFSETS  + optional FeldNr
      UART_OUT_TBL(Para1,Para2);
      break;
    case 8:                                                             // Ausgabe TBL von bis Adresse auf UART 
      // a_AUS( AdrFrom,  AdrTo,  ToDevice,  Cursor_Spalte,  Cursor_Zeile,  HideEmptys)
      //             0,    10       nix      1=mit Adr.      nix            0=mit Leerzeilen     
      a_AUS (Para1,Para2,0,Para3,Para4, 0 ); 
      break;
    case 9:                                                             // Ausgabe TBL von bis Adresse auf LCD SPALTE,ZEILE 
      a_AUS (Para1,Para2,1,Para3,Para4 ); 
      break;  
    case 10:                                                            // MACRO-STATUS  Abfragen / Setzen vom Macro-Nr Para1  auf  Para2  0...3 
      // Para1=MacroNr  Para2=0(Ask)1(Let)  Para3=0...3 Wert
      if (Para1 > _MACRO_COUNT) {Para1 = _MACRO_COUNT;} 
      if (Para2 == 1) {                                                 // Status setzen
        m_info[Para1].LastState1 = Para3; }
       else {                                                           // Status abfragen
        RetVal=m_info[Para1].LastState1; 
        Notify(9,0); Serial.print(Para1);
        Notify(10,0);Serial.println(RetVal); 
        }    
      break;
    case 11:                                                            // MACRO-Nr FROM - TO  KILL KOMMANDO
      if (Para2 > _MACRO_COUNT) {Para2 = _MACRO_COUNT;}
      for (uint8_t i=Para1; i <= Para2; i++) {                          // VON - BIS  MACRO 
        if (m_info[Para1].Running1 == 1) {                              // Wenn das Macro als LAUFEND markiert ist
          m_info[Para1].StepNr1 = 31;                                   // siehe Task_Type.h wg. Erklärung
        }
      }   
    case 12:                                                            // MACRO Reserve
      break;
    case 13:                                                            // ERROR-LOG WRITE   Para1=Err-Nr  Para2=  1(unconfirmed Count)  2(confirmed Count) +1 erhöhen
      Error_Add_Cnt(Para1,Para2);
      break;
    case 14:                                                            // ERROR-LOG READ    Para1=Err-Nr  Para2=  1(unconfirmed Counter)   2(confirmed Counter) auslesen und liefern
      RetVal=Error_Read(Para1,Para2);                                   //                                         3 last (timestamp.JJJJ .MO .DD .HH. MM. SS         5 Byte struct)
                                                                        // timestamp wurde mit dem jeweiligen Error-Ereignis gefüllt
        Serial.print(F("Err:"));Serial.print(Para1);
        if (Para2 ==1) {
          Serial.print(F(" unconfirmed Count:")); 
          Serial.println(RetVal);
        }
        if (Para2 ==2) {
          Serial.print(F(" total Count:"));
          Serial.println(RetVal);
        }
        if (Para2 ==3) {
          Serial.print(F(" occured at:"));
          Timestamp2OUT(0,0,1); // Zeitstempel Ausgabe auf UART  1=ohne LF  2=mit LF
          Serial.println();
        }
      break;
    case 15:                                                            // ERROR-LOG RESET  FROM - TO  1=unconfirmed 2=confirmed 3=both 
      Error_Clear(Para1,Para2);                                         //   Para1 0 = ALL Reset   Para2 1=unconf  2=conf.  3=both  4=both+Time to 0
      break;
    case 16:                                                            // ERROR Reserve 
    case 17:                                                            // ERROR Reserve 
      break;
    case 18:                                                            // LET / ASK  Indicator Common BIT-Array manually 
      // sStati BITs ändern/fragen   Para1 =BitNr 1-8   Para2 = 0=ASK  1=LET   Para3=Set BitValue 1=0=LOW  2=1=HIGH
      if (Para1 > 8) Para1 = 8;   // BIT Check
      if (Para1 < 1) Para1 = 1;   // BIT Check 
      
      if (Para3 < 0) Para3 = 0;   // 0 = undefined 
      if (Para3 ==1) Para3 = 0;   // 1 = LOW
      if (Para3 > 1) Para3 = 1;   // 2 = HIGH 
      if (Para2 == 0) {
        RetVal = bitRead( sStati, Para1-1) +1;                          // damit wird bei LOW 1  und bei HIGH 2 geliefert  
        Serial.print(F("sStati["));Serial.print(Para1-1);Serial.print(F("]:"));Serial.println(RetVal);
      }
        else {
                bitWrite( sStati, Para1-1, Para3 );                     
        }
      break;
      
    case 19:                // GERÄT  - BIT                             // LET / ASK  Indicator g - BIT-Array manually
      // obj_stateBIN[] ändern/fragen Para1 =(g)erät-NR      Para2 = 0=ASK 1=LET 0=LOW 2=HIGH
      RetVal = obj_Status_BIT(UART_GER, Para1, Para2);
      if (Para2 == 0) {Serial.print(F("g-BIT "));Serial.print(Para1);Serial.print(F(" :"));Serial.println(RetVal); }
      break;
    case 20:               // GERÄT  - VALUE BYTE                       // LET / ASK  g - BYTE-Array manually 
      // obj_stateDEC[] ändern/fragen Para1 =(g)erät-NR      Para2 = 0=ASK 1=LET OFF 2=LET ON 3..255 PWM
      RetVal = obj_Status_DEC(UART_GER, Para1, Para2);
      if (Para2 == 0) {Serial.print(F("g-BYTE "));Serial.print(Para1);Serial.print(F(" :"));Serial.println(RetVal); }
      break;
      
    case 21:               // KOMBINATION - BIT                        // LET / ASK  Indicator k - BIT-Array manually 
      // obj_stateBIN[] ändern/fragen Para1 =(k)ombination-NR      Para2 = 0=ASK 1=LET 0=LOW 2=HIGH
      RetVal = obj_Status_BIT(UART_KOM, Para1, Para2);
      if (Para2 == 0) {Serial.print(F("k-BIT "));Serial.print(Para1);Serial.print(F(" :"));Serial.println(RetVal); }
      break;
    case 22:               // KOMBINATION - VALUE BYTE                 // LET / ASK  k - BYTE-Array manually
      // obj_stateDEC[] ändern/fragen Para1 =(k)ombination-NR      Para2 = 0=ASK 1=LET OFF  2=LET ON  3=ALL (g)OFF  4=ALL (g)ON  5=ALL like ON=2 but invert
      RetVal = obj_Status_DEC(UART_KOM, Para1, Para2);
      if (Para2 == 0) {Serial.print(F("k-BYTE "));Serial.print(Para1);Serial.print(F(" :"));Serial.println(RetVal); }
      break;

// bis hier (inkl. 22)alles getestet. 2019-02-08 
        
     // LCD_Numb(0,0,  Para1, Para2 );                // Ausgabe einer ZAHL formatiert auf bestimmte Länge auf LCD 0,0
            

    //    case 10: //  Task für das Aufbauen und Konstanthalten des System-Soll-Druckes 0=stop 1=run
    //      bitWrite(sStati,3,Para);  // D R U C K H A L T E - T A S K
    //      if(Para > 0) {t12.enableIfNot();} else {t12.disable();}
    //      break;
    //      
    //    case 12: // SPEICHERE die im RAM befindlichen Wasserzählimpulse ins EEPROM 
    //      EE_Save_WaterImpulse(WaterImpulses[1]);  //  S A V E   Impulses (Wasserzählerstand)
    //      WaterImpulses[1]=0;  
    //      break;
    case 99:  // nur damit der Compiler keine unverwendete Variable anmeckert
      Serial.print(F("c_DoCmd("));Serial.print(CmdNr);Serial.print(F("): P1:"));Serial.print(Para1);
      Serial.print(F(" P2:"));Serial.print(Para2);Serial.print(F(" P3:"));Serial.print(Para3);Serial.print(F(" P4:"));Serial.println(Para4);
    default:
      // Serial.print(F(" DoCmd Nr:"));Serial.print(Nr);Serial.print(F(" Para:"));Serial.print(Para);Serial.println(F(" nicht bekannt !"));
      break;
  }
  // Para3 = 0; Para4 = 0; // damit bis zur Verwendung der Compiler nichts anjammert
  return RetVal;
}

//-------------------------------------------------------------------------------------------------------------
// Bereichs-Start Ausgabe aus TBL_  zum Zweck der Überprüfung ob die Positionen in der TBL noch stimmig sind 
// zum PRÜFEN ob die Struktur der TBL noch in Ordnung ist.
//-------------------------------------------------------------------------------------------------------------
int8_t UART_OUT_TBL(int32_t Para1, int32_t =0);
int8_t UART_OUT_TBL(int32_t Para1, int32_t Para2) {
  Serial.print(F("Adr:"));
  switch (Para1) {
    case  0: Serial.print(_S00_OFFS + Para2);PBLANKS();Serial.println(F2X(_S00_OFFS+Para2)); break;  // en, de, fr
    case  1: Serial.print(_S01_OFFS + Para2);PBLANKS();Serial.println(F2X(_S01_OFFS+Para2)); break;  // engl,germ,fren
    case  2: Serial.print(_S02_OFFS + Para2);PBLANKS();Serial.println(F2X(_S02_OFFS+Para2)); break;  // Meldungen Notify()
    case  6: Serial.print(_S06_OFFS + Para2);PBLANKS();Serial.println(F2X(_S06_OFFS+Para2)); break;  // --
    case  7: Serial.print(_S07_OFFS + Para2);PBLANKS();Serial.println(F2X(_S07_OFFS+Para2)); break;  // date:%04..
    case  8: Serial.print(_S08_OFFS + Para2);PBLANKS();Serial.println(F2X(_S08_OFFS+Para2)); break;  // frei
    case  9: Serial.print(_S09_OFFS + Para2);PBLANKS();Serial.println(F2X(_S09_OFFS+Para2)); break;  // cmd 0-9
    case 10: Serial.print(_S10_OFFS + Para2);PBLANKS();Serial.println(F2X(_S10_OFFS+Para2)); break;  // DS18
    case 11: Serial.print(_S11_OFFS + Para2);PBLANKS();Serial.println(F2X(_S11_OFFS+Para2)); break;  // DS18 Pin, Adr
    
    case 12: 
      Serial.print(_S12_OFFS+Para2);PBLANKS();Serial.println(F2X(_S12_OFFS+Para2,Buffer2));
      Serial.print(F("Inhalt von Buffer2:"));Serial.println(Buffer2);
      Serial.print(_S12_OFFS+Para2);PBLANKS();Serial.println(F2X(_S12_OFFS+Para2));
      Serial.print(F("Inhalt von Buffer1:"));Serial.println(Buffer1);
      break;
    
    // case 12: Serial.print(_S12_OFFS + Para2);PBLANKS();Serial.println(F2X(_S12_OFFS+Para2)); break;  // menu
    case 13: Serial.print(_S13_OFFS + Para2);PBLANKS();Serial.println(F2X(_S13_OFFS+Para2)); break;  // menu jmp 
    case 14: Serial.print(_S14_OFFS + Para2);PBLANKS();Serial.println(F2X(_S14_OFFS+Para2)); break;  // [g] gram, [kg] kilo,
    case 15: Serial.print(_S15_OFFS + Para2);PBLANKS();Serial.println(F2X(_S15_OFFS+Para2)); break;  // sunday,monday,
    case 16: Serial.print(_S16_OFFS + Para2);PBLANKS();Serial.println(F2X(_S16_OFFS+Para2)); break;  // sun,mon,wed,
    case 17: Serial.print(_S17_OFFS + Para2);PBLANKS();Serial.println(F2X(_S17_OFFS+Para2)); break;  // January,February,
    case 18: Serial.print(_S18_OFFS + Para2);PBLANKS();Serial.println(F2X(_S18_OFFS+Para2)); break;  // Jan,Feb,Mar,
    case 19: Serial.print(_S19_OFFS + Para2);PBLANKS();Serial.println(F2X(_S19_OFFS+Para2)); break;  // 
    case 20: Serial.print(_S20_OFFS + Para2);PBLANKS();Serial.println(F2X(_S20_OFFS+Para2)); break;  // Parameter sprachabhängiger Teil
    case 21: Serial.print(_S21_OFFS + Para2);PBLANKS();Serial.println(F2X(_S21_OFFS+Para2)); break;  // Parameter Daten Teil
    case 22: Serial.print(_S22_OFFS + Para2);PBLANKS();Serial.println(F2X(_S22_OFFS+Para2)); break;  // ERROR sprachabhängiger Teil
    case 23: Serial.print(_S23_OFFS + Para2);PBLANKS();Serial.println(F2X(_S23_OFFS+Para2)); break;  // ERROR Daten Teil
    case 28: Serial.print(_S28_OFFS + Para2);PBLANKS();Serial.println(F2X(_S28_OFFS+Para2)); break;  // EINGANG/SCHALTER sprachabhängiger Teil
    case 29: Serial.print(_S29_OFFS + Para2);PBLANKS();Serial.println(F2X(_S29_OFFS+Para2)); break;  // EINGANG/SCHALTER Daten Teil
    case 30: Serial.print(_S30_OFFS + Para2);PBLANKS();Serial.println(F2X(_S30_OFFS+Para2)); break;  // Geräte Sprachen
    case 31: Serial.print(_S31_OFFS + Para2);PBLANKS();Serial.println(F2X(_S31_OFFS+Para2)); break;  // Geräte Daten
    case 32: Serial.print(_S32_OFFS + Para2);PBLANKS();Serial.println(F2X(_S32_OFFS+Para2)); break;  // Kombination Sprachen
    case 33: Serial.print(_S33_OFFS + Para2);PBLANKS();Serial.println(F2X(_S33_OFFS+Para2)); break;  // Kombination Daten
    case 34: Serial.print(_S34_OFFS + Para2);PBLANKS();Serial.println(F2X(_S34_OFFS+Para2)); break;  // Spezial Schalter Sprachen
    case 35: Serial.print(_S35_OFFS + Para2);PBLANKS();Serial.println(F2X(_S35_OFFS+Para2)); break;  // Spezial Schalter Daten   
    case 40: Serial.print(_S40_OFFS + Para2);PBLANKS();Serial.println(F2X(_S40_OFFS+Para2)); break;  // MACRO-KOPF  Sprachabhängig
    case 41: Serial.print(_S41_OFFS + Para2);PBLANKS();Serial.println(F2X(_S41_OFFS+Para2)); break;  // MACRO-KOPF  Daten    
    case 43: Serial.print(_S43_OFFS + Para2);PBLANKS();Serial.println(F2X(_S43_OFFS+Para2)); break;  // MACRO-SCHRITTE  Daten    
    case 99: Serial.print(_S99_OFFS + Para2);PBLANKS();Serial.println(F2X(_S99_OFFS+Para2)); break;  // ENDE
    //case 18: Serial.print(_S.._OFFS + Para2);Serial.print(F(" "));Serial.println(F2X(_S0a_OFFS+Para2)); break;    
    //case 19: Serial.println(F2X(_S00_OFFS+Para2)); break;
    //case 20: Serial.println(F2X(_S0a_OFFS)); break;
  }
  return 0;
}


//--------------------------------------------------------------------------------------------------------------
// Leere Stellen können nun mittels Parameter OutEmpty=0 ausgelassen oder 1=ausgegeben werden. 
// Ausgabe auf ToDevice=0=LCD 1=UART  einer Stelle (laut AdrFrom) aus Tbl bis Stelle (laut AdrTo)
// Achtung der Buffer1[_FLASH_BUFFLEN] wird mit dem Inhalt Tbl_ von Adresse Adr befüllt.
// Achtung wenn diese Funktion mit einem großen Adressbereich aufgerufen wird ist der arduino
// mit dem Lesen und Ausgeben beschäftigt !! (wenn das stört müsste ein Task bemüht und diese Funktion zerteilt werden)
// Ist AdrFrom > AdrTo wird in umgekehrter Reihenfolge ausgegeben !!
// Bei Ausgabe auf UART wird Cursor_Spalte interpretiert ob die Adress-Nummer gefolgt von PUNKT vor den Inhalt zu stellen ist.
// Bsp:   Cursor_Spalte=1 dann  1.ge <ENTER>  2.fr <ENTER>   Cursor_Spalte=0 dann  ge <ENTER> fr <ENTER>
//        Cursor_Zeile=0 dann ge de fr                       Cursor_Zeile=10 dann ge <ENTER> weil ENTER = 10 ist
//    VonAdr,      BisAdr,      0=UART       Wenn BisAdr < VonAdr  --> Fallende Ausgabe   
//    VonAdr,      BisAdr,      0=UART       1=Lfd.Text   10=Default=ENTER 9=TAB
//    VonAdr,      VonAdr,      0=UART       0=nur Text   Wert für Zeilen-Trenner  
//    VonAdr,      BisAdr,      1=LCD        LCD_Spalte   LCD_Zeile
//--------------------------------------------------------------------------------------------------------------
int8_t a_AUS(int32_t AdrFrom, int32_t AdrTo, int32_t ToDevice, uint8_t Cursor_Spalte, uint8_t Cursor_Zeile, int8_t HideEmptys) {
  int16_t AdrI = AdrFrom;           // Schleifenzähler 
  int8_t StepNr = 1;                // wenn AdrTo > AdrFrom wird automatisch von UNTEN nach OBEN gelesen sonst normal von OBEN nach UNTEN 
  if (Cursor_Zeile == 0)    { Cursor_Zeile = 10; }  // Default ist ENTER = 10 
  if (AdrFrom  > AdrTo)     { StepNr = -1; }
  if (AdrTo    > _S99_OFFS) { AdrTo = _S99_OFFS; } 
  
  do {
      F2X(AdrI);         // Buffer1[] mit dem Inhalt aus dem Flash an Adresse Adr befüllen, ohne den gesamten Buffer1[] zuvor mit 00 zu füllen
      if (HideEmptys==1)  {Buffer1Len=F_bufflen();}  else {Buffer1Len= 1;}  // echte Buffer1Len bzw. fix 1 wenn auch LEERE auszugeben sind.
      
      if (Buffer1Len > 0) { 
        if (ToDevice == 0) { 
          // Ausgabe auf UART -----BEGINN----
            if (Cursor_Spalte == 1) {                       // Die Adress-Zahl gefolgt von .  
              Serial.print(AdrI); Serial.print(F(". "));    // 0.    1.     2.   usw.
            }
            Serial.print(Buffer1);                         // Ausgabe der Daten aus TBL 
            Serial.write(Cursor_Zeile);                     // Ausgabe von dem gewünschten Trennzeichen z.B.: 10 = ENTER  9=TAB 
          // Ausgabe auf UART -----ENDE-----
        } else {
          
          // Ausgabe auf LCD  ----BEGINN-----
            if (Cursor_Zeile > 3) { Cursor_Zeile = 0; } 
            i_LCD(Cursor_Spalte, Cursor_Zeile, Buffer1, 0);
            Cursor_Zeile ++;
          // Ausgabe auf LCD  ----ENDE-------
        }
      }
      AdrI += StepNr;               // je nach dem ob BIS kleiner als VON war wird hier subtrahiert oder addiert da StepNr positiv oder negativ ist
  } while ((AdrI - StepNr) != AdrTo );

  return 1;                         // Rückgabe noch ohne Bedeutung
}

//--------------------------------------------------------------------------------------------
// Ausgabe auf LCD beginne bei Cursor_Spalte, in Cursor_Zeile, 
// die Zeichen aus xbuffer[] von [xbufferStartPos] bis maximal 20 Zeichen 
//--------------------------------------------------------------------------------------------
int8_t i_LCD(int8_t Cursor_Spalte, int8_t Cursor_Zeile, char xbuffer[], uint8_t xbufferStartPos) {
  char * xbufferPt = & xbuffer[xbufferStartPos];  // Zeiger auf die Stelle ab welcher der xbuffer[] ausgegeben werden soll
  xbuffer[xbufferStartPos+20] = 0x00;             // Zeichenkenn-ENDE setzen sodaß nie mehr als 20 Zeichen ausgegeben werden (ACHTUNG xbuffer[] wird hier überschrieben !!!
  LCD_BackLight(2);                               // Hintergrundbeleuchtung EINSCHALTEN
  lcd.setCursor(Cursor_Spalte,Cursor_Zeile);      // Cursor an die gewünschte Stelle positionieren
  lcd.print(xbufferPt);                           // Ausgabe der Zeichenkette laut Zeiger
  return 0;
}

//----------------------------------------------------------------------------------------------------
// Formatierte Ausgabe einer Zahl auf LCD   damit die EINER - Stelle immer an gleicher Position ist
// Liefert die Anzahl gesamt ausgegebener Zeichen 
//---------------------------------------------------------------------------------------------------- 
uint8_t LCD_Numb(int8_t Cursor_Spalte, int8_t Cursor_Zeile, int32_t Numb, int8_t Size ) {
  char    hbuffer[21];              // Hilfsbuffer der für die Formatierung einer LCD-Zeile die maximal mögliche Größe +1 hat.
  char    formatstr[6] ;            // so geht es nicht wegen der Size die mehr als ein Element als String einnimt = {'%',char(Size),'l','d',0x00}; 
  int8_t  formatpos=0;              // Hilfsposition für format[]
  uint8_t Ret=0;                    // Rückgabevariable
  char  * SizeStrPt;                // Zeiger auf die zum Text konvertierte Size
  
  if (Size > 11)  {Size = 11; }     // sicherstellen daß in weiterer Folge hier kein Überlauf erfolgt
  formatstr[formatpos] = '%';       // ist fix für Variablenplatz im üblichen Formatstring '%xxld' von sprintf()
  formatpos++;
  SizeStrPt = NrToStr(Size);        // liefert einen Zeiger auf die aus Size konvertierte Zeichenfolge Bsp: [0]='1'  [1]='0' 
  if (Size > 9) { formatstr[formatpos] = * SizeStrPt; formatpos++; SizeStrPt++;   } // wenn eine Anzahl Stellen > 9 erfolgen soll 
  formatstr[formatpos] = * SizeStrPt;  // fülle char mit Inhalt der an Adresse SizeStrPt zu finden ist.
  formatpos++;
  formatstr[formatpos]='l';         // long 
  formatpos++;         
  formatstr[formatpos]='d';         // Zahl mit ggf. Vorzeichen wenn - 
  formatpos++;
  formatstr[formatpos]=0x00;        // abschluss 
      // Serial.println(formatstr);
  Ret = sprintf(hbuffer, formatstr, Numb);    //  Serial.println(hbuffer);
      // Ret = sprintf(hbuffer, "%10ld", Numb);  Serial.println(hbuffer);
  i_LCD(Cursor_Spalte, Cursor_Zeile, hbuffer, 0);   // Ausgabe auf LCD
  return Ret;
}




//------------------------------------------------------------------------
// Ausgabe vom UART-buffer auf die Serielle
//------------------------------------------------------------------------
void Print_UART_buffer2UART(uint8_t AsChar, uint8_t WithCr) {
  (void) AsChar;  // damit der Compiler hier Ruhe gibt wegen der unbenützen Variable
  Serial.print(UART_buffer); // gibt den Inhalt als Char aus
  for (uint8_t i = 0; i < WithCr; i++) {Serial.println(); }  // Anzahl der gewünschten Zeilenumbrüche anfügen
}

//------------------------------------------------------------------------------------
// Ausgabe vom UART-buffer auf LCD
//------------------------------------------------------------------------------------
void Print_UART_buffer2LCD(uint8_t AsChar, uint8_t LCD_Spalte, uint8_t LCD_Zeile) {
  if (LCD_Spalte > 19) LCD_Spalte = 0;
  if (LCD_Zeile  >  3) LCD_Zeile  = 0;
  lcd.setCursor(LCD_Spalte,LCD_Zeile);
  for (uint8_t i=0; i < UART_bufferlen; i++) {
    if (AsChar == 0) {
      lcd.print(UART_buffer[i]); }
    else {
      lcd.print(char(UART_buffer[i]));
    }
  }
}




//-----------------------------------------------
// ab hier NUR TEST und Muster 
//-----------------------------------------------
float FVar;
uint8_t UVar;
int16_t I16Var;
const char FormatStr[] = "Formatierungsstring %4d, %5d, %06d";
uint16_t Idx;

void TestFormat() {
      sprintf(Buffer1, "test %4d, %05d, %6d", 1,2,4);
      Serial.println(Buffer1);
      sprintf(Buffer1, "test %+04d, %+5d, %+6d", 10,20,400);
      Serial.println(Buffer1);
      FVar= -343.14252;
      Serial.print(F("Float2Char 12,3:"));
      Serial.println(Float2Char(FVar,12,3));
      FVar= -343.14253;
      Serial.print(F("Float2Char 14,4:"));
      Serial.println(Float2Char(FVar,14,4));
      sprintf(Buffer1, "text: %s", Float2Char(12.2345, 10,3)); Serial.println(Buffer1);
      sprintf(Buffer1, "text: %s", Float2Char(1243, 10,3)); Serial.println(Buffer1);
      sprintf(Buffer1, "text: %s", Float2Char(2345, 10,0)); Serial.println(Buffer1);
      UVar=33;
      sprintf(Buffer1, "text: %s", Float2Char(UVar, 10,0)); Serial.println(Buffer1);
      I16Var=-5453;
      sprintf(Buffer1, "text: %s", Float2Char(I16Var, 50,0)); Serial.println(Buffer1);   
      Serial.println(Float2Char(123.345,10,2));   
}
int8_t LCD_Out_TEST(char c1 ) {
  int n1 = 32000;
  int16_t n2 = 20000;
  long int n3 = 84500;
  int32_t n4 = 120400;
  lcd.clear();
  LCD_BackLight(2);
  lcd.setCursor(0,0);
  //Serial.println(c1);
  sprintf(UART_buffer,  "%2c %d, %d", c1, n1, n2);
  lcd.print(UART_buffer);
  lcd.setCursor(0,1);
  sprintf(UART_buffer,  "%ld, %ld FF", n3, n4);
  lcd.print(UART_buffer);
  return 0;
}
