#include <esp_now.h>
#include <WiFi.h>
#include "espnow_comm.h"
#include "soil_sensors.h"
#include "config.h"

SoilData data;

void OnSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void initESPNow() {
  if (esp_now_init() != ESP_OK) {
    Serial.println("❌ ESP-NOW init failed");
    return;
  }

  esp_now_register_send_cb(OnSent);

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, (const uint8_t *)MAIN_ESP32_MAC, 6);
  peerInfo.channel = 0;  // use current channel
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("❌ Failed to add peer");
  } else {
    Serial.println("✅ Peer added successfully");
  }
}

void sendSoilData() {
  for (int i = 0; i < 3; i++) {
    data.moisture[i] = soilValues[i];
  }

  Serial.printf("Sending: %d%%, %d%%, %d%%\n", data.moisture[0], data.moisture[1], data.moisture[2]);
  esp_now_send((const uint8_t *)MAIN_ESP32_MAC, (uint8_t *)&data, sizeof(data));
}
