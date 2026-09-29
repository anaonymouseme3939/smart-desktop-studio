import 'dart:async';
import 'dart:convert';
import 'dart:typed_data';

import 'package:flutter/foundation.dart';
import 'package:flutter_bluetooth_serial/flutter_bluetooth_serial.dart';
import 'package:web_socket_channel/web_socket_channel.dart';

class NetworkManagerService extends ChangeNotifier {
  // Connection
  String selectedMedium = 'AUTO';
  bool isConnected = false;
  String connectionStatus = 'Disconnected';

  // Relay states
  List<bool> relayStates = [false, false, false, false];
  bool isMasterOn = false;

  // LED & Audio
  int brightness = 255;
  int sensitivity = 2200;
  String currentMode = 'OFF';
  int vuLevel = 0;
  String solidColor = '#00FFFF';
  int clapMask = 0x0F;

  // Device info
  String ip = '0.0.0.0';
  String ssid = 'N/A';
  int uptime = 0;
  int packetCount = 0;

  // Bluetooth
  BluetoothConnection? _btConnection;
  bool _isBluetoothConnecting = false;
  StreamSubscription? _btSubscription;

  // WiFi WebSocket
  WebSocketChannel? _wsChannel;
  StreamSubscription? _wsSubscription;

  // Telemetry
  List<String> telemetryLog = [];
  Timer? _telemetryTimer;
  String lastTelemetry = '{}';

  NetworkManagerService() {
    _initAutoConnect();
  }

  void _initAutoConnect() async {
    await Future.delayed(const Duration(seconds: 2));
    if (selectedMedium == 'AUTO') {
      _attemptAutoConnect();
    }
  }

  Future<void> _attemptAutoConnect() async {
    // Try WebSocket to local IP
    await connectToWebSocket('192.168.1.100', 81);
    if (isConnected) return;

    // Try SoftAP
    await connectToWebSocket('192.168.4.1', 81);
    if (isConnected) return;

    // Try Bluetooth (if previously bonded)
    // Note: Would need to implement bonded device listing
  }

  // ============= BLUETOOTH =============

  Future<void> connectBluetooth(BluetoothDevice device) async {
    if (_isBluetoothConnecting) return;

    _isBluetoothConnecting = true;
    _updateConnection('BLE', 'Connecting to ${device.name}...');

    try {
      _btConnection = await BluetoothConnection.toAddress(device.address);

      if (_btConnection != null && _btConnection!.isConnected) {
        _updateConnection('BLE', 'Connected to ${device.name}');
        _setupBluetoothListener();
        _telemetryTimer = Timer.periodic(
          const Duration(milliseconds: 100),
          (_) => sendPayload({'cmd': 'status'}),
        );
      }
    } catch (ex) {
      _updateConnection('BLE', 'BT Error: $ex');
      _btConnection = null;
    } finally {
      _isBluetoothConnecting = false;
    }
  }

  void _setupBluetoothListener() {
    _btSubscription?.cancel();
    _btSubscription = _btConnection?.input?.listen(
      (Uint8List data) {
        final message = String.fromCharCodes(data).trim();
        if (message.isNotEmpty) {
          _parseIncomingData(message);
        }
      },
      onDone: () => disconnectBluetooth(),
      onError: (error) {
        _updateConnection('BLE', 'BT Error: $error');
        disconnectBluetooth();
      },
    );
  }

  void disconnectBluetooth() {
    _btSubscription?.cancel();
    _btConnection?.dispose();
    _btConnection = null;
    _updateConnection('Disconnected', 'BT Disconnected');
  }

  // ============= WEBSOCKET =============

  Future<bool> connectToWebSocket(String ip, int port) async {
    try {
      final wsUrl = Uri.parse('ws://$ip:$port');
      _wsChannel = WebSocketChannel.connect(wsUrl);

      _wsSubscription = _wsChannel?.stream.listen(
        (message) {
          _parseIncomingData(message.toString());
        },
        onDone: () => disconnectWebSocket(),
        onError: (error) {
          debugPrint('WebSocket error: $error');
          disconnectWebSocket();
        },
      );

      _updateConnection('WIFI', 'Connected to $ip:$port');
      this.ip = ip;
      _telemetryTimer = Timer.periodic(
        const Duration(milliseconds: 100),
        (_) => sendPayload({'cmd': 'status'}),
      );
      return true;
    } catch (e) {
      debugPrint('WebSocket connection failed: $e');
      return false;
    }
  }

  void disconnectWebSocket() {
    _wsSubscription?.cancel();
    _wsChannel?.sink.close();
    _wsChannel = null;
    _updateConnection('Disconnected', 'WebSocket Disconnected');
  }

  // ============= RELAY CONTROL =============

  void toggleRelay(int index) {
    if (index >= 0 && index < relayStates.length) {
      final newState = !relayStates[index];
      relayStates[index] = newState;
      sendPayload({
        'relay': index + 1,
        'state': newState ? 1 : 0,
      });
      _updateMasterState();
      notifyListeners();
    }
  }

  void toggleMasterRelays() {
    final newMasterState = !isMasterOn;
    sendPayload({
      'master_relays': newMasterState ? 1 : 0,
    });
    for (int i = 0; i < relayStates.length; i++) {
      relayStates[i] = newMasterState;
    }
    isMasterOn = newMasterState;
    notifyListeners();
  }

  void _updateMasterState() {
    isMasterOn = relayStates.any((state) => state);
  }

  // ============= LED CONTROL =============

  void setLedMode(String mode) {
    currentMode = mode;
    sendPayload({'mode': mode});
    notifyListeners();
  }

  void setBrightness(int value) {
    brightness = value.clamp(0, 255);
    notifyListeners();
  }

  void setSensitivity(int value) {
    sensitivity = value.clamp(500, 5000);
    notifyListeners();
  }

  void setColor(String hexColor) {
    solidColor = hexColor;
    sendPayload({'color': hexColor});
    notifyListeners();
  }

  void setClapMask(int mask) {
    clapMask = mask;
    notifyListeners();
  }

  // ============= COMMUNICATION =============

  void sendPayload(Map<String, dynamic> payload) {
    if (!isConnected) {
      _addLog('⚠️ NOT CONNECTED - Command dropped: ${jsonEncode(payload)}');
      return;
    }

    final jsonString = jsonEncode(payload) + '\n';

    try {
      if (_btConnection?.isConnected ?? false) {
        _btConnection?.output.add(Uint8List.fromList(jsonString.codeUnits));
        _btConnection?.output.add([13]); // CR
      } else if (_wsChannel != null) {
        _wsChannel?.sink.add(jsonString);
      }
      _addLog('📤 TX: $jsonString');
      packetCount++;
    } catch (e) {
      _addLog('❌ Send error: $e');
    }
  }

  void sendRawCommand(String command) {
    if (!isConnected) {
      _addLog('⚠️ NOT CONNECTED - Command dropped: $command');
      return;
    }

    final cmdString = '$command\n';

    try {
      if (_btConnection?.isConnected ?? false) {
        _btConnection?.output.add(Uint8List.fromList(cmdString.codeUnits));
      } else if (_wsChannel != null) {
        _wsChannel?.sink.add(cmdString);
      }
      _addLog('📤 RAW TX: $cmdString');
      packetCount++;
    } catch (e) {
      _addLog('❌ Send error: $e');
    }
  }

  void _parseIncomingData(String data) {
    _addLog('📥 RX: $data');

    try {
      final json = jsonDecode(data);
      if (json is Map) {
        lastTelemetry = data;
        _parseJson(json);
      }
    } catch (e) {
      // Not JSON, ignore
    }
  }

  void _parseJson(Map<String, dynamic> json) {
    if (json.containsKey('uptime')) uptime = json['uptime'] ?? 0;
    if (json.containsKey('ip')) ip = json['ip'] ?? ip;
    if (json.containsKey('ssid')) ssid = json['ssid'] ?? ssid;

    if (json.containsKey('relays')) {
      final relays = json['relays'] as List?;
      if (relays != null && relays.length == 4) {
        for (int i = 0; i < 4; i++) {
          relayStates[i] = relays[i] == 1;
        }
        _updateMasterState();
      }
    }

    if (json.containsKey('vu')) vuLevel = json['vu'] ?? 0;
    if (json.containsKey('mode_idx')) currentMode = _getModeFromIndex(json['mode_idx'] ?? 0);
    if (json.containsKey('brightness')) brightness = json['brightness'] ?? brightness;
    if (json.containsKey('sensitivity')) sensitivity = json['sensitivity'] ?? sensitivity;
    if (json.containsKey('color')) solidColor = json['color'] ?? solidColor;
    if (json.containsKey('clap_mask')) clapMask = json['clap_mask'] ?? clapMask;

    notifyListeners();
  }

  String _getModeFromIndex(int index) {
    const modes = [
      'OFF',
      'HYPERION',
      'VU',
      'DROP',
      'RAINBOW',
      'CHASER',
      'CLUSTER',
      'POLICE',
      'SOLID'
    ];
    return index >= 0 && index < modes.length ? modes[index] : 'OFF';
  }

  void _updateConnection(String medium, String status) {
    selectedMedium = medium;
    connectionStatus = status;
    isConnected = status.contains('Connected');
    if (!isConnected) {
      _telemetryTimer?.cancel();
    }
    notifyListeners();
  }

  void _addLog(String message) {
    telemetryLog.insert(0, '[${DateTime.now().toIso8601String().split('T')[1]}] $message');
    if (telemetryLog.length > 100) {
      telemetryLog.removeLast();
    }
    notifyListeners();
  }

  @override
  void dispose() {
    disconnectBluetooth();
    disconnectWebSocket();
    _telemetryTimer?.cancel();
    super.dispose();
  }
}
