#include <OneWire.h>
#include <DallasTemperature.h>

#define DS_LINE_1 4
#define DS_LINE_2 5
#define DS_LINE_3 18

OneWire oneWire1(DS_LINE_1);
OneWire oneWire2(DS_LINE_2);
OneWire oneWire3(DS_LINE_3);

DallasTemperature sensors1(&oneWire1);
DallasTemperature sensors2(&oneWire2);
DallasTemperature sensors3(&oneWire3);

float soilTemps[9];

void initDS18B20() {
  sensors1.begin();
  sensors2.begin();
  sensors3.begin();
}

void readDS18B20() {
  sensors1.requestTemperatures();
  sensors2.requestTemperatures();
  sensors3.requestTemperatures();

  for (int i = 0; i < 3; i++) {
    soilTemps[i]   = sensors1.getTempCByIndex(i);
    soilTemps[i+3] = sensors2.getTempCByIndex(i);
    soilTemps[i+6] = sensors3.getTempCByIndex(i);
  }
}
