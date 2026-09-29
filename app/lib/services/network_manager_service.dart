import 'dart:convert';

import 'package:flutter/material.dart';

class NetworkManagerService extends ChangeNotifier {
  String selectedMedium = 'AUTO';
  bool isConnected = false;
  int brightness = 255;
  int sensitivity = 2200;
  String mode = 'OFF';
  List<bool> relayStates = [false, false, false, false];
  String ip = '0.0.0.0';
  String ssid = 'N/A';
  String lastTelemetry = '{}';

  void setMedium(String medium) {
    selectedMedium = medium;
    notifyListeners();
  }

  void setConnected(bool value) {
    isConnected = value;
    notifyListeners();
  }

  void setBrightness(int value) {
    brightness = value;
    notifyListeners();
  }

  void setSensitivity(int value) {
    sensitivity = value;
    notifyListeners();
  }

  void setMode(String newMode) {
    mode = newMode;
    notifyListeners();
  }

  void updateRelay(int index, bool value) {
    if (index >= 0 && index < relayStates.length) {
      relayStates[index] = value;
      notifyListeners();
    }
  }

  void sendPayload(Map<String, dynamic> payload) {
    final jsonString = jsonEncode(payload);
    debugPrint('Sending payload: $jsonString');
  }

  void sendRawCommand(String cmd) {
    debugPrint('Sending raw command: $cmd');
  }

  void updateTelemetry(Map<String, dynamic> telemetry) {
    lastTelemetry = jsonEncode(telemetry);
    if (telemetry['brightness'] != null) {
      brightness = telemetry['brightness'] as int;
    }
    if (telemetry['sensitivity'] != null) {
      sensitivity = telemetry['sensitivity'] as int;
    }
    if (telemetry['mode'] != null) {
      mode = telemetry['mode'] as String;
    }
    if (telemetry['ip'] != null) {
      ip = telemetry['ip'] as String;
    }
    if (telemetry['ssid'] != null) {
      ssid = telemetry['ssid'] as String;
    }
    final relays = telemetry['relays'];
    if (relays is List) {
      for (int i = 0; i < relayStates.length && i < relays.length; i++) {
        relayStates[i] = relays[i] == 1;
      }
    }
    notifyListeners();
  }
}
