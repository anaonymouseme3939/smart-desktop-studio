import 'package:flutter/material.dart';
import 'package:provider/provider.dart';

import '../services/network_manager_service.dart';

class RawTerminal extends StatefulWidget {
  const RawTerminal({super.key});

  @override
  State<RawTerminal> createState() => _RawTerminalState();
}

class _RawTerminalState extends State<RawTerminal> {
  late TextEditingController _commandController;
  final ScrollController _scrollController = ScrollController();

  @override
  void initState() {
    super.initState();
    _commandController = TextEditingController();
  }

  @override
  void dispose() {
    _commandController.dispose();
    _scrollController.dispose();
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
              'Raw Terminal',
              style: Theme.of(context).textTheme.titleSmall,
            ),
            const SizedBox(height: 8),
            Container(
              height: 120,
              decoration: BoxDecoration(
                color: Colors.black26,
                borderRadius: BorderRadius.circular(4),
                border: Border.all(color: Colors.grey[700]!),
              ),
              child: SingleChildScrollView(
                controller: _scrollController,
                child: Padding(
                  padding: const EdgeInsets.all(8),
                  child: Column(
                    crossAxisAlignment: CrossAxisAlignment.start,
                    children: service.telemetryLog
                        .take(15)
                        .map((log) => Text(
                              log,
                              style: const TextStyle(
                                fontSize: 10,
                                fontFamily: 'monospace',
                              ),
                            ))
                        .toList(),
                  ),
                ),
              ),
            ),
            const SizedBox(height: 8),
            TextField(
              controller: _commandController,
              decoration: InputDecoration(
                hintText: 'Enter command (e.g., STATUS, HELP, 1, 2, 3, 4, M)',
                border: OutlineInputBorder(
                  borderRadius: BorderRadius.circular(8),
                ),
                suffixIcon: IconButton(
                  icon: const Icon(Icons.send),
                  onPressed: () {
                    if (_commandController.text.isNotEmpty) {
                      service.sendRawCommand(_commandController.text);
                      _commandController.clear();
                      _scrollController.animateTo(
                        _scrollController.position.maxScrollExtent,
                        duration: const Duration(milliseconds: 200),
                        curve: Curves.easeOut,
                      );
                    }
                  },
                ),
              ),
              onSubmitted: (cmd) {
                if (cmd.isNotEmpty) {
                  service.sendRawCommand(cmd);
                  _commandController.clear();
                }
              },
            ),
          ],
        ),
      ),
    );
  }
}
