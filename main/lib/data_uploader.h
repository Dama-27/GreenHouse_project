#pragma once
#include <HTTPClient.h>
#include "secrets.h"
extern int soilValues[6];
extern float dsTemps[9], dhtTemp, dhtHum, lux;
extern int r1, r2, r3;

void uploadData() {
  String json = "{";
  for (int i = 0; i < 6; i++) json += "\"soil" + String(i+1) + "\":" + String(soilValues[i]) + ",";
  for (int i = 0; i < 9; i++) json += "\"ds18b20_" + String(i+1) + "\":" + String(dsTemps[i]) + ",";
  json += "\"dht_temp\":" + String(dhtTemp) + ",";
  json += "\"dht_hum\":" + String(dhtHum) + ",";
  json += "\"light\":" + String(lux) + ",";
  json += "\"remote_soil1\":" + String(r1) + ",";
  json += "\"remote_soil2\":" + String(r2) + ",";
  json += "\"remote_soil3\":" + String(r3);
  json += "}";

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(scriptURL);
    http.addHeader("Content-Type", "application/json");
    int code = http.POST(json);
    Serial.print("HTTP Response code: "); Serial.println(code);
    http.end();
  }
}
