#include "BluetoothServer.h"

BluetoothServer::BluetoothServer(RelayManager& relayManager, LEDController& ledController)
    : relayManagerRef(relayManager), ledControllerRef(ledController) {}

void BluetoothServer::begin() {
  serialBT.begin(DEVICE_NAME);
}

void BluetoothServer::update() {
  while (serialBT.available()) {
    char ch = (char)serialBT.read();
    if (ch == '\n' || ch == '\r') {
      if (inputBuffer.length() > 0) {
        processCommand(inputBuffer);
        inputBuffer = "";
      }
      continue;
    }

    inputBuffer += ch;

    if (inputBuffer.length() == 1 && (inputBuffer[0] == '1' || inputBuffer[0] == '2' || inputBuffer[0] == '3' || inputBuffer[0] == '4' || inputBuffer[0] == 'M' || inputBuffer[0] == 'm' || inputBuffer[0] == 'A')) {
      processCommand(inputBuffer);
      inputBuffer = "";
    }
  }
}

void BluetoothServer::processCommand(const String& command) {
  String trimmed = command;
  trimmed.trim();
  if (trimmed.length() == 0) {
    return;
  }

  if (trimmed.startsWith("{") || trimmed.startsWith("[")) {
    processJsonCommand(trimmed);
    return;
  }

  if (trimmed == "STATUS") {
    serialBT.println("STATUS_OK");
    return;
  }

  if (trimmed == "HELP") {
    serialBT.println("1,2,3,4,M,00,11,22,33,44,55,66,77,88,STATUS,HELP,REBOOT");
    return;
  }

  if (trimmed == "REBOOT") {
    ESP.restart();
    return;
  }

  if (trimmed == "1" || trimmed == "2" || trimmed == "3" || trimmed == "4") {
    handleSingleRelay(trimmed.toInt());
    return;
  }

  if (trimmed == "M" || trimmed == "m" || trimmed == "ALL") {
    relayManagerRef.toggleMasterRelays();
    return;
  }

  handleModeCommand(trimmed);
}

void BluetoothServer::processJsonCommand(const String& json) {
  StaticJsonDocument<256> doc;
  DeserializationError error = deserializeJson(doc, json);
  if (error) {
    return;
  }

  if (doc.containsKey("relay") && doc.containsKey("state")) {
    int relay = doc["relay"].as<int>();
    bool state = doc["state"].as<bool>();
    if (relay >= 1 && relay <= 4) {
      relayManagerRef.setRelayState(relay - 1, state);
    }
  }

  if (doc.containsKey("master_relays")) {
    bool state = doc["master_relays"].as<bool>();
    relayManagerRef.setAllRelays(state);
  }

  if (doc.containsKey("mode")) {
    if (doc["mode"].is<int>()) {
      int mode = doc["mode"].as<int>();
      if (mode >= MODE_OFF && mode <= MODE_SOLID) {
        ledControllerRef.setMode((LedMode)mode);
      }
    } else if (doc["mode"].is<const char*>()) {
      String modeName = doc["mode"].as<const char*>();
      modeName.toUpperCase();
      if (modeName == "HYPERION") ledControllerRef.setMode(MODE_HYPERION);
      if (modeName == "VU") ledControllerRef.setMode(MODE_AUDIO_VU);
      if (modeName == "DROP") ledControllerRef.setMode(MODE_DROP_STACK);
      if (modeName == "RAINBOW") ledControllerRef.setMode(MODE_FLUID_SPECTRUM);
      if (modeName == "CHASER") ledControllerRef.setMode(MODE_VELOCITY_CHASER);
      if (modeName == "BOUNCE") ledControllerRef.setMode(MODE_BOUNCING_CLUSTER);
      if (modeName == "POLICE") ledControllerRef.setMode(MODE_POLICE_STROBE);
      if (modeName == "SOLID") ledControllerRef.setMode(MODE_SOLID);
    }
  }

  if (doc.containsKey("brightness")) {
    uint8_t brightness = doc["brightness"].as<int>();
    ledControllerRef.setBrightness(brightness);
  }

  if (doc.containsKey("color")) {
    String color = doc["color"].as<const char*>();
    ledControllerRef.setSolidColorHex(color);
  }

  if (doc.containsKey("sensitivity")) {
    uint16_t sensitivity = doc["sensitivity"].as<int>();
  }
}

void BluetoothServer::handleSingleRelay(int relayIndex) {
  if (relayIndex >= 1 && relayIndex <= 4) {
    relayManagerRef.toggleRelay(relayIndex - 1);
  }
}

void BluetoothServer::handleModeCommand(const String& command) {
  String cmd = command;
  cmd.toUpperCase();

  if (cmd == "00" || cmd == "OFF") {
    ledControllerRef.setMode(MODE_OFF);
  } else if (cmd == "11" || cmd == "HYP") {
    ledControllerRef.setMode(MODE_HYPERION);
  } else if (cmd == "22" || cmd == "VU") {
    ledControllerRef.setMode(MODE_AUDIO_VU);
  } else if (cmd == "33" || cmd == "DROP") {
    ledControllerRef.setMode(MODE_DROP_STACK);
  } else if (cmd == "44" || cmd == "RAIN") {
    ledControllerRef.setMode(MODE_FLUID_SPECTRUM);
  } else if (cmd == "55" || cmd == "CHAS") {
    ledControllerRef.setMode(MODE_VELOCITY_CHASER);
  } else if (cmd == "66" || cmd == "BOUN") {
    ledControllerRef.setMode(MODE_BOUNCING_CLUSTER);
  } else if (cmd == "77" || cmd == "POL") {
    ledControllerRef.setMode(MODE_POLICE_STROBE);
  } else if (cmd == "88" || cmd == "SOL") {
    ledControllerRef.setMode(MODE_SOLID);
  }
}

void BluetoothServer::sendTelemetry(String uptime, bool relayStates[NUM_RELAYS], uint8_t vuLevel, uint8_t modeIdx, uint8_t brightness, String colorHex, uint16_t sensitivity, uint8_t clapMask, String ip, String ssid) {
  StaticJsonDocument<256> doc;
  doc["uptime"] = uptime.toInt();
  JsonArray relays = doc.createNestedArray("relays");
  for (int i = 0; i < NUM_RELAYS; i++) {
    relays.add(relayStates[i] ? 1 : 0);
  }
  doc["vu"] = vuLevel;
  doc["mode_idx"] = modeIdx;
  doc["brightness"] = brightness;
  doc["color"] = colorHex;
  doc["sensitivity"] = sensitivity;
  doc["clap_mask"] = clapMask;
  doc["ip"] = ip;
  doc["ssid"] = ssid;

  String out;
  serializeJson(doc, out);
  serialBT.println(out);
}
