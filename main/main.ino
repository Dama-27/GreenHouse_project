#include "lib/config.h"
#include "lib/wifi_setup.h"
#include "lib/soil_sensor.h"
#include "lib/dht_sensor.h"
#include "lib/ds18b20_sensor.h"
#include "lib/bh1750_sensor.h"
#include "lib/espnow_receiver.h"
#include "lib/data_uploader.h"
#include <esp_wifi.h>

void setup() {
  Serial.begin(115200);
  Serial.println("=== ESP32-A Greenhouse Gateway Starting ===");
  
  // Initialize WiFi first
  setupWiFi();
  
  // Print current WiFi channel and MAC
  uint8_t channel;
  wifi_second_chan_t second;
  esp_wifi_get_channel(&channel, &second);
  Serial.printf("ESP32-A channel: %d\n", channel);
  Serial.printf("ESP32-A MAC: %s\n", WiFi.macAddress().c_str());

  // Initialize all sensors
  initSoilSensors();
  initDHT();
  initDS18B20();
  initBH1750();
  initESPNow();
  
  Serial.println("=== ESP32-A Initialization Complete ===");
  Serial.printf("Upload interval: %d ms\n", DATA_UPLOAD_INTERVAL);
}

void loop() {
  Serial.println("\n--- Reading Sensors ---");
  
  // Read all local sensors
  readLocalSoil();
  readDS18B20();
  readDHT();
  readBH1750();
  
  // Handle remote data from ESP32-B
  handleRemoteData();
  
  // Upload data to Google Sheets
  uploadData();
  
  Serial.printf("Waiting %d ms before next reading...\n", DATA_UPLOAD_INTERVAL);
  delay(DATA_UPLOAD_INTERVAL);
}
