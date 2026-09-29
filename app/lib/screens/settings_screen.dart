import 'package:flutter/material.dart';
import 'package:provider/provider.dart';

import '../services/network_manager_service.dart';

class SettingsScreen extends StatelessWidget {
  const SettingsScreen({super.key});

  @override
  Widget build(BuildContext context) {
    final service = context.watch<NetworkManagerService>();

    return Scaffold(
      appBar: AppBar(
        title: const Text('Settings'),
      ),
      body: Padding(
        padding: const EdgeInsets.all(16),
        child: ListView(
          children: [
            TextField(
              decoration: const InputDecoration(labelText: 'Wi-Fi IP'),
              onSubmitted: (value) {
                service.ip = value;
                service.notifyListeners();
              },
            ),
            const SizedBox(height: 12),
            ElevatedButton(
              onPressed: () => service.sendRawCommand('STATUS'),
              child: const Text('Request Status'),
            ),
            const SizedBox(height: 12),
            ElevatedButton(
              onPressed: () => service.sendRawCommand('REBOOT'),
              child: const Text('Reboot ESP32'),
            ),
            const SizedBox(height: 12),
            Card(
              child: Padding(
                padding: const EdgeInsets.all(12),
                child: Column(
                  crossAxisAlignment: CrossAxisAlignment.start,
                  children: [
                    const Text('Telemetry'),
                    const SizedBox(height: 8),
                    SelectableText(service.lastTelemetry),
                  ],
                ),
              ),
            ),
          ],
        ),
      ),
    );
  }
}
