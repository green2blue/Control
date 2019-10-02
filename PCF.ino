// **************************************************************************************************
// 2017-12-17  hier sind Funktionen HAUPTSÄCHLICH zum Porterweiterungsmodul PCF8574 zusammengefasst
// im Moment im Einsatz befindliche I2C-Adressen:
// Found address: 33 (0x21)  = Gashahn benötigt 2 Pins wegen Polaritätsumschaltung & Wasserventile 1..4  und Osmose-Ventil 
// Found address: 34 (0x22)  = Hilfs-Schütz an Pin 0,  alle anderen PIN sind FREI !!! 
// Found address: 56 (0x38)  = Schwenk-Motore  Kanäle pro Motor sind 2 PIN nötig
// Found address: 57 (0x39)  = Endschalter zu den Schwenkmotoren
// Found address: 63 (0x3F)  = LCD-Display
// mittels hier verfügbarem I2C_Scann() können die Adressen jederzeit überprüft werden !
// 
// **************************************************************************************************

//------------------------------------------------------------------------------------------------------
//  RAM Bereich für 16 Werte aller möglichen PCF-Module damit man rasch
//  einzelne BITs umschießen kann ohne jedesmal vorher das Modul einlesen
//  zu müssen. 
//  Der Zustand wird einmalig eingelesen und bei jeder Aktion gemeinsam mit den Modulen synchronisiert.
//  Hier liegt also immer ein aktueller Zustand der PCF-Module vor.
//  Die tatsächliche I2C-Adresse zum dazugehörigen Modul errechnet sich nach der
//  Formel: PCFIdx(int16_t PCF_Address) {if (PCF_Address > 55) {return PCF_Address-48;} else {return PCF_Address-32;} }
//------------------------------------------------------------------------------------------------------
uint8_t PCFVal[16];        // für jeden der möglichen Porterweiterungsmodule ein BYTE im RAM führen 

boolean datenkomm=true; // Merker ob die Kommunikation mit dem PCF-Modul funktioniert
 

// -------------------------------------------------------------------------------------------------------------
// 2017-12-21  nach I2C Adressen suchen und die Adresse auf der Seriellen ausgeben, für Fehlersuche entstanden. 
// -------------------------------------------------------------------------------------------------------------
void I2C_Scann() {
    for (uint8_t i=8;i<120;i++)  { Wire.beginTransmission(i);
      if (Wire.endTransmission() == 0) {
        I2C_Adr_To_Ser(i); } // Anzeige der Adresse auf der seriellen Schnittstelle
    } 
}
// 2018-01-06  Ausgabe einer I2C-Adresse auf der Seriellen
void I2C_Adr_To_Ser(uint8_t Adr) {
  Serial.print(F(" I2C-Adr Dec:"));Serial.print(Adr,DEC);Serial.print(F(" Hex:"));Serial.print(Adr,HEX);Serial.print(F(" Bin:"));Serial.println(Adr,BIN);
}

// -------------------------------------------------------------------------------------------------------------
//  zu einem PORTERWEITERUNGSMODUL ganzen WERT schreiben und diesen Zustand auch im RAM-Array merken           !
//  Bei als AUSGANG verwendeten Modulen kann der Zustand dieser Module daher direkt aus dem RAM gelesen werden !
// -------------------------------------------------------------------------------------------------------------
void PCFSetVal(int16_t PCF_Address,uint8_t Val) {
  PCFMemVal(PCF_Address,Val);     // Damit merkt man sich den aktuellen Wert des Moduls auch im RAM. 
                                  // Fragen über den Zustand können daher auch direkt an den RAM gestellt werden.
  PCF8574_Write(PCF_Address,Val); // Den Zustand nun auch tatsächlich an das PCF8574 Modul ausgeben
}

// ---------------------------------------------------------------------------------------------
//  PORTERWEITERUNGSMODUL  einzelnen PIN setzen  0=LOW  1=HIGH
//  üblicherweise wird Leistung gegen 0=LOW betrieben, der Verbraucher hängt also fix an HIGH
// ---------------------------------------------------------------------------------------------
void PCFSetPin(int16_t PCF_Address,uint8_t PinNr,uint8_t PinVal) { 
  bitWrite(PCFVal[PCFIdx(PCF_Address)],PinNr,PinVal);       // setzt das entsprechende BIT im RAM
  PCF8574_Write(PCF_Address, PCFVal[PCFIdx(PCF_Address)]);  // schreibt den manipulierten Wert aus dem RAM zum Porterweiterungsmodul
}

// -----------------------------------------------------------------
//  PORTERWEITERUNGSMODUL  einzelnen PIN umschalten  
// -----------------------------------------------------------------
void PCFTogglePin(int16_t PCF_Address,uint8_t PinNr) {
  if (bitRead(PCFVal[PCFIdx(PCF_Address)],PinNr)==0) 
   {PCFSetPin(PCF_Address,PinNr,1);} else   // Pin war 0 und wird jetzt 1 gesetzt
   {PCFSetPin(PCF_Address,PinNr,0);}        // Pin war 1 und wird jetzt 0 gesetzt
}

// --------------------------------------------------------------------------------------------------
//  PORTERWEITERUNGSMODUL  liefert den kompletten Zustand, als BYTE 
//    ggf. einlesen des kompletten Zustandes vom MODUL in den RAM und liefern des ganzen Wertes
// --------------------------------------------------------------------------------------------------
uint8_t PCFGetVal(int16_t PCF_Address,uint8_t ReadNew) {
  if (ReadNew == 1) {PCFMemVal(PCF_Address, PCF8574_Read(PCF_Address));} // RAM-Merker mit dem neu eingelesenen Wert aktualisieren
  return PCFVal[PCFIdx(PCF_Address)];
}

// -------------------------------------------------------------------------------------------------
//  PORTERWEITERUNGSMODUL  liefert den Zustand EINES   PINs  als 0 bzw. 1 
//   ggf. einlesen des kompletten Zustandes vom MODUL in den RAM und liefern des gefragten PINs 
//  beginnen bei BIT=0 ganz rechts bis BIT=7 ganz links
// -------------------------------------------------------------------------------------------------
uint8_t PCFGetPin(int16_t PCF_Address,uint8_t PinNr,uint8_t ReadNew) {
  if (ReadNew == 1) {PCFMemVal(PCF_Address, PCF8574_Read(PCF_Address));} // RAM-Merker mit dem neu eingelesenen Wert aktualisieren
  return bitRead(PCFVal[PCFIdx(PCF_Address)],PinNr);
}

// ------------------------------------------------------------------------------------------------------
//  zum direkten LESEN eines PCF8574-Bausteins unter Mitgabe der Adresse, geliefert wird das ganze uint8_t !
//  Diese Fkt sollte nicht direkt verwendet werden da diese nicht im RAM den aktuellen Zustand ablegt   !
//  vom Anwender zu verwenden ist: PCFGetVal() bzw. PCFGetPin()    so der alte Joda spricht             !
// ------------------------------------------------------------------------------------------------------
uint8_t PCF8574_Read(int16_t adresse) {
  uint8_t datenByte=0xff;
  Wire.requestFrom(adresse,1);
  if (Wire.available()) { datenByte=Wire.read(); datenkomm=true; } else { datenkomm=false; }
  return datenByte;
}

// -------------------------------------------------------------------------------------------------------
//  zum direkten SCHREIBEN auf einen PCF8574-Baustein mit der adresse  geschrieben wird das ganze BYTE   !
// (Achtung diese Fkt darf nicht direkt verwendet werden. IMMER    Fkt PCFSetVal() verwenden !           ! 
// -------------------------------------------------------------------------------------------------------
void PCF8574_Write(int16_t adresse, uint8_t daten) {
  Wire.beginTransmission(adresse);
  Wire.write(daten);
  Wire.endTransmission();
}

// --------------------------------------------------------------------------------------------------
//  PORTERWEITERUNGSMODUL- Wert ZUSTAND  im RAM-Array speichern
//  Damit kann bei einzelnen BIT_Änderungsvorgängen rasch der aktuelle Zustand der anderen 7 BITs
//  ermittelt werden. Sonst müsste man vor jeder BIT_Änderung das Modul einlesen.
//  Merken des Zustandes vom Porterweiterungsmodul im RAM
// --------------------------------------------------------------------------------------------------
void PCFMemVal(int16_t PCF_Address,uint8_t Val) { PCFVal[PCFIdx(PCF_Address)] = Val; }  
// ----------------------------------------------------------------------------------------------------------
//  PORTERWEITERUNGSMODUL  INDEX für RAM-Zugriff ergibt sich aus der angesprochenen Adresse des Moduls
//  für jede mögliche Adresse eines PCF8574 ergibt sich automatisch ein Platz im RAM-Array indem
//  man von der PCF-Adresse 48 od. 32 abzieht. Damit beginnt dann das RAM-Array bei Index 0 und geht bis maximal 15
//  Es ist grundsätzlich für JEDES mögliche Porterweiterungsmodul RAM reserviert.
// ----------------------------------------------------------------------------------------------------------
uint8_t PCFIdx(int16_t PCF_Address) {if (PCF_Address > 55) {return PCF_Address-48;} else {return PCF_Address-32;} }
// ------------------------------------------------------------------------------------------------------------
//  PORTERWEITERUNGSMODUL  liefern des gefragten PIN eines als Ausgang definierten PCF-Moduls NUR aus dem RAM
// ------------------------------------------------------------------------------------------------------------
uint8_t PCFGetRamPin(int16_t PCF_Address,uint8_t PinNr) { return bitRead(PCFVal[PCFIdx(PCF_Address)],PinNr); }
