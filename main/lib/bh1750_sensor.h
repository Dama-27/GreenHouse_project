#pragma once
#include <Wire.h>
#include <BH1750.h>
BH1750 lightMeter;
float lux = -1;

void initBH1750() {
  Wire.begin(21, 22);
  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println("BH1750 OK");
  } else {
    Serial.println("BH1750 failed");
  }
}

void readBH1750() {
  lux = lightMeter.readLightLevel();
}
