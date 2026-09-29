import 'package:flutter/material.dart';

class SettingsScreen extends StatelessWidget {
  const SettingsScreen({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: const Text('Settings')),
      body: ListView(
        padding: const EdgeInsets.all(16),
        children: const [
          ListTile(
            leading: Icon(Icons.bluetooth),
            title: Text('Bluetooth device scanner'),
          ),
          ListTile(
            leading: Icon(Icons.wifi),
            title: Text('Wi-Fi configuration'),
          ),
          ListTile(
            leading: Icon(Icons.tune),
            title: Text('Clap relay mask'),
          ),
          ListTile(
            leading: Icon(Icons.replay),
            title: Text('Reboot'),
          ),
          ListTile(
            leading: Icon(Icons.delete_forever),
            title: Text('Factory reset'),
          ),
        ],
      ),
    );
  }
}
