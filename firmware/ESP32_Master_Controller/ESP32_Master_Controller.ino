#include <Arduino.h>
#include <FastLED.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include <WiFi.h>
#include <BluetoothSerial.h>
#include "Config.h"
#include "PinMap.h"
#include "RelayManager.h"
#include "LEDController.h"
#include "AudioEngine.h"
#include "BluetoothServer.h"
#include "StudioNetworkManager.h"

RelayManager relayManager;
LEDController ledController;
AudioEngine audioEngine;
BluetoothServer bluetoothServer;
StudioNetworkManager networkManager;

volatile uint8_t g_vuLevel = 0;

void TaskSystem(void* parameter);
void TaskRealTime(void* parameter);

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  delay(1000);

  Serial.println("\n\n========================================");
  Serial.println("  SMART DESKTOP STUDIO v1.0.0");
  Serial.println("  ESP32 MASTER CONTROLLER");
  Serial.println("========================================\n");

  // Initialize hardware in sequence
  Serial.print("[SETUP] Initializing GPIO...");
  pinMode(PinMap::STATUS_LED_PIN, OUTPUT);
  digitalWrite(PinMap::STATUS_LED_PIN, LOW);
  Serial.println(" OK");

  Serial.print("[SETUP] Initializing Relays...");
  relayManager.begin();
  Serial.println(" OK");

  Serial.print("[SETUP] Initializing LED Controller...");
  ledController.begin();
  Serial.println(" OK");

  Serial.print("[SETUP] Initializing Audio Engine...");
  audioEngine.begin();
  Serial.println(" OK");

  Serial.print("[SETUP] Initializing Bluetooth...");
  bluetoothServer.begin();
  Serial.println(" OK");

  Serial.print("[SETUP] Initializing Network Manager...");
  networkManager.begin();
  Serial.println(" OK");

  // Relay self-test
  Serial.print("[SETUP] Running relay self-test...");
  relayManager.selfTestRelays();
  Serial.println(" OK");

  // LED startup animation
  Serial.print("[SETUP] Running LED startup...");
  for (int i = 0; i < 50; i++) {
    ledController.update(0);
  }
  Serial.println(" OK");

  // Start FreeRTOS tasks
  Serial.println("\n[SETUP] Starting FreeRTOS tasks...");
  xTaskCreatePinnedToCore(
      TaskSystem,
      "TaskSystem",
      TASK_STACK_SYSTEM,
      NULL,
      1,
      NULL,
      CORE_SYSTEM
  );

  xTaskCreatePinnedToCore(
      TaskRealTime,
      "TaskRealTime",
      TASK_STACK_REALTIME,
      NULL,
      2,
      NULL,
      CORE_REALTIME
  );

  Serial.println("\n[SETUP] COMPLETE - System ready!");
  Serial.println("\nBluetooth: ESP32_SMART_DESK");
  Serial.println("WebSocket: ws://192.168.1.100:81 or ws://192.168.4.1:81\n");
}

void loop() {
  // FreeRTOS handles everything
  delay(10000);
}

void TaskSystem(void* parameter) {
  (void)parameter;
  TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t xFrequency = pdMS_TO_TICKS(10);

  uint32_t telemetryCounter = 0;
  uint32_t lastStatusMs = millis();

  for (;;) {
    vTaskDelayUntil(&xLastWakeTime, xFrequency);

    // Update network
    networkManager.update();

    // Update Bluetooth
    bluetoothServer.update();

    // Status LED blink
    if (networkManager.isConnected()) {
      digitalWrite(PinMap::STATUS_LED_PIN, (millis() / 500) % 2 == 0 ? HIGH : LOW);
    } else {
      digitalWrite(PinMap::STATUS_LED_PIN, (millis() / 200) % 2 == 0 ? HIGH : LOW);
    }

    // Telemetry broadcast
    telemetryCounter++;
    if (telemetryCounter >= 10) { // Every 100ms (10 * 10ms)
      StaticJsonDocument<256> doc;
      doc["uptime"] = millis();
      doc["ip"] = networkManager.getIpAddress();
      doc["ssid"] = networkManager.getConnectedSsid();

      bool relayStates[NUM_RELAYS];
      relayManager.getAllRelayStates(relayStates);

      JsonArray relays = doc.createNestedArray("relays");
      for (int i = 0; i < NUM_RELAYS; i++) {
        relays.add(relayStates[i] ? 1 : 0);
      }

      doc["vu"] = g_vuLevel;
      doc["mode_idx"] = ledController.getMode();
      doc["brightness"] = ledController.getBrightness();

      String output;
      serializeJson(doc, output);
      bluetoothServer.sendTelemetry(output);
      networkManager.broadcastTelemetry(output);

      telemetryCounter = 0;
    }

    // Print status every 5 seconds to Serial
    if (millis() - lastStatusMs > 5000) {
      Serial.print("[HEARTBEAT] Uptime: ");
      Serial.print(millis() / 1000);
      Serial.print("s | Connected: ");
      Serial.print(networkManager.isConnected() ? "YES" : "NO");
      Serial.print(" | IP: ");
      Serial.print(networkManager.getIpAddress());
      Serial.print(" | VU: ");
      Serial.println(g_vuLevel);

      lastStatusMs = millis();
    }
  }
}

void TaskRealTime(void* parameter) {
  (void)parameter;
  TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t xFrequency = pdMS_TO_TICKS(5);

  for (;;) {
    vTaskDelayUntil(&xLastWakeTime, xFrequency);

    // Update audio engine (gets VU level)
    audioEngine.update();
    g_vuLevel = audioEngine.getCurrentVU();

    // Update LED controller with current VU
    ledController.update(g_vuLevel);
  }
}
