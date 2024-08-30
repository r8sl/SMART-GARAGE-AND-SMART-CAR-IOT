import 'package:flutter/material.dart';
import '../mqtt.dart'; // Import MQTT wrapper
import 'drawer_widget.dart'; // Import the drawer widget
import 'dart:convert';

class DashboardScreen extends StatefulWidget {
  final MQTTService mqttService;

  const DashboardScreen({super.key, required this.mqttService});

  @override
  _DashboardScreenState createState() => _DashboardScreenState();
}

class _DashboardScreenState extends State<DashboardScreen> {
  String _irSensorValue = "Waiting...";
  String _temperatureValue = "Waiting...";
  String _gasSensorValue = "Waiting...";
  String _lcdStatusValue = "Waiting...";

  @override
  void initState() {
    super.initState();
    widget.mqttService.listenToMessages(_onMessageReceived);
    widget.mqttService
        .subscribeToTopic('garage/sensor'); // Subscribe to the correct topic
  }

  void _onMessageReceived(String topic, String message) {
    if (topic == 'garage/sensor') {
      final sensorData = parseJson(message);
      setState(() {
        _irSensorValue = sensorData['ir'].toString();
        _temperatureValue = "${sensorData['temperature']}°C";
        _gasSensorValue = sensorData['gas'].toString();
        _lcdStatusValue = sensorData['lcd'];
      });
    }
  }

  Map<String, dynamic> parseJson(String jsonString) {
    try {
      return jsonDecode(jsonString);
    } catch (e) {
      print('Failed to decode JSON: $e');
      return {};
    }
  }

  Widget _buildSensorCard(String title, String value, IconData icon) {
    return Card(
      elevation: 8,
      shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(16)),
      child: Padding(
        padding: const EdgeInsets.all(16.0),
        child: Column(
          children: [
            Icon(icon, size: 40, color: Colors.deepOrange),
            const SizedBox(height: 16),
            Text(
              title,
              style: const TextStyle(fontSize: 18, fontWeight: FontWeight.bold),
            ),
            const SizedBox(height: 8),
            Text(
              value,
              style: const TextStyle(fontSize: 24, fontWeight: FontWeight.bold),
            ),
          ],
        ),
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Dashboard'),
      ),
      drawer: AppDrawer(),
      body: Padding(
        padding: const EdgeInsets.all(16.0),
        child: GridView.count(
          crossAxisCount: 2,
          childAspectRatio: 0.8,
          mainAxisSpacing: 16,
          crossAxisSpacing: 16,
          children: [
            _buildSensorCard('IR Sensor', _irSensorValue, Icons.sensors),
            _buildSensorCard(
                'Temperature', _temperatureValue, Icons.thermostat),
            _buildSensorCard('Gas Sensor', _gasSensorValue, Icons.warning),
            _buildSensorCard('LCD Status', _lcdStatusValue, Icons.check_circle),
          ],
        ),
      ),
      floatingActionButton: FloatingActionButton.extended(
        onPressed: () {
          Navigator.pushNamed(context, '/control');
        },
        label: const Text('Control Panel'),
        icon: const Icon(Icons.settings),
      ),
    );
  }
}
