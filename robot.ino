#include <WiFi.h>
#include <HTTPClient.h>

#define TRIG_PIN 13
#define ECHO_PIN 34
#define SOUND_SPEED 0.034  // cm/µs
#define MOTOR_APIN1 26
#define MOTOR_APIN2 27
#define MOTOR_BPIN1 25
#define MOTOR_BPIN2 33
#define ENABLE1_PIN 14
#define ENABLE2_PIN 32

// WiFi Credentials
const char* apSSID = "GestureRobotNetwork";
const char* apPassword = "RobotControl123";

// Server URL
const String serverUrl = "http://192.168.4.1";

// Task Handles
TaskHandle_t ultrasonicTaskHandle;
TaskHandle_t flexSensorTaskHandle;

// Global Variables
long duration;
float distanceCm;
HTTPClient http;

void setup() {
  Serial.begin(115200);

  // Motor pins setup
  pinMode(MOTOR_APIN1, OUTPUT);
  pinMode(MOTOR_APIN2, OUTPUT);
  pinMode(MOTOR_BPIN1, OUTPUT);
  pinMode(MOTOR_BPIN2, OUTPUT);
  pinMode(ENABLE1_PIN, OUTPUT);
  pinMode(ENABLE2_PIN, OUTPUT);

  // Ultrasonic sensor pins setup
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Connect to WiFi Access Point
  WiFi.begin(apSSID, apPassword);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");

  // Create FreeRTOS tasks
  xTaskCreatePinnedToCore(ultrasonicTask, "Ultrasonic Task", 10000, NULL, 1, &ultrasonicTaskHandle, 0); // Higher priority
  xTaskCreatePinnedToCore(flexSensorTask, "Flex Sensor Task", 10000, NULL, 2, &flexSensorTaskHandle, 1); // Lower priority
}

void ultrasonicTask(void* parameter) {
  Serial.println("Ultrasonic Task running...");

  for (;;) {
    // Measure distance using ultrasonic sensor
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    duration = pulseIn(ECHO_PIN, HIGH);
    distanceCm = duration * SOUND_SPEED / 2;

    if (distanceCm < 10) {
      // Obstacle detected, stop motors and suspend the flex sensor task
      stopMotors();
      Serial.println("Obstacle detected! Stopping motors.");
      vTaskSuspend(flexSensorTaskHandle);  // Suspend Flex Sensor Task
    } else {
      // No obstacle, resume flex sensor task if suspended
      if (eTaskGetState(flexSensorTaskHandle) == eSuspended) {
        Serial.println("No obstacle, resuming Flex Sensor Task.");
        vTaskResume(flexSensorTaskHandle);  // Resume Flex Sensor Task
      }
      moveForward();  // Move forward if no obstacle
    }

    // Wait for 100ms before checking again
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void flexSensorTask(void* parameter) {
  Serial.println("Flex Sensor Task running...");

  for (;;) {
    // Get command from the server
    String command = getCommandFromServer();
    if (!command.isEmpty()) {
      Serial.println(command);
      processCommand(command);
    }

    // Wait for 200ms
    vTaskDelay(pdMS_TO_TICKS(200));
  }
}

String getCommandFromServer() {
  String fullUrl = serverUrl + "/move";
  http.begin(fullUrl);
  int httpResponseCode = http.GET();
  String command = "";

  if (httpResponseCode > 0) {
    command = http.getString();
  }
  http.end();

  return command;
}

void processCommand(String command) {
  stopMotors();
  if (command == "FORWARD") {
    moveForward();
  } else if (command == "BACKWARD") {
    moveBackward();
  } else if (command == "LEFT") {
    turnLeft();
  } else if (command == "RIGHT") {
    turnRight();}
  // } else if (command == "STOP") {
  //   Serial.println("yo");
  //   stopMotors();
  // }
}

void moveForward() {
  digitalWrite(ENABLE1_PIN, HIGH);
  digitalWrite(ENABLE2_PIN, HIGH);
  digitalWrite(MOTOR_APIN1, LOW);
  digitalWrite(MOTOR_APIN2, HIGH);
  digitalWrite(MOTOR_BPIN1, LOW);
  digitalWrite(MOTOR_BPIN2, HIGH);
}

void moveBackward() {
  digitalWrite(ENABLE1_PIN, HIGH);
  digitalWrite(ENABLE2_PIN, HIGH);
  digitalWrite(MOTOR_APIN1, HIGH);
  digitalWrite(MOTOR_APIN2, LOW);
  digitalWrite(MOTOR_BPIN1, HIGH);
  digitalWrite(MOTOR_BPIN2, LOW);
}

void turnLeft() {
  digitalWrite(ENABLE1_PIN, HIGH);
  digitalWrite(ENABLE2_PIN, HIGH);
  digitalWrite(MOTOR_APIN1, LOW);
  digitalWrite(MOTOR_APIN2, HIGH);
  digitalWrite(MOTOR_BPIN1, HIGH);
  digitalWrite(MOTOR_BPIN2, LOW);
}

void turnRight() {
  digitalWrite(ENABLE1_PIN, HIGH);
  digitalWrite(ENABLE2_PIN, HIGH);
  digitalWrite(MOTOR_APIN1, HIGH);
  digitalWrite(MOTOR_APIN2, LOW);
  digitalWrite(MOTOR_BPIN1, LOW);
  digitalWrite(MOTOR_BPIN2, HIGH);
}

void stopMotors() {
  digitalWrite(ENABLE1_PIN, HIGH);
  digitalWrite(ENABLE2_PIN, HIGH);
  digitalWrite(MOTOR_APIN1, LOW);
  digitalWrite(MOTOR_APIN2, LOW);
  digitalWrite(MOTOR_BPIN1, LOW);
  digitalWrite(MOTOR_BPIN2, LOW);
  delay(500);
}

void loop() {
  // Empty because FreeRTOS manages the tasks
}
