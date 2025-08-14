#include <WiFi.h>
#include <esp_now.h>
#include "esp_wifi.h"

uint8_t mainESPAddress[] = {0x68, 0x25, 0xDD, 0x33, 0x74, 0x8C};  // MAC of ESP32-A

// Soil sensors
const int soilPins[3] = {32, 33, 34};  // ADC1 only
int soilValues[3];

typedef struct SoilData {
  int moisture[3];
} SoilData;

SoilData data;

void OnSent(const wifi_tx_info_t *info, esp_now_send_status_t status) {
  Serial.print("Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}


void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Set to same channel as ESP32-A (e.g., 9)
  esp_wifi_set_channel(7, WIFI_SECOND_CHAN_NONE);
  Serial.print("ESP32-B MAC: ");
  Serial.println(WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("❌ ESP-NOW init failed");
    return;
  }

  esp_now_register_send_cb(OnSent);

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, mainESPAddress, 6);
  peerInfo.channel = 0;  // use current channel
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("❌ Failed to add peer");
  } else {
    Serial.println("✅ Peer added successfully");
  }
}

void loop() {
  for (int i = 0; i < 3; i++) {
    int raw = analogRead(soilPins[i]);  // Read raw ADC
    int percent = map(raw, 3600, 1590, 0, 100);  // Example calibration
    soilValues[i] = constrain(percent, 0, 100);
  }

  data.moisture[0] = soilValues[0]; //soilValues[0];
  data.moisture[1] = soilValues[1];
  data.moisture[2] = soilValues[2];

  Serial.printf("Sending: %d%%, %d%%, %d%%\n", soilValues[0], soilValues[1], soilValues[2]);
  esp_now_send(mainESPAddress, (uint8_t *)&data, sizeof(data));

  delay(900000);
}
