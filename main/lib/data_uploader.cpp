#include "data_uploader.h"
#include <WiFi.h>

void uploadData() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected, skipping upload");
    return;
  }

  String jsonData = buildJsonPayload();
  Serial.println("Uploading data to Google Sheets...");
  Serial.println("JSON payload: " + jsonData);
  
  bool success = uploadToGoogleSheets(jsonData);
  if (success) {
    Serial.println("Data upload completed successfully");
  } else {
    Serial.println("Data upload failed");
  }
}

String buildJsonPayload() {
  String json = "{";
  
  // Local soil sensors
  for (int i = 0; i < NUM_LOCAL_SOIL_SENSORS; i++) {
    json += "\"" + String(JsonFields::SOIL_PREFIX) + String(i+1) + "\":" + String(soilValues[i]);
    if (i < NUM_LOCAL_SOIL_SENSORS - 1) json += ",";
  }
  
  // DS18B20 temperature sensors
  for (int i = 0; i < NUM_DS18B20_SENSORS; i++) {
    json += ",\"" + String(JsonFields::DS18B20_PREFIX) + String(i+1) + "\":" + String(dsTemps[i]);
  }
  
  // DHT22 sensor
  json += ",\"" + String(JsonFields::DHT_TEMP) + "\":" + String(dhtTemp);
  json += ",\"" + String(JsonFields::DHT_HUM) + "\":" + String(dhtHum);
  
  // BH1750 light sensor
  json += ",\"" + String(JsonFields::LIGHT) + "\":" + String(lux);
  
  // Remote soil sensors
  json += ",\"" + String(JsonFields::REMOTE_SOIL_PREFIX) + "1\":" + String(r1);
  json += ",\"" + String(JsonFields::REMOTE_SOIL_PREFIX) + "2\":" + String(r2);
  json += ",\"" + String(JsonFields::REMOTE_SOIL_PREFIX) + "3\":" + String(r3);
  
  json += "}";
  return json;
}

bool uploadToGoogleSheets(const String& jsonData) {
  HTTPClient http;
  http.begin(scriptURL);
  http.addHeader("Content-Type", "application/json");
  
  int httpCode = http.POST(jsonData);
  printUploadStatus(httpCode);
  
  http.end();
  
  return (httpCode == 200);
}

void printUploadStatus(int httpCode) {
  Serial.print("HTTP Response code: ");
  Serial.println(httpCode);
  
  switch (httpCode) {
    case 200:
      Serial.println("Upload successful");
      break;
    case 400:
      Serial.println("Bad request - check JSON format");
      break;
    case 401:
      Serial.println("Unauthorized - check script URL");
      break;
    case 403:
      Serial.println("Forbidden - check script permissions");
      break;
    case 500:
      Serial.println("Server error - check Google Apps Script");
      break;
    default:
      Serial.println("Unknown error");
      break;
  }
} 