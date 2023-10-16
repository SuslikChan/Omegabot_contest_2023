#include <Servo.h>
Servo servo1;
Servo servo2;
int datPin=11;
bool flag= false;

const int leftSensorPin = A3;
const int rightSensorPin = A2;
const int leftMotorPin1 = 6;
const int leftMotorPin2 = 7;
const int rightMotorPin1 = 5;
const int rightMotorPin2 = 4;

#define PIN_TRIG 3
#define PIN_ECHO 2

long duration, cm;

// Пороговые значения для определения черной и белой линии
const int blackThreshold = 700;
const int whiteThreshold = 200;

// Скорость движения моторов (от 0 до 255)
const int motorSpeed = 140;
const int deltaSpeed = 40;

void setup() {

  Serial.begin(9600);
  servo1.attach(9);
  servo2.attach(10);
  servo1.write(110);
  servo2.write(20 0);
  pinMode(datPin,INPUT);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
// Настройка пинов датчиков линии как входов
 pinMode(leftSensorPin, INPUT);
 pinMode(rightSensorPin, INPUT);
 
// Настройка пинов моторов как выходов
 pinMode(leftMotorPin1, OUTPUT);
 pinMode(leftMotorPin2, OUTPUT);
 pinMode(rightMotorPin1, OUTPUT);2
 pinMode(rightMotorPin2, OUTPUT);
}


void loop() {

  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(5);
  digitalWrite(PIN_TRIG, HIGH);

  // Выставив высокий уровень сигнала, ждем около 10 микросекунд. В этот момент датчик будет посылать сигналы с частотой 40 КГц.
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  //  Время задержки акустического сигнала на эхолокаторе.
  duration = pulseIn(PIN_ECHO, HIGH);

  // Теперь осталось преобразовать время в расстояние
  cm = (duration / 2) / 29.1;


  if ((cm<=3 )and(flag== false)){
    servo2.write(30);
    delay(250);
    servo1.write(170);
    flag= true;
  }

// Чтение значений с датчиков линии
 int leftSensorValue = analogRead(leftSensorPin);
 int rightSensorValue = analogRead(rightSensorPin);
 
// Определение цвета линии на основе пороговых значений
 bool isLeftLineBlack = leftSensorValue > blackThreshold;
 bool isRightLineBlack = rightSensorValue > blackThreshold;
 
// Проверка состояния линии и управление моторами
 if (isLeftLineBlack && isRightLineBlack) {
// Оба датчика на черной линии - движение вперед
  moveForward();
 } else if (!isLeftLineBlack && isRightLineBlack) {
// Только правый датчик на черной линии - поворот влево
   turnLeft();
   } else if (isLeftLineBlack && !isRightLineBlack) {
// Только левый датчик на черной линии - поворот вправо
 turnRight();
 } else {
// Оба датчика на белой линии - остановка
//  stopMoving();
 }
}

// Функция для движения вперед
void moveForward() {
 analogWrite(leftMotorPin1, motorSpeed);
 digitalWrite(leftMotorPin2, HIGH);
 analogWrite(rightMotorPin1, motorSpeed);
 digitalWrite(rightMotorPin2, HIGH);
}

// Функция для поворота влево
void turnLeft() {
 analogWrite(leftMotorPin1, motorSpeed);
 digitalWrite(leftMotorPin2, LOW);
 analogWrite(rightMotorPin1, motorSpeed+deltaSpeed);
 digitalWrite(rightMotorPin2, HIGH);
}

// Функция для поворота вправо
void turnRight() {
 analogWrite(leftMotorPin1, motorSpeed+deltaSpeed);
 digitalWrite(leftMotorPin2, HIGH);
 analogWrite(rightMotorPin1, motorSpeed);
 digitalWrite(rightMotorPin2, LOW);
}

// Функция для остановки движения
// void stopMoving() {
//  digitalWrite(leftMotorPin1, LOW);
//  digitalWrite(leftMotorPin2, LOW);
//  digitalWrite(rightMotorPin1, LOW);
//  digitalWrite(rightMotorPin2, LOW);
//  }