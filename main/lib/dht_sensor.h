#pragma once
#include <DHT.h>
#include "config.h"

extern DHT dht;
extern float dhtTemp, dhtHum;

void initDHT();
void readDHT();
bool isDHTValid();
