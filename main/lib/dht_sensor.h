#pragma once
#include <DHT.h>
#define DHTPIN 16
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
float dhtTemp, dhtHum;

void initDHT() {
  dht.begin();
}
void readDHT() {
  dhtTemp = dht.readTemperature();
  dhtHum = dht.readHumidity();
}
