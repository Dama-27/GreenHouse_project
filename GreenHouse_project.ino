#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <BH1750.h>
#include "secrets.h"


// Wi-Fi
const char* ssid = "DD 6839";
const char* password = "12345678D";

// Google Web App URL (replace with your deployed URL)
const char* scriptURL = "https://script.google.com/macros/s/AKfycbzzz2gBjXKp2KB6XkZ-Aga1WRt8TWJuWC5epZgG83U5DCPK9pgchIvrX1AHQWrJbtCB/exec";

// Soil Sensor
#define SOIL_PIN 32

// DS18B20
#define ONE_WIRE_BUS 4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature ds18b20(&oneWire);

// DHT22
#define DHTPIN 16
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// BH1750
BH1750 lightMeter;

void setup() {
  Serial.begin(115200);

  // WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected!");

  // Init sensors
  ds18b20.begin();
  dht.begin();
  Wire.begin();
  lightMeter.begin();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    // 🌱 Soil
    int rawSoil = analogRead(SOIL_PIN);
    int soilPercent = map(rawSoil, 2970, 1850, 0, 100);
    soilPercent = constrain(soilPercent, 0, 100);

    // 🌡️ DS18B20
    ds18b20.requestTemperatures();
    float dsTemp = ds18b20.getTempCByIndex(0);

    // 🌡️ DHT22
    float dhtTemp = dht.readTemperature();
    float dhtHum = dht.readHumidity();

    // ☀️ BH1750
    float lux = lightMeter.readLightLevel();

    // Create JSON string
    String postData = "{";
    postData += "\"soil1\":" + String(soilPercent) + ",";
    postData += "\"ds18b20_1\":" + String(dsTemp) + ",";
    postData += "\"dht_temp\":" + String(dhtTemp) + ",";
    postData += "\"dht_hum\":" + String(dhtHum) + ",";
    postData += "\"light\":" + String(lux);
    postData += "}";

    // Send HTTP POST
    HTTPClient http;
    http.begin(scriptURL);
    http.addHeader("Content-Type", "application/json");

    int httpResponseCode = http.POST(postData);

    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    http.end();
  } else {
    Serial.println("❌ WiFi not connected.");
  }

  delay(20000);  // 20 seconds delay
}
