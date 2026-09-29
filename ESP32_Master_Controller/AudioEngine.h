#include "LEDController.h"

LEDController::LEDController()
    : brightness(DEFAULT_BRIGHTNESS), mode(MODE_OFF), solidColor(CRGB::Blue), lastPoliceToggleMs(0), lastHueShiftMs(0), hue(0), hyperionEnabled(false) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
  }
  for (int i = 0; i < NUM_LEDS * 3; i++) {
    hyperionBuffer[i] = 0;
  }
}

void LEDController::begin() {
  FastLED.addLeds<LED_TYPE, LED_DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(brightness);
  FastLED.clear(true);
}

void LEDController::setMode(LedMode newMode) {
  mode = newMode;
}

LedMode LEDController::getMode() const {
  return mode;
}

void LEDController::setBrightness(uint8_t newBrightness) {
  brightness = newBrightness;
  FastLED.setBrightness(brightness);
}

uint8_t LEDController::getBrightness() const {
  return brightness;
}

void LEDController::setSolidColor(const CRGB& color) {
  solidColor = color;
}

void LEDController::setSolidColorHex(const String& colorHex) {
  if (colorHex.length() != 7 || colorHex[0] != '#') {
    return;
  }

  char hex[7];
  colorHex.substring(1).toCharArray(hex, 7);
  long value = strtol(hex, nullptr, 16);
  solidColor = CRGB((value >> 16) & 0xFF, (value >> 8) & 0xFF, value & 0xFF);
}

void LEDController::updateHyperionBuffer(const uint8_t* buffer, size_t length) {
  if (length > NUM_LEDS * 3) {
    length = NUM_LEDS * 3;
  }

  for (size_t i = 0; i < length; i++) {
    hyperionBuffer[i] = buffer[i];
  }
  hyperionEnabled = true;
}

void LEDController::toggleHyperionMode() {
  if (mode == MODE_HYPERION) {
    mode = MODE_OFF;
  } else {
    mode = MODE_HYPERION;
  }
}

void LEDController::update(uint8_t vuLevel) {
  switch (mode) {
    case MODE_OFF:
      renderOff();
      break;
    case MODE_HYPERION:
      renderHyperion();
      break;
    case MODE_AUDIO_VU:
      renderAudioVU(vuLevel);
      break;
    case MODE_DROP_STACK:
      renderDropStack();
      break;
    case MODE_FLUID_SPECTRUM:
      renderFluidSpectrum();
      break;
    case MODE_VELOCITY_CHASER:
      renderVelocityChaser();
      break;
    case MODE_BOUNCING_CLUSTER:
      renderBouncingCluster();
      break;
    case MODE_POLICE_STROBE:
      renderPoliceStrobe();
      break;
    case MODE_SOLID:
      renderSolid();
      break;
    default:
      renderOff();
      break;
  }

  FastLED.show();
}

void LEDController::renderOff() {
  fill_solid(leds, NUM_LEDS, CRGB::Black);
}

void LEDController::renderHyperion() {
  if (!hyperionEnabled) {
    fill_solid(leds, NUM_LEDS, CRGB::Blue);
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i].nscale8(30);
    }
    return;
  }

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].r = hyperionBuffer[i * 3];
    leds[i].g = hyperionBuffer[i * 3 + 1];
    leds[i].b = hyperionBuffer[i * 3 + 2];
  }
}

void LEDController::renderAudioVU(uint8_t vuLevel) {
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  int lit = map(vuLevel, 0, 255, 0, NUM_LEDS);
  for (int i = 0; i < lit; i++) {
    uint8_t hueValue = map(i, 0, NUM_LEDS, 96, 0);
    leds[i] = CHSV(hueValue, 255, 255);
  }
}

void LEDController::renderDropStack() {
  static int dropPosition = 0;
  static int stackHeight = 0;

  fill_solid(leds, NUM_LEDS, CRGB::Black);

  if (dropPosition >= NUM_LEDS) {
    dropPosition = 0;
    stackHeight = 0;
  }

  int blockIndex = dropPosition;
  for (int i = 0; i < 3; i++) {
    if (blockIndex + i < NUM_LEDS) {
      leds[blockIndex + i] = CRGB::Green;
    }
  }

  dropPosition += 1;
  if (dropPosition >= NUM_LEDS - 2) {
    dropPosition = 0;
    stackHeight = 0;
  }
}

void LEDController::renderFluidSpectrum() {
  fill_rainbow(leds, NUM_LEDS, hue, 5);
  hue += 2;
}

void LEDController::renderVelocityChaser() {
  fadeToBlackBy(leds, NUM_LEDS, 50);
  int head = (millis() / 35) % NUM_LEDS;
  for (int i = 0; i < 3; i++) {
    int idx = (head + i) % NUM_LEDS;
    leds[idx] = CHSV((i * 30 + hue) % 255, 255, 255);
  }
  hue += 2;
}

void LEDController::renderBouncingCluster() {
  static int pos = 0;
  static int dir = 1;

  fadeToBlackBy(leds, NUM_LEDS, 70);

  for (int i = 0; i < 6; i++) {
    int idx = pos + i;
    if (idx >= 0 && idx < NUM_LEDS) {
      leds[idx] = CHSV((millis() / 25 + i * 10) % 255, 255, 255);
    }
  }

  pos += dir;
  if (pos <= 0 || pos >= NUM_LEDS - 6) {
    dir *= -1;
  }
}

void LEDController::renderPoliceStrobe() {
  uint32_t now = millis();
  if (now - lastPoliceToggleMs > 80) {
    lastPoliceToggleMs = now;
  }

  fill_solid(leds, NUM_LEDS, CRGB::Black);
  for (int i = 0; i < NUM_LEDS / 2; i++) {
    leds[i] = CRGB::Red;
  }
  for (int i = NUM_LEDS / 2; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Blue;
  }

  if ((now / 80) % 2 == 1) {
    for (int i = 0; i < NUM_LEDS / 2; i++) {
      leds[i] = CRGB::Blue;
    }
    for (int i = NUM_LEDS / 2; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Red;
    }
  }
}

void LEDController::renderSolid() {
  fill_solid(leds, NUM_LEDS, solidColor);
}
