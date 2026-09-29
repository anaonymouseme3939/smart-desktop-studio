# Smart Desktop Studio - Complete Setup Guide

## 📦 What You Have

You have TWO main parts:

1. **Firmware** (ESP32 controller - runs on the microcontroller)
2. **Mobile App** (Android - runs on your phone)

---

## 🚀 QUICK START

### Step 1: Build the Android APK (5-10 minutes)

**WINDOWS:**
1. Right-click `BUILD_APK.bat`
2. Select "Run as administrator"
3. Wait for completion

**MAC/LINUX:**
1. Open terminal
2. Run: `chmod +x BUILD_APK.sh && ./BUILD_APK.sh`

**Result:** `app/build/app/outputs/flutter-apk/app-release.apk`

### Step 2: Upload Firmware to ESP32 (10-15 minutes)

1. **Install Arduino IDE** from https://www.arduino.cc/en/software

2. **Install ESP32 Board:**
   - File → Preferences
   - Add to "Additional Boards Manager URLs":
     ```
     https://dl.espressif.com/dl/package_esp32_index.json
     ```
   - Tools → Board Manager → Search "ESP32" → Install

3. **Install Libraries:**
   - Sketch → Include Library → Manage Libraries
   - Search and install:
     - FastLED
     - ArduinoJson (v6.x)
     - BluetoothSerial (built-in)

4. **Open Firmware:**
   - Navigate to: `firmware/ESP32_Master_Controller/`
   - Open: `ESP32_Master_Controller.ino`

5. **Configure Board:**
   - Tools → Board → DOIT ESP32 DEVKIT V1
   - Tools → Upload Speed → 115200
   - Tools → Flash Size → 4MB (32Mb)
   - Tools → Partition Scheme → Default 4MB with spiffs

6. **Upload:**
   - Connect ESP32 via USB
   - Click Upload button
   - Wait for "Done uploading"

### Step 3: Wire Hardware

| Component | ESP32 Pin | Notes |
|-----------|-----------|-------|
| Relay IN1 | GPIO 18 | LED Strip Power |
| Relay IN2 | GPIO 19 | Monitors |
| Relay IN3 | GPIO 21 | Lamps |
| Relay IN4 | GPIO 22 | Fan |
| LED Data | GPIO 16 | WS2812B |
| Microphone | GPIO 34 | Audio input |
| Relay VCC | 5V | Power |
| Relay GND | GND | Ground |

### Step 4: Install Android App

1. **Copy APK to phone** via USB or cloud
2. **Tap the APK file**
3. **Grant permissions:**
   - Bluetooth
   - WiFi
   - Location (required by Android)
4. **Open app and enjoy!**

---

## 🎮 Using the App

### Dashboard Tab
- **Connection Status** - Shows if connected
- **Master Power** - Toggle all relays
- **Relay Cards** - Control each relay (1-4)
- **Audio Visualizer** - Shows microphone level
- **LED Modes** - 9 animation modes
- **Brightness** - 0-255
- **Color Picker** - RGB hex colors
- **Microphone Sensitivity** - 500-5000
- **System Health** - Uptime, IP, telemetry

### Settings Tab
- **Bluetooth Scanner** - Auto-connect to ESP32_SMART_DESK
- **WiFi Settings** - Enter ESP32 IP address
- **WiFi Configuration** - Update SSID/password on ESP32
- **Clap Relay Mask** - Choose which relays respond to claps
- **Quick Commands** - Status, Help, Reboot, Reset
- **Raw Terminal** - Send raw commands

### Terminal Commands

```
1          Toggle Relay 1
2          Toggle Relay 2
3          Toggle Relay 3
4          Toggle Relay 4
M or ALL   Toggle all relays
00         LED Off
11         LED Hyperion mode
22         LED Audio VU
33         LED Drop Stack
44         LED Rainbow
55         LED Chaser
66         LED Cluster
77         LED Police Strobe
88         LED Solid
STATUS     Get system status
HELP       List all commands
REBOOT     Restart ESP32
```

---

## 📡 Connection Methods

### Bluetooth (Easiest)
1. Settings tab → "Scan Devices"
2. Look for "ESP32_SMART_DESK"
3. Tap "Connect"
4. App shows "Connected"

### WiFi (Faster)
1. Settings tab → Enter IP: `192.168.1.100` or `192.168.4.1`
2. Tap "Connect via WebSocket"
3. App shows "Connected"

### WiFi Auto-Connect
1. App will try multiple methods in order:
   - Home Router WiFi
   - Backup WiFi
   - ESP32 SoftAP (192.168.4.1)
   - Bluetooth fallback

---

## 🎛️ LED Animation Modes

1. **OFF** - All LEDs off
2. **HYPERION** - Syncs with PC screen (Ambilight-like)
3. **VU** - Responds to microphone level (green to red)
4. **DROP** - 3-LED block falls and stacks
5. **RAINBOW** - Flowing rainbow spectrum
6. **CHASER** - Colored dots moving across strip
7. **CLUSTER** - 6-LED cluster bounces left/right
8. **POLICE** - Red and blue flashing strobe
9. **SOLID** - Single color (pick with color picker)

---

## 🔊 Audio Features

- **Microphone Input:** GPIO 34 (analog)
- **VU Display:** Real-time audio level visualization
- **Sensitivity Control:** Adjustable from 500-5000
- **Clap Detection:** Single clap toggles relays, double clap toggles Hyperion mode
- **Clap Mask:** Choose which relays respond to claps

---

## 🌐 Network Configuration

### Default WiFi Credentials (in firmware)
```
SSID 1: Trojan_Horse_Wi-Fi
Password: Tharayil@#1234

SSID 2: jiofibre_Tharayil_2.4ghz
Password: Tharayil@#1234

SoftAP: ESP32_STUDIO_AP
Password: 12345678
```

**To change WiFi credentials:**
1. App → Settings tab
2. Enter new SSID and password
3. Tap "Save & Update"
4. ESP32 will reconnect with new credentials

---

## 📊 Telemetry & Monitoring

### System Health Monitor (Dashboard tab)
- **Uptime** - How long ESP32 has been running
- **Connection Mode** - BT, WiFi, or SoftAP
- **IP Address** - Current IP on network
- **Packet Count** - Number of commands sent
- **Last Telemetry** - Most recent status JSON
- **Live Log** - Real-time RX/TX events

### Serial Monitor (Firmware Debug)
- Connect ESP32 via USB
- Arduino IDE → Tools → Serial Monitor
- Baud Rate: 115200
- Watch initialization and heartbeat messages

---

## ⚙️ Troubleshooting

### APK won't build
- ❌ Flutter not installed → Install from https://flutter.dev
- ❌ Not enough disk space → Need 5GB+
- ❌ Internet issue → Check connection, try again
- ✅ Solution: Delete `app/pubspec.lock`, run script again

### Firmware won't upload
- ❌ Wrong board selected → Tools → Board → DOIT ESP32 DEVKIT V1
- ❌ Wrong COM port → Tools → Port (check in Device Manager)
- ❌ USB cable issue → Try different cable
- ✅ Solution: Hold BOOT button while uploading

### App won't connect
- ❌ Bluetooth off → Enable in Android Settings
- ❌ Wrong IP → Use 192.168.1.100 or 192.168.4.1
- ❌ Permissions not granted → Grant Bluetooth + WiFi permissions
- ✅ Solution: Restart both app and ESP32

### LEDs won't light
- ❌ Relay 1 is OFF → Relay 1 powers the 5V strip
- ❌ GPIO 16 disconnected → Check wiring
- ❌ LED power supply → Check 5V power to strip
- ✅ Solution: Manually toggle Relay 1 ON

### Relays won't click
- ❌ Relay module not powered → Check 5V supply
- ❌ GPIO pins disconnected → Check wiring
- ❌ Active LOW logic → Ensure relay module is Active LOW
- ✅ Solution: Check Serial Monitor for relay state

---

## 📁 Project Structure

```
smart-desktop-studio-main/
├── app/
│   ├── lib/
│   │   ├── main.dart              ← App entry point
│   │   ├── services/
│   │   │   └── network_manager_service.dart   ← BT/WiFi handling
│   │   ├── screens/
│   │   │   ├── home_screen.dart
│   │   │   ├── dashboard_screen.dart
│   │   │   └── settings_screen.dart
│   │   └── widgets/
│   │       ├── connection_bar.dart
│   │       ├── relay_card.dart
│   │       ├── led_mode_selector.dart
│   │       ├── bluetooth_scanner.dart
│   │       ├── wifi_settings.dart
│   │       └── raw_terminal.dart
│   ├── build/
│   │   └── app/outputs/flutter-apk/
│   │       └── app-release.apk    ← YOUR APK
│   └── pubspec.yaml
│
├── firmware/
│   └── ESP32_Master_Controller/
│       ├── ESP32_Master_Controller.ino    ← Main sketch
│       ├── Config.h                       ← Configuration
│       ├── PinMap.h                       ← GPIO pins
│       ├── RelayManager.h/.cpp
│       ├── LEDController.h/.cpp
│       ├── AudioEngine.h/.cpp
│       ├── BluetoothServer.h/.cpp
│       ├── StudioNetworkManager.h/.cpp
│       └── README.md
│
├── BUILD_APK.bat                   ← Windows builder (RIGHT-CLICK → RUN AS ADMIN)
├── BUILD_APK.sh                    ← macOS/Linux builder
├── APK_BUILD_GUIDE.md              ← Detailed build instructions
└── README.md                       ← This file
```

---

## 🎯 Next Steps

1. ✅ **Build APK** using BUILD_APK script
2. ✅ **Upload Firmware** using Arduino IDE
3. ✅ **Wire Hardware** according to GPIO table
4. ✅ **Power On ESP32**
5. ✅ **Install APK** on Android phone
6. ✅ **Connect** via Bluetooth or WiFi
7. ✅ **Control** your desktop!

---

## 📞 Support

If something doesn't work:

1. Check **Serial Monitor** on ESP32 (115200 baud)
2. Check **app logs** in System Health Monitor
3. Verify **wiring** against GPIO table
4. Try **factory reset** in Settings → Quick Commands
5. **Reboot** both app and ESP32

---

**Enjoy your Smart Desktop Studio! 🚀**
