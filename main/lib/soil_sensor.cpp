#include "soil_sensor.h"
#include <Arduino.h>

int soilValues[NUM_LOCAL_SOIL_SENSORS];

void initSoilSensors() {
  for (int i = 0; i < NUM_LOCAL_SOIL_SENSORS; i++) {
    pinMode(SOIL_PINS[i], INPUT);
  }
  Serial.println("Soil sensors initialized");
}

void readLocalSoil() {
  for (int i = 0; i < NUM_LOCAL_SOIL_SENSORS; i++) {
    int raw = analogRead(SOIL_PINS[i]);
    soilValues[i] = calibrateSoilMoisture(raw);
  }
}

int calibrateSoilMoisture(int rawValue) {
  int percent = map(rawValue, SOIL_DRY_VALUE, SOIL_WET_VALUE, SOIL_MIN_PERCENT, SOIL_MAX_PERCENT);
  return constrain(percent, SOIL_MIN_PERCENT, SOIL_MAX_PERCENT);
} 