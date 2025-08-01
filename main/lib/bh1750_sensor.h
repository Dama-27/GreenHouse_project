#pragma once
#include <Wire.h>
#include <BH1750.h>
#include "config.h"

extern BH1750 lightMeter;
extern float lux;

void initBH1750();
void readBH1750();
bool isBH1750Valid();
