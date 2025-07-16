#include <Wire.h>
#include <BH1750.h>

BH1750 lightMeter;
float lightLevel;

void initLight() {
  Wire.begin(21, 22); // SDA, SCL
  lightMeter.begin();
}

void readLight() {
  lightLevel = lightMeter.readLightLevel();
}
