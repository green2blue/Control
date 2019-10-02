// --------------------------------------------------------------------------------------------------------------------
// Bitte hier alle ben�tigten Sonderzeichen defninieren sodass man bei Bedarf mit:  
// lcd.print( "480" LCD_SZ_OHM );  oder einfach "Text"LCD_SZ_ENDLESS   
// einfach darauf zur�ckgreifen kann. Die Nummern sind aus dem Datenblatt zur LCD-Anzeige  
// --------------------------------------------------------------------------------------------------------------------
  #define LCD_SZ_OHM      "\xF4"    // OHM    Zeichen
  #define LCD_SZ_AT       "\x40"    // @
  #define LCD_SZ_ALPHA    "\xE3"    // ALPHA  Winkelzeichen
  #define LCD_SZ_ENDLESS  "\xF3"    // UNENDLICH Zeichen
  #define LCD_SZ_MUE      "\xE4"    // � 
  #define LCD_SZ_SUMM     "\xF6"    // SUMME  Zeichen
  #define LCD_SZ_PI       "\xF7"    // PI     Zeichen
  #define LCD_SZ_WURZEL   "\xE8"    // Wurzel 
  #define LCD_SZ_GRADCELS "\xDF"    // �  Grad Celsius Zeichen ?
  #define LCD_SZ_ZEIGRE   "\x7E"    // -> in einem Zeichen
  #define LCD_SZ_ZEIGLI   "\x7F"    // <- in einem Zeichen
  #define LCD_SZ_ae       "\xE1"    // �  kleines Umlaut a
  #define LCD_SZ_oe       "\xEF"    // �  kleines Umlatu o
  #define LCD_SZ_ue       "\xF5"    // �  kleines Umlaut �
  #define LCD_SZ_SS       "\xE2"    // �  scharfes s
 // #define LCD_SZ_AE       "\x     // �  ich glaube das m�sste dann �ber kurzfristig selbstdefinierte Zeichen erfolgen oder man kann das Display mal kurz in einen anderen Zeichensatz umschalten, Zeichen ausgeben und wieder zur�ckschalten.
 // #define LCD_SZ_OE       "\x     // �
 // #define LCD_SZ_UE       "\x     // �