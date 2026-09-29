import 'package:flutter/material.dart';
import 'package:provider/provider.dart';

import '../services/network_manager_service.dart';
import '../widgets/audio_visualizer.dart';
import '../widgets/relay_card.dart';

class DashboardScreen extends StatelessWidget {
  const DashboardScreen({super.key});

  @override
  Widget build(BuildContext context) {
    final service = context.watch<NetworkManagerService>();

    return Scaffold(
      appBar: AppBar(
        title: const Text('Smart Desktop Studio'),
        actions: [
          Chip(
            label: Text(service.connectionState ? 'Connected' : 'Offline'),
            avatar: Icon(
              service.connectionState ? Icons.wifi : Icons.wifi_off,
              color: service.connectionState ? Colors.green : Colors.red,
            ),
          ),
        ],
      ),
      body: SingleChildScrollView(
        padding: const EdgeInsets.all(16),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.stretch,
          children: [
            Row(
              children: [
                Expanded(
                  child: _ConnectionChip(label: 'AUTO', selected: service.selectedMedium == 'AUTO'),
                ),
                Expanded(
                  child: _ConnectionChip(label: 'BLE', selected: service.selectedMedium == 'BLE'),
                ),
                Expanded(
                  child: _ConnectionChip(label: 'WIFI', selected: service.selectedMedium == 'WIFI_LAN'),
                ),
                Expanded(
                  child: _ConnectionChip(label: 'AP', selected: service.selectedMedium == 'SOFT_AP'),
                ),
              ],
            ),
            const SizedBox(height: 16),
            Row(
              children: [
                Expanded(
                  child: RelayCard(
                    label: 'Relay 1',
                    isOn: service.relayStates[0] == 1,
                    onToggle: () => service.toggleRelay(0),
                  ),
                ),
                Expanded(
                  child: RelayCard(
                    label: 'Relay 2',
                    isOn: service.relayStates[1] == 1,
                    onToggle: () => service.toggleRelay(1),
                  ),
                ),
              ],
            ),
            Row(
              children: [
                Expanded(
                  child: RelayCard(
                    label: 'Relay 3',
                    isOn: service.relayStates[2] == 1,
                    onToggle: () => service.toggleRelay(2),
                  ),
                ),
                Expanded(
                  child: RelayCard(
                    label: 'Relay 4',
                    isOn: service.relayStates[3] == 1,
                    onToggle: () => service.toggleRelay(3),
                  ),
                ),
              ],
            ),
            const SizedBox(height: 16),
            AudioVisualizer(
              vuLevel: service.vuLevel,
              modeIndex: service.selectedMode,
              brightness: service.brightness,
            ),
            const SizedBox(height: 16),
            ElevatedButton(
              onPressed: () => service.sendPayload({'master_relays': 1}),
              child: const Text('Master Power ON'),
            ),
            const SizedBox(height: 8),
            ElevatedButton(
              onPressed: () => service.sendPayload({'master_relays': 0}),
              child: const Text('Master Power OFF'),
            ),
          ],
        ),
      ),
    );
  }
}

class _ConnectionChip extends StatelessWidget {
  final String label;
  final bool selected;

  const _ConnectionChip({required this.label, required this.selected});

  @override
  Widget build(BuildContext context) {
    return Padding(
      padding: const EdgeInsets.symmetric(horizontal: 4),
      child: ChoiceChip(
        label: Text(label),
        selected: selected,
        onSelected: (_) {},
      ),
    );
  }
}
