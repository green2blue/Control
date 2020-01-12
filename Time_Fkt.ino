#define RTC_I2C_ADDRESS 0x68        // I2C Adresse der RTC     auf dem DS3231 Modul
#define RTC_EEPROM_I2C_ADDRESS 0x57 // I2C Adresse des EEPROMS auf dem DS3231 Modul Ev. zum Abspeichern von Timern geeigent ?
//---------------------------------------------------------------------------------------------------
// 2018-04-07  auf dem RTC-Modul befindet sich auch ein 4 K Byte EEPROM mit der 
//        AT24C32  32768 Bit  4096 Byte  12 Bit Adresslänge  32 Byte Seitengröße  128 Seiten
// Code für den Zugriff auf dieses EEPROM muß hier noch folgen
// Was man am besten darauf ablegt ist offen, sollte nicht allzu wichtig sein da das Modul einfach gewechselt werden könnte.
// Vielleicht mal Error-Meldungen.
//---------------------------------------------------------------------------------------------------

#include "Func_Time.h"  // Struktur und manche Funktionen
#include "Global.h"

//char    Buffer1[_BUFFERLEN1];
//int16_t Buffer1Len;

//------------------------------------------------------------------------
// Zeit von RealtimeClock ABHOLEN und in Struktur timestamp SPEICHERN
// im RTC-Modul ist immer die Zonenzeit am Laufen. 
// eine sonstige Zeitverschiebung wie Sommer-Winterzeit wird hier ebenfalls berücksichtigt.
//-----------------------------------------------------------------------
void RTC2timestamp() {
  Wire.beginTransmission(RTC_I2C_ADDRESS); //Aufbau der Verbindung 
  Wire.write(0);
  Wire.endTransmission();
  Wire.requestFrom(RTC_I2C_ADDRESS, 7);               // Adresse des Moduls
  timestamp.SS    = bcdToDec(Wire.read() & 0x7f);     // SEKUNDE
  timestamp.MM    = bcdToDec(Wire.read());            // MINUTE
  timestamp.HH    = bcdToDec(Wire.read() & 0x3f);     // STUNDE
  timestamp.DD    = bcdToDec(Wire.read());            // TAG
  timestamp.MO    = bcdToDec(Wire.read());            // MONAT
  timestamp.JJJJ  = bcdToDec(Wire.read())+2000;       // JAHR 2-stellig + 2000 
  timestamp.HH    = timestamp.HH + SuWi_Time_EU(1);   // SOMMER / WINTER ZEITUMSTELLUNGSFORMEL !
}

//------------------------------------------------------------------------------------------------------------
// 2019-01-22  Liefert einen Zeiger auf eine Zeichenkette die laut der  
//    Zeichenkette FormatStr formatiert wurde und mit den Zeitdaten an entspr. Stelle befüllt ist
//  Bsp: TimeInfo("Jahr:%0000d Monat:%00d Tag:%00d") --> liefert * "Jahr:2019 Monat:01 Tag:21"
//------------------------------------------------------------------------------------------------------------
char * TimeInfo(char * FormatStr) {
  char buf[51];   //  Buffer für sprintf in dem die formatierte Zeichenkette abgelegt wird
  char * bufpt = & buf[0];  // Zeiger auf die Zeichenkette buf[12]
  sprintf(buf, FormatStr, timestamp.JJJJ, timestamp.MO, timestamp.DD, timestamp.HH, timestamp.MM, timestamp.SS); 
  //sprintf(buf, "Jahr:%04d Monat:%02d Tag:%02d", timestamp.JJJJ, timestamp.MO, timestamp.DD);  // formatiert buf mit Nr und man hat damit eine Zeichenkette aus einer Zahl gemacht.
  return bufpt;  
}
//-----------------------------------------------------------------------------------------
// 2019-01-22 Liefert einen speziellen Teil der aktuellen Zeit als  Unsigned INTEGER 4 Byte
//  je nach Parameter PartNr wird ein anderer Teil als Zahl geliefert.
// 1 = Jahreszahl  
//-----------------------------------------------------------------------------------------
uint32_t TimePartNr(int8_t PartNr) {
  uint32_t Ret;
  switch ( PartNr) {
    case 0:   Ret = 0; break;
    case 1:   Ret =timestamp.JJJJ; break;
    case 2:   Ret =timestamp.MO;   break;
    case 3:   Ret =timestamp.DD;   break;
    case 4:   Ret =timestamp.HH;   break;
    case 5:   Ret =timestamp.MM;   break;
    case 6:   Ret =timestamp.SS;   break;
    case 7:   Ret =millis();       break;
    case 8:   Ret =micros();       break;
    case 9:   Ret =DayofWeek(timestamp.JJJJ,timestamp.MO,timestamp.DD);   break;
    case 10:  Ret =KalWeekNr(timestamp.JJJJ, timestamp.MO, timestamp.DD); break;
    default: Ret=0; break;
  }
  return Ret;
}
char * TimePartTx(int8_t PartNr) {
  char empty[2] = {'.',0x00};
  char * empty_pt = empty;
  switch (PartNr) {
    case 12: return DayName(DayofWeek(timestamp.JJJJ,timestamp.MO,timestamp.DD), CurrLanguage()) ;  break;  // TAGESNAME
    case 13: return MonthName(timestamp.MO, CurrLanguage());                                        break;  // MONATSNAME
    default: return empty_pt; break;
  }
  return empty_pt;
}

//--------------------------------------------------------------------
// Ausgabe Zeit auf UART
//--------------------------------------------------------------------
void Printtimestamp() {
  Serial.print(F(" timestamp:"));
  Timestamp2OUT(0,0,1);
  Serial.print(F(" Wo-Tag:"));
  Serial.print(DayofWeek(timestamp.JJJJ, timestamp.MO, timestamp.DD));
  Serial.print(F(" KW:"));
  Serial.print(KalWeekNr(timestamp.JJJJ,timestamp.MO,timestamp.DD));
  Serial.print(F(" "));
  DayName(DayofWeek(timestamp.JJJJ, timestamp.MO, timestamp.DD),0); // 0=engl. 1=Deutsch
  Serial.print(F(" "));
  MonthName(timestamp.MO, 0); // 0=engl. 1=Deutsch
  Serial.print(F(" "));
  Serial.print(getTempFromRTC() ); Serial.print(F(" °C"));
  Serial.println();
}

//--------------------------------------------------------------------
// Ausgabe Zeit auf LCD in Format  JJJJ.MO.DD HH:MM:SS
// OUT 0=zu LCD  1=zu UART    2=zu UART +crlf 
//--------------------------------------------------------------------
void Timestamp2OUT(int8_t SPALTE,int8_t ZEILE,int8_t OUT) {
  String result = "";
   // Datum 
  result+=timestamp.JJJJ; result+="."; 
  if (timestamp.MO<10) {result+="0";}; result+=timestamp.MO; result+="."; 
  if (timestamp.DD<10) {result+="0";}; result+=timestamp.DD; result+=" ";
   // Uhrzeit
  if (timestamp.HH<10) {result+="0";}; result+=timestamp.HH; result+=":"; 
  if (timestamp.MM<10) {result+="0";}; result+=timestamp.MM; result+=":";
  if (timestamp.SS<10) {result+="0";}; result+=timestamp.SS;
  if (OUT==0) { lcd.setCursor(SPALTE,ZEILE); lcd.print(result); return; }
  if (OUT==1) {Serial.print(result); return; }
  if (OUT==2) {Serial.println(result); return; }
}


//------------------------------------------------------
// SCHREIBEN der Zeit IN das RTC-Modul aus timestamp
// 2018-04-08 ich glaube auch hier sollte ich noch einbauen daß im Falle der Sommerzeit das RTC-Modul
//  immer mit der UTC  befüllt wird. Beim auslesen soll das dann ja auch berücksichtigt werden.
//------------------------------------------------------
void RTCWriteTime(){
  Wire.beginTransmission(RTC_I2C_ADDRESS);
  Wire.write(0);    // Der Wert 0 aktiviert das RTC Modul.
  Wire.write(decToBcd(timestamp.SS));    
  Wire.write(decToBcd(timestamp.MM));
  Wire.write(decToBcd(timestamp.HH-SuWi_Time_EU(1) ));     // gespeichert wird immer die Zonenzeit 
  Wire.write(decToBcd(0)); // Wochentag unberücksichtigt
  Wire.write(decToBcd(timestamp.DD));
  Wire.write(decToBcd(timestamp.MO));
  if(timestamp.JJJJ < 2000) {timestamp.JJJJ += 2000; } // falls mal versehentlich beim Befüllen das Addieren von 2000 vorher vergessen wurde.
  Wire.write(decToBcd(timestamp.JJJJ-2000));  
  Wire.endTransmission();  
}

//-----------------------------------------------------------------------------------
// ermittelt die Tag-Nummer 0...6 Tag der Woche aus dem übergebenen Datumswerten.
// 0=Sonntag 1=Montag.... (Diese wilde Formel kann in wikipedia ggf.nachvollzogen werden)
//-----------------------------------------------------------------------------------
uint8_t DayofWeek(uint16_t jahr, int8_t monat, int8_t tag) {
  uint8_t t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
  jahr -= monat < 3;
  return ((jahr+jahr/4-jahr/100+jahr/400+ t[monat-1]+tag) % 7); 
}
// ungetesteter Zeller-Algor zum Berechnen des Wochentages, kommt aber ohne Tabelle aus, daher vermutlich mit weniger RAM 
// Samstag = 0, Sonntag = 1 ...Freitag = 6
uint8_t zeller(int16_t iYear, int8_t iMonth, int8_t iDay ){
  int16_t iDivYear;
  int16_t iModYear;
  int8_t  iResult;
  if(iMonth< 3 ){iMonth += 10; iYear--;} else {iMonth -= 2;}
  iModYear = iYear % 100;
  iDivYear = iYear / 100;
  iResult = (26*iMonth-2)/10 +iDay+iModYear+(iModYear/4)+(iDivYear/4)-2*iDivYear;
  return(iResult % 7);
}
// ungetesteter Zeller-Algor zum Berechnen des Wochentages, kommt ebenfalls ohne Tabelle aus.
int untested_dayofweek(uint16_t iYear, int8_t iMonth, int8_t iDay) {
   /** Variation of Sillke for the Gregorian calendar. **/
   /** With 0=Sunday, 1=Monday, ... 6=Saturday         **/
   if ((iMonth -= 2) <= 0) {iMonth += 12; iYear--;}
   return (83*iMonth/32 + iDay + iYear + iYear/4 - iYear/100 + iYear/400) % 7;
}
// ungeteste Kalenderwochen Funkiton (aus Informix nachgebaut)




// KONVERTIERT Dezimalzeichen in binäre Zeichen.
byte decToBcd(byte val){ return ( (val/10*16) + (val%10) );}
// KONVERTIERT binäre Zeichen in Dezimal Zeichen.
byte bcdToDec(byte val){ return ( (val/16*10) + (val%16) );}

//-----------------------------------------------------
// String 2 INTEGER die wievielte Zahl: num 
//-----------------------------------------------------
int16_t getIntFromString (char *stringWithInt, byte num){
  char *tail; 
  while (num>0) { num--;
    while ((!isdigit (*stringWithInt))&&(*stringWithInt!=0)){ stringWithInt++; } // Suche einer Stelle die nummerisch ist
    tail=stringWithInt;
    while ((isdigit(*tail))&&(*tail!=0)){ tail++; }  
    if (num>0){stringWithInt=tail;}
  }  
  return(strtol(stringWithInt, &tail, 10));
}

//----------------------------------------------------------------------
// Ausgabe SoZ(Sommerzeit)  / WiZ(Winterzeit)  
// OUT 0=zu LCD  1=zu UART
//----------------------------------------------------------------------
void TimestampSoWiOUT(int8_t SPALTE,int8_t ZEILE,int8_t OUT) {
  lcd.setCursor(SPALTE,ZEILE);
  if (OUT==0) {if (SuWi_Time_EU(1) > 0) {lcd.print(F("S"));} else {lcd.print(F("W"));} return; }
  if (OUT==1) {if (SuWi_Time_EU(1) > 0) {Serial.println(F("S"));} else {Serial.println(F("W"));} return;}
}

//-------------------------------------------------------------------------------
// Formelwerk um die zur Zeit in der EU gültige Sommer/Winterzeit zu ermitteln
// Sollte das abgeschafft werden ist hier einfach FIX 0 bzw. 1 einzustellen 
// bzw. könnte man das auch als Variable im EEPROM ablegen ob die Zeitumstellung gilt oder nicht gilt.
//-------------------------------------------------------------------------------
int8_t SuWi_Time_EU(int8_t tzHours) {
  if (timestamp.MO<3 || timestamp.MO>10) return 0;   // keine Sommerzeit in Jan, Feb, Nov, Dez
  if (timestamp.MO>3 && timestamp.MO<10) return 1;   // Sommerzeit in Apr, Mai, Jun, Jul, Aug, Sep
  // if (timestamp.MO==3 && (timestamp.HH+24     * timestamp.DD) >= (1 + tzHours + 24*(31-  (5 * timestamp.JJJJ /4 + 4) % 7))   ||   timestamp.MO==10 &&   (timestamp.HH + 24 * timestamp.DD)<(1 + tzHours + 24*(31 - (5 * timestamp.JJJJ /4 + 1) % 7))  )
  if  (((timestamp.MO==3) && ((timestamp.HH + 24 * timestamp.DD) >= (1 + tzHours + 24*(31 - (5 * timestamp.JJJJ /4 + 4) % 7)))) || ((timestamp.MO==10) && ((timestamp.HH + 24 * timestamp.DD)<(1 + tzHours + 24*(31 - (5 * timestamp.JJJJ /4 + 1) % 7))))) 
   return 1;
  else
   return 0;
}

//--------------------------------------------------------------------------------------
// 2018-04-08  solange es diese sinnlose Zeitumstellung gibt brauchen wird dies
// Sommerzeit Winterzeit 
// European Daylight Savings Time calculation by "jurs" for German Arduino Forum
// input parameters: "normal time" for year, month, day, hour and tzHours (0=UTC, 1=MEZ)
// return value: returns 1 during Daylight Saving Time, 0 otherwise
//--------------------------------------------------------------------------------------
int8_t summertime_EU(int16_t year, int8_t month, int8_t day, int8_t hour, int8_t tzHours) {
 if (month<3 || month>10) return 0;   // keine Sommerzeit in Jan, Feb, Nov, Dez
 if (month>3 && month<10) return 1;   // Sommerzeit in Apr, Mai, Jun, Jul, Aug, Sep
// if (month==3 && (hour+24*day)>=(1+tzHours+24*(31-(5*year/4+4) % 7)) || month==10 && (hour+24*day)<(1+tzHours+24*(31-(5*year/4+1) % 7)))
// if  (((timestamp.MO==3) && ((timestamp.HH + 24 * timestamp.DD) >= (1 + tzHours + 24*(31 - (5 * timestamp.JJJJ /4 + 4) % 7)))) || ((timestamp.MO==10) && ((timestamp.HH + 24 * timestamp.DD)<(1 + tzHours + 24*(31 - (5 * timestamp.JJJJ /4 + 1) % 7))))) 
 if (((month==3)&&((hour+24*day)>=(1+tzHours+24*(31-(5*year/4+4)%7)))) || ((month==10)&&((hour+24*day)<(1+tzHours+24*(31-(5*year/4+1)%7)))))
   return 1;
 else
   return 0;
}

void TimestampKWOUT(int8_t SPALTE,int8_t ZEILE,int8_t OUT) {
  if (OUT==0) {LCD_Numb(SPALTE,ZEILE, KalWeekNr(timestamp.JJJJ, timestamp.MO, timestamp.DD),2);  return;}
  if (OUT==1) {Serial.print(F("KW-NR:"));Serial.println(KalWeekNr(timestamp.JJJJ,timestamp.MO,timestamp.DD)); return;}
}

////------------------------------------------------------------------------------------------
//// liefert 1 wenn das Jahr ein Schaltjahr ist, wenn der Februal also 29 anstatt 28 Tage hat
////         0 wenn kein Schaltjahr vorliegt
//// bin kein Fan von true od. false da sowieso ein ganzes Byte verbraucht wird, kann man auch gleiche eine Zahl liefern.
////------------------------------------------------------------------------------------------
//int8_t isYearSwitchYear(uint16_t iJJJJ) {
//  if(iJJJJ % 400==0 || (iJJJJ % 100 !=0 && iJJJJ % 4==0)) {return 1;}
//  return 0;
//}                 

////--------------------------------------------------------------------------
//// liefert die Anzahl Tage die ein Monat hat. Bsp: 1=31, 2=28/29, 3=31 usw.
//// bei den Monatszahlen 1,3,5,7,8,10,12  ist BIT0 und BIT3 immer <>  --> 31 Tage
//// bei den Moantszahlen 4,6,9,11         ist BIT0 und BIT3 immer ==  --> 30 Tage
//// Monatszahl 2 hat 28 + isYearSwitchYear Tage 
//// Monatstagezahl=  iMM(Bit0) XOR iMM(BIT3) + 30
////--------------------------------------------------------------------------
//int8_t MonthhasDays(int16_t iJJJJ, uint8_t iMM) {
//  if (iMM==2) {return 28+isYearSwitchYear(iJJJJ); }
//  if (bitRead(iMM,0)==bitRead(iMM,3)) {return 30;} else {return 31;}
//}

// noch effizientere Methode die maximale Anzahl an Tagen die ein Monat hat zu ermitteln.
int8_t MonthDayMax(int16_t iJJJJ, uint8_t iMM) {
  if (iMM==2) {return 28+ isYearSwitchYear(iJJJJ); }
  return 30 + ( ( (iMM>>3) ^ iMM) &1);
}

//--------------------------------------------------------------------------------
// ermittelt zum gegebenen Datum um den wievielten Tag des Jahres es sich handelt.
//--------------------------------------------------------------------------------
int16_t DayOfYear(int16_t iJJJJ, int8_t iMM, int8_t iDD) {
  int16_t LDay = iDD;
  int8_t  LMon = iMM;
  while (LMon > 1) {
    LMon --;
    LDay += MonthDayMax(iJJJJ, LMon); 
  }
  return LDay;
}

//------------------------------------------------------------------------
// wieviele Tage hat das Jahr  ?
//------------------------------------------------------------------------
int16_t YearDays(int16_t iJJJJ) { return 365 + isYearSwitchYear(iJJJJ); }

//------------------------------------------------------------------------------------
// wieviele Tage liegen 2 Datums auseinander ?
//------------------------------------------------------------------------------------
int16_t DayDiff(int16_t fJJJJ, int8_t fMM, int8_t fDD, int16_t tJJJJ, int8_t tMM, int8_t tDD) {
   int16_t LYear=fJJJJ;
   int16_t LDay;
   int16_t RDay;
   int8_t UpDown=1;
   if (fJJJJ < tJJJJ) {UpDown=-1; }
   while (LYear != tJJJJ) {  // solange noch eine ganze Jahresdifferenz vorliegt 
     LDay += YearDays(LYear);
     LYear += UpDown;  }
   RDay = DayOfYear(tJJJJ, tMM, tDD) - DayOfYear(fJJJJ, fMM, fDD);
   return LDay * UpDown + RDay;  // Anzahl Tage über die unterschiedlichen Jahre + Tagesdifferenz innerhalb des gleichen Jahres
}

//------------------------------------------------------------------------------------
// Kalender-Wochen-Nummer zu einem Datum ermitteln (passt für DACH)
//------------------------------------------------------------------------------------
int8_t KalWeekNr(int16_t iJJJJ, int8_t iMO, int8_t iDD) {
  int16_t nDayOYear  = DayOfYear(iJJJJ, iMO, iDD);
  int8_t  nWoDay1Jan = DayofWeek(iJJJJ, 1, 1);    //  Wochentag 1.Januar
  int8_t  nKW;
  int8_t  isSwY;
  if (nWoDay1Jan >= 5) {nWoDay1Jan -= 7; }  // Sonderfall Freitag und Samstag 
  if ((nDayOYear + nWoDay1Jan) <=1) {return KalWeekNr(iJJJJ-1,12,31);} // Jahresanfang mit KW-Nr aus Vorjahr 
  nKW = ((nDayOYear + nWoDay1Jan + 5) / 7); 
    // 53 Kw hat nur ein Jahr das mit Do beginnt !    In Schaltjahren ist das auch an einem Mi möglich Bsp: 1992 sonst ist diese KW schon die KW1 des Folgejahres
  if (nKW==53) { isSwY = isYearSwitchYear(iJJJJ);   
    if ((nWoDay1Jan==4) ||(nWoDay1Jan== -3) || ((nWoDay1Jan==3) && isSwY==1) || ((nWoDay1Jan== -4) && isSwY==1)) { 
              // in diesem Fall ist die KW 53 tatsächlich ok 
    } else {nKW = 1; }
  }
  return nKW;
}

//--------------------------------------------------------------------------------
// Addiert zum gegebenen Datum eine beliebige Anzahl Tage.
//    diese Fkt kann also zum Konvertieren eines DayOfYear verwendet werden UND
//                      zum Addieren/Subtrahieren einer Tageszahl
//--------------------------------------------------------------------------------
void AddDays(int16_t &iJJJJ, int8_t &iMM, int8_t &iDD, int16_t DayNrOfYear) {
  int16_t LDay=DayNrOfYear;
  int8_t  LMon=iMM;
  if (LMon == 0) {LMon=1; iMM=1; }
  if (LDay == 0) {LDay=1; iDD=1; }
  
  if (LDay < 0)  {                  // Tage SUBTRAHIEREN 
     LDay += DayOfYear(iJJJJ, iMM, iDD) -1; 
     while (LDay < 0) {   // Falls die Anzahl Tage die zu subtrahieren sind über mehrere Jahre gehen.
        LDay += YearDays(iJJJJ -1);
        iJJJJ --;
     }  // nun liegt die richtige Jahreszahl und die Anzahl Tage vor, den Rest macht die "normale" Addierfunktion in Folge
     iMM = 1; iDD = 1;
  }
  
  while (LDay > YearDays(iJJJJ) ) {  // normale Tage ADDIERFUNKTION 
     LDay -= YearDays(iJJJJ);
     iJJJJ ++;   }
  while ((LDay + iDD - 1) > MonthDayMax(iJJJJ, LMon)) {
     LDay -= MonthDayMax(iJJJJ, LMon);
     iMM ++;
     LMon = iMM;  }
  iDD += LDay;  // die noch verbliebenen Tage hinzufügen
  if (iMM < 1) { iMM = 1;}
  if (iDD < 1) { iDD = 1;}
}

//---------------------------------------------------------------
// Tagesname aus I_Tbl über die Tag-Nummer  
//---------------------------------------------------------------
char * DayName(int8_t DayNr, uint8_t Language) {
  F2X(T_Idx(_S_DAY,DayNr,0,Language),Buffer2);   //   (_WEEKDAYLONG + DayNr + Language * 7, 0);
  return Buffer2;
}

char * MonthName(int8_t MonthNr, uint8_t Language) {
  F2X(T_Idx(_S_MON,MonthNr,0,Language),Buffer2);   //  F2buffer(_MONTHNAMELONG + MonthNr + Language * 13, 0);
  return Buffer2;
}

////-----------------------------------------------------------
//// Prüft das Datum in der Struktur timestamp auf Gültigkeit.
////-----------------------------------------------------------
//int8_t checkTimestamp(){
//  int8_t result = 0;
//  if(timestamp.JJJJ>2000){result=1;} else {return result;}
//  if(timestamp.MO<13) {if(timestamp.DD<=MonthhasDays(timestamp.JJJJ,timestamp.MO)) {result=1;} else {return 0;} }
//  if(timestamp.HH<24 && timestamp.MM<60 && timestamp.SS<60 && timestamp.HH>=0 && timestamp.MM>=0 && timestamp.SS>=0) {result=1;} else {return 0;}
//  return result; 
//}

//---------------------------------------------------------------
// Temperatur vom temperaturkompensierten RTC-Modul 3231 abholen
//---------------------------------------------------------------
int8_t getTempFromRTC() {
  Wire.beginTransmission(RTC_I2C_ADDRESS);
  uint16_t writtenBytes = Wire.write(0x11); 
  Wire.endTransmission();
  Wire.requestFrom(RTC_I2C_ADDRESS, writtenBytes); // 2 Byte Daten vom DS3231 holen
  int8_t msb = Wire.read();
  int8_t lsb = Wire.read();
  return ((msb<< 2)+(lsb>> 6))/4; // Kommazahlen bringen bei diesem relativ ungenauen Sensor nichts
}

//----------------------------------------------------------------------------
// 2018-04-07  Auf dem SQW-Pin des RTC-Moduls ein exaktes 1Hz Signal ausgeben 
//----------------------------------------------------------------------------
void RTC_SQL_1Hz() {
  Wire.beginTransmission(RTC_I2C_ADDRESS); //DS3231 RTC address = 104 dec
  Wire.write(0x0e);             //control register
  Wire.write(0);                //zeroing all bits outputs 1 Hz on SQW
  Wire.endTransmission(); 
}




//--------------------------------------------------------------------- 
// setzen der Uhrzeit über UART 
//---------------------------------------------------------------------
void RTCSetTime(){
  char linebuf[30];
  uint8_t counter;
  if (Serial.available()){
    delay(100); // Warte auf das Eintreffen aller Zeichen vom seriellen Monitor
    memset(linebuf,0,sizeof(linebuf)); // Zeilenpuffer löschen
    counter=0; // Zähler auf Null
    while (Serial.available()){
      linebuf[counter]=Serial.read(); // Zeichen in den Zeilenpuffer einfügen
      if (counter<sizeof(linebuf)-1) counter++; // Zeichenzähler erhöhen
    }
    if (strstr(linebuf,"set")==linebuf){ // Wenn in der Zeile 'set' vorkommt
      timestamp.DD   =getIntFromString (linebuf,1);
      timestamp.MO   =getIntFromString (linebuf,2);
      timestamp.JJJJ =getIntFromString (linebuf,3);
      timestamp.HH   =getIntFromString (linebuf,4);
      timestamp.MM   =getIntFromString (linebuf,5);
      timestamp.SS   =getIntFromString (linebuf,6);
    } else {Serial.println(F("Befehl unbekannt.")); return; }
    
    // Werte einer groben Plausibilitätsprüfung unterziehen
    if (checkTimestamp() == 0){
      Serial.println(linebuf);
      Serial.println(F("Fehlerhafte Zeitangabe im 'set' Befehl"));
      Serial.println(F("Beispiel: set 28.08.2013 10:54"));
      return;
    }
    RTCWriteTime(); // timestamp Struktur in die RTC schreiben
    Serial.println(F("Zeit und Datum wurden auf neue Werte gesetzt."));
  }
}

//-------------------------------------------------------------------
// Ausgabe der aktuellen RTC-Zeit + Details auf UART
//-------------------------------------------------------------------
void Now2UART() {
  RTC2timestamp();  // Zeit abholen
  Printtimestamp(); // Zeit ausgeben
}
