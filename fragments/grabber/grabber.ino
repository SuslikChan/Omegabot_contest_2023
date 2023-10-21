#include <Servo.h>
#include <GyverPID.h>

#define SENS_PIN A0 

//1111

Servo servo1;     // Инициализация Серв
Servo servo2;     // 
#define b1 11     // Кнопка

int moveSpd = 100;
int slowSpd = 60;

int openAng = 35;
int closeAng = 110;
int grabDelay = 100;

#define L_PWM 6
#define L_ctrl 7
#define R_PWM 5
#define R_ctrl 4

//GyverPID regulator1(0, 0, 0,10);//подключаем регуляторы, ставим коэфы

float len_sensor(){ // Чтение дистанции с ИК дальномера

  float measur  = analogRead(SENS_PIN);     // Чтение значения с АЦП
  float voltage = measur * 5 / 1023;        // Преобразование АЦП в реальный вольтаж
  float dist    = 65 * pow(voltage, -1.10); // Преобразование волтажа в мм

  return dist;
}

void pick_up() {        // Взять предмет
  servo1.write(openAng);     
  delay(300);
  servo2.write(60);
}

void place_down() {     // Опустить предмет
  servo2.write(0);
  delay(300);
  servo1.write(closeAng);
}

void jam_bot () {       // Принудительная остановка бота (ДО ПЕРЕЗАПУСКА)
  while (1 > 0) {}
}

void setup() {
  
  pinMode(b1, OUTPUT);
  pinMode(L_PWM, OUTPUT);
  pinMode(L_ctrl, OUTPUT);
  pinMode(R_PWM, OUTPUT);
  pinMode(R_ctrl, OUTPUT);
  servo1.attach(3);          //  Привязка Серв к пинам
  servo2.attach(2);          // 
  servo1.write(110);         //  Начальное положение Серв
  servo2.write(0);           //
  pinMode(SENS_PIN, INPUT);
  Serial.begin(9600);

  digitalWrite(L_ctrl,HIGH);      // Задаём направление прямо
  digitalWrite(R_ctrl,HIGH);      //
  analogWrite(L_PWM, moveSpd);    // Подаём на моторы скорость 
  analogWrite(R_PWM, moveSpd);    // 

}

void loop() {
  float dist = len_sensor();
  //Serial.println(dist);

  if (dist < 20) {
    analogWrite(L_PWM, slowSpd);
    analogWrite(R_PWM, slowSpd);
    delay(grabDelay);
    analogWrite(L_PWM, 0);    // Остановка
    analogWrite(R_PWM, 0);    //

    pick_up();
    delay(2000);
    place_down();

    jam_bot();
  }
}
