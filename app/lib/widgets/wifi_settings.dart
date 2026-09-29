import 'package:flutter/material.dart';
import 'package:provider/provider.dart';

import '../services/network_manager_service.dart';

class WiFiSettings extends StatefulWidget {
  const WiFiSettings({super.key});

  @override
  State<WiFiSettings> createState() => _WiFiSettingsState();
}

class _WiFiSettingsState extends State<WiFiSettings> {
  late TextEditingController _ipController;
  late TextEditingController _ssid1Controller;
  late TextEditingController _pass1Controller;
  late TextEditingController _ssid2Controller;
  late TextEditingController _pass2Controller;

  @override
  void initState() {
    super.initState();
    final service = context.read<NetworkManagerService>();
    _ipController = TextEditingController(text: service.ip);
    _ssid1Controller = TextEditingController();
    _pass1Controller = TextEditingController();
    _ssid2Controller = TextEditingController();
    _pass2Controller = TextEditingController();
  }

  @override
  void dispose() {
    _ipController.dispose();
    _ssid1Controller.dispose();
    _pass1Controller.dispose();
    _ssid2Controller.dispose();
    _pass2Controller.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    final service = context.watch<NetworkManagerService>();

    return Card(
      child: Padding(
        padding: const EdgeInsets.all(12),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            Text(
              'Wi-Fi Connection',
              style: Theme.of(context).textTheme.titleSmall,
            ),
            const SizedBox(height: 12),
            TextField(
              controller: _ipController,
              decoration: InputDecoration(
                labelText: 'ESP32 IP Address',
                border: OutlineInputBorder(
                  borderRadius: BorderRadius.circular(8),
                ),
                contentPadding: const EdgeInsets.symmetric(horizontal: 12, vertical: 8),
              ),
            ),
            const SizedBox(height: 8),
            ElevatedButton.icon(
              onPressed: () async {
                final connected = await service.connectToWebSocket(_ipController.text, 81);
                ScaffoldMessenger.of(context).showSnackBar(
                  SnackBar(
                    content: Text(
                      connected ? 'Connected via WebSocket' : 'Connection failed',
                    ),
                  ),
                );
              },
              icon: const Icon(Icons.wifi),
              label: const Text('Connect via WebSocket'),
            ),
            const SizedBox(height: 12),
            const Divider(),
            const SizedBox(height: 12),
            Text(
              'Configure ESP32 Wi-Fi',
              style: Theme.of(context).textTheme.titleSmall,
            ),
            const SizedBox(height: 8),
            TextField(
              controller: _ssid1Controller,
              decoration: InputDecoration(
                labelText: 'Primary SSID',
                border: OutlineInputBorder(
                  borderRadius: BorderRadius.circular(8),
                ),
              ),
            ),
            const SizedBox(height: 8),
            TextField(
              controller: _pass1Controller,
              obscureText: true,
              decoration: InputDecoration(
                labelText: 'Primary Password',
                border: OutlineInputBorder(
                  borderRadius: BorderRadius.circular(8),
                ),
              ),
            ),
            const SizedBox(height: 8),
            TextField(
              controller: _ssid2Controller,
              decoration: InputDecoration(
                labelText: 'Backup SSID',
                border: OutlineInputBorder(
                  borderRadius: BorderRadius.circular(8),
                ),
              ),
            ),
            const SizedBox(height: 8),
            TextField(
              controller: _pass2Controller,
              obscureText: true,
              decoration: InputDecoration(
                labelText: 'Backup Password',
                border: OutlineInputBorder(
                  borderRadius: BorderRadius.circular(8),
                ),
              ),
            ),
            const SizedBox(height: 12),
            ElevatedButton.icon(
              onPressed: () {
                service.sendPayload({
                  'cmd': 'set_wifi',
                  'ssid1': _ssid1Controller.text,
                  'pass1': _pass1Controller.text,
                  'ssid2': _ssid2Controller.text,
                  'pass2': _pass2Controller.text,
                });
                ScaffoldMessenger.of(context).showSnackBar(
                  const SnackBar(content: Text('Wi-Fi config sent')),
                );
              },
              icon: const Icon(Icons.save),
              label: const Text('Save & Update'),
            ),
          ],
        ),
      ),
    );
  }
}
