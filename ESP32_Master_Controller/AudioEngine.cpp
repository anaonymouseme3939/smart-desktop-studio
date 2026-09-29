#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include <Arduino.h>
#include "Config.h"
#include "PinMap.h"

class AudioEngine {
public:
  AudioEngine();

  void begin();
  void update();
  uint8_t getVuLevel() const;
  void setSensitivity(uint16_t sensitivity);
  uint16_t getSensitivity() const;

private:
  uint16_t sensitivity;
  uint8_t vuLevel;
  uint32_t firstClapTime;
  bool firstClapDetected;
  bool clapState;
};

#endif
