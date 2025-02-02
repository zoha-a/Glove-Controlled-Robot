#include <WiFi.h>
#include <WebServer.h>

// Access Point Credentials
const char* apSSID = "GestureRobotNetwork";
const char* apPassword = "RobotControl123";

// Web Server Setup
WebServer server(80);  // Standard HTTP port

// Flex Sensor Pin
const int flexPin = A0;  // Analog pin for flex sensor

void setup() {
  Serial.begin(115200);
  
  // Set up Access Point
  WiFi.softAP(apSSID, apPassword);
  
  Serial.println("Access Point Started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  // Define server routes
  //server.on("/", HTTP_GET, handleRoot);
  server.on("/move", HTTP_GET, handleMove);
  
  // Start server
  server.begin();
}

void loop() {
  // Handle client requests
  int flexValue;
  flexValue = analogRead(flexPin);
  Serial.print("sensor: ");
  Serial.println(flexValue);
 
  delay(20);
  server.handleClient();
}

// void handleRoot() {
//   String html = "<html><body>";
//   html += "<h1>Gesture Controlled Robot</h1>";
//   html += "<p>Current Flex Sensor Value: " + String(analogRead(flexPin)) + "</p>";
//   html += "</body></html>";
//   server.send(200, "text/html", html);
// }

void handleMove() {
  // Read flex sensor value
  int flexValue = analogRead(flexPin);
  
  // Determine movement command based on flex sensor
  String command = mapFlexToCommand(flexValue);
  
  // Send response to client
  server.send(200, "text/plain", command);
  delay(500);
}

String mapFlexToCommand(int flexValue) {
  if (flexValue < 2300) {
    return "BACKWARD";
  }
  if (flexValue > 2700) {
    return "FORWARD";
  }
  else {
    return "STOP";
  }
}