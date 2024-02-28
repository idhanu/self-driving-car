#include <Arduino.h>

const int TRIGGER_PIN = 2; // Purple
const int ECHO_PIN = 4; // Blue - VCC - white
const int LED_PIN = LED_BUILTIN; // Use the built-in LED pin

const int MOTOR_TURN_LEFT = 15;
const int MOTOR_TURN_RIGHT = 13;
const int MOTOR_MOVE_FORWARD = 14;
const int MOTOR_MOVE_BACKWARD = 12;  

void setup() {
  Serial.begin(74880);
  pinMode(MOTOR_TURN_LEFT, OUTPUT);
  pinMode(MOTOR_TURN_RIGHT, OUTPUT);
  pinMode(MOTOR_MOVE_FORWARD, OUTPUT);
  pinMode(MOTOR_MOVE_BACKWARD, OUTPUT);

  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT); // Set LED pin as output
}

void moveForward(int speed) {
  analogWrite(MOTOR_MOVE_FORWARD, speed);
  digitalWrite(MOTOR_MOVE_BACKWARD, LOW);
}

void moveBackward(int speed) {
  digitalWrite(MOTOR_MOVE_FORWARD, LOW);
  analogWrite(MOTOR_MOVE_BACKWARD, speed);
}

void turnLeft() {
  analogWrite(MOTOR_TURN_LEFT, 230);
  digitalWrite(MOTOR_TURN_RIGHT, LOW);
}

void turnRight() {
  digitalWrite(MOTOR_TURN_LEFT, LOW);
  analogWrite(MOTOR_TURN_RIGHT, 230);
}

void stopTurning() {
  digitalWrite(MOTOR_TURN_LEFT, LOW);
  digitalWrite(MOTOR_TURN_RIGHT, LOW);
}

void stop() {
  digitalWrite(MOTOR_MOVE_FORWARD, LOW);
  digitalWrite(MOTOR_MOVE_BACKWARD, LOW);
}

int measureDistance() {
  static long distances[3]; // Array to store the last three distance readings
  static int index = 0; // Index to keep track of the current position in the array
  
  long duration, distance;
  
  // Send a 10us pulse to the trigger pin
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  
  // Measure the duration of the pulse on the echo pin
  duration = pulseIn(ECHO_PIN, HIGH);
  
  // Calculate the distance in centimeters
  distance = duration * 0.034 / 2;
  
  // Store the distance in the array
  distances[index] = distance;
  
  // Increment the index and wrap around if necessary
  index = (index + 1) % 3;
  
  // Find the minimum distance from the last three readings
  long minDistance = distances[0];
  for (int i = 1; i < 3; i++) {
    if (distances[i] < minDistance) {
      minDistance = distances[i];
    }
  }
  
  // Print the minimum distance to the serial monitor
  // Serial.print("Minimum Distance: ");
  // Serial.print(minDistance);
  // Serial.println(" cm");

  return minDistance;
}

void loop() {
  int distance = measureDistance();
  
  if (distance < 14) {
    stop();
    stopTurning();
  } else if (distance < 40) {
    stopTurning();
    moveBackward(255);
  } else if (distance < 80) {
    turnLeft();
    moveForward(235);
  } else {
    stopTurning();
    moveForward(distance * 2);
  }
  
  delay(100);
}
