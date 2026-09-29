import 'package:flutter/material.dart';

class RelayCard extends StatelessWidget {
  final String label;
  final bool isOn;
  final VoidCallback onToggle;

  const RelayCard({
    super.key,
    required this.label,
    required this.isOn,
    required this.onToggle,
  });

  @override
  Widget build(BuildContext context) {
    return Card(
      child: Padding(
        padding: const EdgeInsets.all(12),
        child: Column(
          children: [
            Text(label, style: const TextStyle(fontWeight: FontWeight.bold)),
            const SizedBox(height: 8),
            Switch(value: isOn, onChanged: (_) => onToggle()),
          ],
        ),
      ),
    );
  }
}
