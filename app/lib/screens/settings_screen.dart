import 'package:flutter/material.dart';
import 'package:provider/provider.dart';

import '../services/network_manager_service.dart';
import '../widgets/bluetooth_scanner.dart';
import '../widgets/wifi_settings.dart';
import '../widgets/raw_terminal.dart';

class SettingsScreen extends StatefulWidget {
  const SettingsScreen({super.key});

  @override
  State<SettingsScreen> createState() => _SettingsScreenState();
}

class _SettingsScreenState extends State<SettingsScreen> {
  @override
  Widget build(BuildContext context) {
    final service = context.watch<NetworkManagerService>();

    return Scaffold(
      appBar: AppBar(
        title: const Text('Settings'),
        elevation: 0,
      ),
      body: SingleChildScrollView(
        child: Padding(
          padding: const EdgeInsets.all(12),
          child: Column(
            spacing: 12,
            children: [
              const BluetoothScanner(),
              const WiFiSettings(),
              Card(
                child: Padding(
                  padding: const EdgeInsets.all(12),
                  child: Column(
                    crossAxisAlignment: CrossAxisAlignment.start,
                    children: [
                      Text(
                        'Clap Relay Mask',
                        style: Theme.of(context).textTheme.titleSmall,
                      ),
                      const SizedBox(height: 8),
                      Wrap(
                        spacing: 8,
                        children: List.generate(4, (index) {
                          final isSelected = (service.clapMask & (1 << index)) != 0;
                          return FilterChip(
                            label: Text('Relay ${index + 1}'),
                            selected: isSelected,
                            onSelected: (value) {
                              int newMask = service.clapMask;
                              if (value) {
                                newMask |= (1 << index);
                              } else {
                                newMask &= ~(1 << index);
                              }
                              service.setClapMask(newMask);
                              service.sendPayload({
                                'cmd': 'set_clap_relays',
                                'mask': newMask,
                              });
                            },
                          );
                        }),
                      ),
                    ],
                  ),
                ),
              ),
              Card(
                child: Padding(
                  padding: const EdgeInsets.all(12),
                  child: Column(
                    crossAxisAlignment: CrossAxisAlignment.start,
                    children: [
                      Text(
                        'Quick Commands',
                        style: Theme.of(context).textTheme.titleSmall,
                      ),
                      const SizedBox(height: 8),
                      Wrap(
                        spacing: 4,
                        runSpacing: 4,
                        children: [
                          ElevatedButton.icon(
                            onPressed: () => service.sendRawCommand('STATUS'),
                            icon: const Icon(Icons.info),
                            label: const Text('Status'),
                          ),
                          ElevatedButton.icon(
                            onPressed: () => service.sendRawCommand('HELP'),
                            icon: const Icon(Icons.help),
                            label: const Text('Help'),
                          ),
                          ElevatedButton.icon(
                            onPressed: () => service.sendRawCommand('REBOOT'),
                            icon: const Icon(Icons.restart_alt),
                            label: const Text('Reboot'),
                          ),
                          ElevatedButton.icon(
                            onPressed: () => showDialog(
                              context: context,
                              builder: (ctx) => AlertDialog(
                                title: const Text('Factory Reset'),
                                content: const Text('Clear all settings?'),
                                actions: [
                                  TextButton(
                                    onPressed: () => Navigator.pop(ctx),
                                    child: const Text('Cancel'),
                                  ),
                                  TextButton(
                                    onPressed: () {
                                      service.sendPayload({
                                        'cmd': 'factory_reset',
                                      });
                                      Navigator.pop(ctx);
                                    },
                                    child: const Text('Reset'),
                                  ),
                                ],
                              ),
                            ),
                            icon: const Icon(Icons.restore),
                            label: const Text('Reset'),
                          ),
                        ],
                      ),
                    ],
                  ),
                ),
              ),
              const RawTerminal(),
              const SizedBox(height: 20),
            ],
          ),
        ),
      ),
    );
  }
}
