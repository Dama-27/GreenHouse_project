#pragma once
#include <esp_now.h>
#include <WiFi.h>
#include "config.h"

void OnSent(const uint8_t *mac_addr, esp_now_send_status_t status);
void initESPNow();
void sendSoilData(const int* soilValues);
void printSendStatus(esp_now_send_status_t status); 