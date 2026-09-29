import 'package:flutter/material.dart';
import 'package:provider/provider.dart';

import '../services/network_manager_service.dart';

class RelayCard extends StatelessWidget {
  final int index;
  final String label;

  const RelayCard({
    super.key,
    required this.index,
    required this.label,
  });

  @override
  Widget build(BuildContext context) {
    final service = context.watch<NetworkManagerService>();
    final isOn = service.relayStates[index];

    return Card(
      color: isOn ? Colors.green[900] : Colors.grey[800],
      child: Padding(
        padding: const EdgeInsets.all(12),
        child: Column(
          children: [
            Text(
              label,
              style: Theme.of(context).textTheme.titleSmall,
            ),
            const SizedBox(height: 8),
            Switch.adaptive(
              value: isOn,
              onChanged: (value) => service.toggleRelay(index),
              activeColor: Colors.green,
              inactiveThumbColor: Colors.grey,
            ),
            Text(
              isOn ? 'ON' : 'OFF',
              style: TextStyle(
                fontSize: 12,
                color: isOn ? Colors.green : Colors.grey,
                fontWeight: FontWeight.bold,
              ),
            ),
          ],
        ),
      ),
    );
  }
}
