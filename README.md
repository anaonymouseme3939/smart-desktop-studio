# Smart Desktop Studio

This repository contains the firmware and Android app for the Smart Desktop Studio project described in the project documentation.

## Contents

- `ESP32_Master_Controller/` — Arduino/ESP32 firmware for the desk controller
- `app/` — Flutter Android application for Bluetooth and Wi-Fi control

## Features

- 4 relay-controlled power outlets
- 50-LED WS2812B ambient lighting
- Hyperion-style ambient sync
- Microphone-based clap detection
- Bluetooth Classic command interface
- Wi-Fi/WebSocket control and telemetry
- NVS-persistent settings

## Quick start

### Firmware

1. Open the Arduino IDE.
2. Install the ESP32 board package.
3. Install the `FastLED` and `ArduinoJson` libraries.
4. Open `ESP32_Master_Controller/ESP32_Master_Controller.ino`.
5. Select board: `DOIT ESP32 DEVKIT V1`.
6. Flash to the ESP32.

### Flutter app

```bash
cd app
flutter pub get
flutter run
```

## Hardware summary

- ESP32 DEVKIT V1
- 4-channel 5V relay board
- 50x WS2812B LED tube
- MAX9814 or Cretile analog microphone
- Android app via Bluetooth Classic or Wi-Fi

## Notes

The project is intended as a working starter implementation based on the Smart Desktop Studio design specification. Update credentials and network parameters for your own environment before deployment.
