import 'package:flutter/material.dart';
import 'package:provider/provider.dart';

import '../services/network_manager_service.dart';

class LedModeSelector extends StatelessWidget {
  const LedModeSelector({super.key});

  @override
  Widget build(BuildContext context) {
    final service = context.watch<NetworkManagerService>();
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

    return Card(
      child: Padding(
        padding: const EdgeInsets.all(12),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            Text(
              'LED Modes',
              style: Theme.of(context).textTheme.titleSmall,
            ),
            const SizedBox(height: 8),
            Wrap(
              spacing: 4,
              runSpacing: 4,
              children: modes.map((mode) {
                final isSelected = service.currentMode == mode;
                return ChoiceChip(
                  label: Text(
                    mode,
                    style: TextStyle(
                      fontSize: 11,
                      fontWeight: isSelected ? FontWeight.bold : FontWeight.normal,
                    ),
                  ),
                  selected: isSelected,
                  onSelected: (_) => service.setLedMode(mode),
                );
              }).toList(),
            ),
          ],
        ),
      ),
    );
  }
}
