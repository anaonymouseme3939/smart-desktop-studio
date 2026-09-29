#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

#include <Arduino.h>
#include <FastLED.h>
#include "Config.h"
#include "PinMap.h"

enum LedMode {
  MODE_OFF = 0,
  MODE_HYPERION = 1,
  MODE_AUDIO_VU = 2,
  MODE_DROP_STACK = 3,
  MODE_FLUID_SPECTRUM = 4,
  MODE_VELOCITY_CHASER = 5,
  MODE_BOUNCING_CLUSTER = 6,
  MODE_POLICE_STROBE = 7,
  MODE_SOLID = 8
};

class LEDController {
public:
  LEDController();
  void begin();
  void update(uint8_t vuLevel);
  void setMode(LedMode newMode);
  LedMode getMode() const;
  void toggleHyperionMode();
  void updateHyperionBuffer(const uint8_t* data, size_t len);
  void setBrightness(uint8_t brightness);
  uint8_t getBrightness() const;
  void setColor(const CRGB& color);
  void setColorHex(const String& hexValue);
  CRGB getColor() const;

private:
  CRGB leds[NUM_LEDS];
  uint8_t hyperionBuffer[NUM_LEDS * 3];
  uint8_t brightness;
  LedMode mode;
  CRGB solidColor;
  uint32_t lastHyperionPacketMs;
  uint32_t lastPoliceToggleMs;
  uint8_t currentHue;
  uint8_t dropStackPos;
  uint8_t dropStackFill;
  bool dropDirection;
  uint8_t clusterPos;
  uint8_t clusterDirection;

  void renderOff();
  void renderHyperion();
  void renderAudioVU(uint8_t vuLevel);
  void renderDropStack();
  void renderFluidSpectrum();
  void renderVelocityChaser();
  void renderBouncingCluster();
  void renderPoliceStrobe();
  void renderSolid();
};

#endif
