#ifndef STUDIO_NETWORK_MANAGER_H
#define STUDIO_NETWORK_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiUDP.h>
#include <Preferences.h>
#include "Config.h"

class StudioNetworkManager {
public:
  StudioNetworkManager();
  void begin();
  void update();
  void configureWiFi(const String& ssid1, const String& pass1, const String& ssid2, const String& pass2);
  void startSoftAP();
  bool isConnected() const;
  String getIpAddress() const;
  String getConnectedSsid() const;
  void broadcastTelemetry(const String& telemetryJson);

private:
  WiFiClient wsClient;
  WiFiServer wsServer;
  WiFiUDP udp;
  Preferences preferences;
  String ssid1;
  String pass1;
  String ssid2;
  String pass2;
  String softApSsid;
  String softApPass;
  bool connected;
  uint32_t lastWiFiCheck;

  void loadCredentials();
  bool connectToWifi(const String& ssid, const String& pass);
  void handleIncomingUdp();
};

#endif
