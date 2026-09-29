#include "StudioNetworkManager.h"

StudioNetworkManager::StudioNetworkManager()
  : wsServer(WEBSOCKET_PORT),
    softApSsid(DEFAULT_SOFTAP_SSID),
    softApPass(DEFAULT_SOFTAP_PASS),
    connected(false),
    lastWiFiCheck(0) {
}

void StudioNetworkManager::begin() {
  Serial.println("[NET] Initializing network manager...");
  loadCredentials();

  // Try WiFi connection
  WiFi.mode(WIFI_STA);
  if (!connectToWifi(ssid1, pass1)) {
    Serial.println("[NET] WiFi1 failed, trying WiFi2...");
    if (!connectToWifi(ssid2, pass2)) {
      Serial.println("[NET] Both WiFi networks failed, starting SoftAP...");
      startSoftAP();
    }
  }

  // Start WebSocket server
  wsServer.begin();
  Serial.println("[NET] WebSocket server started on port " + String(WEBSOCKET_PORT));

  // Start UDP for Hyperion
  udp.begin(HYPERION_UDP_PORT);
  Serial.println("[NET] UDP server started on port " + String(HYPERION_UDP_PORT));
}

void StudioNetworkManager::update() {
  // Check WiFi status periodically
  if (millis() - lastWiFiCheck > 10000) {
    if (WiFi.getMode() == WIFI_STA && WiFi.status() != WL_CONNECTED) {
      Serial.println("[NET] WiFi disconnected, attempting reconnect...");
      WiFi.reconnect();
    }
    lastWiFiCheck = millis();
  }

  // Handle WebSocket
  if (!wsClient || !wsClient.connected()) {
    wsClient = wsServer.available();
  }

  // Handle UDP (Hyperion)
  handleIncomingUdp();
}

void StudioNetworkManager::configureWiFi(const String& inSsid1, const String& inPass1,
                                         const String& inSsid2, const String& inPass2) {
  ssid1 = inSsid1;
  pass1 = inPass1;
  ssid2 = inSsid2;
  pass2 = inPass2;

  preferences.begin("smartdesk", false);
  preferences.putString("wifi1_ssid", ssid1);
  preferences.putString("wifi1_pass", pass1);
  preferences.putString("wifi2_ssid", ssid2);
  preferences.putString("wifi2_pass", pass2);
  preferences.end();

  Serial.println("[NET] WiFi credentials updated");
}

void StudioNetworkManager::startSoftAP() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(softApSsid.c_str(), softApPass.c_str());
  connected = true;
  Serial.println("[NET] SoftAP started: " + softApSsid + " (192.168.4.1)");
}

bool StudioNetworkManager::isConnected() const {
  return (WiFi.status() == WL_CONNECTED) || (WiFi.getMode() == WIFI_AP);
}

String StudioNetworkManager::getIpAddress() const {
  if (WiFi.getMode() == WIFI_AP) {
    return WiFi.softAPIP().toString();
  }
  return WiFi.localIP().toString();
}

String StudioNetworkManager::getConnectedSsid() const {
  if (WiFi.getMode() == WIFI_STA) {
    return WiFi.SSID();
  }
  return softApSsid;
}

void StudioNetworkManager::broadcastTelemetry(const String& telemetryJson) {
  if (wsClient && wsClient.connected()) {
    wsClient.println(telemetryJson);
  }
}

void StudioNetworkManager::loadCredentials() {
  preferences.begin("smartdesk", false);
  ssid1 = preferences.getString("wifi1_ssid", DEFAULT_WIFI1_SSID);
  pass1 = preferences.getString("wifi1_pass", DEFAULT_WIFI1_PASS);
  ssid2 = preferences.getString("wifi2_ssid", DEFAULT_WIFI2_SSID);
  pass2 = preferences.getString("wifi2_pass", DEFAULT_WIFI2_PASS);
  softApSsid = preferences.getString("ap_ssid", DEFAULT_SOFTAP_SSID);
  softApPass = preferences.getString("ap_pass", DEFAULT_SOFTAP_PASS);
  preferences.end();
}

bool StudioNetworkManager::connectToWifi(const String& ssid, const String& pass) {
  Serial.println("[NET] Attempting WiFi: " + ssid);
  WiFi.begin(ssid.c_str(), pass.c_str());

  const uint32_t start = millis();
  while (millis() - start < WIFI_TIMEOUT_MS) {
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("[NET] WiFi connected! IP: " + WiFi.localIP().toString());
      connected = true;
      return true;
    }
    delay(250);
  }

  Serial.println("[NET] WiFi connection timeout");
  return false;
}

void StudioNetworkManager::handleIncomingUdp() {
  int packetSize = udp.parsePacket();
  if (packetSize <= 0) {
    return;
  }

  if (packetSize >= NUM_LEDS * 3) {
    uint8_t packet[NUM_LEDS * 3];
    udp.read(packet, packetSize);
    // Hyperion data processed by main firmware
  }
}
