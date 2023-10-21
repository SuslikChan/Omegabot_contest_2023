#include <GyverPID.h>
const int leftSensorPin = A2;
const int rightSensorPin = A1;
const int L_PWM = 6;
const int L_ctrl = 7;
const int R_PWM = 5;
const int R_ctrl = 4;
int h;

  GyverPID regulator1(0, 0, 0,10);//подключаем регуляторы, ставим коэфы
const int norm = 0;//приводим значение черной линии
void setup(){
  regulator1.setDirection(NORMAL);// ставим "направление" регулятора- NORMAL=>выход регулятора напрямую влияет на датчик
  regulator1.setpoint=norm;// ставим значение, к которму будет стремиться регулятор
  Serial.begin(9600);
// Настройка пинов датчиков линии как входов
 pinMode(leftSensorPin, INPUT);
 pinMode(rightSensorPin, INPUT);
 
// Настройка пинов моторов как выходов
 pinMode(L_PWM, OUTPUT);
 pinMode(L_ctrl, OUTPUT);
 pinMode(R_PWM, OUTPUT);
 pinMode(R_ctrl, OUTPUT);
 //настройка коэф
  regulator1.Kp = 2;
  regulator1.Ki += 0;
  regulator1.Kd = 0;
}
void loop() {
  
// Чтение значений с датчиков линии
 int leftSensorValue = analogRead(leftSensorPin);
 int rightSensorValue = analogRead(rightSensorPin);
 regulator1.input =leftSensorValue-rightSensorValue;//скармливаем левому регулятору значение левого датчика
 digitalWrite(L_ctrl,HIGH);
 digitalWrite(R_ctrl,HIGH);
 h = map(regulator1.getResultTimer(),0 ,255, 0 ,100);
 if (regulator1.getResultTimer()>0){
  analogWrite(L_PWM,100-h);
  analogWrite(R_PWM,100+h);
 }
 else if (regulator1.getResultTimer()<0){
  analogWrite(L_PWM,100+h);
  analogWrite(R_PWM,100-h);
 }
 else{
  analogWrite(L_PWM,200);
  analogWrite(R_PWM,200);
 }
 }
