#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

#define DEVICE_NAME          "ESP32_SMART_DESK"
#define FIRMWARE_VERSION     "1.0.0"
#define SERIAL_BAUD_RATE     115200

#define DEFAULT_WIFI1_SSID   "Trojan_Horse_Wi-Fi"
#define DEFAULT_WIFI1_PASS   "Tharayil@#1234"
#define DEFAULT_WIFI2_SSID   "jiofibre_Tharayil_2.4ghz"
#define DEFAULT_WIFI2_PASS   "Tharayil@#1234"
#define DEFAULT_SOFTAP_SSID  "ESP32_STUDIO_AP"
#define DEFAULT_SOFTAP_PASS  "12345678"

#define WEBSOCKET_PORT       81
#define HYPERION_UDP_PORT    19446
#define CORE_SYSTEM          0
#define CORE_REALTIME        1

#define TASK_STACK_SYSTEM    12288
#define TASK_STACK_REALTIME  8192

#define NUM_LEDS             50
#define LED_TYPE             WS2812B
#define COLOR_ORDER          GRB
#define DEFAULT_BRIGHTNESS   255

#define NUM_RELAYS           4
#define RELAY_ACTIVE_LOW     true
#define POST_RELAY_TEST_MS   1500

#define AUDIO_SAMPLES_BUFFER 256
#define DEFAULT_SENSITIVITY  2200
#define CLAP_WINDOW_MIN_MS   180
#define CLAP_WINDOW_MAX_MS   850
#define CLAP_ECHO_LOCKOUT_MS 140
#define DEFAULT_CLAP_RELAY_MASK 0x0F
#define TELEMETRY_INTERVAL_MS 100
#define HYPERION_IDLE_TIMEOUT_MS 2000

#define WIFI_TIMEOUT_MS      10000
#define BT_FLUSH_TIMEOUT_MS  150

#endif
