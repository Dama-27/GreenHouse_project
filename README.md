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

