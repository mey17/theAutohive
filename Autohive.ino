// First, include the required libraries for the Servo motor and LiquidCrystal display
#include <Servo.h>
#include <LiquidCrystal.h>

// Define pin assignments for dynamic buzzer, green and red LED lights
#define dynamic 8
#define red 4
#define green 5

// Define pins for the second ultrasonic sensor
#define trigPin2 A0
#define echoPin2 A1

// Initialize Servo and LiquidCrystal objects
Servo servo;
LiquidCrystal lcd(13, 12, 11, 7, 6, 2);

// Define pins for the first ultrasonic sensor
const int trigPin = 9;
const int echoPin = 10;

// Initialize variables for ultrasonic measurements and parking space count
float duration, duration2;
int distance, distance2, count = 0;

void setup() {
  // Begin serial communication and initialize the LCD display
  Serial.begin(9600); 
  lcd.begin(16, 2);

  // Display a welcome message on the LCD
  lcd.setCursor(1, 0);
  lcd.print("Welcome to");
  lcd.setCursor(3, 1);
  lcd.print("the Autohive");

  // Set up pins for dynamic buzzer and LEDs
  pinMode(dynamic, OUTPUT);
  servo.attach(3);
  pinMode(trigPin, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(echoPin, INPUT);

  // Generate a dynamic notification sound
  for (int i = 0; i < 2; i++) {
    tone(dynamic, 1600);
    delay(200);
    tone(dynamic, 2000);
    delay(200);
  }
  noTone(dynamic);

  // Initialize and display the LCD
  lcd.noDisplay();
  delay(500);
  lcd.display();
  delay(500);
}

// Function to perform distance measurement using the first ultrasonic sensor
void distanceometer() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
}

// Function to perform distance measurement using the second ultrasonic sensor
void distanceometer2() {
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
}

// Function to handle the scenario when all parking spaces are occupied
void full() {
  if (count == 4) {
    lcd.setCursor(0, 1);
    lcd.print("No Parking Place");

    // Generate a dynamic sound indicating no available parking
    for (int i = 0; i < 5000; i += 20) {
      delay(2);
      tone(dynamic, i);
    }
    for (int i = 5000; i >= 0; i -= 20) {
      delay(2);
      tone(dynamic, i);
    }
    noTone(dynamic);
  }

  // Display a message on the LCD indicating no available parking
  lcd.noDisplay();
  delay(500);
  lcd.display();
  delay(500);
}

void loop() {
  // Measure distance using the first ultrasonic sensor
  distanceometer();
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  // Measure distance using the second ultrasonic sensor
  distanceometer2();
  duration2 = pulseIn(echoPin2, HIGH);
  distance2 = duration2 * 0.034 / 2;

  // Check if there is available parking space and open the barrier
  if (distance > 5 && distance < 7 && count != 4) {
    servo.write(90);
    digitalWrite(red, LOW);
    digitalWrite(green, HIGH);
    delay(2500);
    count += 1;
    delay(500);

    // Display the remaining available parking spaces on the LCD
    lcd.setCursor(0, 0);
    lcd.print("Free Place ");
    lcd.print(4 - count, DEC);
  }
  // Handle the scenario when all parking spaces are occupied
  else if (distance > 5 && distance < 7 && count == 4) {
    full();
  }
  // Close the barrier when no car is approaching
  else {
    servo.write(0);
    digitalWrite(green, LOW);
    digitalWrite(red, HIGH);
  }

  // Check if a car has left, update the available parking spaces, and close the barrier
  if (distance2 > 5 && distance2 < 7 && count != 0) {
    servo.write(90);
    digitalWrite(red, LOW);
    digitalWrite(green, HIGH);
    delay(2500);

    count -= 1;
    delay(500);

    // Display the remaining available parking spaces on the LCD
    lcd.setCursor(0, 0);
    lcd.print("Free Place ");
    lcd.print(4 - count, DEC);
  }
}
