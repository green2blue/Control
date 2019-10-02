//------------------------------------------------------------------------------
// Funktionen die aus dem LCD-Menü direkt aufgerufen werden 
// UND Funktionen die der allgemeinen LCD-Ausgabe gelten, ganz zu Beginn.
//------------------------------------------------------------------------------

void LCD_SHOW() {
  
}



void Mnu_Info( uint8_t param) {
  if(LCDML.FUNC_setup())     { (void)param;  // so wird ein unbenutzer Parameter nicht angemeckert vom Compiler 
  
    lcd.setCursor(0, 0);
    lcd.print(F("To close this"));
    lcd.setCursor(0, 1);
    lcd.print(F("function press"));
    lcd.setCursor(0, 2);
    lcd.print(F("any button or use"));
    lcd.setCursor(0, 3);
    lcd.print(F("back button"));
  }

  if(LCDML.FUNC_loop())           // ****** LOOP *********
  {
    // loop function, can be run in a loop when LCDML_DISP_triggerMenu(xx) is set
    // the quit button works in every DISP function without any checks; it starts the loop_end function
    if(LCDML.BT_checkAny()) { // check if any button is pressed (enter, up, down, left, right)
      // LCDML_goToMenu stops a running menu function and goes to the menu
      LCDML.FUNC_goBackToMenu();
    }
  }

  if(LCDML.FUNC_close())      // ****** STABLE END *********
  {
    // you can here reset some global vars or do nothing
  }
}






// *********************************************************************
//  MENÜ  RÜCKSPRUNG  zum  MENÜPUNKT  DARÜBER
// *********************************************************************
// *********************************************************************
void F_back(uint8_t param)  {
// *********************************************************************
  param=param;
  if(LCDML.FUNC_setup())        {   // ****** SETUP *********
    LCDML.FUNC_goBackToMenu(1); }   // leave this function and go a layer back
}

// *********************************************************************
void mFunc_goToRootMenu(uint8_t param) {
// *********************************************************************
  param=param;
  if(LCDML.FUNC_setup())   {        // ****** SETUP *********
    LCDML.MENU_goRoot();   }        // go to root and display menu
}
