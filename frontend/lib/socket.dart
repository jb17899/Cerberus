import "dart:io";
import "dart:convert";
import "dart:typed_data";

const host = "127.0.0.1", port = 12048;


String readfull(List<int> data) {
  String val = String.fromCharCodes(data.sublist(4));
  return val;
}

String sendfull(String text) {
  // Limit the message length to 1024 bytes (adjust as needed)
  final maxLen = 1024;
  final bytes = utf8.encode(text);
  final limitedBytes = bytes.length > maxLen ? bytes.sublist(0, maxLen) : bytes;
  final len = limitedBytes.length;
    final lenBytes = Uint8List(4);
  final lenByteData = ByteData.sublistView(lenBytes);
  lenByteData.setInt32(0, len, Endian.little);
  String lenStr = String.fromCharCodes(lenBytes);
  String val = lenStr;
  val = val+text;
  return val;
}

void main() async {
  final socket = await Socket.connect(host, port);
  print("Connected to server.");

  // Send first message
  stdout.write("You: ");
  final name = stdin.readLineSync() ?? "Anonymous";
  socket.write(sendfull(name));

  // Start listening to server
  socket.listen((data) async {
    final response = readfull(data);
    print("Server: $response");

    // Prompt user to send another message
    stdout.write("You: ");
    final input = stdin.readLineSync();
    if (input == null || input.toLowerCase() == 'exit') {
      print("Closing connection.");
      await socket.close();
      exit(0);
    }
    socket.write(sendfull(input));
  });
}
