# Smart Desktop Studio - Complete Firmware

## Arduino IDE Setup

1. **Install ESP32 Board**
   - Open Arduino IDE → File → Preferences
   - Add to "Additional Boards Manager URLs":
     ```
     https://dl.espressif.com/dl/package_esp32_index.json
     ```
   - Tools → Board → Boards Manager → Search "esp32" → Install by Espressif

2. **Install Libraries**
   - Sketch → Include Library → Manage Libraries
   - Search and install:
     - `FastLED` by Daniel Garcia
     - `ArduinoJson` by Benoit Blanchon (v6.x)
     - `WebSockets` by Markus Sattler (optional, for advanced WebSocket)

3. **Board Settings**
   - Tools → Board → ESP32 Arduino → DOIT ESP32 DEVKIT V1
   - Tools → Upload Speed → 115200
   - Tools → Flash Size → 4MB (32Mb)
   - Tools → Partition Scheme → Default 4MB with spiffs
   - Tools → CPU Frequency → 240MHz
   - Tools → Core Debug Level → None

4. **Upload**
   - Connect ESP32 via USB
   - Press Upload button in Arduino IDE

## File Structure

When you unzip the firmware folder:

```
firmware/
└── ESP32_Master_Controller/
    ├── ESP32_Master_Controller.ino       (Main sketch - OPEN THIS)
    ├── Config.h                          (Configuration)
    ├── PinMap.h                          (GPIO Pin Definitions)
    ├── RelayManager.h                    (Relay header)
    ├── RelayManager.cpp                  (Relay implementation)
    ├── LEDController.h                   (LED header)
    ├── LEDController.cpp                 (LED implementation)
    ├── AudioEngine.h                     (Audio header)
    ├── AudioEngine.cpp                   (Audio implementation)
    ├── BluetoothServer.h                 (BT header)
    ├── BluetoothServer.cpp               (BT implementation)
    ├── StudioNetworkManager.h            (Network header)
    ├── StudioNetworkManager.cpp          (Network implementation)
    └── README.md                         (This file)
```

## How to Compile & Upload

1. Extract the entire `firmware` folder
2. Navigate to `firmware/ESP32_Master_Controller/`
3. Double-click `ESP32_Master_Controller.ino` - it will open in Arduino IDE
4. Verify all `.h` and `.cpp` files are in the same folder
5. Click **Sketch → Verify/Compile** to check for errors
6. Click **Sketch → Upload** to flash to ESP32

## Wiring Reference

| Component | ESP32 Pin | Notes |
|-----------|-----------|-------|
| Relay IN1 | GPIO 18 | Pixel Tube 5V Power |
| Relay IN2 | GPIO 19 | Monitors/Speakers |
| Relay IN3 | GPIO 21 | Ambient Lamps |
| Relay IN4 | GPIO 22 | Fan/Subwoofer |
| LED Data | GPIO 16 | WS2812B DIN |
| Microphone | GPIO 34 | Audio IN |
| Status LED | GPIO 2 | Onboard blue LED |
| Relay VCC | 5V | Power relay module |
| Relay GND | GND | Common ground |

## Serial Monitor

After upload:
- Tools → Serial Monitor
- Baud Rate: **115200**
- Watch for startup messages confirming relay test, LED init, BT advertise

## Initial Telemetry

You should see:
```
[Setup] Relays initialized
[Setup] LEDs initialized
[Setup] Audio engine started
[Setup] Bluetooth advertised as ESP32_SMART_DESK
[Setup] WiFi attempting connection...
```

## Troubleshooting

**Upload fails:**
- Check USB cable
- Try 115200 upload speed
- Hold BOOT button if needed

**"Sketch too large":**
- Ensure Partition Scheme is set to "Default 4MB with spiffs"

**Serial Monitor shows garbage:**
- Change baud rate to 115200
- Check USB cable connection

**LEDs not lighting:**
- Verify Relay 1 is ON (powers the 5V strip)
- Check GPIO 16 connectivity

**Relays not clicking:**
- Confirm relay module is Active LOW
- Check relay power supply (5V)

## Communicating with the Device

### Bluetooth Terminal (Android)

1. Pair with "ESP32_SMART_DESK" in Android Bluetooth settings
2. Use any Bluetooth Serial Terminal app
3. Send commands:
   ```
   1       Toggle relay 1
   2       Toggle relay 2
   3       Toggle relay 3
   4       Toggle relay 4
   M       Toggle all relays
   00      LED Off
   11      LED Hyperion mode
   22      LED Audio VU mode
   33      LED Drop mode
   44      LED Rainbow mode
   55      LED Chaser mode
   66      LED Cluster mode
   77      LED Police strobe
   88      LED Solid mode
   STATUS  Get system status
   HELP    List all commands
   REBOOT  Restart ESP32
   ```

### WiFi WebSocket (Flutter App or any WS client)

Connect to: `ws://192.168.1.100:81` (or `192.168.4.1:81` for SoftAP)

Send JSON:
```json
{"relay": 1, "state": 1}
{"mode": "HYPERION"}
{"brightness": 200}
{"color": "#FF0055"}
{"sensitivity": 2500}
{"cmd": "set_clap_relays", "mask": 5}
{"cmd": "reboot"}
```

## Version Info

- **Firmware Version:** 1.0.0
- **Board:** ESP32 DEVKIT V1 (DOIT, 30-pin)
- **Flash:** 4MB
- **Core 0:** System tasks (WiFi, Bluetooth, JSON parsing)
- **Core 1:** Real-time tasks (Audio, LED rendering)
