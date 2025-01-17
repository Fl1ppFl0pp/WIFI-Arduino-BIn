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

  // Paconnect par
  connectToWiFi();
}

void loop() {
  // Measure distance for lid control
  distanceLid = getDistance(trigPinLid, echoPinLid);
  
  // If motion detected (less than 20 cm)
  if (distanceLid < 20) {
    binServo.write(90); // Open lid
    delay(2000); // Wait for 2 seconds
    binServo.write(0); // Close lid
  }

  // Measure distance for waste monitoring
  distanceWaste = getDistance(trigPinWaste, echoPinWaste);

  // Waste Level as %
  int wasteLevel = map(distanceWaste, 0, binDepth, 100, 0);
  wasteLevel = constrain(wasteLevel, 0, 100);

  // Send that data through WIFI
  sendData(wasteLevel);

  delay(1000); // Update/Second
}

// DIstance Measurer 9000
long getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  long distance = duration * 0.034 / 2; // in CM
  return distance;
}

// Connect that mf to WIFI
void connectToWiFi() {
  esp8266.println("AT+CWJAP=\"ZTE_2.4G_ccAPgG\",\"phhsfM3p\""); // Akin na WIFI mo toll
  delay(5000); // Malag wait
  esp8266.println("AT+CIPMUX=1"); // Pag gusto kumonnect ng tropa
  delay(1000);
  esp8266.println("AT+CIPSERVER=1,80"); // Start server on port 80
  delay(1000);
}

// Function to send data to connected device
void sendData(int wasteLevel) {
  String data = "Waste Level: " + String(wasteLevel) + "%\r\n";
  esp8266.println("AT+CIPSEND=0," + String(data.length() + 2)); // Adjust for data length
  delay(100);
  esp8266.println(data);
  delay(100);
}

