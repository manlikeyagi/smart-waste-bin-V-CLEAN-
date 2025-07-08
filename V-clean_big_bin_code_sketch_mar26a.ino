#include <AccelStepper.h>

// Pin definitions for the stepper motor
#define dirPin 2
#define stepPin 3
#define enablePin 4

// Pin definitions for the ultrasonic sensor
#define trigPin 9
#define echoPin 10

AccelStepper stepper(AccelStepper::DRIVER, stepPin, dirPin);

// Distance threshold for triggering the motor (in centimeters)
const int triggerDistance = 40;

long stepsToOpen = 4500;  // You can adjust this to change how far the lid opens
long duration, distance;

void setup() {
  pinMode(enablePin, OUTPUT);
  digitalWrite(enablePin, LOW); // Enable driver

  stepper.setMaxSpeed(1000);      // Set motor speed
  stepper.setAcceleration(500);   // Set acceleration

  // Set up the ultrasonic sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  Serial.begin(9600);  // Start serial communication for debugging
}

void loop() {
  // Trigger the ultrasonic sensor to send a pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the pulse duration from the echo pin
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.0344 / 2;  // Convert to centimeters

  // Print distance to serial monitor (for debugging)
  Serial.print("Distance: ");
  Serial.println(distance);

  // Check if the distance is less than or equal to the threshold
  if (distance <= triggerDistance) {
    // Move lid up
    stepper.moveTo(stepsToOpen);
    while (stepper.distanceToGo() != 0) {
      stepper.run();
    }

    delay(5000);  // Wait 5 seconds before moving the lid back down

    // Move lid back down
    stepper.moveTo(0);
    while (stepper.distanceToGo() != 0) {
      stepper.run();
    }
  }
  
  delay(100);  // Short delay before the next sensor reading
}
