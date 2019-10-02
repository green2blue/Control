//------------------------------------------------------------------
// Konvertierungsfunktionen und andere Hilfsfunktionen
//------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------
// 2019-01-02  konvertiert ein FLOAT oder eine Nummer zu einer Zeichenkette mit definierter Länge
//  und liefert einen Zeiger auf die Zeichenkette die damit auch gleich ausgegeben werden kann.
//    width = gesamte Anzahl Zeichen Vorkomma + Punkt + Nachkomma  
//    prec  = Anzahl Nachkommastellen
// Verwendungsbsp: sprintf(Buffer1, "text: %s", Float2Char(12.2345, 10,3)); Serial.println(Buffer1); -->
//                 liefert: text:     12.234
//-------------------------------------------------------------------------------------------------------
char * Float2Char(float FVarI, uint8_t width, uint8_t prec) {
  char FloatChar[20];
  if (width > 20) width = 20;   // sonst erfolgt u.U. ein Programmabsturz
  return dtostrf(FVarI, width, prec, FloatChar);
}

//------------------------------------------------------------------------------
//  Prüfen ob das/die Zeichen KeyStr  in der Zeichenkette Str vorkommt/en.
//  Geliefert wird eine Zahl welche die Position in Str anzeigt oder 0.
//  Muster für Verwendung: StrPos = InStr( & UART_Buffer1[0], & CmdDeli[0] );
//------------------------------------------------------------------------------
uint8_t InStr(char * Str,  char * KeyStr) {
  char * pch   = NULL;
  uint8_t ret  = 0;
     // Serial.print(F("Instr Str:"));Serial.print(Str); Serial.print(F(" KeyStr:"));Serial.println(KeyStr);
  pch = strstr(Str,KeyStr);     // Zeiger wo KeyStr in Str liegt oder NULL
     // Serial.print(F("Instr pch:")); Serial.println(pch); // gibt ab dem gefundenen Punkt den Inhalt der Zeichenkette aus
  if (pch != NULL) {
     ret = pch - Str;  // Zeigerpos der gefundenen Stelle - Beginn_Position ergibt die Stellen-Zahl
  }
  return  ret;
}

//------------------------------------------------------------------------------
//  Prüfen ob das/die Zeichen KeyStr  in der Zeichenkette Str vorkommt/en.
//  Geliefert wird ein Zeiger wo das erste Vorkommen liegt oder NULL.
//------------------------------------------------------------------------------
char * InStrPt(char * Str,  char * KeyStr) {
  char * pch = NULL;
     // Serial.print(F("Instr Str:"));Serial.print(Str); Serial.print(F(" KeyStr:"));Serial.println(KeyStr);
  pch = strstr(Str, KeyStr);     // Zeiger auf das Vorkommen von KeyStr in Str  oder NULL
     // Serial.print(F("InstrPt pch:")); Serial.println(pch); // gibt ab dem gefundenen Punkt den Inhalt der Zeichenkette aus
  return  pch;
}

//-------------------------------------------------------------------------
//  Liefert einen Zeiger auf die Position FromPos in einer Zeichenkette
//  nur als Code-Muster
//-------------------------------------------------------------------------
char *FktToTxt(uint8_t FromPos) {
  return &UART_buffer[FromPos];
}
