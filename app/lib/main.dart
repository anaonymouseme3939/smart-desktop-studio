import 'package:flutter/material.dart';
import 'package:provider/provider.dart';

import 'services/network_manager_service.dart';
import 'screens/home_screen.dart';

void main() async {
  WidgetsFlutterBinding.ensureInitialized();
  runApp(
    ChangeNotifierProvider(
      create: (_) => NetworkManagerService(),
      child: const SmartDesktopApp(),
    ),
  );
}

class SmartDesktopApp extends StatelessWidget {
  const SmartDesktopApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      debugShowCheckedModeBanner: false,
      title: 'Smart Desktop Studio',
      theme: ThemeData(
        brightness: Brightness.dark,
        colorScheme: ColorScheme.fromSeed(
          seedColor: Colors.teal,
          brightness: Brightness.dark,
        ),
        useMaterial3: true,
        cardTheme: CardTheme(
          color: Colors.grey[900],
          elevation: 4,
        ),
      ),
      home: const HomeScreen(),
    );
  }
}
