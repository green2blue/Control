// diverse Funktionen in engem und weitem Zusammenhang mit Tasks 
void tDis_Macro() {
  
}

bool tEn_Macro() {
  
  return true;
}

void tf_Macro() {
  
}

void tf_Temp() {
  
}

void tf_Press_disable() {
    
}

void tf_Press() {
  
}

void tf_H2t() {
  
}

void tf_blink() {
   
}




//-------------------------------------------------------------------------
// Task für die Abarbeitung der EIN/AUS/NOT-AUS Schaltzustände vorbereiten
//-------------------------------------------------------------------------
void Prepare_Task_stask() {
  //stPWR.setWaiting();       // set the statusrequest object for waiting 
  //stask.waitFor(&stPWR);    //  task stask wait on the object st
}

//-------------------------------------
// Task für EIN/AUS/NOT-AUS  ARBEIT
//-------------------------------------
void sfunc(){
  // RunMacro( sfuncMacroNr,   0,      0,       0,        0,         0); // das entsprechende Makro starten
  Prepare_Task_stask();     // damit hört der Task erneut auf ein stPWR.signalComplete()
}
