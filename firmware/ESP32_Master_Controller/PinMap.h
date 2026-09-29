#ifndef PINMAP_H
#define PINMAP_H

#include <Arduino.h>

namespace PinMap {
  // Relay module pins (GPIO 18, 19, 21, 22)
  // These control the relay IN1, IN2, IN3, IN4 inputs
  static constexpr uint8_t RELAY_PINS[4] = {18, 19, 21, 22};

  // WS2812B LED strip data line
  // Note: GPIO 16 is UART2 RX but unused in this firmware
  static constexpr uint8_t LED_DATA_PIN = 16;

  // MAX9814 microphone analog input
  // GPIO 34 is ADC1_6 (input-only, no GPIO matrix conflicts)
  static constexpr uint8_t MICROPHONE_PIN = 34;

  // Onboard status LED (blue)
  // Built into DEVKIT, no external wiring needed
  static constexpr uint8_t STATUS_LED_PIN = 2;
}

#endif // PINMAP_H
