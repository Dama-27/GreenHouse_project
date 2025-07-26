#include "soil_sensors.h"

int soilPins[NUM_SOIL_SENSORS] = {32, 33, 34};
int soilValues[NUM_SOIL_SENSORS];

void initSoilSensors() {
  for (int i = 0; i < NUM_SOIL_SENSORS; i++) {
    pinMode(soilPins[i], INPUT);
  }
}

void readSoilSensors() {
  for (int i = 0; i < NUM_SOIL_SENSORS; i++) {
    int raw = analogRead(soilPins[i]);
    int percent = map(raw, 2970, 1850, 0, 100);
    soilValues[i] = constrain(percent, 0, 100);
  }
}
