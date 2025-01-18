
#include <Servo.h>
#include <SoftwareSerial.h>

// Define pins for sensors and servo
const int trigPinLid = 8;
const int echoPinLid = 9;
const int trigPinWaste = 10;
const int echoPinWaste = 11;
const int servoPin = 6;

// ESP8266 Wi-Fi module
SoftwareSerial esp8266(2, 3); 

// Create Servo object
Servo binServo;

// Variables for distances
long duration, distanceLid, distanceWaste;
const int binDepth = 18; // Depth of the bin in cm

void setup() {
  // Set up pins
  pinMode(trigPinLid, OUTPUT);
  pinMode(echoPinLid, INPUT);
  pinMode(trigPinWaste, OUTPUT);
  pinMode(echoPinWaste, INPUT);

  // Attach servo
  binServo.attach(servoPin);
  binServo.write(0); // Close lid initially

  // Initialize Serial and Wi-Fi module
  Serial.begin(9600);
  esp8266.begin(9600); 

  // Connect to Wi-Fi
  connectToWiFi();
}

void loop() {
  // Measure distance for lid control
  distanceLid = getDistance(trigPinLid, echoPinLid);
  
  // If motion detected (less than 20 cm)
  if (distanceLid < 20) {
    binServo.write(90); // Open lid
  } else {
    binServo.write(0); // Close lid
  }

  delay(100); // Short delay to stabilize sensor readings
}

// Function to measure distance
long getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  long distance = duration * 0.034 / 2; // Calculate distance in cm
  return distance;
}

// Function to connect to Wi-Fi (stub implementation)
void connectToWiFi() {
  // Wi-Fi connection logic here
}
