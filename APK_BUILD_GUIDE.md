# Smart Desktop Studio - APK Builder Guide

## Windows Users

### Quick Start (ONE CLICK)

1. **Download** the project
2. **Extract** the ZIP file
3. **Right-click** on `BUILD_APK.bat`
4. Select **"Run as administrator"**
5. Wait for the build to complete (5-10 minutes)
6. APK will be created at: `app/build/app/outputs/flutter-apk/app-release.apk`

### What the Script Does

✅ Checks if Flutter is installed
✅ Cleans previous builds
✅ Downloads all dependencies
✅ Builds the release APK
✅ Shows you where the APK is saved

### Requirements

- **Windows 10/11**
- **Flutter SDK** - Install from https://flutter.dev/docs/get-started/install/windows
- **Java JDK 11+** - Usually comes with Flutter
- **Android SDK** - Usually comes with Flutter
- **At least 5GB free disk space**

### If Build Fails

**"Flutter not found" error:**
- Install Flutter: https://flutter.dev/docs/get-started/install/windows
- Add Flutter to PATH (the installer does this)
- Restart your computer
- Try again

**"Administrator required" error:**
- Right-click `BUILD_APK.bat` → "Run as administrator"

**"pub get failed" error:**
- Check internet connection
- Delete `app/pubspec.lock` file
- Run again

---

## macOS/Linux Users

### Quick Start (ONE CLICK)

1. **Download** the project
2. **Extract** the ZIP file
3. **Open Terminal** in the project folder
4. **Run this command:**
   ```bash
   chmod +x BUILD_APK.sh
   ./BUILD_APK.sh
   ```
5. Wait for the build to complete (5-10 minutes)
6. APK will be created at: `app/build/app/outputs/flutter-apk/app-release.apk`

### What the Script Does

✅ Checks if Flutter is installed
✅ Cleans previous builds
✅ Downloads all dependencies
✅ Builds the release APK
✅ Shows you where the APK is saved

### Requirements

- **Flutter SDK** - Install from https://flutter.dev/docs/get-started/install/macos
- **Java JDK 11+**
- **Android SDK** - Can be installed via Flutter
- **At least 5GB free disk space**

---

## Manual Build (If Scripts Don't Work)

If the scripts fail, build manually:

```bash
# 1. Open terminal/cmd in the project folder

# 2. Navigate to app directory
cd app

# 3. Get dependencies
flutter pub get

# 4. Build APK
flutter build apk --release

# 5. APK is ready at:
# app/build/app/outputs/flutter-apk/app-release.apk
```

---

## Installing the APK on Your Phone

### For Windows/Mac/Linux:

1. **Connect** Android phone via USB
2. **Enable USB Debugging** on phone:
   - Settings → Developer Options → USB Debugging
3. **Install APK:**
   ```bash
   adb install app/build/app/outputs/flutter-apk/app-release.apk
   ```

### Or Manually:

1. **Copy** `app-release.apk` to your phone (USB cable or cloud)
2. **Open** file manager on phone
3. **Tap** the APK file
4. **Tap Install**
5. **Grant permissions** for Bluetooth and WiFi

---

## First Time Setup

1. **Open** Smart Desktop Studio app
2. **ESP32 Firmware must be uploaded first** (see firmware README)
3. **Go to Settings tab**
4. **Scan Bluetooth:**
   - Click "Scan Devices"
   - Look for "ESP32_SMART_DESK"
   - Click "Connect"
5. **Or connect via WiFi:**
   - Enter ESP32 IP: `192.168.1.100` or `192.168.4.1`
   - Click "Connect via WebSocket"
6. **Dashboard tab** - Control relays, LEDs, audio!

---

## Troubleshooting

### APK won't install
- Enable "Unknown Sources" in Android Settings → Security
- Check if your phone's Android version is 5.0+

### App crashes on startup
- Uninstall the app
- Reinstall from APK
- Check that ESP32 is powered and running firmware

### Bluetooth won't connect
- Ensure Bluetooth is enabled on phone
- ESP32 should be powered and advertising as "ESP32_SMART_DESK"
- Grant Bluetooth permission to app

### WiFi won't connect
- Check ESP32 IP address (default: 192.168.1.100 or 192.168.4.1)
- Ensure ESP32 is on the same WiFi network
- Grant location permission to app (required by Android for WiFi scan)

---

## File Locations After Build

```
smart-desktop-studio-main/
├── app/
│   ├── build/
│   │   └── app/
│   │       └── outputs/
│   │           └── flutter-apk/
│   │               └── app-release.apk  ← YOUR APK FILE
│   ├── lib/
│   ├── pubspec.yaml
│   └── ...
├── firmware/  ← Upload to ESP32 separately
├── BUILD_APK.bat  ← Windows build script
├── BUILD_APK.sh   ← macOS/Linux build script
└── README.md
```

---

## Next Steps

1. **Upload Firmware to ESP32:**
   - See `firmware/ESP32_Master_Controller/README.md`
   - Use Arduino IDE
   - Connect ESP32 via USB
   - Select DOIT ESP32 DEVKIT V1 board
   - Upload the sketch

2. **Power on ESP32** with relay module and LEDs connected

3. **Open app on Android phone**

4. **Connect and enjoy!**

---

## Support

If you encounter issues:

1. Check internet connection
2. Ensure Flutter is properly installed
3. Try deleting `app/pubspec.lock` and running the script again
4. Check Flutter version: `flutter --version`
5. Check Android SDK: `flutter doctor`

