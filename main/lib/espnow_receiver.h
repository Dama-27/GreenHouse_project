#pragma once
#include <esp_now.h>

typedef struct SoilData {
  int moisture[3];
} SoilData;

SoilData remoteData;
bool newRemoteData = false;
int r1 = -1, r2 = -1, r3 = -1;

void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
  if (len == sizeof(remoteData)) {
    memcpy(&remoteData, data, sizeof(remoteData));
    newRemoteData = true;
  }
}

void initESPNow() {
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);
  Serial.println("ESP-NOW Ready");
}

void handleRemoteData() {
  if (newRemoteData) {
    r1 = remoteData.moisture[0];
    r2 = remoteData.moisture[1];
    r3 = remoteData.moisture[2];
    newRemoteData = false;
  }
}
