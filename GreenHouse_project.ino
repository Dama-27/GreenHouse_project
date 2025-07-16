#include <Wire.h>
#include "soil_sensor.h"
#include "ds18b20_sensor.h"
#include "dht22_sensor.h"
#include "bh1750_sensor.h"

void setup() {
  Serial.begin(115200);
  initDS18B20();
  initDHT();
  initBH1750();
}

void loop() {
  Serial.println("\n=== Sensor Readings ===");
  readSoilMoisture();
  readDS18B20();
  readDHT();
  readBH1750();
  delay(5000);  // Wait 5 seconds
}
