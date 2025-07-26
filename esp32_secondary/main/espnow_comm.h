#ifndef ESPNOW_COMM_H
#define ESPNOW_COMM_H

#include <Arduino.h>

typedef struct SoilData {
  int moisture[3];
} SoilData;

void initESPNow();
void sendSoilData();

#endif
