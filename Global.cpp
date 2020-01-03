#include "Global.h"


 DeviceStatus gStatus = DeviceStatus::Init;

 float gfDeviceTemperature = 0;


 void Global_SetDefaultValue()
 {
     gfDeviceTemperature = DEFAULT_TEMPERATUR;
     gStatus = DeviceStatus::Init;

     if (gStatus == DeviceStatus::Working)
     {
         if (gfDeviceTemperature > 30)
         {
             gStatus = DeviceStatus::Error;
         }
     }



 }
