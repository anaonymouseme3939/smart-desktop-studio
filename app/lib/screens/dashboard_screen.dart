import 'package:flutter/material.dart';
import 'package:provider/provider.dart';

import '../services/network_manager_service.dart';

class DashboardScreen extends StatelessWidget {
  const DashboardScreen({super.key});

  @override
  Widget build(BuildContext context) {
    final service = context.watch<NetworkManagerService>();

    return Scaffold(
      appBar: AppBar(
        title: const Text('Dashboard'),
      ),
      body: Padding(
        padding: const EdgeInsets.all(16),
        child: ListView(
          children: [
            Card(
              child: Padding(
                padding: const EdgeInsets.all(16),
                child: Row(
                  mainAxisAlignment: MainAxisAlignment.spaceBetween,
                  children: [
                    Text('Connection: ${service.isConnected ? 'Connected' : 'Disconnected'}'),
                    DropdownButton<String>(
                      value: service.selectedMedium,
                      items: const [
                        DropdownMenuItem(value: 'AUTO', child: Text('AUTO')),
                        DropdownMenuItem(value: 'BLE', child: Text('BLE')),
                        DropdownMenuItem(value: 'WIFI_LAN', child: Text('WI-FI')),
                        DropdownMenuItem(value: 'SOFT_AP', child: Text('SOFT AP')),
                      ],
                      onChanged: (value) {
                        if (value != null) {
                          service.setMedium(value);
                        }
                      },
                    ),
                  ],
                ),
              ),
            ),
            const SizedBox(height: 12),
            Wrap(
              spacing: 12,
              runSpacing: 12,
              children: List.generate(4, (index) {
                final label = 'Relay ${index + 1}';
                return Card(
                  child: Padding(
                    padding: const EdgeInsets.all(12),
                    child: Column(
                      children: [
                        Text(label),
                        Switch(
                          value: service.relayStates[index],
                          onChanged: (value) {
                            service.updateRelay(index, value);
                            service.sendPayload({'relay': index + 1, 'state': value ? 1 : 0});
                          },
                        ),
                      ],
                    ),
                  ),
                );
              }),
            ),
            const SizedBox(height: 12),
            Card(
              child: Padding(
                padding: const EdgeInsets.all(16),
                child: Column(
                  crossAxisAlignment: CrossAxisAlignment.start,
                  children: [
                    const Text('Audio / LED Controls'),
                    Slider(
                      value: service.brightness.toDouble(),
                      min: 0,
                      max: 255,
                      onChanged: (value) {
                        service.setBrightness(value.round());
                        service.sendPayload({'brightness': value.round()});
                      },
                    ),
                    Slider(
                      value: service.sensitivity.toDouble(),
                      min: 500,
                      max: 5000,
                      onChanged: (value) {
                        service.setSensitivity(value.round());
                        service.sendPayload({'sensitivity': value.round()});
                      },
                    ),
                    Wrap(
                      spacing: 8,
                      children: [
                        'OFF', 'HYPERION', 'VU', 'DROP', 'RAINBOW', 'CHASER', 'CLUSTER', 'POLICE', 'SOLID'
                      ].map((mode) {
                        final selected = service.mode == mode;
                        return ChoiceChip(
                          label: Text(mode),
                          selected: selected,
                          onSelected: (_) {
                            service.setMode(mode);
                            service.sendPayload({'mode': mode});
                          },
                        );
                      }).toList(),
                    ),
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
