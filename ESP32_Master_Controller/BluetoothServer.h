#include "AudioEngine.h"

AudioEngine::AudioEngine() : sensitivity(DEFAULT_SENSITIVITY), vuLevel(0), firstClapTime(0), firstClapDetected(false), clapState(false) {}

void AudioEngine::begin() {
  analogReadResolution(12);
  pinMode(AUDIO_INPUT_PIN, INPUT);
}

void AudioEngine::update() {
  uint32_t peak = 0;
  for (int i = 0; i < AUDIO_SAMPLES_BUFFER; i++) {
    uint16_t sample = analogRead(AUDIO_INPUT_PIN);
    if (sample > peak) {
      peak = sample;
    }
  }

  if (peak > sensitivity) {
    vuLevel = map(peak, 0, sensitivity, 0, 255);
  } else {
    vuLevel = 0;
  }

  if (peak > sensitivity && !firstClapDetected) {
    firstClapDetected = true;
    firstClapTime = millis();
  }

  if (firstClapDetected) {
    uint32_t elapsed = millis() - firstClapTime;
    if (elapsed > CLAP_WINDOW_MAX_MS) {
      firstClapDetected = false;
      clapState = true;
    }
  }
}

uint8_t AudioEngine::getVuLevel() const {
  return vuLevel;
}

void AudioEngine::setSensitivity(uint16_t newSensitivity) {
  sensitivity = newSensitivity;
}

uint16_t AudioEngine::getSensitivity() const {
  return sensitivity;
}
