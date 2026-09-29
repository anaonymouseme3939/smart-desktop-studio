import 'package:flutter/material.dart';
import 'package:provider/provider.dart';

import '../services/network_manager_service.dart';
import '../widgets/connection_bar.dart';
import '../widgets/relay_card.dart';
import '../widgets/audio_visualizer.dart';
import '../widgets/led_mode_selector.dart';
import '../widgets/color_picker.dart';
import '../widgets/system_health_monitor.dart';

class DashboardScreen extends StatelessWidget {
  const DashboardScreen({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Smart Desktop Studio'),
        elevation: 0,
      ),
      body: Consumer<NetworkManagerService>(
        builder: (context, service, _) {
          return SingleChildScrollView(
            child: Padding(
              padding: const EdgeInsets.all(12),
              child: Column(
                spacing: 12,
                children: [
                  const ConnectionBar(),
                  Card(
                    child: Padding(
                      padding: const EdgeInsets.all(12),
                      child: Row(
                        mainAxisAlignment: MainAxisAlignment.spaceBetween,
                        children: [
                          Text(
                            'Master Power',
                            style: Theme.of(context).textTheme.titleMedium,
                          ),
                          Switch(
                            value: service.isMasterOn,
                            onChanged: (value) {
                              service.toggleMasterRelays();
                            },
                          ),
                        ],
                      ),
                    ),
                  ),
                  Wrap(
                    spacing: 8,
                    runSpacing: 8,
                    alignment: WrapAlignment.spaceBetween,
                    children: List.generate(4, (index) {
                      return SizedBox(
                        width: (MediaQuery.of(context).size.width - 32) / 2,
                        child: RelayCard(
                          index: index,
                          label: 'Relay ${index + 1}',
                        ),
                      );
                    }),
                  ),
                  const AudioVisualizer(),
                  const LedModeSelector(),
                  Card(
                    child: Padding(
                      padding: const EdgeInsets.all(12),
                      child: Column(
                        crossAxisAlignment: CrossAxisAlignment.start,
                        children: [
                          Text(
                            'Brightness',
                            style: Theme.of(context).textTheme.titleSmall,
                          ),
                          Slider(
                            value: service.brightness.toDouble(),
                            min: 0,
                            max: 255,
                            onChanged: (value) {
                              service.setBrightness(value.round());
                              service.sendPayload({
                                'brightness': value.round(),
                              });
                            },
                          ),
                          Text('${service.brightness}/255'),
                        ],
                      ),
                    ),
                  ),
                  const ColorPickerWidget(),
                  Card(
                    child: Padding(
                      padding: const EdgeInsets.all(12),
                      child: Column(
                        crossAxisAlignment: CrossAxisAlignment.start,
                        children: [
                          Text(
                            'Microphone Sensitivity',
                            style: Theme.of(context).textTheme.titleSmall,
                          ),
                          Slider(
                            value: service.sensitivity.toDouble(),
                            min: 500,
                            max: 5000,
                            onChanged: (value) {
                              service.setSensitivity(value.round());
                              service.sendPayload({
                                'sensitivity': value.round(),
                              });
                            },
                          ),
                          Text('${service.sensitivity}/5000'),
                        ],
                      ),
                    ),
                  ),
                  const SystemHealthMonitor(),
                  const SizedBox(height: 20),
                ],
              ),
            ),
          );
        },
      ),
    );
  }
}
