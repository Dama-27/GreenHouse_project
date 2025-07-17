#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <BH1750.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include "secrets.h"  // Contains ssid, password, scriptURL

// Local Sensors
const int soilPins[6] = {32, 33, 34, 35, 36, 39};
int soilValues[6];

#define ONE_WIRE_BUS 4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature ds18b20(&oneWire);

#define DHTPIN 16
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

BH1750 lightMeter;

// Remote Data
typedef struct SoilData {
  int moisture[3];
} SoilData;
SoilData remoteData;
bool newRemoteData = false;

void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
  if (len == sizeof(remoteData)) {
    memcpy(&remoteData, data, sizeof(remoteData));
    newRemoteData = true;
  }
}

void setup() {
  Serial.begin(115200);

  // Connect WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println(" Connected!");

  delay(3000);  // Let WiFi settle

  // Print current WiFi channel
  uint8_t channel;
  wifi_second_chan_t second;
  esp_wifi_get_channel(&channel, &second);
  Serial.print("ESP32-A channel: "); Serial.println(channel);

  Serial.print("ESP32-A MAC: "); Serial.println(WiFi.macAddress());

  // Init sensors
  ds18b20.begin();
  dht.begin();
  Wire.begin(21, 22);
  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println("BH1750 OK");
  } else {
    Serial.println("BH1750 failed");
  }

  // ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("❌ ESP-NOW init failed");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);
  Serial.println("✅ ESP-NOW Ready");
}

void loop() {
  // Local Soil
  for (int i = 0; i < 6; i++) {
    int raw = analogRead(soilPins[i]);
    int percent = map(raw, 2970, 1850, 0, 100);
    soilValues[i] = constrain(percent, 0, 100);
  }

  // DS18B20
  ds18b20.requestTemperatures();
  float dsTemps[9];
  for (int i = 0; i < 9; i++) {
    dsTemps[i] = ds18b20.getTempCByIndex(i);
  }

  // DHT22
  float dhtTemp = dht.readTemperature();
  float dhtHum = dht.readHumidity();

  // BH1750
  float lux = lightMeter.readLightLevel();

  // Remote
  int r1 = -1, r2 = -1, r3 = -1;
  if (newRemoteData) {
    r1 = remoteData.moisture[0];
    r2 = remoteData.moisture[1];
    r3 = remoteData.moisture[2];
    newRemoteData = false;
  }

  // JSON
  String postData = "{";
  for (int i = 0; i < 6; i++) postData += "\"soil" + String(i+1) + "\":" + String(soilValues[i]) + ",";
  for (int i = 0; i < 9; i++) postData += "\"ds18b20_" + String(i+1) + "\":" + String(dsTemps[i]) + ",";
  postData += "\"dht_temp\":" + String(dhtTemp) + ",";
  postData += "\"dht_hum\":" + String(dhtHum) + ",";
  postData += "\"light\":" + String(lux) + ",";
  postData += "\"remote_soil1\":" + String(r1) + ",";
  postData += "\"remote_soil2\":" + String(r2) + ",";
  postData += "\"remote_soil3\":" + String(r3);
  postData += "}";

  // Upload to Google Sheets
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(scriptURL);
    http.addHeader("Content-Type", "application/json");
    int code = http.POST(postData);
    Serial.print("HTTP Response code: "); Serial.println(code);
    http.end();
  } else {
    Serial.println("❌ WiFi not connected");
  }

  delay(20000);  // 20s delay
}
