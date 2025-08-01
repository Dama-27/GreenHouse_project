#include "ds18b20_sensor.h"

OneWire oneWire(DS18B20_PIN);
DallasTemperature ds18b20(&oneWire);
float dsTemps[NUM_DS18B20_SENSORS];

void initDS18B20() {
  ds18b20.begin();
  int deviceCount = getDS18B20Count();
  Serial.printf("DS18B20 sensors initialized. Found %d devices\n", deviceCount);
}

void readDS18B20() {
  ds18b20.requestTemperatures();
  
  Serial.print("DS18B20 temperatures: ");
  for (int i = 0; i < NUM_DS18B20_SENSORS; i++) {
    dsTemps[i] = ds18b20.getTempCByIndex(i);
    if (isDS18B20Valid(i)) {
      Serial.printf("%.1f°C ", dsTemps[i]);
    } else {
      Serial.print("ERR ");
    }
  }
  Serial.println();
}

int getDS18B20Count() {
  return ds18b20.getDeviceCount();
}

bool isDS18B20Valid(int index) {
  return !isnan(dsTemps[index]) && dsTemps[index] != DEVICE_DISCONNECTED_C;
} 