import 'package:flutter/material.dart';
import 'drawer_widget.dart'; // Import the drawer widget
import '../mqtt.dart'; // Import MQTT service

class ControlScreen extends StatefulWidget {
  final MQTTService mqttService;

  const ControlScreen({super.key, required this.mqttService});

  @override
  _ControlScreenState createState() => _ControlScreenState();
}

class _ControlScreenState extends State<ControlScreen> {
  double _garageDoorServo = 90;
  double _garageTendaServo = 90;

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Control Panel'),
      ),
      drawer: AppDrawer(), // Add the drawer here
      body: Padding(
        padding: const EdgeInsets.all(16.0),
        child: Column(
          children: [
            _buildServoSlider(
              'Garage Door Servo',
              _garageDoorServo,
              70, // Min value for the garage door servo
              180, // Max value for the garage door servo
              (value) {
                setState(() {
                  _garageDoorServo = value;
                });
                widget.mqttService.publishMessage(
                    'garage/door', value.toString()); // Publish to MQTT
              },
            ),
            _buildServoSlider(
              'Garage Tenda Servo',
              _garageTendaServo,
              0, // Min value for the garage tenda servo
              90, // Max value for the garage tenda servo
              (value) {
                setState(() {
                  _garageTendaServo = value;
                });
                widget.mqttService.publishMessage(
                    'garage/tenda', value.toString()); // Publish to MQTT
              },
            ),
          ],
        ),
      ),
    );
  }

  Widget _buildServoSlider(String label, double value, double min, double max,
      ValueChanged<double> onChanged) {
    return Card(
      elevation: 8,
      shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(16)),
      child: Padding(
        padding: const EdgeInsets.all(16.0),
        child: Column(
          children: [
            Text(
              label,
              style: const TextStyle(fontSize: 18, fontWeight: FontWeight.bold),
            ),
            Slider(
              value: value,
              min: min, // Use specific min value for the slider
              max: max, // Use specific max value for the slider
              onChanged: onChanged,
              activeColor: Colors.deepOrange,
              inactiveColor: Colors.blueGrey,
            ),
            Text('${value.toStringAsFixed(0)}°',
                style: const TextStyle(fontSize: 16)),
          ],
        ),
      ),
    );
  }
}
