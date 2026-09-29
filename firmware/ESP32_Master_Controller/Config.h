#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ========== DEVICE IDENTIFICATION ==========
#define DEVICE_NAME          "ESP32_SMART_DESK"
#define FIRMWARE_VERSION     "1.0.0"
#define SERIAL_BAUD_RATE     115200

// ========== WiFi CREDENTIALS ==========
#define DEFAULT_WIFI1_SSID   "Trojan_Horse_Wi-Fi"
#define DEFAULT_WIFI1_PASS   "Tharayil@#1234"
#define DEFAULT_WIFI2_SSID   "jiofibre_Tharayil_2.4ghz"
#define DEFAULT_WIFI2_PASS   "Tharayil@#1234"
#define DEFAULT_SOFTAP_SSID  "ESP32_STUDIO_AP"
#define DEFAULT_SOFTAP_PASS  "12345678"

// ========== NETWORK PORTS ==========
#define WEBSOCKET_PORT       81
#define HYPERION_UDP_PORT    19446

// ========== FreeRTOS CORES & STACKS ==========
#define CORE_SYSTEM          0       // WiFi, Bluetooth, JSON parsing
#define CORE_REALTIME        1       // Audio, LED rendering
#define TASK_STACK_SYSTEM    12288   // 12KB
#define TASK_STACK_REALTIME  8192    // 8KB

// ========== LED CONFIGURATION ==========
#define NUM_LEDS             50
#define LED_TYPE             WS2812B
#define COLOR_ORDER          GRB
#define DEFAULT_BRIGHTNESS   255     // 0-255
#define HYPERION_IDLE_TIMEOUT_MS 2000

// ========== RELAY CONFIGURATION ==========
#define NUM_RELAYS           4
#define RELAY_ACTIVE_LOW     true   // LOW = ON, HIGH = OFF
#define POST_RELAY_TEST_MS   1500

// ========== AUDIO ENGINE ==========
#define AUDIO_SAMPLES_BUFFER 256
#define DEFAULT_SENSITIVITY  2200    // 0-4095 ADC range
#define CLAP_WINDOW_MIN_MS   180
#define CLAP_WINDOW_MAX_MS   850
#define CLAP_ECHO_LOCKOUT_MS 140
#define DEFAULT_CLAP_RELAY_MASK 0x0F  // All 4 relays

// ========== TIMING & TELEMETRY ==========
#define TELEMETRY_INTERVAL_MS 100   // 10 Hz
#define WIFI_TIMEOUT_MS       10000
#define BT_FLUSH_TIMEOUT_MS   150

#endif // CONFIG_H
