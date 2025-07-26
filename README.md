# GreenHouse_project

# 🌱 Greenhouse Monitoring System with ESP32

A smart greenhouse monitoring system built using multiple ESP32 microcontrollers that collects real-time environmental data and uploads it to Google Sheets for remote monitoring.

---

## 📦 Project Structure

```
GREENHOUSE_PROJECT/
├── esp32_main/              # ESP32-A (main unit)
│   └── main/
│       ├── main.ino
│       ├── wifi_setup.*     # WiFi connection
│       ├── espnow_receiver.*# ESP-NOW receive logic
│       ├── soil_sensor.*    # 6 local soil sensors
│       ├── ds18b20_sensor.* # 9 temperature probes
│       ├── dht_sensor.*     # DHT22 temp/humidity
│       ├── bh1750_sensor.*  # Light sensor
│       ├── data_uploader.*  # JSON → Google Sheets
│       ├── config.h         # Pins and thresholds
│       └── secrets.h        # WiFi + Google Script
│
├── esp32_secondary/         # ESP32-B (remote soil sensor)
│   └── main/
│       ├── main.ino
│       ├── config.h
│       ├── espnow_comm.*    # Send data via ESP-NOW
│       ├── soil_sensors.*   # 3 soil moisture sensors
│
├── secrets_template.h       # Example secrets file
├── LICENSE
└── README.md
```

---

## 📡 Features

- 🌿 **ESP32-A** (Main Node)
  - Reads **6 local soil sensors** (ADC)
  - Reads **9 DS18B20 temperature probes** (1-Wire)
  - Reads **DHT22** temp & humidity
  - Reads **BH1750** light intensity
  - Receives **3 remote soil readings** via **ESP-NOW** from ESP32-B
  - Uploads all data to **Google Sheets** every 20 seconds

- 🌱 **ESP32-B** (Secondary Node)
  - Reads 3 soil moisture sensors
  - Sends data to ESP32-A via ESP-NOW

---

## 🛠️ Hardware Requirements

| Sensor                | Quantity | ESP32 Pin                    |
|-----------------------|----------|------------------------------|
| Capacitive Soil Sensor| 9        | A: GPIO32–36,39, B: GPIO32–34|
| DS18B20 Waterproof    | 9        | A: GPIO4 (1-Wire shared)     |
| DHT22 / AM2302        | 1        | A: GPIO16                    |
| BH1750 Light Sensor   | 1        | A: SDA: GPIO21, SCL: GPIO22  |
| ESP32 DevKit v1       | 2        | -                            |

⚠️ Use only **ADC1 pins** for analog soil sensors.

---

## 🔌 Wiring Notes

- **Soil Sensors**: Connect to ADC1 only (e.g., GPIO32 to GPIO39).
- **DS18B20**: All 9 sensors connected to a shared 1-Wire bus (GPIO4) with a **4.7kΩ pull-up**.
- **BH1750**: I2C wiring with default address `0x23`.
- **ESP-NOW**: BothESP32s connected. Ensure both ESP32s are on the **same Wi-Fi channel**.

---

## 🌐 Google Sheets Integration

- Deploy a **Google Apps Script Web App** from `Google Script` editor.
- Use the following sample script:

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
    data.dht_temp, data.dht_hum,
    data.light
  ]);

  return ContentService.createTextOutput("OK");
}
```

---

## 🔒 Secrets Configuration

Copy `secrets_template.h` ➝ `secrets.h` and fill in:

```cpp
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";
const char* scriptURL = "https://script.google.com/macros/s/XXX/exec";
```

---

## ⚡ Power Supply Recommendation

- Use a **5V 3A regulated power supply** with proper decoupling capacitors.
- For longer sensor cables (>12m), add:
  - Shielded cables
  - Pull-up resistors for DS18B20
  - Capacitor filtering on soil sensor power lines

---

## 📋 Future Improvements

- OLED display for local status
- Mobile app and Web integration
- Full Automation with other advanced sensors and valves

---
## ✏️ Stay Tuned for Part 2

---

## 📜 License

MIT License © 2025 Damath De Silva

