#include "RelayManager.h"

RelayManager::RelayManager() : clapRelayMask(DEFAULT_CLAP_RELAY_MASK), stateChangedCallback(nullptr) {
  for (int i = 0; i < NUM_RELAYS; i++) {
    relayStates[i] = false;
  }
}

void RelayManager::setOnStateChangedCallback(void (*callback)()) {
  stateChangedCallback = callback;
}

void RelayManager::begin() {
  pinMode(RELAY_PIN_1, OUTPUT);
  pinMode(RELAY_PIN_2, OUTPUT);
  pinMode(RELAY_PIN_3, OUTPUT);
  pinMode(RELAY_PIN_4, OUTPUT);

  for (int i = 0; i < NUM_RELAYS; i++) {
    relayStates[i] = false;
  }

  digitalWrite(RELAY_PIN_1, HIGH);
  digitalWrite(RELAY_PIN_2, HIGH);
  digitalWrite(RELAY_PIN_3, HIGH);
  digitalWrite(RELAY_PIN_4, HIGH);
}

void RelayManager::setRelayState(int index, bool on) {
  if (index < 0 || index >= NUM_RELAYS) {
    return;
  }

  relayStates[index] = on;
  bool output = RELAY_ACTIVE_LOW ? !on : on;

  if (index == 0) digitalWrite(RELAY_PIN_1, output ? LOW : HIGH);
  if (index == 1) digitalWrite(RELAY_PIN_2, output ? LOW : HIGH);
  if (index == 2) digitalWrite(RELAY_PIN_3, output ? LOW : HIGH);
  if (index == 3) digitalWrite(RELAY_PIN_4, output ? LOW : HIGH);

  if (stateChangedCallback != nullptr) {
    stateChangedCallback();
  }
}

void RelayManager::setAllRelays(bool on) {
  for (int i = 0; i < NUM_RELAYS; i++) {
    setRelayState(i, on);
  }
}

void RelayManager::toggleRelay(int index) {
  if (index < 0 || index >= NUM_RELAYS) {
    return;
  }

  setRelayState(index, !relayStates[index]);
}

void RelayManager::toggleMasterRelays() {
  bool anyOn = false;
  for (int i = 0; i < NUM_RELAYS; i++) {
    if (relayStates[i]) {
      anyOn = true;
      break;
    }
  }

  setAllRelays(!anyOn);
}

bool RelayManager::getRelayState(int index) const {
  if (index < 0 || index >= NUM_RELAYS) {
    return false;
  }
  return relayStates[index];
}

void RelayManager::getAllRelayStates(bool states[NUM_RELAYS]) const {
  for (int i = 0; i < NUM_RELAYS; i++) {
    states[i] = relayStates[i];
  }
}

void RelayManager::selfTestRelays() {
  for (int i = 0; i < NUM_RELAYS; i++) {
    setRelayState(i, true);
    delay(80);
    setRelayState(i, false);
  }
}

void RelayManager::setClapRelayMask(uint8_t mask) {
  clapRelayMask = mask & 0x0F;
}

uint8_t RelayManager::getClapRelayMask() const {
  return clapRelayMask;
}

void RelayManager::toggleClapRelays(uint8_t mask) {
  for (int i = 0; i < NUM_RELAYS; i++) {
    if ((mask >> i) & 0x01) {
      toggleRelay(i);
    }
  }
}
