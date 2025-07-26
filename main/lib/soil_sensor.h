#pragma once
const int soilPins[6] = {32, 33, 34, 35, 36, 39};
int soilValues[6];

void initSoilSensors() {}
void readLocalSoil() {
  for (int i = 0; i < 6; i++) {
    int raw = analogRead(soilPins[i]);
    int percent = map(raw, 2970, 1850, 0, 100);
    soilValues[i] = constrain(percent, 0, 100);
  }
}
