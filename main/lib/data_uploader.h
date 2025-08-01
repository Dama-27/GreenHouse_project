#pragma once
#include <HTTPClient.h>
#include "secrets.h"
#include "config.h"

// External sensor data
extern int soilValues[NUM_LOCAL_SOIL_SENSORS];
extern float dsTemps[NUM_DS18B20_SENSORS];
extern float dhtTemp, dhtHum;
extern float lux;
extern int r1, r2, r3;

void uploadData();
String buildJsonPayload();
bool uploadToGoogleSheets(const String& jsonData);
void printUploadStatus(int httpCode);
