#ifndef SOIL_SENSORS_H
#define SOIL_SENSORS_H

#include <Arduino.h>

#define NUM_SOIL_SENSORS 3
extern int soilPins[NUM_SOIL_SENSORS];
extern int soilValues[NUM_SOIL_SENSORS];

void initSoilSensors();
void readSoilSensors();

#endif
