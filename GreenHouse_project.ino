#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include "secrets.h" 

//Local Soil Moisture Sensors (ADC1 Pins)
const int soilPins[6] = {32, 33, 34, 35, 36, 39};
int soilValues[6];

//DS18B20 Sensors on GPIO4, GPIO13, GPIO14
#define ONE_WIRE_BUS_1 4
#define ONE_WIRE_BUS_2 13
#define ONE_WIRE_BUS_3 14

OneWire oneWire1(ONE_WIRE_BUS_1);
OneWire oneWire2(ONE_WIRE_BUS_2);
OneWire oneWire3(ONE_WIRE_BUS_3);

DallasTemperature dsGroup1(&oneWire1);
DallasTemperature dsGroup2(&oneWire2);
DallasTemperature dsGroup3(&oneWire3);

DeviceAddress dsAddr1[3];
DeviceAddress dsAddr2[3];
DeviceAddress dsAddr3[3];
int count1 = 0, count2 = 0, count3 = 0;

//DHT22 Temperature & Humidity Sensor
#define DHTPIN 16
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

//Remote ESP32-B Soil Data (via ESP-NOW)
typedef struct SoilData {
  int moisture[3];
} SoilData;
SoilData remoteData;
bool newRemoteData = false;

//ESP-NOW Receive Callback
void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
  if (len == sizeof(remoteData)) {
    memcpy(&remoteData, data, sizeof(remoteData));
    newRemoteData = true;
  }
}

void setup() {
  Serial.begin(115200);

  //Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Connected!");
  delay(3000);

  //Print Current Wi-Fi Channel and MAC
  uint8_t channel;
  wifi_second_chan_t second;
  esp_wifi_get_channel(&channel, &second);
  Serial.print("ESP32-A channel: ");
  Serial.println(channel);
  Serial.print("ESP32-A MAC: ");
  Serial.println(WiFi.macAddress());

  //Initialize Sensors
  dsGroup1.begin();
  dsGroup2.begin();
  dsGroup3.begin();
  dht.begin();

  //Search for DS18B20 sensors
  Serial.println("Searching DS18B20 on GPIO4");
  while (oneWire1.search(dsAddr1[count1]) && count1 < 3) count1++;
  Serial.println("Searching DS18B20 on GPIO13");
  while (oneWire2.search(dsAddr2[count2]) && count2 < 3) count2++;
  Serial.println("Searching DS18B20 on GPIO14");
  while (oneWire3.search(dsAddr3[count3]) && count3 < 3) count3++;

  //Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);
  Serial.println("ESP-NOW Ready");
}

void loop() {
  //Manual Calibration of Local Soil Sensors
  //S1
  int raw1 = analogRead(soilPins[0]);
  int soil1 = map(raw1, 3830, 1610, 0, 100);
  soil1 = constrain(soil1, 0, 100);

  //S2
  int raw2 = analogRead(soilPins[1]);
  int soil2 = map(raw2, 3330, 1480, 0, 100);
  soil2 = constrain(soil2, 0, 100);

  //S3
  int raw3 = analogRead(soilPins[2]);
  int soil3 = map(raw3, 3040, 2330, 0, 100);
  soil3 = constrain(soil3, 0, 100);

  //S4
  int raw4 = analogRead(soilPins[3]);
  int soil4 = map(raw4, 3770, 1612, 0, 100);
  soil4 = constrain(soil4, 0, 100);

  //S5
  int raw5 = analogRead(soilPins[4]);
  int soil5 = map(raw5, 3885, 1650, 0, 100);
  soil5 = constrain(soil5, 0, 100);

  //S6
  int raw6 = analogRead(soilPins[5]);
  int soil6 = map(raw6, 3150, 1930, 0, 100);
  soil6 = constrain(soil6, 0, 100);

  //Read DS18B20 Sensors
  float dsTemps[9];
  dsGroup1.requestTemperatures();
  for (int i = 0; i < count1; i++) {
    dsTemps[i] = dsGroup1.getTempC(dsAddr1[i]);
  }

  dsGroup2.requestTemperatures();
  for (int i = 0; i < count2; i++) {
    dsTemps[i + 3] = dsGroup2.getTempC(dsAddr2[i]);
  }

  dsGroup3.requestTemperatures();
  for (int i = 0; i < count3; i++) {
    dsTemps[i + 6] = dsGroup3.getTempC(dsAddr3[i]);
  }

  //Read DHT22 Sensor
  float dhtTemp = dht.readTemperature();
  float dhtHum = dht.readHumidity();

  //Remote Data (from ESP-B)
  int r1 = -1, r2 = -1, r3 = -1;
  if (newRemoteData) {
    r1 = remoteData.moisture[0];
    r2 = remoteData.moisture[1];
    r3 = remoteData.moisture[2];
    newRemoteData = false;
  }

  //Prepare JSON Payload
  String postData = "{";
  postData += "\"soil1\":" + String(soil1) + ",";
  postData += "\"soil2\":" + String(soil2) + ",";
  postData += "\"soil3\":" + String(soil3) + ",";
  postData += "\"soil4\":" + String(soil4) + ",";
  postData += "\"soil5\":" + String(soil5) + ",";
  postData += "\"soil6\":" + String(soil6) + ",";
  for (int i = 0; i < 9; i++) postData += "\"ds18b20_" + String(i + 1) + "\":" + String(dsTemps[i]) + ",";
  postData += "\"dht_temp\":" + String(dhtTemp) + ",";
  postData += "\"dht_hum\":" + String(dhtHum) + ",";
  postData += "\"remote_soil1\":" + String(r1) + ",";
  postData += "\"remote_soil2\":" + String(r2) + ",";
  postData += "\"remote_soil3\":" + String(r3);
  postData += "}";

  Serial.println("=== JSON Payload ===");
  Serial.println(postData);

  //Upload to Google Sheets
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(scriptURL);
    http.addHeader("Content-Type", "application/json");
    int code = http.POST(postData);
    Serial.print("HTTP Response code: ");
    Serial.println(code);
    http.end();
  } else {
    Serial.println("WiFi not connected");
  }

  delay(1800000);  //30 min delay
}
