#include "StudioNetworkManager.h"

StudioNetworkManager::StudioNetworkManager() : connected(false), currentSsid(""), currentIp("0.0.0.0") {}

void StudioNetworkManager::begin() {
  WiFi.mode(WIFI_STA);
  connectToWifi();
}

void StudioNetworkManager::update() {
  if (WiFi.status() == WL_CONNECTED) {
    connected = true;
    currentSsid = WiFi.SSID();
    currentIp = WiFi.localIP().toString();
  } else {
    connected = false;
  }

  handleUdpPackets();
}

void StudioNetworkManager::connectToWifi() {
  WiFi.begin(DEFAULT_WIFI1_SSID, DEFAULT_WIFI1_PASS);
  for (int i = 0; i < 20 && WiFi.status() != WL_CONNECTED; i++) {
    delay(500);
  }

  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(DEFAULT_WIFI2_SSID, DEFAULT_WIFI2_PASS);
    for (int i = 0; i < 20 && WiFi.status() != WL_CONNECTED; i++) {
      delay(500);
    }
  }

  if (WiFi.status() != WL_CONNECTED) {
    startAccessPoint();
  }
}

void StudioNetworkManager::startAccessPoint() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(DEFAULT_SOFTAP_SSID, DEFAULT_SOFTAP_PASS);
  currentSsid = DEFAULT_SOFTAP_SSID;
  currentIp = WiFi.softAPIP().toString();
}

void StudioNetworkManager::handleUdpPackets() {
  int packetSize = udp.parsePacket();
  if (packetSize <= 0) {
    return;
  }

  uint8_t packet[150];
  int len = udp.read(packet, sizeof(packet));
  if (len > 0) {
    // Placeholder handling for Hyperion UDP payloads.
  }
}

String StudioNetworkManager::getIpAddress() const {
  return currentIp;
}

String StudioNetworkManager::getSsid() const {
  return currentSsid;
}
