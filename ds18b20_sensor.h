#ifndef DS18B20_SENSOR_H
#define DS18B20_SENSOR_H

#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void initDS18B20() {
  sensors.begin();
}

void readDS18B20() {
  sensors.requestTemperatures();
  for (int i = 0; i < sensors.getDeviceCount(); i++) {
    float tempC = sensors.getTempCByIndex(i);
    Serial.print("DS18B20 #");
    Serial.print(i);
    Serial.print(": ");
    Serial.print(tempC);
    Serial.println(" °C");
  }
}
#endif
