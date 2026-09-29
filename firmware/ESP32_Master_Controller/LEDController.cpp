#include "LEDController.h"

LEDController::LEDController()
  : brightness(DEFAULT_BRIGHTNESS),
    mode(MODE_OFF),
    solidColor(CRGB::Cyan),
    lastHyperionPacketMs(0),
    lastPoliceToggleMs(0),
    currentHue(0),
    dropStackPos(0),
    dropStackFill(0),
    dropDirection(true),
    clusterPos(0),
    clusterDirection(1) {
  // Initialize LED array
  for (size_t i = 0; i < NUM_LEDS; ++i) {
    leds[i] = CRGB::Black;
  }
  // Initialize Hyperion buffer
  for (size_t i = 0; i < NUM_LEDS * 3; ++i) {
    hyperionBuffer[i] = 0;
  }
}

void LEDController::begin() {
  FastLED.addLeds<LED_TYPE, PinMap::LED_DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(brightness);
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
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
  }

  FastLED.show();
}

void LEDController::setMode(LedMode newMode) {
  mode = newMode;
  currentHue = 0;
  dropStackPos = 0;
  dropStackFill = 0;
  clusterPos = 0;
}

LedMode LEDController::getMode() const {
  return mode;
}

void LEDController::toggleHyperionMode() {
  if (mode == MODE_HYPERION) {
    mode = MODE_OFF;
  } else {
    mode = MODE_HYPERION;
  }
}

void LEDController::updateHyperionBuffer(const uint8_t* data, size_t len) {
  if (len > NUM_LEDS * 3) {
    len = NUM_LEDS * 3;
  }
  for (size_t i = 0; i < len; ++i) {
    hyperionBuffer[i] = data[i];
  }
  lastHyperionPacketMs = millis();
}

void LEDController::setBrightness(uint8_t newBrightness) {
  brightness = constrain(newBrightness, 0, 255);
  FastLED.setBrightness(brightness);
}

uint8_t LEDController::getBrightness() const {
  return brightness;
}

void LEDController::setColor(const CRGB& color) {
  solidColor = color;
}

void LEDController::setColorHex(const String& hexValue) {
  if (hexValue.length() != 7 || hexValue[0] != '#') {
    return;
  }
  long number = strtol(hexValue.substring(1).c_str(), nullptr, 16);
  solidColor = CRGB((number >> 16) & 0xFF, (number >> 8) & 0xFF, number & 0xFF);
}

CRGB LEDController::getColor() const {
  return solidColor;
}

void LEDController::renderOff() {
  fill_solid(leds, NUM_LEDS, CRGB::Black);
}

void LEDController::renderHyperion() {
  if (millis() - lastHyperionPacketMs > HYPERION_IDLE_TIMEOUT_MS) {
    fill_solid(leds, NUM_LEDS, CRGB::Blue);
    for (size_t i = 0; i < NUM_LEDS; ++i) {
      leds[i].nscale8_video(30);
    }
    return;
  }

  for (size_t i = 0; i < NUM_LEDS; ++i) {
    const size_t idx = i * 3;
    leds[i] = CRGB(hyperionBuffer[idx], hyperionBuffer[idx + 1], hyperionBuffer[idx + 2]);
  }
}

void LEDController::renderAudioVU(uint8_t vuLevel) {
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  const uint8_t lit = map(vuLevel, 0, 255, 0, NUM_LEDS);

  for (uint8_t i = 0; i < NUM_LEDS; ++i) {
    if (i < lit) {
      uint8_t hue = map(i, 0, NUM_LEDS - 1, 96, 0); // green to red
      leds[i] = CHSV(hue, 255, 255);
    }
  }
}

void LEDController::renderDropStack() {
  fill_solid(leds, NUM_LEDS, CRGB::Black);

  if (dropStackFill >= NUM_LEDS) {
    dropStackFill = 0;
  }

  for (uint8_t i = 0; i < dropStackFill; ++i) {
    leds[i] = CRGB::Aqua;
  }

  dropStackPos = (dropStackPos + 1) % NUM_LEDS;
  if (dropStackPos == 0) {
    dropStackFill = constrain((uint8_t)(dropStackFill + 1), 0, NUM_LEDS);
  }
}

void LEDController::renderFluidSpectrum() {
  fill_rainbow(leds, NUM_LEDS, currentHue, 5);
  currentHue += 2;
}

void LEDController::renderVelocityChaser() {
  fadeToBlackBy(leds, NUM_LEDS, 50);
  const uint8_t head = (millis() / 30) % NUM_LEDS;
  leds[head] = CHSV(currentHue, 255, 255);
  if (head + 1 < NUM_LEDS) leds[head + 1] = CHSV(currentHue + 20, 255, 180);
  if (head + 2 < NUM_LEDS) leds[head + 2] = CHSV(currentHue + 40, 255, 120);
  currentHue += 3;
}

void LEDController::renderBouncingCluster() {
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  const uint8_t clusterSize = 6;

  if (clusterPos >= NUM_LEDS - clusterSize) {
    clusterDirection = 0;
  }
  if (clusterPos == 0) {
    clusterDirection = 1;
  }

  clusterPos += clusterDirection ? 1 : 255; // 255 wraps as unsigned -1

  for (uint8_t i = 0; i < clusterSize && clusterPos + i < NUM_LEDS; ++i) {
    leds[clusterPos + i] = CHSV(currentHue + i * 10, 255, 255);
  }
  currentHue += 2;
}

void LEDController::renderPoliceStrobe() {
  const uint32_t now = millis();
  if (now - lastPoliceToggleMs > 80) {
    lastPoliceToggleMs = now;
  }

  fill_solid(leds, NUM_LEDS, CRGB::Black);
  bool redSide = ((now / 80) % 2) == 0;

  for (uint8_t i = 0; i < NUM_LEDS; ++i) {
    if (i < NUM_LEDS / 2) {
      leds[i] = redSide ? CRGB::Red : CRGB::Blue;
    } else {
      leds[i] = redSide ? CRGB::Blue : CRGB::Red;
    }
  }
}

void LEDController::renderSolid() {
  fill_solid(leds, NUM_LEDS, solidColor);
}
