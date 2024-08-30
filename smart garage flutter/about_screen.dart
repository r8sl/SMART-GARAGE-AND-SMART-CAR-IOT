import 'package:flutter/material.dart';
import 'drawer_widget.dart'; // Import the drawer widget

class AboutScreen extends StatelessWidget {
  const AboutScreen({super.key});

  // Define the list of users
  final List<Map<String, String>> users = const [
    {
      'name': 'Mohamed Nader',
      'location': 'Alexandria, Egypt',
      'phone': '+20123456789',
      'image': 'assets/about_placeholder.png',
    },
    {
      'name': 'Aly Waleed',
      'location': 'Alexandria, Egypt',
      'phone': '+20123456780',
      'image': 'assets/about_placeholder.png',
    },
    {
      'name': 'Ahmed Hamed',
      'location': 'Alexandria, Egypt',
      'phone': '+20123456781',
      'image': 'assets/about_placeholder.png',
    },
    {
      'name': 'Marwan Abdelhamid',
      'location': 'Alexandria, Egypt',
      'phone': '+20123456782',
      'image': 'assets/about_placeholder.png',
    },
    {
      'name': 'Mahmoud Mohamed',
      'location': 'Alexandria, Egypt',
      'phone': '+20123456783',
      'image': 'assets/about_placeholder.png',
    },
    {
      'name': 'Peter Hany',
      'location': 'Alexandria, Egypt',
      'phone': '+20123456784',
      'image': 'assets/about_placeholder.png',
    },
    {
      'name': 'Filopatier Awad',
      'location': 'Alexandria, Egypt',
      'phone': '+20123456785',
      'image': 'assets/about_placeholder.png',
    },
  ];

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('About Us'),
      ),
      drawer: AppDrawer(), // Add the drawer here
      body: Padding(
        padding: const EdgeInsets.all(16.0),
        child: ListView.builder(
          itemCount: users.length,
          itemBuilder: (context, index) {
            final user = users[index];
            return Card(
              margin: const EdgeInsets.symmetric(vertical: 8),
              child: Padding(
                padding: const EdgeInsets.all(16.0),
                child: Column(
                  children: [
                    CircleAvatar(
                      radius: 50,
                      backgroundImage: AssetImage(user['image']!),
                    ),
                    const SizedBox(height: 16),
                    Text(
                      user['name']!,
                      style: const TextStyle(
                          fontSize: 24, fontWeight: FontWeight.bold),
                    ),
                    const SizedBox(height: 8),
                    Text(
                      'Location: ${user['location']}',
                      style:
                          const TextStyle(fontSize: 16, color: Colors.white70),
                    ),
                    const SizedBox(height: 8),
                    Text(
                      'Phone: ${user['phone']}',
                      style:
                          const TextStyle(fontSize: 16, color: Colors.white70),
                    ),
                  ],
                ),
              ),
            );
          },
        ),
      ),
    );
  }
}
