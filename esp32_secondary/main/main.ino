#include <WiFi.h>
#include "esp_wifi.h"
#include "soil_sensors.h"
#include "espnow_comm.h"
#include "config.h"

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();  // Disable Wi-Fi connection to prevent interference
  delay(100);

  esp_wifi_set_channel(CHANNEL, WIFI_SECOND_CHAN_NONE);
  Serial.print("ESP32-B MAC: ");
  Serial.println(WiFi.macAddress());

  initSoilSensors();
  initESPNow();
}

void loop() {
  readSoilSensors();
  sendSoilData();
  delay(20000);  // Send every 20 seconds
}
