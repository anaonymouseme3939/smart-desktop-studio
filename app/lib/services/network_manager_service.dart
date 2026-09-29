import 'package:flutter/material.dart';

class AudioVisualizer extends StatelessWidget {
  final int vuLevel;
  final int modeIndex;
  final int brightness;

  const AudioVisualizer({
    super.key,
    required this.vuLevel,
    required this.modeIndex,
    required this.brightness,
  });

  @override
  Widget build(BuildContext context) {
    return Card(
      child: Padding(
        padding: const EdgeInsets.all(16),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            Text('LED Mode: $modeIndex'),
            const SizedBox(height: 12),
            SizedBox(
              height: 80,
              child: Row(
                children: List.generate(20, (index) {
                  final active = index < (vuLevel / 255 * 20).round();
                  return Expanded(
                    child: Container(
                      margin: const EdgeInsets.symmetric(horizontal: 2),
                      decoration: BoxDecoration(
                        color: active ? Colors.green : Colors.grey.shade300,
                        borderRadius: BorderRadius.circular(4),
                      ),
                    ),
                  );
                }),
              ),
            ),
            const SizedBox(height: 12),
            Slider(
              value: brightness.toDouble(),
              min: 0,
              max: 255,
              onChanged: (_) {},
            ),
          ],
        ),
      ),
    );
  }
}

