#pragma once
#include "config.h"

extern int soilValues[NUM_LOCAL_SOIL_SENSORS];

void initSoilSensors();
void readLocalSoil();

// Soil sensor calibration function
int calibrateSoilMoisture(int rawValue);
