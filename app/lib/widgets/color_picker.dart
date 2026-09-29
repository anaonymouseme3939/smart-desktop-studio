import 'package:flutter/material.dart';
import 'package:provider/provider.dart';

import '../services/network_manager_service.dart';

class ColorPickerWidget extends StatefulWidget {
  const ColorPickerWidget({super.key});

  @override
  State<ColorPickerWidget> createState() => _ColorPickerWidgetState();
}

class _ColorPickerWidgetState extends State<ColorPickerWidget> {
  late TextEditingController _hexController;

  @override
  void initState() {
    super.initState();
    final service = context.read<NetworkManagerService>();
    _hexController = TextEditingController(text: service.solidColor);
  }

  @override
  void dispose() {
    _hexController.dispose();
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
              'LED Color',
              style: Theme.of(context).textTheme.titleSmall,
            ),
            const SizedBox(height: 8),
            Row(
              children: [
                Container(
                  width: 60,
                  height: 60,
                  decoration: BoxDecoration(
                    color: _parseColor(service.solidColor),
                    border: Border.all(color: Colors.white54),
                    borderRadius: BorderRadius.circular(8),
                  ),
                ),
                const SizedBox(width: 12),
                Expanded(
                  child: TextField(
                    controller: _hexController,
                    decoration: InputDecoration(
                      hintText: '#RRGGBB',
                      border: OutlineInputBorder(
                        borderRadius: BorderRadius.circular(8),
                      ),
                      contentPadding: const EdgeInsets.symmetric(horizontal: 12, vertical: 8),
                    ),
                    onSubmitted: (value) {
                      if (_isValidHexColor(value)) {
                        service.setColor(value);
                      }
                    },
                  ),
                ),
              ],
            ),
            const SizedBox(height: 8),
            Wrap(
              spacing: 4,
              children: [
                _colorButton(Colors.red, '#FF0000', service),
                _colorButton(Colors.green, '#00FF00', service),
                _colorButton(Colors.blue, '#0000FF', service),
                _colorButton(Colors.yellow, '#FFFF00', service),
                _colorButton(Colors.cyan, '#00FFFF', service),
                _colorButton(Colors.purple, '#FF00FF', service),
                _colorButton(Colors.white, '#FFFFFF', service),
                _colorButton(Colors.black, '#000000', service),
              ],
            ),
          ],
        ),
      ),
    );
  }

  Widget _colorButton(Color color, String hex, NetworkManagerService service) {
    return GestureDetector(
      onTap: () {
        _hexController.text = hex;
        service.setColor(hex);
      },
      child: Container(
        width: 40,
        height: 40,
        decoration: BoxDecoration(
          color: color,
          border: Border.all(
            color: service.solidColor == hex ? Colors.white : Colors.transparent,
            width: 2,
          ),
          borderRadius: BorderRadius.circular(4),
        ),
      ),
    );
  }

  Color _parseColor(String hexColor) {
    try {
      final hex = hexColor.replaceFirst('#', '');
      return Color(int.parse('FF$hex', radix: 16));
    } catch (e) {
      return Colors.cyan;
    }
  }

  bool _isValidHexColor(String hex) {
    return RegExp(r'^#[0-9A-Fa-f]{6}$').hasMatch(hex);
  }
}
