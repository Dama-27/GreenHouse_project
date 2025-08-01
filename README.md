# 🌱 Greenhouse Monitoring System

A modular ESP32-based greenhouse monitoring system with two boards:
- **ESP32-A (Gateway)**: Reads local sensors and uploads data to Google Sheets
- **ESP32-B (Remote)**: Reads soil sensors and sends data to ESP32-A via ESP-NOW

## 📁 Project Structure

```
main/
├── main.ino                    # ESP32-A Gateway (WiFi + Google Sheets)
├── esp32_b_main.ino           # ESP32-B Remote (ESP-NOW sender)
├── secrets.h                   # WiFi credentials and Google Script URL
├── lib/
│   ├── config.h               # Centralized configuration
│   ├── wifi_setup.h/.cpp      # WiFi connection management
│   ├── soil_sensor.h/.cpp     # Soil moisture sensors
│   ├── dht_sensor.h/.cpp      # DHT22 temperature/humidity
│   ├── ds18b20_sensor.h/.cpp  # DS18B20 temperature sensors
│   ├── bh1750_sensor.h/.cpp   # BH1750 light sensor
│   ├── espnow_receiver.h/.cpp # ESP-NOW receiver (ESP32-A)
│   ├── espnow_sender.h/.cpp   # ESP-NOW sender (ESP32-B)
│   └── data_uploader.h/.cpp   # Google Sheets upload
```

## 🔧 Hardware Setup

### ESP32-A (Gateway)
- **Soil Sensors**: 6x analog soil moisture sensors (pins 32, 33, 34, 35, 36, 39)
- **Temperature**: 9x DS18B20 sensors (pin 4)
- **Environment**: DHT22 (pin 16)
- **Light**: BH1750 (pins 21, 22)
- **WiFi**: Connected to router for Google Sheets upload

### ESP32-B (Remote)
- **Soil Sensors**: 3x analog soil moisture sensors (pins 32, 33, 34)
- **Communication**: ESP-NOW to ESP32-A

## ⚙️ Configuration

### 1. Board Selection
Edit `lib/config.h` to select which board you're programming:

```cpp
#define ESP32_A  // For gateway board
// #define ESP32_B  // For remote board
```

### 2. MAC Addresses
Update MAC addresses in `lib/config.h`:

```cpp
// ESP32-A MAC address (Gateway)
const uint8_t ESP32_A_MAC[6] = { 0x68, 0x25, 0xDD, 0x33, 0x74, 0x8C };

// ESP32-B MAC address (Remote)
const uint8_t ESP32_B_MAC[6] = { 0x24, 0x6F, 0x28, 0x33, 0x74, 0x8C };
```

### 3. WiFi Configuration
Edit `secrets.h` with your credentials:

```cpp
const char* ssid = "Your_WiFi_SSID";
const char* password = "Your_WiFi_Password";
const char* scriptURL = "Your_Google_Script_URL";
```

### 4. Sensor Calibration
Adjust soil sensor calibration in `lib/config.h`:

```cpp
#define SOIL_DRY_VALUE 2970    // ADC value when soil is dry
#define SOIL_WET_VALUE 1850    // ADC value when soil is wet
```

## 🚀 Usage

### ESP32-A (Gateway)
1. Set `#define ESP32_A` in `lib/config.h`
2. Upload `main.ino` to ESP32-A
3. Board will:
   - Connect to WiFi
   - Initialize all sensors
   - Listen for ESP-NOW data from ESP32-B
   - Upload combined data to Google Sheets every 20 seconds

### ESP32-B (Remote)
1. Set `#define ESP32_B` in `lib/config.h`
2. Upload `esp32_b_main.ino` to ESP32-B
3. Board will:
   - Initialize soil sensors
   - Send data to ESP32-A via ESP-NOW every 20 seconds

## 📊 Data Format

The system uploads JSON data to Google Sheets with these fields:

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
  "light": 1250.5,
  "remote_soil1": 78,
  "remote_soil2": 45,
  "remote_soil3": 92
}
```

## 🔍 Troubleshooting

### ESP-NOW Issues
- Ensure both boards use the same WiFi channel (default: 6)
- Verify MAC addresses are correct
- Check that ESP32-B is sending to ESP32-A's MAC address

### Sensor Issues
- Check wiring connections
- Verify sensor power supply
- Calibrate soil sensors for your specific soil type

### WiFi Issues
- Verify SSID and password in `secrets.h`
- Check Google Script URL is correct
- Ensure ESP32-A has good WiFi signal

## 📝 Dependencies

Required libraries:
- `WiFi.h` (built-in)
- `HTTPClient.h` (built-in)
- `esp_now.h` (built-in)
- `DHT.h` (Adafruit DHT library)
- `OneWire.h` (OneWire library)
- `DallasTemperature.h` (DallasTemperature library)
- `BH1750.h` (BH1750 library)

## 🔄 Migration Notes

This refactored version includes:
- ✅ Modular code structure with separate .h/.cpp files
- ✅ Centralized configuration in `config.h`
- ✅ Improved error handling and validation
- ✅ Consistent JSON field naming
- ✅ Better ESP-NOW communication with timestamps
- ✅ Comprehensive logging and debugging output
- ✅ Reusable sensor calibration functions
- ✅ Board-specific pin definitions

## 📄 License

This project is licensed under the MIT License - see the LICENSE file for details.

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

Apache License 2.0 © 2025 Damath De Silva

