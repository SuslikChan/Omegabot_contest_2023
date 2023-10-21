#include <GyverPID.h> //1234567
#include <ServoSmooth.h>

#define SENS_PIN A0     // Инфракрасник
#define AMOUNT 2        // кол-во серво
#define b1 11           // Кнопка

int moveSpd = 100;      // Обычная скорость
int slowSpd = 60;       // Скорость при замедлении

int openAng = 35;       // Угол захвата
int closeAng = 110;     // Угол отпуска
int grabDelay = 100;    // Время замедления

ServoSmooth servos[AMOUNT];
uint32_t servoTimer;
uint32_t turnTimer;
int position1 = 60;   // первое положение сервы 1 (вверх-вниз)
int position2 = 0;  // второе положение сервы 1 (вверх-вниз)

bool flag;

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
    servos[0].write(openAng);           // Хвать банку
    delay(300);
    servos[1].setTargetDeg(position1);  // До куда поднять 
    flag = true;                        // Начало подъёма
  while (flag) {
    // каждые 20 мс
    if (millis() - servoTimer >= 20) {  // взводим таймер на 20 мс (как в библиотеке)
      servoTimer += 20;
        servos[1].tickManual();         // двигаем серву 1
    } 
    else !flag;                         // Конец подъёма
  }   
}

void place_down() {     // Опустить предмет
  servos[1].setTargetDeg(position2);
  flag = true; 
  while (flag) {
    // каждые 20 мс
    if (millis() - servoTimer >= 20) {  // взводим таймер на 20 мс (как в библиотеке)
      servoTimer += 20;
        servos[1].tickManual();         // двигаем серву 1
    } 
    else !flag;
}
  delay(300); 
  servos[0].write(closeAng);            // Отпускаем банку
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
  servos[0].attach(3);
  servos[1].attach(2);     
  servos[0].setSpeed(360);
  servos[1].setAccel(0.5);
  servos[1].setSpeed(180);
  servos[0].setAccel(0.5); 
  servos[0].write(closeAng);        //  Начальное положение Серв
  servos[1].write(position2);       //
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
