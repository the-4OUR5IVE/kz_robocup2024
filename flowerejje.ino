#include <NewPing.h> // Библиотека для ультразвукового датчика

const int ENB = 3;   
const int IN3 = 4;   
const int IN4 = 5;   
const int ENA = 6;   
const int IN1 = 7;   
const int IN2 = 8;
const int ENC = 11;   
const int IN5 = 10;   
const int IN6 = 9;

const int TRIG_PIN = 13; // Пин триггера ультразвукового датчика
const int ECHO_PIN = 12; // Пин эха ультразвукового датчика
const int MAX_DISTANCE = 200; // Максимальное расстояние для измерения

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

int brightness = 0; // Начальная яркость
int fadeAmount = 5; // Количество увеличения/уменьшения яркости

unsigned long previousMillis = 0;
const long interval = 30; // Интервал для обновления яркости светодиода

enum State {
  NORMAL,
  OBSTACLE_DETECTED,
  FADE_OUT
};

State currentState = NORMAL;
unsigned long obstacleDetectedTime = 0;
const long fadeOutDuration = 1000; // Длительность плавного выключения

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENC, OUTPUT);
  pinMode(IN5, OUTPUT);
  pinMode(IN6, OUTPUT);
  
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  digitalWrite(IN5, LOW);
  digitalWrite(IN6, LOW);
  
  Serial.begin(9600); // Начало серийной связи для отладки
}

void loop() {
  unsigned long currentMillis = millis();
  unsigned int distance = sonar.ping_cm(); // Получение расстояния в см

  switch (currentState) {
    case NORMAL:
      if (distance > 0 && distance < 60) {
        currentState = OBSTACLE_DETECTED;
        obstacleDetectedTime = currentMillis;
        analogWrite(ENA, 150);
        analogWrite(ENB, 150);
        analogWrite(ENC,150);
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
        digitalWrite(IN5, LOW);
        digitalWrite(IN6, HIGH);  
      } else {
        if (currentMillis - previousMillis >= interval) {
          previousMillis = currentMillis;
          
          // Постепенное изменение яркости светодиодов от 0 до 230 и обратно
          analogWrite(ENA, brightness);
          analogWrite(ENB, brightness);
          analogWrite(ENC, brightness);

          brightness += fadeAmount;
          if (brightness <= 0 || brightness >= 150) {
            fadeAmount = -fadeAmount;
          }
        }
      }
      break;

    case OBSTACLE_DETECTED:
      if (currentMillis - obstacleDetectedTime >= 1000) {
        currentState = FADE_OUT;
        brightness = 150;
      }
      break;

    case FADE_OUT:
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;

        // Плавное выключение светодиодов
        brightness -= fadeAmount;
        if (brightness <= 0) {
          brightness = 0;
          currentState = NORMAL; // Переключаемся обратно в нормальный режим
        }

        analogWrite(ENA, brightness);
        analogWrite(ENB, brightness);
        analogWrite(ENC, brightness);
      }
      break;
  }
}
