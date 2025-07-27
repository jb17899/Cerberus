import 'package:flutter/material.dart';
void main() {
  runApp(MaterialApp(
    home:MyApp(),
    theme:ThemeData(
      primarySwatch: Colors.purple
    ),
  ));
}
class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        backgroundColor: Colors.purple,
        title:Center(
          child:Text("Cereberus")
          
        ),
      ),
      body: Container(
        child:Text("hello from flutter!")
      ),
    );
  }
}
