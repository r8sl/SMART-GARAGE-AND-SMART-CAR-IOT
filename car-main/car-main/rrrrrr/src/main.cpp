#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <DabbleESP32.h>

// Right motor
int enableRightMotor = 12; 
int rightMotorPin1 = 32;
int rightMotorPin2 = 33;

// Left motor
int enableLeftMotor = 14;
int leftMotorPin1 = 26;
int leftMotorPin2 = 27;

#define MAX_MOTOR_SPEED 255

const int PWMFreq = 1000; /* 1 KHz */
const int PWMResolution = 8;
const int rightMotorPWMSpeedChannel = 4;
const int leftMotorPWMSpeedChannel = 5;

// Additional pins
int buzzerPin = 13;   // Buzzer connected to pin 13
int ledPin1 = 25;     // LED 1 connected to pin 25 (Triangle button and Ultrasonic)
int ledPin2 = 23;     // LED 2 connected to pin 23 (Square button and Ultrasonic)

// Ultrasonic sensor pins
const int trigPin = 21; // Trigger pin for ultrasonic sensor
const int echoPin = 19; // Echo pin for ultrasonic sensor

const int parkingThreshold = 20; // Distance in cm to trigger parking mode

unsigned long previousMillis = 0; // Store the last time the sensor was checked
const long interval = 100; // Interval at which to check the sensor (in milliseconds)

void rotateMotor(int rightMotorSpeed, int leftMotorSpeed) {
    if (rightMotorSpeed < 0) {
        digitalWrite(rightMotorPin1, LOW);
        digitalWrite(rightMotorPin2, HIGH);    
    } else if (rightMotorSpeed > 0) {
        digitalWrite(rightMotorPin1, HIGH);
        digitalWrite(rightMotorPin2, LOW);      
    } else {
        digitalWrite(rightMotorPin1, LOW);
        digitalWrite(rightMotorPin2, LOW);      
    }
  
    if (leftMotorSpeed < 0) {
        digitalWrite(leftMotorPin1, LOW);
        digitalWrite(leftMotorPin2, HIGH);    
    } else if (leftMotorSpeed > 0) {
        digitalWrite(leftMotorPin1, HIGH);
        digitalWrite(leftMotorPin2, LOW);      
    } else {
        digitalWrite(leftMotorPin1, LOW);
        digitalWrite(leftMotorPin2, LOW);      
    }
  
    ledcWrite(rightMotorPWMSpeedChannel, abs(rightMotorSpeed));
    ledcWrite(leftMotorPWMSpeedChannel, abs(leftMotorSpeed));  
}

void setUpPinModes() {
    pinMode(enableRightMotor, OUTPUT);
    pinMode(rightMotorPin1, OUTPUT);
    pinMode(rightMotorPin2, OUTPUT);
  
    pinMode(enableLeftMotor, OUTPUT);
    pinMode(leftMotorPin1, OUTPUT);
    pinMode(leftMotorPin2, OUTPUT);

    // Set up additional pins
    pinMode(buzzerPin, OUTPUT);
    pinMode(ledPin1, OUTPUT);
    pinMode(ledPin2, OUTPUT);

    // Set up ultrasonic sensor pins
    pinMode(trigPin, OUTPUT); // Set the trigger pin as output
    pinMode(echoPin, INPUT);  // Set the echo pin as input

    // Set up PWM for speed
    ledcSetup(rightMotorPWMSpeedChannel, PWMFreq, PWMResolution);
    ledcSetup(leftMotorPWMSpeedChannel, PWMFreq, PWMResolution);  
    ledcAttachPin(enableRightMotor, rightMotorPWMSpeedChannel);
    ledcAttachPin(enableLeftMotor, leftMotorPWMSpeedChannel); 

    rotateMotor(0, 0); 
}

long getDistance() {
    // Send a 10us pulse to trigger the sensor
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Measure the duration of the echo pulse
    long duration = pulseIn(echoPin, HIGH);

    // Calculate distance in cm
    return duration * 0.034 / 2;
}

void handleUltrasonicSensor() {
    long distance = getDistance();

    // Print distance to Serial Monitor
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    if (distance <= 5) {
        // Very close distance: steady buzzer and LEDs on
        digitalWrite(buzzerPin, HIGH);
        digitalWrite(ledPin1, HIGH);
        digitalWrite(ledPin2, HIGH);
    } else if (distance <= parkingThreshold && distance > 0) {
        // Close distance: buzzer and LEDs blink
        digitalWrite(buzzerPin, HIGH);
        digitalWrite(ledPin1, HIGH);
        digitalWrite(ledPin2, HIGH);
        delay(100);   // Short delay for blinking effect
        digitalWrite(buzzerPin, LOW);
        digitalWrite(ledPin1, LOW);
        digitalWrite(ledPin2, LOW);
        delay(100);   // Short delay for blinking effect
    } else {
        // Safe distance: turn off buzzer and LEDs
        digitalWrite(buzzerPin, LOW);
        digitalWrite(ledPin1, LOW);
        digitalWrite(ledPin2, LOW);
    }
}

void setup() {
    Serial.begin(9600);  // Initialize serial communication
    setUpPinModes();
    Dabble.begin("MyBluetoothCar"); 
}

void loop() {
    unsigned long currentMillis = millis();

    // Check ultrasonic sensor at regular intervals
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis; // Save the last time the sensor was checked
        handleUltrasonicSensor();
    }

    int rightMotorSpeed = 0;
    int leftMotorSpeed = 0;
    Dabble.processInput();

    if (GamePad.isUpPressed()) {
        rightMotorSpeed = MAX_MOTOR_SPEED;
        leftMotorSpeed = MAX_MOTOR_SPEED;
    }

    if (GamePad.isDownPressed()) {
        rightMotorSpeed = -MAX_MOTOR_SPEED;
        leftMotorSpeed = -MAX_MOTOR_SPEED;
    }

    if (GamePad.isLeftPressed()) {
        rightMotorSpeed = MAX_MOTOR_SPEED;
        leftMotorSpeed = -MAX_MOTOR_SPEED;
    }

    if (GamePad.isRightPressed()) {
        rightMotorSpeed = -MAX_MOTOR_SPEED;
        leftMotorSpeed = MAX_MOTOR_SPEED;
    }

    rotateMotor(rightMotorSpeed, leftMotorSpeed);

    // Buzzer control with Circle button
    if (GamePad.isCirclePressed()) {
        digitalWrite(buzzerPin, HIGH);  // Turn on the buzzer
    } else {
        digitalWrite(buzzerPin, LOW);   // Turn off the buzzer
    }

    // LED control with Triangle and Square buttons
    if (GamePad.isTrianglePressed()) {
        digitalWrite(ledPin1, HIGH);    // Turn on LED 1
    } else {
        digitalWrite(ledPin1, LOW);     // Turn off LED 1
    }

    if (GamePad.isSquarePressed()) {
        digitalWrite(ledPin2, HIGH);    // Turn on LED 2
    } else {
        digitalWrite(ledPin2, LOW);     // Turn off LED 2
    }
}
