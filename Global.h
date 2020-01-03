#pragma once

#include <Arduino.h>

#define DEFAULT_TEMPERATUR 22

#define PI_TEMPERATURE_1 23



enum DeviceStatus
{
	Init = 10,
	Working = 20,
	Error = 90
};


extern DeviceStatus gStatus;
extern float gfDeviceTemperature;


void Global_SetDefaultValue();
