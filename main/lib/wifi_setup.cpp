#include "wifi_setup.h"

void setupWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  
  int attempts = 0;
  const int maxAttempts = 20; // 10 seconds max
  
  while (WiFi.status() != WL_CONNECTED && attempts < maxAttempts) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println(" Connected!");
    printWiFiInfo();
  } else {
    Serial.println(" Connection failed!");
  }
  
  delay(1000);
}

void printWiFiInfo() {
  Serial.printf("SSID: %s\n", WiFi.SSID().c_str());
  Serial.printf("IP Address: %s\n", WiFi.localIP().toString().c_str());
  Serial.printf("MAC Address: %s\n", WiFi.macAddress().c_str());
  Serial.printf("Signal Strength: %d dBm\n", WiFi.RSSI());
}

bool isWiFiConnected() {
  return WiFi.status() == WL_CONNECTED;
} 