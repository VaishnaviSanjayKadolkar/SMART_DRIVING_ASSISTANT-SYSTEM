// Define pins
const int trigPin = 5;
const int echoPin = 18;
const int leftIRPin = 13;
const int rightIRPin = 12;
const int motorIN1 = 23;
const int motorIN2 = 22;
const int motorIN3 = 21;
const int motorIN4 = 19;
const int buzzerPin = 17;

// Variables for distance measurement
long duration;
int distance;

// Setup the pins
void setup() {
  // Start Serial Monitor
  Serial.begin(115200);
  
  // Setup pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(leftIRPin, INPUT);
  pinMode(rightIRPin, INPUT);
  pinMode(motorIN1, OUTPUT);
  pinMode(motorIN2, OUTPUT);
  pinMode(motorIN3, OUTPUT);
  pinMode(motorIN4, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  
  // Initialize motors to stop
  stopMotors();
}

// Main loop
void loop() {
  // Check for obstacle using ultrasonic sensor
  distance = getDistance();
  
  // If obstacle is detected, stop the vehicle and activate the buzzer
  if (distance < 20) { // Obstacle detected within 20 cm
    digitalWrite(buzzerPin, HIGH);  // Turn on buzzer
    stopMotors();  // Stop motors
    Serial.println("Obstacle detected!");
  } else {
    digitalWrite(buzzerPin, LOW);   // Turn off buzzer
    followLane();   // Follow the lane using IR sensors
  }
  
  delay(100); // Small delay to avoid unnecessary looping
}

// Function to calculate distance using ultrasonic sensor
int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  distance = (duration * 0.0343) / 2; // Calculate distance in cm
  
  return distance;
}

// Function to make the vehicle follow the lane using IR sensors
void followLane() {
  int leftIRState = digitalRead(leftIRPin);  // Read the left IR sensor
  int rightIRState = digitalRead(rightIRPin);  // Read the right IR sensor

  // Debugging: Print sensor states
  Serial.print("Left IR: ");
  Serial.print(leftIRState);
  Serial.print(", Right IR: ");
  Serial.println(rightIRState);

  // Both sensors detect the white line (vehicle is centered), move forward
  if (leftIRState == HIGH && rightIRState == HIGH) {
    Serial.println("Both IR sensors HIGH: Moving forward");
    moveForward();
  }
  // Left sensor detects the white line (move right to correct)
  else if (leftIRState == HIGH && rightIRState == LOW) {
    Serial.println("Left IR HIGH, Right IR LOW: Turning right");
    turnRight();
  }
  // Right sensor detects the white line (move left to correct)
  else if (leftIRState == LOW && rightIRState == HIGH) {
    Serial.println("Left IR LOW, Right IR HIGH: Turning left");
    turnLeft();
  }
  // Neither sensor detects the white line (stop or move slowly)
  else {
    Serial.println("Both IR sensors LOW: Stopping");
    stopMotors();
  }
}

// Functions to control the motors
void moveForward() {
  digitalWrite(motorIN1, HIGH);
  digitalWrite(motorIN2, LOW);
  digitalWrite(motorIN3, HIGH);
  digitalWrite(motorIN4, LOW);
}

void turnLeft() {
  digitalWrite(motorIN1, LOW);
  digitalWrite(motorIN2, HIGH);
  digitalWrite(motorIN3, HIGH);
  digitalWrite(motorIN4, LOW);
}

void turnRight() {
  digitalWrite(motorIN1, HIGH);
  digitalWrite(motorIN2, LOW);
  digitalWrite(motorIN3, LOW);
  digitalWrite(motorIN4, HIGH);
}

void stopMotors() {
  digitalWrite(motorIN1, LOW);
  digitalWrite(motorIN2, LOW);
  digitalWrite(motorIN3, LOW);
  digitalWrite(motorIN4, LOW);
}