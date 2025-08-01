#include "bh1750_sensor.h"

BH1750 lightMeter;
float lux = -1;

void initBH1750() {
  Wire.begin(BH1750_SDA, BH1750_SCL);
  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println("BH1750 light sensor initialized successfully");
  } else {
    Serial.println("BH1750 light sensor initialization failed");
  }
}

void readBH1750() {
  lux = lightMeter.readLightLevel();
  
  if (isBH1750Valid()) {
    Serial.printf("BH1750 - Light: %.1f lux\n", lux);
  } else {
    Serial.println("BH1750 read failed");
  }
}

bool isBH1750Valid() {
  return !isnan(lux) && lux >= 0;
} 