#define NUM_MOISTURE 9
const int moisturePins[NUM_MOISTURE] = {32, 33, 34, 35, 25, 26, 27, 14, 12};
int moistureVals[NUM_MOISTURE];

void readMoisture() {
  for (int i = 0; i < NUM_MOISTURE; i++) {
    moistureVals[i] = analogRead(moisturePins[i]);
  }
}
