#include <DHT.h>

#define DHTPIN 15
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
float airTemp, airHumidity;

void initDHT() {
  dht.begin();
}

void readDHT() {
  airTemp = dht.readTemperature();
  airHumidity = dht.readHumidity();
}
