#ifndef STUDIONETWORKMANAGER_H
#define STUDIONETWORKMANAGER_H

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>

#include "Config.h"

class StudioNetworkManager {
public:
  StudioNetworkManager();

  void begin();
  void update();
  String getIpAddress() const;
  String getSsid() const;

private:
  WiFiClient client;
  WiFiUDP udp;
  bool connected;
  String currentSsid;
  String currentIp;

  void connectToWifi();
  void startAccessPoint();
  void handleUdpPackets();
};

#endif
