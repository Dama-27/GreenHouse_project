#pragma once
#include <WiFi.h>
#include <esp_wifi.h>
#include "secrets.h"
#include "config.h"

void setupWiFi();
void printWiFiInfo();
bool isWiFiConnected();
