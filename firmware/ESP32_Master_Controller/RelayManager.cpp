#include "RelayManager.h"

RelayManager::RelayManager() {
  for (uint8_t i = 0; i < NUM_RELAYS; ++i) {
    relayStates[i] = false;
  }
}

void RelayManager::begin() {
  preferences.begin("smartdesk", false);

  // Initialize all relay pins
  for (uint8_t i = 0; i < NUM_RELAYS; ++i) {
    pinMode(PinMap::RELAY_PINS[i], OUTPUT);
    // Load state from NVS
    relayStates[i] = loadRelayStateFromNVS(i);
    // Apply to hardware
    applyHardwarePinState(i, relayStates[i]);
  }
}

void RelayManager::setRelayState(uint8_t index, bool state) {
  if (index >= NUM_RELAYS) {
    return;
  }

  relayStates[index] = state;
  saveRelayStateToNVS(index, state);
  applyHardwarePinState(index, state);
}

bool RelayManager::getRelayState(uint8_t index) const {
  if (index >= NUM_RELAYS) {
    return false;
  }
  return relayStates[index];
}

void RelayManager::toggleRelay(uint8_t index) {
  if (index >= NUM_RELAYS) {
    return;
  }
  setRelayState(index, !relayStates[index]);
}

void RelayManager::toggleMasterRelays() {
  bool anyOn = false;
  for (uint8_t i = 0; i < NUM_RELAYS; ++i) {
    if (relayStates[i]) {
      anyOn = true;
      break;
    }
  }
  setAllRelays(!anyOn);
}

void RelayManager::setAllRelays(bool state) {
  for (uint8_t i = 0; i < NUM_RELAYS; ++i) {
    setRelayState(i, state);
  }
}

void RelayManager::selfTestRelays() {
  // Click each relay on then off
  for (uint8_t i = 0; i < NUM_RELAYS; ++i) {
    setRelayState(i, true);
    delay(80);
    setRelayState(i, false);
    delay(40);
  }
}

void RelayManager::getAllRelayStates(bool states[NUM_RELAYS]) const {
  for (uint8_t i = 0; i < NUM_RELAYS; ++i) {
    states[i] = relayStates[i];
  }
}

void RelayManager::applyHardwarePinState(uint8_t index, bool logicalState) {
  if (index >= NUM_RELAYS) {
    return;
  }

  const uint8_t pin = PinMap::RELAY_PINS[index];
  
  // Active LOW: ON = LOW, OFF = HIGH
  if (RELAY_ACTIVE_LOW) {
    digitalWrite(pin, logicalState ? LOW : HIGH);
  } else {
    digitalWrite(pin, logicalState ? HIGH : LOW);
  }
}

void RelayManager::saveRelayStateToNVS(uint8_t index, bool state) {
  String key = "relay_" + String(index + 1);
  preferences.putUChar(key.c_str(), state ? 1 : 0);
}

bool RelayManager::loadRelayStateFromNVS(uint8_t index) const {
  String key = "relay_" + String(index + 1);
  uint8_t value = preferences.getUChar(key.c_str(), 0);
  return value != 0;
}
