import 'package:flutter/material.dart';
import 'package:provider/provider.dart';

import '../services/network_manager_service.dart';

class SystemHealthMonitor extends StatefulWidget {
  const SystemHealthMonitor({super.key});

  @override
  State<SystemHealthMonitor> createState() => _SystemHealthMonitorState();
}

class _SystemHealthMonitorState extends State<SystemHealthMonitor> {
  bool _isExpanded = false;

  @override
  Widget build(BuildContext context) {
    final service = context.watch<NetworkManagerService>();

    return Card(
      child: Padding(
        padding: const EdgeInsets.all(12),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            GestureDetector(
              onTap: () => setState(() => _isExpanded = !_isExpanded),
              child: Row(
                children: [
                  Icon(_isExpanded ? Icons.expand_less : Icons.expand_more),
                  const SizedBox(width: 8),
                  Text(
                    'System Health',
                    style: Theme.of(context).textTheme.titleSmall,
                  ),
                  const Spacer(),
                  Text(
                    'Uptime: ${_formatUptime(service.uptime)}',
                    style: const TextStyle(fontSize: 12),
                  ),
                ],
              ),
            ),
            if (_isExpanded) ...
              [
                const SizedBox(height: 12),
                Container(
                  padding: const EdgeInsets.all(8),
                  decoration: BoxDecoration(
                    color: Colors.black26,
                    borderRadius: BorderRadius.circular(4),
                  ),
                  child: SingleChildScrollView(
                    child: Column(
                      crossAxisAlignment: CrossAxisAlignment.start,
                      children: [
                        Text('SSID: ${service.ssid}'),
                        Text('IP: ${service.ip}'),
                        Text('Packets: ${service.packetCount}'),
                        const Divider(height: 8),
                        Text(
                          'Last Telemetry:',
                          style: const TextStyle(fontWeight: FontWeight.bold),
                        ),
                        Text(
                          service.lastTelemetry,
                          style: const TextStyle(fontSize: 10),
                        ),
                        const Divider(height: 12),
                        Text(
                          'Live Log (${service.telemetryLog.length}):',
                          style: const TextStyle(fontWeight: FontWeight.bold),
                        ),
                        const SizedBox(height: 8),
                        ...service.telemetryLog.take(20).map(
                          (log) => Text(
                            log,
                            style: const TextStyle(fontSize: 10, fontFamily: 'monospace'),
                          ),
                        ),
                      ],
                    ),
                  ),
                ),
              ]
          ],
        ),
      ),
    );
  }

  String _formatUptime(int ms) {
    final seconds = ms ~/ 1000;
    final minutes = seconds ~/ 60;
    final hours = minutes ~/ 60;
    final days = hours ~/ 24;

    if (days > 0) return '${days}d ${hours % 24}h';
    if (hours > 0) return '${hours}h ${minutes % 60}m';
    if (minutes > 0) return '${minutes}m ${seconds % 60}s';
    return '${seconds}s';
  }
}
