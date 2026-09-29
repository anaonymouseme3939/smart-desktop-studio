#ifndef RELAYMANAGER_H
#define RELAYMANAGER_H

#include <Arduino.h>
#include "Config.h"
#include "PinMap.h"

class RelayManager {
public:
  RelayManager();

  void begin();
  void setRelayState(int index, bool on);
  void setAllRelays(bool on);
  void toggleRelay(int index);
  void toggleMasterRelays();
  bool getRelayState(int index) const;
  void getAllRelayStates(bool states[NUM_RELAYS]) const;
  void selfTestRelays();

  void setOnStateChangedCallback(void (*callback)());
  void setClapRelayMask(uint8_t mask);
  uint8_t getClapRelayMask() const;
  void toggleClapRelays(uint8_t mask);

private:
  bool relayStates[NUM_RELAYS];
  uint8_t clapRelayMask;
  void (*stateChangedCallback)();
};

#endif
