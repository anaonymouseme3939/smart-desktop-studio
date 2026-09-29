#ifndef AUDIO_ENGINE_H
#define AUDIO_ENGINE_H

#include <Arduino.h>
#include <Preferences.h>
#include "Config.h"
#include "PinMap.h"

class AudioEngine {
public:
  AudioEngine();
  void begin();
  void update();
  uint16_t getSensitivity() const;
  void setSensitivity(uint16_t value);
  uint8_t getClapRelayMask() const;
  void setClapRelayMask(uint8_t mask);
  uint8_t getCurrentVU() const;

private:
  Preferences preferences;
  uint16_t sensitivity;
  uint8_t clapRelayMask;
  uint8_t currentVU;
  uint32_t lastPeakTime;
};

#endif // AUDIO_ENGINE_H
