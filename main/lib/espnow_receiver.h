#pragma once
#include <esp_now.h>
#include <WiFi.h>
#include "config.h"

extern SoilData remoteData;
extern bool newRemoteData;
extern int r1, r2, r3;

void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *data, int len);
void initESPNow();
void handleRemoteData();
void printRemoteData();
