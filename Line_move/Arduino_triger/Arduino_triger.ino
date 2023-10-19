//Номера пинов
//Пины управления мощностью левых и правых двигателей
#define L_UPR 6
#define R_UPR 5
//Пины управления направлением левых и правых двигателей
#define L_DIR 7
#define R_DIR 4

//Пины аналоговых ИК датчиков с правой и левой сторон
#define R_IK A0
#define L_IK A1

#define button 12

//Константа контраста, которую показывает датчик на темной линии
//Измерить ее можно в примере с ИК датчиком
//(Это значение должно быть меньше, чем измеренное, примерно на 100)
#define LEVEL_OF_COLOR 300

//Функция движения
//left_direction и right_direction - направление движения пар колес, 1 - вперед, 0 - назад
//left_power и right_power - мощность, подаваемая на двигатели, 0 - нет движения, 255 - максимальная скорость
//рекомендуемый минимум - 100
void drive(bool left_direction, bool right_direction, int left_power, int right_power) {
    //Выставляем направление движения левых двигателей в зависимости от поданного  значения
    if (left_direction == 0)
        digitalWrite(L_DIR, HIGH);
    else
        digitalWrite(L_DIR, LOW);

    //Выставляем направление движения правых двигателей в зависимости от поданного  значения
    if (right_direction == 0)
        digitalWrite(R_DIR, HIGH);
    else
        digitalWrite(R_DIR, LOW);

    //Выставляем мощность на двигателях в зависимости от поданного  значения
    analogWrite(R_UPR, right_power);
    analogWrite(L_UPR, left_power);
}

int fromLow_L;
int fromHigh_L;
int fromLow_R;
int fromHigh_R;

int midArifm(int pin) {
  long sum = 0;                       // локальная переменная sum
  for (int i = 0; i < 20; i++)  // согласно количеству усреднений
    sum += analogRead(pin);                  // суммируем значения с любого датчика в переменную sum
  return ((float)sum / 20);
}

//Функция инициализации, выполняющаяся один раз после запуска ардуино
void setup() {
    //Пины управления мощностью двигателей работают на выход
    pinMode(R_IK, INPUT);
    pinMode(L_IK, INPUT);

    pinMode(R_DIR, OUTPUT);
    pinMode(L_DIR, OUTPUT);

    pinMode(R_UPR, OUTPUT);
    pinMode(L_UPR, OUTPUT);

    pinMode(button, INPUT_PULLUP);

    Serial.begin(9600); 

    while (digitalRead(button)) {delay(50);}
    fromLow_L = midArifm(L_IK);
    fromLow_R = midArifm(R_IK);
    while (!digitalRead(button)) {delay(50);}
    while (digitalRead(button)) {delay(50);}
    fromHigh_L = midArifm(L_IK);
    fromHigh_R = midArifm(R_IK);
    while (!digitalRead(button)) {delay(50);}
    while (digitalRead(button)) {delay(50);}
    delay(1000);

}

//Функция программы, выполняющаяся постоянно в цикле после запуска ардуино
void loop() {
    int R = map(midArifm(R_IK), fromLow_R, fromHigh_R, 0, 1024);
    int L = map(midArifm(L_IK), fromLow_L, fromHigh_L, 0, 1024);
    Serial.println(String(R) + " " + String(L) + " ___ " + String(analogRead(R_IK)) + " " + String(analogRead(L_IK)));
    //delay(500);
    // программная пауза при нажатии на кнопку
    if (!digitalRead(button)){
      drive(1, 1, 0, 0);
      while (!digitalRead(button)) {delay(50);}
      while (digitalRead(button)) {delay(50);}
      delay(1000);
    }
    //Если оба датчика вне линии, едем вперед
    if (abs(R - L) < LEVEL_OF_COLOR) {
        Serial.println("move");
        drive(1, 1, 100, 100);
    }
    //Иначе, если левый датчик заехал на линию, поворачиваем влево
    else if (R - L < 0) {
        Serial.println("move to L");
        drive(0, 1, 50, 170);
    }
    //Иначе, если правый датчик заехал на линию, поворачиваем вправо
    else if (R - L > 0) {
        Serial.println("move to R");
        drive(1, 0, 170, 50);
    }
}