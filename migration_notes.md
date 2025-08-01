# Migration Notes - Greenhouse Monitoring System Refactor

## Overview
Refactored legacy ESP32-A and ESP32-B sketches into modular, maintainable codebase.

## Key Changes

### 1. Modular Structure
- Split monolithic code into separate .h/.cpp files
- Created `lib/config.h` for centralized configuration
- Separated ESP-NOW into sender/receiver modules

### 2. New Files Created
- `main/lib/config.h` - Centralized configuration
- `main/lib/soil_sensor.h/.cpp` - Soil sensor management
- `main/lib/dht_sensor.h/.cpp` - DHT22 interface
- `main/lib/ds18b20_sensor.h/.cpp` - DS18B20 sensors
- `main/lib/bh1750_sensor.h/.cpp` - BH1750 light sensor
- `main/lib/espnow_receiver.h/.cpp` - ESP-NOW receiver
- `main/lib/espnow_sender.h/.cpp` - ESP-NOW sender
- `main/lib/data_uploader.h/.cpp` - Google Sheets upload
- `main/lib/wifi_setup.h/.cpp` - WiFi management
- `main/esp32_b_main.ino` - ESP32-B main file

### 3. Configuration System
- Board selection: `#define ESP32_A` or `#define ESP32_B`
- Centralized pin definitions and calibration constants
- MAC addresses and WiFi channel configuration

### 4. Improvements
- Better error handling and validation
- Consistent JSON field naming
- Enhanced ESP-NOW with timestamps
- Comprehensive debugging output
- Reusable sensor calibration functions

### 5. Usage
- ESP32-A: Upload `main.ino` (gateway with WiFi + Google Sheets)
- ESP32-B: Upload `esp32_b_main.ino` (remote sensor node)
- Configure board type in `lib/config.h`
- Update MAC addresses and WiFi credentials

## Migration Complete ✅ 