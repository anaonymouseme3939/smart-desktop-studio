#ifndef BLUETOOTHSERVER_H
#define BLUETOOTHSERVER_H

#include <Arduino.h>
#include <BluetoothSerial.h>
#include <ArduinoJson.h>

#include "Config.h"
#include "RelayManager.h"
#include "LEDController.h"

class BluetoothServer {
public:
  BluetoothServer(RelayManager& relayManager, LEDController& ledController);

  void begin();
  void update();
  void sendTelemetry(String uptime, bool relayStates[NUM_RELAYS], uint8_t vuLevel, uint8_t modeIdx, uint8_t brightness, String colorHex, uint16_t sensitivity, uint8_t clapMask, String ip, String ssid);

private:
  BluetoothSerial serialBT;
  RelayManager& relayManagerRef;
  LEDController& ledControllerRef;
  String inputBuffer;

  void processCommand(const String& command);
  void processJsonCommand(const String& json);
  void handleSingleRelay(int relayIndex);
  void handleModeCommand(const String& command);
};

#endif
