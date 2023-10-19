
#include <GyverPID.h>

#define RM_DIR  4   // Направление левого мотора
#define RM_PWM  5   // ШИМ левого мотора
#define LM_DIR  7   // Направление правого мотора
#define LM_PWM  6   // ШИМ правого мотора

#define NORMAL_SPEED 220

const int leftSensorPin = A2;
const int rightSensorPin = A1;

GyverPID PID(0, 0, 0, 1);//подключаем регуляторы, ставим коэфы
const int norm = 0;//приводим значение черной линии

void SetSpeed(int pwm, uint8_t dir_pin, uint8_t pwm_pin){  // Установка скорости мотора
  bool dir = 0;                        // Состояние пина направления
  if (pwm > 0) dir = 1;                // Движение прямо                          
  if (abs(pwm) > 255) pwm = 255;       // Ограничение скорости 

  digitalWrite(dir_pin, dir);     // Направление
  analogWrite(pwm_pin, abs(pwm)); // Скорость
} 
void SetLSpeed(int speed){                         // Установка скорости левого мотора
  SetSpeed(speed, LM_DIR, LM_PWM);
}
void SetRSpeed(int speed){                         // Установка скорости правого мотора
  SetSpeed(speed, RM_DIR, RM_PWM);
}
void SetTankSpeed(int L_speed, int R_speed){       // Установка скорости для двух моторов
  SetLSpeed(L_speed);
  SetRSpeed(R_speed);
  Serial.println(String(L_speed) + "  " + String(R_speed));
}


void setup(){
  PID.setDirection(NORMAL);// ставим "направление" регулятора- NORMAL=>выход регулятора напрямую влияет на датчик
  PID.setpoint=norm;// ставим значение, к которму будет стремиться регулятор
  Serial.begin(9600);
  // Настройка пинов датчиков линии как входов
  pinMode(leftSensorPin, INPUT);
  pinMode(rightSensorPin, INPUT);
  
  // Настройка пинов моторов как выходов
  pinMode(LM_PWM, OUTPUT);
  pinMode(LM_DIR, OUTPUT);
  pinMode(RM_PWM, OUTPUT);
  pinMode(RM_DIR, OUTPUT);
 //настройка коэф
  PID.setLimits(-255, 255); 
  PID.Kp = 25;
  PID.Ki = 0;
  PID.Kd = 0.1;
}
void loop() {
  
// Чтение значений с датчиков линии
    int leftSensorValue = analogRead(leftSensorPin);
    int rightSensorValue = analogRead(rightSensorPin);
    int err =leftSensorValue-rightSensorValue;

    PID.input = err;

    int pid_ret = PID.getResultTimer();
    SetTankSpeed(NORMAL_SPEED + pid_ret, NORMAL_SPEED - pid_ret);

    //Serial.println("0," + String(err));
 }