#include "espnow_receiver.h"
#include <esp_wifi.h>

SoilData remoteData;
bool newRemoteData = false;
int r1 = -1, r2 = -1, r3 = -1;

void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
  if (len == sizeof(SoilData)) {
    memcpy(&remoteData, data, sizeof(SoilData));
    newRemoteData = true;
    Serial.println("Remote data received successfully");
  } else {
    Serial.printf("Invalid data length: %d, expected: %d\n", len, sizeof(SoilData));
  }
}

void initESPNow() {
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }
  
  esp_now_register_recv_cb(OnDataRecv);
  
  // Set WiFi channel for ESP-NOW
  esp_wifi_set_channel(WIFI_CHANNEL, WIFI_SECOND_CHAN_NONE);
  
  Serial.println("ESP-NOW Receiver Ready");
  Serial.printf("Listening on channel: %d\n", WIFI_CHANNEL);
}

void handleRemoteData() {
  if (newRemoteData) {
    r1 = remoteData.moisture[0];
    r2 = remoteData.moisture[1];
    r3 = remoteData.moisture[2];
    
    printRemoteData();
    newRemoteData = false;
  }
}

void printRemoteData() {
  Serial.printf("Remote Soil Data - 1: %d%%, 2: %d%%, 3: %d%% (Timestamp: %lu)\n", 
                r1, r2, r3, remoteData.timestamp);
} 