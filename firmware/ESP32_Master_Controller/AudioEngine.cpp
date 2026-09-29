#include "AudioEngine.h"

AudioEngine::AudioEngine()
  : sensitivity(DEFAULT_SENSITIVITY),
    clapRelayMask(DEFAULT_CLAP_RELAY_MASK),
    currentVU(0),
    lastPeakTime(0) {
}

void AudioEngine::begin() {
  pinMode(PinMap::MICROPHONE_PIN, INPUT);
  preferences.begin("smartdesk", false);

  sensitivity = preferences.getUShort("mic_sens", DEFAULT_SENSITIVITY);
  clapRelayMask = preferences.getUChar("clap_mask", DEFAULT_CLAP_RELAY_MASK);
}

void AudioEngine::update() {
  uint32_t maxValue = 0;
  uint32_t minValue = 4095;

  // Sample audio at high speed
  for (uint16_t i = 0; i < AUDIO_SAMPLES_BUFFER; ++i) {
    const uint16_t sample = analogRead(PinMap::MICROPHONE_PIN);
    maxValue = max(maxValue, (uint32_t)sample);
    minValue = min(minValue, (uint32_t)sample);
  }

  const uint16_t peakToPeak = maxValue - minValue;
  currentVU = constrain(map(peakToPeak, 0, sensitivity, 0, 255), 0, 255);
}

uint16_t AudioEngine::getSensitivity() const {
  return sensitivity;
}

void AudioEngine::setSensitivity(uint16_t value) {
  sensitivity = value;
  preferences.putUShort("mic_sens", sensitivity);
}

uint8_t AudioEngine::getClapRelayMask() const {
  return clapRelayMask;
}

void AudioEngine::setClapRelayMask(uint8_t mask) {
  clapRelayMask = mask;
  preferences.putUChar("clap_mask", clapRelayMask);
}

uint8_t AudioEngine::getCurrentVU() const {
  return currentVU;
}
