#include "dht_sensor.h"

DHT dht(DHT_PIN, DHT22);
float dhtTemp, dhtHum;

void initDHT() {
  dht.begin();
  Serial.println("DHT22 sensor initialized");
}

void readDHT() {
  dhtTemp = dht.readTemperature();
  dhtHum = dht.readHumidity();
  
  if (isDHTValid()) {
    Serial.printf("DHT22 - Temp: %.1f°C, Humidity: %.1f%%\n", dhtTemp, dhtHum);
  } else {
    Serial.println("DHT22 read failed");
  }
}

bool isDHTValid() {
  return !isnan(dhtTemp) && !isnan(dhtHum);
} 