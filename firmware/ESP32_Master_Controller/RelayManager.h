#ifndef RELAY_MANAGER_H
#define RELAY_MANAGER_H

#include <Arduino.h>
#include <Preferences.h>
#include <functional>

#include "Config.h"
#include "PinMap.h"

class RelayManager {
public:
  RelayManager();
  void begin();
  void setRelayState(uint8_t index, bool state);
  bool getRelayState(uint8_t index) const;
  void toggleRelay(uint8_t index);
  void toggleMasterRelays();
  void setAllRelays(bool state);
  void selfTestRelays();
  void getAllRelayStates(bool states[NUM_RELAYS]) const;
  void setOnStateChangedCallback(std::function<void()> cb);

private:
  bool relayStates[NUM_RELAYS];
  Preferences preferences;
  std::function<void()> onStateChanged;

  void applyHardwarePinState(uint8_t index, bool logicalState);
  void saveRelayStateToNVS(uint8_t index, bool state);
  bool loadRelayStateFromNVS(uint8_t index) const;
};

#endif
