#ifndef DHT22_SENSOR_H
#define DHT22_SENSOR_H

#include <DHT.h>
#define DHTPIN 16
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

void initDHT() {
  dht.begin();
}

void readDHT() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  if (!isnan(temp) && !isnan(hum)) {
    Serial.print("DHT22 Temp: ");
    Serial.print(temp);
    Serial.print(" °C | Humidity: ");
    Serial.print(hum);
    Serial.println(" %");
  } else {
    Serial.println("❌ DHT22 read failed.");
  }
}
#endif
