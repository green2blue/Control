
#define _LCDML_DISP_cnt       30        // = Anzahl Menüelemente -1

// --------------------------------------------------------------------------------------------------------
  LCDML_add   (0 , LCDML_0      , 1, "Information"        , Mnu_Info);             
  LCDML_add   (1 , LCDML_0      , 2, "screensaver"        , Mnu_Info);         
//---------TEMPERATUR-SENSOREN----------------------------  ------------------------------------
  LCDML_add   (2 , LCDML_0      , 3, "Temperatures"       , NULL);     
  LCDML_add   (3 , LCDML_0_3    , 1, LCD_SZ_ZEIGLI" back" , F_back);
  LCDML_add   (4 , LCDML_0_3    , 2, "Temperatures show"  , F_back);    // 2018-03-21 passt 
  LCDML_add   (5 , LCDML_0_3    , 3, "show Pin+Addr."     , F_back);    // 2018-03-21 passt
  LCDML_add   (6 , LCDML_0_3    , 4, "search Sensors"     , F_back);      // 2018-03-21 passt
  LCDML_add   (7 , LCDML_0_3    , 5, "EEPROM to RAM"      , F_back);  // 2018-03-21  im Test
  LCDML_add   (8 , LCDML_0_3    , 6, "RAM to EEPROM "     , F_back);    // 2018-03-21 im Test weiß nicht wofür 
  LCDML_add   (9 , LCDML_0_3    , 7, "CLEAR EEPROM !"     , F_back); // 2018-03-21 passt
  LCDML_add   (10, LCDML_0_3    , 8, "identify Sensors"   , F_back);    // 2018-03-21 im Test
//---------ERROR's----------------------------------------  ------------------------------------
  LCDML_add   (11, LCDML_0      , 4, "Error's"            , NULL);
  LCDML_add   (12, LCDML_0_4    , 1, LCD_SZ_ZEIGLI" back" , F_back);
  LCDML_add   (13, LCDML_0_4    , 2, "show Error's"       , F_back);          // 2018-03-28 neue,>0,alle Fehler auflisten, bestätigen  passt
  LCDML_add   (14, LCDML_0_4    , 3, "all reset"          , F_back);         // 2018-03-28 alle je aufgetretenen Fehler auf 0 stellen passt
//---------PARAMETER--------------------------------------  ------------------------------------
  LCDML_add   (15, LCDML_0      , 5, "Parameter & Time"   , NULL);                
  LCDML_add   (16, LCDML_0_5    , 1, LCD_SZ_ZEIGLI" back" , F_back);
  LCDML_add   (17, LCDML_0_5    , 2, "Parameterlist"      , F_back);           // 2018-04-02 passt 
  LCDML_add   (18, LCDML_0_5    , 3, "I2C-scan/show"      , F_back);          // 2018-04-02 passt  
  LCDML_add   (19, LCDML_0_5    , 4, "HX711 Calibrate"    , F_back);   // 2018-04-19 passt  
  LCDML_add   (20, LCDML_0_5    , 5, "TIME settings"      , F_back);              // 2018-04-02 passt
  LCDML_add   (21, LCDML_0_5    , 6, "factory RESET"      , F_back);         // 2018-05-07 Werkseinstellungen + Alle Parameter auf Default-Wert setzen im EEPROM
  LCDML_add   (22, LCDML_0_5    , 7, "Timer Menue plan"   , F_back);        // 2018-04-02 nur eine Idee
//---------HILFSPROCEDUREN--------------------------------  ------------------------------------
  LCDML_add   (23, LCDML_0      , 6, "Aktors/Switches" , NULL);
  LCDML_add   (24, LCDML_0_6    , 1, LCD_SZ_ZEIGLI" back" , F_back);  
  LCDML_add   (25, LCDML_0_6    , 2, "(g)-List/act"       , F_back);            // 2018-04-02 passt
  LCDML_add   (26, LCDML_0_6    , 3, "(k)-List/act"       , F_back);            // 2018-04-02 passt
  LCDML_add   (27, LCDML_0_6    , 4, "(s)-List/show"      , F_back);            // 2018-04-02 passt
  LCDML_add   (28, LCDML_0_6    , 5, "(f)-List/show"      , F_back);            // 2018-04-03 passt  
  LCDML_add   (29, LCDML_0_6    , 6, "(c)-Commands"       , F_back);            // 2018-04-04 passt  
  LCDML_add   (30, LCDML_0_6    , 7, "(m)-Macros"         , F_back);            // 2018-04-05 passt  

  LCDML_createMenu(_LCDML_DISP_cnt);
