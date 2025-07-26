#pragma once
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature ds18b20(&oneWire);
float dsTemps[9];

void initDS18B20() {
  ds18b20.begin();
}

void readDS18B20() {
  ds18b20.requestTemperatures();
  for (int i = 0; i < 9; i++) {
    dsTemps[i] = ds18b20.getTempCByIndex(i);
  }
}
