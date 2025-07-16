#ifndef BH1750_SENSOR_H
#define BH1750_SENSOR_H

#include <Wire.h>
#include <BH1750.h>

BH1750 lightMeter;

void initBH1750() {
  Wire.begin(21, 22);  // SDA, SCL
  lightMeter.begin();
}

void readBH1750() {
  float lux = lightMeter.readLightLevel();
  Serial.print("BH1750 Light: ");
  Serial.print(lux);
  Serial.println(" lux");
}
#endif
