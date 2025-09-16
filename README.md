# Smart Greenhouse Monitoring System (ESP32)

A complete IoT system for real-time monitoring of soil moisture, temperature, and humidity in a greenhouse. The setup uses two ESP32 boards, ESP-NOW for low-power peer-to-peer data transfer, and a Google Apps Script Web App to log readings into Google Sheets every 30 minutes.

> Project highlights: 6 local and 3 remote soil sensors; 9 DS18B20 sensors distributed across 3 separate 1-Wire buses for reliability; stable power and error handling improvements; consistent Google Sheets uploads.

---

## Project Structure

```
GreenHouse_project/
├── GreenHouse_project.ino   # ESP32-A (Gateway): local sensors + Wi-Fi + Google Sheets upload + ESP-NOW receiver
├── espB/
│   └── espB.ino             # ESP32-B (Remote): 3 soil sensors + ESP-NOW sender
├── secrets.h                # Wi-Fi SSID/PASS and Google Apps Script URL (do not commit real secrets)
├── migration_notes.md       # Notes from earlier refactor exploration (not reflected in current code)
├── LICENSE                  # Apache-2.0
└── README.md
```

---

## Features

- ESP32-A (Gateway)
  - Reads 6 local capacitive soil moisture sensors on ADC1 pins: 32, 33, 34, 35, 36, 39
  - Reads 9 DS18B20 temperature sensors spread across 3 OneWire buses: GPIO4, GPIO13, GPIO14
  - Reads DHT22 temperature and humidity on GPIO16
  - Receives 3 remote soil moisture readings from ESP32-B via ESP-NOW
  - Uploads all readings to Google Sheets via HTTP POST every 30 minutes

- ESP32-B (Remote)
  - Reads 3 capacitive soil moisture sensors on pins 32, 33, 34
  - Sends readings to ESP32-A over ESP-NOW

---

## Hardware

- 2x ESP32 DevKit
- 9x DS18B20 waterproof temperature probes (3 per OneWire bus)
- 1x DHT22 (AM2302) temperature & humidity sensor
- 9x capacitive soil moisture sensors (6 local on ESP32-A, 3 remote on ESP32-B)
- 4.7 kΩ pull-up resistors for each DS18B20 bus
- Stable 5V power supply with adequate current and decoupling

Pin summary (ESP32-A):
- Soil moisture: GPIO 32, 33, 34, 35, 36, 39 (ADC1 only)
- DS18B20 buses: GPIO4, GPIO13, GPIO14
- DHT22: GPIO16

Pin summary (ESP32-B):
- Soil moisture: GPIO 32, 33, 34

Note: Keep ESP32-A and ESP32-B on the same Wi-Fi channel for ESP-NOW.

---

## Software Requirements

Install in Arduino IDE or PlatformIO:
- WiFi (built-in)
- HTTPClient (built-in)
- esp_now (built-in)
- OneWire
- DallasTemperature
- DHT sensor library (Adafruit DHT)

---

## Configuration

1) Create `secrets.h` in the project root:
```cpp
// secrets.h
#pragma once

extern const char* ssid;
extern const char* password;
extern const char* scriptURL;
```
And in your local build, define them in a non-committed file or keep `secrets.h` but avoid pushing real values. The current code expects literal values; you can follow the pattern below:
```cpp// secrets.h
const char* ssid = "Your_WiFi_SSID";
const char* password = "Your_WiFi_Password";
const char* scriptURL = "https://script.google.com/macros/s/XXX/exec";
```

2) ESP-NOW peer settings:
- In `espB/espB.ino`, set `mainESPAddress` to the MAC address of ESP32-A (Gateway):
```cpp
uint8_t mainESPAddress[] = {0x68, 0x25, 0xDD, 0x33, 0x74, 0x8C};
```
- Ensure both boards use the same Wi-Fi channel. ESP32-B sets a fixed channel in code:
```cpp
esp_wifi_set_channel(7, WIFI_SECOND_CHAN_NONE);
```
ESP32-A prints its current channel at boot; adjust ESP32-B to match.

3) Soil sensor calibration:
- Calibration mapping is hardcoded in `GreenHouse_project.ino` for each sensor. Example:
```cpp
int soil1 = map(raw1, 3830, 1610, 0, 100);
```
Adjust the dry/wet ADC endpoints per sensor for your soil type and sensor batch.

---

## Build & Upload

- ESP32-A (Gateway)
  1. Open `GreenHouse_project.ino`
  2. Connect ESP32-A and upload

- ESP32-B (Remote)
  1. Open `espB/espB.ino`
  2. Update `mainESPAddress` and Wi-Fi channel if needed
  3. Connect ESP32-B and upload

---

## Data Flow and Format

- ESP32-B reads 3 soil sensors, sends via ESP-NOW to ESP32-A
- ESP32-A reads 6 local soil sensors, 9 DS18B20 sensors, DHT22, merges remote 3 soil values
- ESP32-A builds a JSON payload and POSTs it to the Google Apps Script Web App
- Interval: 30 minutes (`delay(1800000)`)

Example JSON (fields match the code):
```json
{
  "soil1": 45,
  "soil2": 67,
  "soil3": 23,
  "soil4": 89,
  "soil5": 34,
  "soil6": 56,
  "ds18b20_1": 22.5,
  "ds18b20_2": 23.1,
  "ds18b20_3": 21.8,
  "ds18b20_4": 24.2,
  "ds18b20_5": 22.9,
  "ds18b20_6": 23.5,
  "ds18b20_7": 22.1,
  "ds18b20_8": 23.8,
  "ds18b20_9": 22.3,
  "dht_temp": 23.4,
  "dht_hum": 65.2,
  "remote_soil1": 78,
  "remote_soil2": 45,
  "remote_soil3": 92
}
```

---

## Google Sheets Integration

Deploy a Google Apps Script Web App and use its deployment URL as `scriptURL`.

Minimal script:
```javascript
function doPost(e) {
  var sheet = SpreadsheetApp.getActiveSheet();
  var data = JSON.parse(e.postData.contents);
  sheet.appendRow([
    new Date(),
    data.soil1, data.soil2, data.soil3, data.soil4, data.soil5, data.soil6,
    data.remote_soil1, data.remote_soil2, data.remote_soil3,
    data.ds18b20_1, data.ds18b20_2, data.ds18b20_3, data.ds18b20_4, data.ds18b20_5,
    data.ds18b20_6, data.ds18b20_7, data.ds18b20_8, data.ds18b20_9,
    data.dht_temp, data.dht_hum
  ]);
  return ContentService.createTextOutput("OK");
}
```

Make sure the Web App is deployed with access set to Anyone with the link (or appropriate permissions).

---

## Reliability Notes & Lessons Learned

- Distributing the 9 DS18B20 sensors across 3 separate OneWire buses resolved bus loading and timing issues seen when all sensors shared a single bus.
- Stabilizing power (adequate current, decoupling, good terminations) eliminated intermittent faulty readings.
- Calibrate each soil sensor individually; sensor-to-sensor variation is significant.

---

## Troubleshooting

- ESP-NOW
  - Verify ESP32-B uses the same Wi-Fi channel as ESP32-A
  - Double-check ESP32-A MAC address in `espB/espB.ino`
- Sensors
  - Use only ADC1 pins for analog soil sensors
  - Add 4.7 kΩ pull-ups on each DS18B20 bus; keep cable lengths reasonable
- Wi-Fi & Upload
  - Confirm `ssid`, `password`, and `scriptURL` in `secrets.h`
  - Print Serial logs to see HTTP status codes and payload

---

## Security

- Do not commit real Wi-Fi credentials or script URLs. Consider adding `secrets.h` to `.gitignore` and keeping a `secrets_example.h` in the repo.

---

## License

This project is licensed under the Apache License 2.0. See `LICENSE` for details.

