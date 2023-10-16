#include <Servo.h>
#include <GyverPID.h>
Servo servo1;
Servo servo2;
int datPin=11;
bool flag= false;

const int leftSensorPin = A3;
const int rightSensorPin = A2;
const int L_PWM = 6;
const int L_ctrl = 7;
const int R_PWM = 5;
const int R_ctrl = 4;

#define PIN_TRIG 3
#define PIN_ECHO 2
  GyverPID regulator1(0, 0, 0,10);//подключаем регуляторы, ставим коэфы
  GyverPID regulator2(0, 0, 0,10);
long duration, cm;
const int black = 800;//приводим значение черной линии
void setup() {

  regulator1.setDirection(NORMAL);// ставим "направление" регулятора- NORMAL=>выход регулятора напрямую влияет на датчик
  regulator2.setDirection(NORMAL);// REVERSE=>выход регулятора влияет на датчик наоборот
  regulator1.setpoint=black;// ставим значение, к которму будет стремиться регулятор
  regulator2.setpoint=black;
  Serial.begin(9600);
  servo1.attach(9);
  servo2.attach(10);
  servo1.write(110);
  servo2.write(180);
  pinMode(datPin,INPUT);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
// Настройка пинов датчиков линии как входов
 pinMode(leftSensorPin, INPUT);
 pinMode(rightSensorPin, INPUT);
 
// Настройка пинов моторов как выходов
 pinMode(L_PWM, OUTPUT);
 pinMode(L_ctrl, OUTPUT);
 pinMode(R_PWM, OUTPUT);
 pinMode(R_ctrl, OUTPUT);
 //настройка коэф
  regulator1.Kp = 5.2;
  regulator1.Ki += 0.5;
  regulator1.Kd = 0;
  regulator2.Kp = 5.2;
  regulator2.Ki += 0.5;
  regulator2.Kd = 0;
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
  if ((cm<=20 )and(flag== false)){
    servo2.write(60);
    delay(300);
    servo1.write(180);
    flag= true;
  }
// Чтение значений с датчиков линии
 int leftSensorValue = analogRead(leftSensorPin);
 int rightSensorValue = analogRead(rightSensorPin);
 regulator1.input =leftSensorValue;//скармливаем левому регулятору значение левого датчика
 digitalWrite(L_ctrl,HIGH);
 analogWrite(L_PWM,regulator1.getResultTimer());//подаем на левый мотор скорость с левого регулятора
 regulator2.input=rightSensorValue;//скармливаем правому регулятору значение правого датчика
 digitalWrite(R_ctrl,HIGH);
 analogWrite(R_PWM,regulator2.getResultTimer());//подаем на правый мотор скорость с правого регулятора
 }