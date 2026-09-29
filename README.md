# Smart Desktop Studio

This repository contains a full project scaffold for the Smart Desktop Manager described in the design brief.

## Included

- ESP32 firmware for the Smart Desktop Manager
  - relay control
  - LED animation engine
  - clap/audio detection
  - Bluetooth command parser
  - Wi-Fi + SoftAP + Hyperion integration
- Flutter mobile app skeleton for dashboard and settings

## Firmware layout

- `firmware/ESP32_Master_Controller/`

## App layout

- `app/`

## Quick start

### ESP32 firmware

1. Open the Arduino IDE.
2. Install the ESP32 board package.
3. Install the required libraries:
   - FastLED
   - ArduinoJson
4. Open the sketch under `firmware/ESP32_Master_Controller/ESP32_Master_Controller.ino`.
5. Select board: `DOIT ESP32 DEVKIT V1`.
6. Compile and upload.

### Flutter app

```bash
cd app
flutter pub get
flutter run
```

## Notes

The code here is intentionally structured according to the project specification and is ready for extension into a complete production implementation.
