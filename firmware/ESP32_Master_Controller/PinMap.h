#ifndef PINMAP_H
#define PINMAP_H

#include <Arduino.h>

namespace PinMap {
  static constexpr uint8_t RELAY_PINS[4] = {18, 19, 21, 22};
  static constexpr uint8_t LED_DATA_PIN = 16;
  static constexpr uint8_t MICROPHONE_PIN = 34;
  static constexpr uint8_t STATUS_LED_PIN = 2;
}

#endif
