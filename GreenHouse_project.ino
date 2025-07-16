#include "sensor_ds18b20.h"
#include "sensor_moisture.h"
#include "sensor_dht22.h"
#include "sensor_bh1750.h"

void setup() {
  Serial.begin(115200);

  initDS18B20();
  initDHT();
  initLight();
  delay(200);
}

void loop() {
  readDS18B20();
  readMoisture();
  readDHT();
  readLight();

  Serial.println("==== Sensor Readings ====");
  for (int i = 0; i < 9; i++) {
    Serial.print("Soil Temp "); Serial.print(i); Serial.print(": ");
    Serial.print(soilTemps[i]); Serial.print("°C\t");

    Serial.print("Moisture "); Serial.print(i); Serial.print(": ");
    Serial.println(moistureVals[i]);
  }

  Serial.print("Air Temp: "); Serial.println(airTemp);
  Serial.print("Humidity: "); Serial.println(airHumidity);
  Serial.print("Light Intensity: "); Serial.print(lightLevel); Serial.println(" lux");

  delay(10000); // Read every 10s
}
