@echo off
REM Smart Desktop Studio - ONE-CLICK APK BUILDER
REM This script builds the Flutter APK automatically
REM Right-click and select "Run as administrator"

color 0A
echo.
echo ========================================
echo   SMART DESKTOP STUDIO APK BUILDER
echo   Version 1.0.0
echo ========================================
echo.

REM Check if running as admin
openfiles >nul 2>&1
if errorlevel 1 (
    echo ERROR: This script must run as Administrator!
    echo.
    echo Solution: Right-click this file and select "Run as administrator"
    pause
    exit /b 1
)

echo [+] Running with Administrator privileges
echo.

REM Check if Flutter is installed
echo [*] Checking Flutter installation...
where flutter >nul 2>&1
if errorlevel 1 (
    echo ERROR: Flutter not found!
    echo.
    echo Please install Flutter from: https://flutter.dev/docs/get-started/install/windows
    echo Make sure to add Flutter to your system PATH
    pause
    exit /b 1
)
echo [+] Flutter found!

REM Change to app directory
echo.
echo [*] Navigating to app directory...
cd /d "%~dp0app" || exit /b 1
echo [+] Current directory: %cd%

REM Clean previous builds
echo.
echo [*] Cleaning previous builds...
flutter clean

REM Get dependencies
echo.
echo [*] Getting Flutter dependencies...
flutter pub get
if errorlevel 1 (
    echo ERROR: Failed to get dependencies!
    pause
    exit /b 1
)
echo [+] Dependencies installed!

REM Build APK
echo.
echo [*] Building APK (this may take 5-10 minutes)...
echo.
flutter build apk --release
if errorlevel 1 (
    echo ERROR: APK build failed!
    pause
    exit /b 1
)

echo.
echo ========================================
echo   BUILD SUCCESSFUL!
echo ========================================
echo.
echo [+] APK Location:
echo     %cd%\build\app\outputs\flutter-apk\app-release.apk
echo.
echo [+] File size:
for %%A in ("build\app\outputs\flutter-apk\app-release.apk") do echo     %%~zA bytes
echo.
echo [*] Installation Instructions:
echo     1. Copy app-release.apk to your Android phone
echo     2. Open file manager and tap the APK
echo     3. Install the app
echo     4. Grant Bluetooth and WiFi permissions
echo.
echo [*] First Launch:
echo     1. Open Smart Desktop Studio app
echo     2. Go to Settings tab
echo     3. Scan for Bluetooth devices (ESP32_SMART_DESK)
    echo     4. Or enter WiFi IP: 192.168.1.100 or 192.168.4.1
echo     5. Connect and control!
echo.
pause
