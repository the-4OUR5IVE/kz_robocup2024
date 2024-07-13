#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Создаем объект для управления PCA9685 с адресом 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

// Ультразвуковой датчик
const int trigPin = 9;
const int echoPin = 10;

// Сервоприводы (номера каналов на PCA9685)
const int servo1Channel = 4;
const int servo2Channel = 5;
const int servo3Channel = 6;
const int servo4Channel = 7;

// RGB светодиоды (номера каналов на PCA9685)
const int redLEDChannel = 4;
const int greenLEDChannel = 5;
const int blueLEDChannel = 6;

// Команда возврата
const char returnCommand[] = "yasm";
bool returnPosition = false;

// Константы для серво
#define SERVOMIN  150 // Минимальная длина импульса для сервопривода
#define SERVOMAX  600 // Максимальная длина импульса для сервопривода

void setup() {
  Serial.begin(9600);

  // Настройка пинов ультразвукового датчика
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Инициализация PCA9685
  pwm.begin();
  pwm.setPWMFreq(60);  // Установка частоты ШИМ в 60 Гц
}

void loop() {
  // Проверка расстояния до цели
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;

  // Если цель на расстоянии меньше 60 см, поворачиваем три сервопривода
  if (distance < 60 && !returnPosition) {
    moveServo(servo1Channel, 40);
    moveServo(servo2Channel, 40);
    moveServo(servo3Channel, 40);
    delay(500); // Небольшая задержка для выполнения движения
  }

  // Чтение команды возврата через Serial
  if (Serial.available() > 0) {
    String input = Serial.readString();
    if (input.equals(returnCommand)) {
      returnPosition = true;
    }
  }

  // Возврат на исходное положение при получении команды
  if (returnPosition) {
    moveServo(servo1Channel, 0);
    moveServo(servo2Channel, 0);
    moveServo(servo3Channel, 0);
    delay(500); // Небольшая задержка для выполнения движения
    returnPosition = false;
  }

  // Параллельное движение 4 сервопривода и управление RGB светодиодами
  moveServo(servo4Channel, random(0, 180));
  setColor(random(0, 255), random(0, 255), random(0, 255));
  delay(500);
}

// Функция для перемещения сервопривода
void moveServo(int channel, int angle) {
  int pulse = map(angle, 0, 180, SERVOMIN, SERVOMAX);
  pwm.setPWM(channel, 0, pulse);
}

// Функция для установки цвета RGB светодиодов
void setColor(int red, int green, int blue) {
  pwm.setPWM(redLEDChannel, 0, map(red, 0, 255, 0, 4095));
  pwm.setPWM(greenLEDChannel, 0, map(green, 0, 255, 0, 4095));
  pwm.setPWM(blueLEDChannel, 0, map(blue, 0, 255, 0, 4095));
}
