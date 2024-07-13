#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Ультразвуковые датчики
const int trigPin1 = 9;
const int echoPin1 = 10;
int relay_pin = 3;

// Драйвер PCA9685
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

// Константы для сервоприводов
const int servoMin = 150; // Минимальное значение пульса
const int servoMax = 600; // Максимальное значение пульса

// Пины для RGB светодиодов
const int redPin = 4;
const int greenPin = 5;
const int bluePin = 6;

// Пины для управления двигателем на L298N
const int motorIn1 = 13;
const int motorIn2 = 12;
const int motorEnable = 11;
int n = 1;

String command = "";
unsigned long previousMillis = 0; // Для отслеживания времени
const long interval = 3000; // Интервал в миллисекундах для RGB цикла

void setup() {
  Serial.begin(9600);

  // Настройка ультразвуковых датчиков
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);

  // Настройка пинов для RGB светодиодов
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(relay_pin, OUTPUT);

  // Настройка пинов для управления двигателем на L298N
  pinMode(motorIn1, OUTPUT);
  pinMode(motorIn2, OUTPUT);
  pinMode(motorEnable, OUTPUT);
  digitalWrite(motorIn1, LOW);
  digitalWrite(motorIn2, LOW);
  analogWrite(motorEnable, 0);

  // Инициализация PCA9685
  pwm.begin();
  pwm.setPWMFreq(60);  // Частота ШИМ 60 Гц

  delay(10);
}

// Функция для установки угла сервопривода
void setServoAngle(uint8_t n, float angle) {
  if (angle > 180) angle = 180;
  if (angle < 0) angle = 0;
  uint16_t pulse = servoMin + (angle * (servoMax - servoMin) / 180);
  pwm.setPWM(n, 0, pulse);
}

// Функция для переливания цветов RGB светодиодов
void cycleRGB() {
  analogWrite(redPin, 255);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, 0);
  delay(1000);
  analogWrite(redPin, 0);
  analogWrite(greenPin, 255);
  analogWrite(bluePin, 0);
  delay(1000);
  analogWrite(redPin, 0);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, 255);
  delay(1000);
}

void stopMotor() {
  digitalWrite(motorIn1, LOW);
  digitalWrite(motorIn2, LOW);
  analogWrite(motorEnable, 0);
}

// Функция для измерения расстояния
long measureDistance() {
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);

  long duration = pulseIn(echoPin1, HIGH);
  long distance = duration * 0.034 / 2;
  return distance;
}

void executeCommand(String cmd) {
  if (cmd == "mya") {
    setServoAngle(4, 80); // Установка сервоприводов в заданное положение по команде "mya"
    delay(700);
    setServoAngle(5, 80);
    delay(700);
    setServoAngle(1, 157);
    delay(700);
    setServoAngle(7, 80);
    delay(700);
    analogWrite(redPin, 0);
    analogWrite(greenPin, 255);
    analogWrite(bluePin, 0);
    delay(1000);
  }
}

void loop() {
  if (Serial.available() > 0) {
    setServoAngle(4, 80); // Установка сервоприводов в заданное положение по команде "mya"
    delay(700);
    setServoAngle(5, 80);
    delay(700);
    setServoAngle(1, 157);
    delay(700);
    setServoAngle(7, 80);
    delay(700);
    analogWrite(redPin, 0);
    analogWrite(greenPin, 255);
    analogWrite(bluePin, 0);
    delay(1000);
  }

  long distance = measureDistance();


  if (distance < 50) {
    digitalWrite(relay_pin, HIGH);
    delay(2500);
    digitalWrite(relay_pin, LOW);
    setServoAngle(1, 116);
    delay(700);
    setServoAngle(5, 40);
    delay(700);
    setServoAngle(4, 40);
    delay(700);
    digitalWrite(motorIn1, LOW);
    digitalWrite(motorIn2, HIGH);
    analogWrite(motorEnable, 155);
    delay(1500);
    stopMotor();
    analogWrite(redPin, 255);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 0);
    delay(2000);
    n = 0;
    delay(3000);
  }

  // Использование millis для параллельного запуска cycleRGB
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    cycleRGB(); // Переливание цветов RGB светодиодов
  }
}
