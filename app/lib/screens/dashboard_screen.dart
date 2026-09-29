import 'dart:convert';

import 'package:flutter/foundation.dart';

class NetworkManagerService extends ChangeNotifier {
  String selectedMedium = 'AUTO';
  bool connectionState = false;
  String ipAddress = '0.0.0.0';
  String ssid = 'Not connected';
  int brightness = 255;
  int selectedMode = 1;
  List<int> relayStates = [0, 0, 0, 0];
  int vuLevel = 0;
  String currentColor = '#00F0FF';
  int sensitivity = 2200;
  int clapMask = 15;

  void setMedium(String medium) {
    selectedMedium = medium;
    notifyListeners();
  }

  void setConnectionState(bool connected) {
    connectionState = connected;
    notifyListeners();
  }

  void setRelayState(int index, bool on) {
    relayStates[index] = on ? 1 : 0;
    notifyListeners();
  }

  void toggleRelay(int index) {
    relayStates[index] = relayStates[index] == 1 ? 0 : 1;
    notifyListeners();
  }

  void setMode(int modeIndex) {
    selectedMode = modeIndex;
    notifyListeners();
  }

  void sendPayload(Map<String, dynamic> payload) {
    final json = jsonEncode(payload);
    if (kDebugMode) {
      print('Sending payload: $json');
    }
    notifyListeners();
  }

  void sendRawCommand(String command) {
    if (kDebugMode) {
      print('Sending raw command: $command');
    }
    notifyListeners();
  }
}
