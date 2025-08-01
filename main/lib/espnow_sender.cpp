#include "espnow_sender.h"
#include <esp_wifi.h>

void OnSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  printSendStatus(status);
}

void initESPNow() {
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }

  esp_now_register_send_cb(OnSent);

  // Set WiFi channel for ESP-NOW
  esp_wifi_set_channel(WIFI_CHANNEL, WIFI_SECOND_CHAN_NONE);

  // Add peer (ESP32-A gateway)
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, ESP32_A_MAC, 6);
  peerInfo.channel = WIFI_CHANNEL;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
  } else {
    Serial.println("Peer added successfully");
    Serial.printf("Target MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
                  ESP32_A_MAC[0], ESP32_A_MAC[1], ESP32_A_MAC[2],
                  ESP32_A_MAC[3], ESP32_A_MAC[4], ESP32_A_MAC[5]);
  }
  
  Serial.println("ESP-NOW Sender Ready");
  Serial.printf("Sending on channel: %d\n", WIFI_CHANNEL);
}

void sendSoilData(const int* soilValues) {
  SoilData data;
  
  // Copy soil values to data structure
  for (int i = 0; i < 3; i++) {
    data.moisture[i] = soilValues[i];
  }
  
  // Add timestamp
  data.timestamp = millis();

  Serial.printf("Sending: %d%%, %d%%, %d%% (Timestamp: %lu)\n", 
                data.moisture[0], data.moisture[1], data.moisture[2], data.timestamp);
  
  esp_err_t result = esp_now_send(ESP32_A_MAC, (uint8_t *)&data, sizeof(data));
  if (result != ESP_OK) {
    Serial.printf("Send failed: %s\n", esp_err_to_name(result));
  }
}

void printSendStatus(esp_now_send_status_t status) {
  Serial.print("Send Status: ");
  if (status == ESP_NOW_SEND_SUCCESS) {
    Serial.println("Success");
  } else {
    Serial.println("Fail");
  }
} 