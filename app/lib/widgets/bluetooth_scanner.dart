import 'package:flutter/material.dart';
import 'package:flutter_bluetooth_serial/flutter_bluetooth_serial.dart';
import 'package:provider/provider.dart';

import '../services/network_manager_service.dart';

class BluetoothScanner extends StatefulWidget {
  const BluetoothScanner({super.key});

  @override
  State<BluetoothScanner> createState() => _BluetoothScannerState();
}

class _BluetoothScannerState extends State<BluetoothScanner> {
  bool _isScanning = false;
  final List<BluetoothDiscoveryResult> _results = [];

  @override
  Widget build(BuildContext context) {
    final service = context.read<NetworkManagerService>();

    return Card(
      child: Padding(
        padding: const EdgeInsets.all(12),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            Text(
              'Bluetooth Devices',
              style: Theme.of(context).textTheme.titleSmall,
            ),
            const SizedBox(height: 8),
            ElevatedButton.icon(
              onPressed: _isScanning ? null : () => _startScan(service),
              icon: const Icon(Icons.bluetooth_searching),
              label: Text(_isScanning ? 'Scanning...' : 'Scan Devices'),
            ),
            const SizedBox(height: 8),
            if (_results.isEmpty && !_isScanning)
              const Text('No devices found', style: TextStyle(fontSize: 12))
            else
              SingleChildScrollView(
                child: Column(
                  children: _results.map((result) {
                    return ListTile(
                      dense: true,
                      title: Text(result.device.name ?? 'Unknown'),
                      subtitle: Text(result.device.address),
                      trailing: ElevatedButton(
                        onPressed: () => service.connectBluetooth(result.device),
                        child: const Text('Connect'),
                      ),
                    );
                  }).toList(),
                ),
              ),
          ],
        ),
      ),
    );
  }

  void _startScan(NetworkManagerService service) async {
    setState(() => _isScanning = true);
    _results.clear();

    try {
      FluetoothBluetoothSerial.instance.startDiscovery().listen(
        (result) {
          if (!_results.any((r) => r.device.address == result.device.address)) {
            setState(() => _results.add(result));
          }
        },
        onDone: () => setState(() => _isScanning = false),
        onError: (error) {
          ScaffoldMessenger.of(context).showSnackBar(
            SnackBar(content: Text('Scan error: $error')),
          );
          setState(() => _isScanning = false);
        },
      );
    } catch (e) {
      ScaffoldMessenger.of(context).showSnackBar(
        SnackBar(content: Text('Error: $e')),
      );
      setState(() => _isScanning = false);
    }
  }
}
