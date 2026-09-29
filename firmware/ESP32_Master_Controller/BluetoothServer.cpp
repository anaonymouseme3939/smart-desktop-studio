#include "BluetoothServer.h"

BluetoothServer::BluetoothServer() : lastBufferActivityMs(0) {}

void BluetoothServer::begin() {
  serialBT.begin(DEVICE_NAME);
  Serial.println("[BT] Bluetooth server started as: " + String(DEVICE_NAME));
}

void BluetoothServer::update() {
  while (serialBT.available()) {
    char ch = serialBT.read();
    inputBuffer += ch;
    lastBufferActivityMs = millis();
    if (ch == '\n' || ch == '\r') {
      processBuffer();
      inputBuffer.clear();
      lastBufferActivityMs = 0;
      continue;
    }
    if (inputBuffer.length() == 1) {
      char c = inputBuffer[0];
      if (c == '1' || c == '2' || c == '3' || c == '4' || c == 'M' || c == 'm') {
        processBuffer();
        inputBuffer.clear();
        lastBufferActivityMs = 0;
      }
    }
  }
  if (!inputBuffer.isEmpty() && (millis() - lastBufferActivityMs) > BT_FLUSH_TIMEOUT_MS) {
    processBuffer();
    inputBuffer.clear();
    lastBufferActivityMs = 0;
  }
}

void BluetoothServer::sendTelemetry(const String& telemetryJson) {
  if (serialBT.connected()) {
    serialBT.println(telemetryJson);
  }
}

bool BluetoothServer::isConnected() const {
  return serialBT.connected();
}

void BluetoothServer::processBuffer() {
  String trimmed = inputBuffer;
  trimmed.trim();
  if (trimmed.length() == 0) return;
  if (trimmed.startsWith("{") && trimmed.indexOf("}") >= 0) {
    StaticJsonDocument<256> doc;
    DeserializationError error = deserializeJson(doc, trimmed);
    if (!error) {
      handleJsonCommand(doc);
    } else {
      Serial.println("[BT] JSON Parse error");
    }
    return;
  }
  handleTerminalCommand(trimmed);
}

void BluetoothServer::handleTerminalCommand(const String& command) {
  String cmd = command;
  cmd.toUpperCase();
  Serial.println("[BT] Command: " + cmd);
  if (cmd == "1") {
    serialBT.println("RELAY 1 TOGGLED");
  } else if (cmd == "2") {
    serialBT.println("RELAY 2 TOGGLED");
  } else if (cmd == "3") {
    serialBT.println("RELAY 3 TOGGLED");
  } else if (cmd == "4") {
    serialBT.println("RELAY 4 TOGGLED");
  } else if (cmd == "M" || cmd == "ALL") {
    serialBT.println("ALL RELAYS TOGGLED");
  } else if (cmd == "STATUS") {
    serialBT.println("{\"status\":\"OK\"}");
  } else if (cmd == "HELP") {
    serialBT.println("Commands: 1,2,3,4,M,00-88,STATUS,HELP,REBOOT");
  } else if (cmd == "REBOOT") {
    serialBT.println("Rebooting...");
    delay(500);
    ESP.restart();
  } else {
    serialBT.println("Unknown command");
  }
}

void BluetoothServer::handleJsonCommand(const JsonDocument& doc) {
  Serial.println("[BT] JSON received");
}
