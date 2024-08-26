import 'package:flutter/foundation.dart' show kIsWeb;
import 'package:mqtt_client/mqtt_client.dart';
import 'package:mqtt_client/mqtt_server_client.dart';
import 'package:mqtt_client/mqtt_browser_client.dart';

class MQTTService {
  MqttClient? client;
  final String broker = '37e6e9413a0247919be5adca7609dc1e.s1.eu.hivemq.cloud';
  final int port = 8883; // TLS MQTT Port
  final int websocketPort = 8884; // WebSocket Port
  final String username = 'Mohamed';
  final String password = 'Mohamed0';

  MQTTService() {
    if (kIsWeb) {
      // Use MqttBrowserClient for web platform
      client = MqttBrowserClient('wss://$broker:$websocketPort/mqtt', '')
        ..port = websocketPort
        ..logging(on: true)
        ..onConnected = onConnected
        ..onDisconnected = onDisconnected
        ..onSubscribed = onSubscribed
        ..onSubscribeFail = onSubscribeFail
        ..onUnsubscribed = onUnsubscribed;
    } else {
      // Use MqttServerClient for mobile platforms (Android/iOS)
      client = MqttServerClient(broker, '')
        ..port = port
        ..secure = true
        ..onConnected = onConnected
        ..onDisconnected = onDisconnected
        ..onSubscribed = onSubscribed
        ..onSubscribeFail = onSubscribeFail
        ..onUnsubscribed = onUnsubscribed;
    }

    client?.logging(on: true);
    client?.setProtocolV311();
  }

  Future<void> connect() async {
    client?.connectionMessage = MqttConnectMessage()
        .withClientIdentifier('FlutterClient')
        .startClean()
        .authenticateAs(username, password)
        .withWillTopic('willtopic')
        .withWillMessage('My Will message')
        .withWillQos(MqttQos.atLeastOnce);

    try {
      await client?.connect();
    } on Exception catch (e) {
      print('Exception: $e');
      disconnect();
    }

    if (client?.connectionStatus?.state == MqttConnectionState.connected) {
      print('MQTT Connected');
    } else {
      print(
          'ERROR: MQTT Connection failed - ${client?.connectionStatus?.state.toString()}');
      disconnect();
    }
  }

  void disconnect() {
    client?.disconnect();
  }

  void onConnected() {
    print('Connected to MQTT Broker');
    subscribeToTopic(
        'garage/sensor'); // Automatically subscribe to the topic when connected
  }

  void onDisconnected() {
    print('Disconnected from MQTT Broker');
  }

  void onSubscribed(String topic) {
    print('Subscribed to $topic');
  }

  void onSubscribeFail(String topic) {
    print('Failed to subscribe to $topic');
  }

  void onUnsubscribed(String? topic) {
    print('Unsubscribed from $topic');
  }

  void publishMessage(String topic, String message) {
    final builder = MqttClientPayloadBuilder();
    builder.addString(message);
    print('Publishing message "$message" to topic $topic');
    client?.publishMessage(topic, MqttQos.atLeastOnce, builder.payload!);
  }

  void listenToMessages(void Function(String topic, String message) onMessage) {
    client?.updates?.listen((List<MqttReceivedMessage<MqttMessage>> event) {
      final MqttPublishMessage recMess = event[0].payload as MqttPublishMessage;
      final message =
          MqttPublishPayload.bytesToStringAsString(recMess.payload.message);
      final topic = event[0].topic;
      onMessage(topic, message);
    });
  }

  void subscribeToTopic(String topic) {
    client?.subscribe(topic, MqttQos.atLeastOnce);
  }
}
