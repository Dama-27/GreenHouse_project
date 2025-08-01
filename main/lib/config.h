#pragma once

// Board Configuration
#define ESP32_A  // Comment this line for ESP32-B
// #define ESP32_B  // Uncomment this line for ESP32-B

// WiFi Configuration
#define WIFI_CHANNEL 6

// ESP-NOW Configuration
#ifdef ESP32_A
// ESP32-A MAC address (Gateway)
const uint8_t ESP32_A_MAC[6] = { 0x68, 0x25, 0xDD, 0x33, 0x74, 0x8C };
#endif

#ifdef ESP32_B
// ESP32-B MAC address (Remote sensor node)
const uint8_t ESP32_B_MAC[6] = { 0x24, 0x6F, 0x28, 0x33, 0x74, 0x8C };
// ESP32-A MAC address (Gateway - target for ESP32-B)
const uint8_t ESP32_A_MAC[6] = { 0x68, 0x25, 0xDD, 0x33, 0x74, 0x8C };
#endif

// Sensor Pin Definitions
#ifdef ESP32_A
// ESP32-A (Gateway) sensor pins
const int SOIL_PINS[6] = {32, 33, 34, 35, 36, 39};
#define DHT_PIN 16
#define DS18B20_PIN 4
#define BH1750_SDA 21
#define BH1750_SCL 22
#endif

#ifdef ESP32_B
// ESP32-B (Remote) sensor pins
const int SOIL_PINS[3] = {32, 33, 34};
#endif

// Sensor Configuration
#define NUM_DS18B20_SENSORS 9
#define NUM_DHT_SENSORS 1
#define NUM_BH1750_SENSORS 1

#ifdef ESP32_A
#define NUM_LOCAL_SOIL_SENSORS 6
#endif

#ifdef ESP32_B
#define NUM_LOCAL_SOIL_SENSORS 3
#endif

// Calibration Constants
// Soil sensor calibration (raw ADC values)
#define SOIL_DRY_VALUE 2970    // ADC value when soil is dry
#define SOIL_WET_VALUE 1850    // ADC value when soil is wet
#define SOIL_MIN_PERCENT 0     // Minimum moisture percentage
#define SOIL_MAX_PERCENT 100   // Maximum moisture percentage

// Timing Configuration
#define DATA_UPLOAD_INTERVAL 20000  // 20 seconds
#define SENSOR_READ_INTERVAL 20000  // 20 seconds

// Data Structure for ESP-NOW communication
typedef struct SoilData {
  int moisture[3];
  uint32_t timestamp;
} SoilData;

// JSON field names for consistency
namespace JsonFields {
  // Local soil sensors
  const char* SOIL_PREFIX = "soil";
  const char* DS18B20_PREFIX = "ds18b20_";
  const char* DHT_TEMP = "dht_temp";
  const char* DHT_HUM = "dht_hum";
  const char* LIGHT = "light";
  
  // Remote soil sensors
  const char* REMOTE_SOIL_PREFIX = "remote_soil";
} 