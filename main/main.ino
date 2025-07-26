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
  setupWiFi();
  // Print current WiFi channel
  uint8_t channel;
  wifi_second_chan_t second;
  esp_wifi_get_channel(&channel, &second);
  Serial.print("ESP32-A channel: "); Serial.println(channel);
  Serial.print("ESP32-A MAC: "); Serial.println(WiFi.macAddress());


  initSoilSensors();
  initDHT();
  initDS18B20();
  initBH1750();
  initESPNow();
}

void loop() {
  readLocalSoil();
  readDS18B20();
  readDHT();
  readBH1750();
  handleRemoteData();
  uploadData();
  delay(20000);
}
