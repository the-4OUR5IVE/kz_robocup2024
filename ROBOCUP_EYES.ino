#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

#define SERVO_MIN  0
#define SERVO_MAX  600
#define SERVO_1 0
#define SERVO_2 1
#define SERVO_3 4
#define SERVO_4 5
byte ena = 3;
byte in1 = 4;
byte in2 = 5;
int n = 0;
unsigned long previousMillis = 0;
const long interval = 30000; // interval for continuous task

void setServoAngle(uint8_t servoNum, uint16_t angle) {
  uint16_t pulse = map(angle, 0, 180, SERVO_MIN, SERVO_MAX);
  pwm.setPWM(servoNum, 0, pulse);
}

void setup() {
  pwm.begin();
  pwm.setPWMFreq(60);
  delay(5000);
  
  pinMode(ena, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  Serial.begin(9600);
  
}

void loop() {

  unsigned long currentMillis = millis();
  
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    n=1;

    if (command == "c") {
      Serial.println("Привет, мир!");
      setServoAngle(SERVO_1, 130);
      setServoAngle(SERVO_3,70);
    }
    if (command == "cl") {
      Serial.println("Привет, мир!");
      setServoAngle(SERVO_1, 150);
    }
     if (command == "r") {
      Serial.println("Привет, мир!");
      setServoAngle(SERVO_1, 80);
      setServoAngle(SERVO_3,40);
     }if (command == "сr") {
      Serial.println("Привет, мир!");
      setServoAngle(SERVO_1, 25);
    }
     if (command == "l") {
      Serial.println("Привет, мир!");
      setServoAngle(SERVO_1, 180);
      setServoAngle(SERVO_3,120);
    }
  }

  if ((currentMillis - previousMillis >= interval) && (n==1)) {
    previousMillis = currentMillis;
    setServoAngle(SERVO_2, 180);
    setServoAngle(SERVO_4, 130);
    delay(2000);
    setServoAngle(SERVO_2, 120);
    setServoAngle(SERVO_4, 60);
  }
  }
