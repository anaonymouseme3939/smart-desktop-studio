#ifndef RELAY_MANAGER_H
#define RELAY_MANAGER_H

#include <Arduino.h>
#include <Preferences.h>
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

private:
  bool relayStates[NUM_RELAYS];
  Preferences preferences;

  void applyHardwarePinState(uint8_t index, bool logicalState);
  void saveRelayStateToNVS(uint8_t index, bool state);
  bool loadRelayStateFromNVS(uint8_t index) const;
};

#endif // RELAY_MANAGER_H
