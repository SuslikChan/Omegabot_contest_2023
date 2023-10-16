//Пины аналоговых ИК датчиков с правой и левой сторон
#define R_IK A0
#define L_IK A1
#define SENSOR_RANGE 1024
#define BUTTON_PIN 12

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

void sensor_calibration(){ // Калибровка датчиков на белое и чёрное через нажатие кнопок

    while (digitalRead(BUTTON_PIN)) {delay(50);}  // Ожидание первого нажатия клавиши
    while (!digitalRead(BUTTON_PIN)) {delay(50);} // Ожидание когда кнопку отпустят

    fromLow_L = midArifm(L_IK); // Запись значения белого с левого датчика
    fromLow_R = midArifm(R_IK); // Запись значения белого с правого датчика
    
    while (digitalRead(BUTTON_PIN)) {delay(50);}  // Ожидание второго нажатия клавиши
    while (!digitalRead(BUTTON_PIN)) {delay(50);} // Ожидание когда кнопку отпустят

    fromHigh_L = midArifm(L_IK); // Запись значения чёрного с левого датчика
    fromHigh_R = midArifm(R_IK); // Запись значения чёрного с правого датчика

    while (digitalRead(BUTTON_PIN)) {delay(50);}  // Ожидание третьего нажатия клавиши
    while (!digitalRead(BUTTON_PIN)) {delay(50);} // Ожидание когда кнопку отпустят
}

void setup() {
 
    pinMode(R_IK, INPUT);
    pinMode(L_IK, INPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    Serial.begin(9600); 
    sensor_calibration(); // Калибровка датчиков
    
}


void loop() {

    int r_real = midArifm(R_IK); // Взятие среднего арифметического значения с датчика 
    int l_real = midArifm(L_IK); // todo поменять на плавающее среднее

    int r_normal = map(r_real, fromLow_R, fromHigh_R, 0, SENSOR_RANGE); // Перевод значений датчиков к нормальному диапазону
    int l_normal = map(l_real, fromLow_L, fromHigh_L, 0, SENSOR_RANGE);

    int sensor_difference = r_normal - r_normal; // Разница с датчиков (ошибка), подаваемая на регулятор

    Serial.println("Reals:     " + String(r_real) + " " + String(l_real));
    Serial.println("Normals:   " + String(r_normal) + " " + String(l_normal));
    Serial.println("Differenc: " + String(sensor_difference));
    //delay(500);
    
}