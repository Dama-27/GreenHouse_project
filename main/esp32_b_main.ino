#include "lib/config.h"
#include "lib/soil_sensor.h"
#include "lib/espnow_sender.h"
#include <WiFi.h>
#include <esp_wifi.h>

// Override config for ESP32-B
#undef ESP32_A
#define ESP32_B

void setup() {
  Serial.begin(115200);
  Serial.println("=== ESP32-B Remote Soil Sensor Starting ===");
  
  // Configure WiFi for ESP-NOW only (no connection needed)
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  
  // Set WiFi channel for ESP-NOW
  esp_wifi_set_channel(WIFI_CHANNEL, WIFI_SECOND_CHAN_NONE);
  Serial.printf("ESP32-B channel: %d\n", WIFI_CHANNEL);
  Serial.printf("ESP32-B MAC: %s\n", WiFi.macAddress().c_str());
  Serial.printf("Target MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
                ESP32_A_MAC[0], ESP32_A_MAC[1], ESP32_A_MAC[2],
                ESP32_A_MAC[3], ESP32_A_MAC[4], ESP32_A_MAC[5]);

  // Initialize sensors and ESP-NOW
  initSoilSensors();
  initESPNow();
  
  Serial.println("=== ESP32-B Initialization Complete ===");
  Serial.printf("Send interval: %d ms\n", SENSOR_READ_INTERVAL);
}

void loop() {
  Serial.println("\n--- Reading Soil Sensors ---");
  
  // Read soil sensors
  readLocalSoil();
  
  // Print local readings
  Serial.print("Local soil readings: ");
  for (int i = 0; i < NUM_LOCAL_SOIL_SENSORS; i++) {
    Serial.printf("%d%% ", soilValues[i]);
  }
  Serial.println();
  
  // Send data to ESP32-A gateway
  sendSoilData(soilValues);
  
  Serial.printf("Waiting %d ms before next reading...\n", SENSOR_READ_INTERVAL);
  delay(SENSOR_READ_INTERVAL);
} 