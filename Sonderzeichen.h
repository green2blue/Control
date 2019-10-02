// --------------------------------------------------------------------------------------------------------------------
// Bitte hier alle benötigten Sonderzeichen defninieren sodass man bei Bedarf mit:  
// lcd.print( "480" LCD_SZ_OHM );  oder einfach "Text"LCD_SZ_ENDLESS   
// einfach darauf zurückgreifen kann. Die Nummern sind aus dem Datenblatt zur LCD-Anzeige  
// --------------------------------------------------------------------------------------------------------------------
  #define LCD_SZ_OHM      "\xF4"    // OHM    Zeichen
  #define LCD_SZ_AT       "\x40"    // @
  #define LCD_SZ_ALPHA    "\xE3"    // ALPHA  Winkelzeichen
  #define LCD_SZ_ENDLESS  "\xF3"    // UNENDLICH Zeichen
  #define LCD_SZ_MUE      "\xE4"    // µ 
  #define LCD_SZ_SUMM     "\xF6"    // SUMME  Zeichen
  #define LCD_SZ_PI       "\xF7"    // PI     Zeichen
  #define LCD_SZ_WURZEL   "\xE8"    // Wurzel 
  #define LCD_SZ_GRADCELS "\xDF"    // °  Grad Celsius Zeichen ?
  #define LCD_SZ_ZEIGRE   "\x7E"    // -> in einem Zeichen
  #define LCD_SZ_ZEIGLI   "\x7F"    // <- in einem Zeichen
  #define LCD_SZ_ae       "\xE1"    // ä  kleines Umlaut a
  #define LCD_SZ_oe       "\xEF"    // ö  kleines Umlatu o
  #define LCD_SZ_ue       "\xF5"    // ü  kleines Umlaut ü
  #define LCD_SZ_SS       "\xE2"    // ß  scharfes s
 // #define LCD_SZ_AE       "\x     // Ä  ich glaube das müsste dann über kurzfristig selbstdefinierte Zeichen erfolgen oder man kann das Display mal kurz in einen anderen Zeichensatz umschalten, Zeichen ausgeben und wieder zurückschalten.
 // #define LCD_SZ_OE       "\x     // Ö
 // #define LCD_SZ_UE       "\x     // Ü