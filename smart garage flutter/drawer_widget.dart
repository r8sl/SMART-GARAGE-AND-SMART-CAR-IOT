import 'package:flutter/material.dart';
import 'package:firebase_auth/firebase_auth.dart';
import 'package:cloud_firestore/cloud_firestore.dart';

class AppDrawer extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    final user = FirebaseAuth.instance.currentUser;

    return Drawer(
      child: ListView(
        padding: EdgeInsets.zero,
        children: <Widget>[
          FutureBuilder<DocumentSnapshot>(
            future: FirebaseFirestore.instance
                .collection('users')
                .doc(user?.uid)
                .get(),
            builder: (context, snapshot) {
              if (snapshot.connectionState == ConnectionState.waiting) {
                return const DrawerHeader(
                  decoration: BoxDecoration(
                    color: Colors.blueGrey,
                  ),
                  child: Center(child: CircularProgressIndicator()),
                );
              }

              if (snapshot.hasError) {
                return const DrawerHeader(
                  decoration: BoxDecoration(
                    color: Colors.blueGrey,
                  ),
                  child: Center(child: Text('Error loading user data')),
                );
              }

              final userData = snapshot.data?.data() as Map<String, dynamic>?;

              return DrawerHeader(
                decoration: BoxDecoration(
                  color: Theme.of(context).primaryColor,
                ),
                child: Column(
                  crossAxisAlignment: CrossAxisAlignment.start,
                  children: [
                    CircleAvatar(
                      radius: 40,
                      backgroundImage: userData?['photoUrl'] != null
                          ? NetworkImage(userData!['photoUrl'])
                          : const AssetImage('assets/user_placeholder.png')
                              as ImageProvider,
                    ),
                    const SizedBox(height: 10),
                    Text(
                      '${userData?['firstName'] ?? 'First Name'} ${userData?['lastName'] ?? 'Last Name'}',
                      style: const TextStyle(
                        color: Colors.white,
                        fontSize: 18,
                      ),
                    ),
                    Text(
                      user?.email ?? 'user@example.com',
                      style: const TextStyle(
                        color: Colors.white70,
                        fontSize: 14,
                      ),
                    ),
                  ],
                ),
              );
            },
          ),
          ListTile(
            leading: const Icon(Icons.home),
            title: const Text('Home'),
            onTap: () {
              Navigator.pushReplacementNamed(context, '/');
            },
          ),
          ListTile(
            leading: const Icon(Icons.settings),
            title: const Text('Servo Control'),
            onTap: () {
              Navigator.pushReplacementNamed(context, '/control');
            },
          ),
          ListTile(
            leading: const Icon(Icons.sensors),
            title: const Text('Readings'),
            onTap: () {
              Navigator.pushReplacementNamed(context, '/dashboard');
            },
          ),
          ListTile(
            leading: const Icon(Icons.person),
            title: const Text('Profile'),
            onTap: () {
              Navigator.pushReplacementNamed(context, '/profile');
            },
          ),
          ListTile(
            leading: const Icon(Icons.info),
            title: const Text('About Us'),
            onTap: () {
              Navigator.pushReplacementNamed(context, '/about');
            },
          ),
          ListTile(
            leading: const Icon(Icons.logout),
            title: const Text('Logout'),
            onTap: () {
              FirebaseAuth.instance.signOut();
              Navigator.pushReplacementNamed(context, '/signin');
            },
          ),
        ],
      ),
    );
  }
}
