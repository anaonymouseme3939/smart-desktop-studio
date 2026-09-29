#!/bin/bash
# Smart Desktop Studio - ONE-CLICK APK BUILDER (macOS/Linux)
# Run with: chmod +x BUILD_APK.sh && ./BUILD_APK.sh

echo ""
echo "========================================"
echo "  SMART DESKTOP STUDIO APK BUILDER"
echo "  Version 1.0.0"
echo "========================================"
echo ""

# Check if Flutter is installed
echo "[*] Checking Flutter installation..."
if ! command -v flutter &> /dev/null; then
    echo "ERROR: Flutter not found!"
    echo ""
    echo "Please install Flutter from: https://flutter.dev/docs/get-started/install"
    echo "Make sure to add Flutter to your PATH"
    exit 1
fi
echo "[+] Flutter found!"

# Navigate to app directory
echo ""
echo "[*] Navigating to app directory..."
cd "$(dirname "$0")/app" || exit 1
echo "[+] Current directory: $(pwd)"

# Clean previous builds
echo ""
echo "[*] Cleaning previous builds..."
flutter clean

# Get dependencies
echo ""
echo "[*] Getting Flutter dependencies..."
flutter pub get
if [ $? -ne 0 ]; then
    echo "ERROR: Failed to get dependencies!"
    exit 1
fi
echo "[+] Dependencies installed!"

# Build APK
echo ""
echo "[*] Building APK (this may take 5-10 minutes)..."
echo ""
flutter build apk --release
if [ $? -ne 0 ]; then
    echo "ERROR: APK build failed!"
    exit 1
fi

echo ""
echo "========================================"
echo "  BUILD SUCCESSFUL!"
echo "========================================"
echo ""
echo "[+] APK Location:"
echo "    $(pwd)/build/app/outputs/flutter-apk/app-release.apk"
echo ""
echo "[+] File size:"
ls -lh build/app/outputs/flutter-apk/app-release.apk | awk '{print "    " $5}'
echo ""
echo "[*] Installation Instructions:"
echo "    1. Copy app-release.apk to your Android phone"
echo "    2. Open file manager and tap the APK"
echo "    3. Install the app"
echo "    4. Grant Bluetooth and WiFi permissions"
echo ""
echo "[*] First Launch:"
echo "    1. Open Smart Desktop Studio app"
echo "    2. Go to Settings tab"
echo "    3. Scan for Bluetooth devices (ESP32_SMART_DESK)"
echo "    4. Or enter WiFi IP: 192.168.1.100 or 192.168.4.1"
echo "    5. Connect and control!"
echo ""
