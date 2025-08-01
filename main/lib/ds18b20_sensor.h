#pragma once
#include <OneWire.h>
#include <DallasTemperature.h>
#include "config.h"

extern OneWire oneWire;
extern DallasTemperature ds18b20;
extern float dsTemps[NUM_DS18B20_SENSORS];

void initDS18B20();
void readDS18B20();
int getDS18B20Count();
bool isDS18B20Valid(int index);
