
#include <GyverPID.h>
const int leftSensorPin = A2;
const int rightSensorPin = A1;
const int L_PWM = 6;
const int L_ctrl = 7;
const int R_PWM = 5;
const int R_ctrl = 4;
int h;
int k;

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
  regulator1.Kp = 400;
  regulator1.Ki += 0;
  regulator1.Kd = 0;
}
void loop() {
  
// Чтение значений с датчиков линии
 int leftSensorValue = analogRead(leftSensorPin);
 int rightSensorValue = analogRead(rightSensorPin);
 h =leftSensorValue-rightSensorValue;
 digitalWrite(L_ctrl,HIGH);
 digitalWrite(R_ctrl,HIGH);
 if (h<0){
  regulator1.input=h;
 }
 else{
  regulator1.input=-h;
 }
 k = map(regulator1.getResultTimer(),0,255,0,40);
 //k = regulator1.getResultTimer();
 if (h<0){
  analogWrite(L_PWM,160+k);
  analogWrite(R_PWM,40-k);
 }
 else{
  analogWrite(L_PWM,40-k);
  analogWrite(R_PWM, 160+k);
 }

 Serial.print(0);
 Serial.print(',');
 Serial.println(regulator1.getResultTimer());
 }