#ifndef BLUETOOTH_SERVER_H
#define BLUETOOTH_SERVER_H

#include <Arduino.h>
#include <BluetoothSerial.h>
#include <ArduinoJson.h>
#include "Config.h"

class BluetoothServer {
public:
  BluetoothServer();
  void begin();
  void update();
  void sendTelemetry(const String& telemetryJson);
  bool isConnected() const;

private:
  BluetoothSerial serialBT;
  String inputBuffer;
  uint32_t lastBufferActivityMs;

  void processBuffer();
  void handleTerminalCommand(const String& command);
  void handleJsonCommand(const JsonDocument& doc);
};

#endif // BLUETOOTH_SERVER_H
