#ifndef SOIL_SENSOR_H
#define SOIL_SENSOR_H

const int soilPins[] = {34, 35, 32, 33};  // Example GPIOs
const int numSoilSensors = sizeof(soilPins) / sizeof(soilPins[0]);

void readSoilMoisture() {
  for (int i = 0; i < numSoilSensors; i++) {
    int val = analogRead(soilPins[i]);
    int moisturePercent = map(val, 2970, 1850, 0, 100);
    moisturePercent = constrain(moisturePercent, 0, 100);
    Serial.print("Soil ");
    Serial.print(i);
    Serial.print(": ");
    Serial.print(val);
    Serial.print(" (");
    Serial.print(moisturePercent);
    Serial.println(" %)");
  }
}
#endif
