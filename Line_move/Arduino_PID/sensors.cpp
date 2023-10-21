#include "sensors.h"
#include <Arduino.h>
#include <GyverFilters.h>

GFilterRA filt_l;
GFilterRA filt_r;

int white_L;
int black_L;
int white_R;
int black_R;

void init_filters(){
  filt_r.setCoef(FILT_COEF);//установка коэффициента фильтрации (0.0... 1.0). Чем меньше, тем плавнее фильтр
  filt_r.setStep(FILT_STEP);// установка шага фильтрации (мс). Чем меньше, тем резче фильтр

  filt_l.setCoef(FILT_COEF);//установка коэффициента фильтрации (0.0... 1.0). Чем меньше, тем плавнее фильтр
  filt_l.setStep(FILT_STEP);// установка шага фильтрации (мс). Чем меньше, тем резче фильтр
}

int midArifm(int pin) {
  long sum = 0;                       // локальная переменная sum
  for (int i = 0; i < 20; i++)  // согласно количеству усреднений
    sum += analogRead(pin);                  // суммируем значения с любого датчика в переменную sum
  return ((float)sum / 20);
}

void sensor_calibration(){ // Калибровка датчиков на белое и чёрное через нажатие кнопок

    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(L_SENS_PIN, INPUT);
    pinMode(R_SENS_PIN, INPUT);

    while (digitalRead(BUTTON_PIN)) {delay(50);}  // Ожидание первого нажатия клавиши
    while (!digitalRead(BUTTON_PIN)) {delay(50);} // Ожидание когда кнопку отпустят

    white_L = midArifm(L_SENS_PIN); // Запись значения белого с левого датчика
    white_R = midArifm(R_SENS_PIN); // Запись значения белого с правого датчика
    
    while (digitalRead(BUTTON_PIN)) {delay(50);}  // Ожидание второго нажатия клавиши
    while (!digitalRead(BUTTON_PIN)) {delay(50);} // Ожидание когда кнопку отпустят

    black_L = midArifm(L_SENS_PIN); // Запись значения чёрного с левого датчика
    black_R = midArifm(R_SENS_PIN); // Запись значения чёрного с правого датчика

    while (digitalRead(BUTTON_PIN)) {delay(50);}  // Ожидание третьего нажатия клавиши

    Serial.print(white_L);
    Serial.println(" ");
    Serial.println(white_R);

    Serial.print(black_L);
    Serial.println(" ");
    Serial.println(black_R);

    while (!digitalRead(BUTTON_PIN)) {delay(50);} // Ожидание когда кнопку отпустят
}

int get_error(){

    int LSensVal = filt_l.filteredTime(analogRead(L_SENS_PIN)); // Чтение значений с датчиков линии
    int RSensVal = filt_r.filteredTime(analogRead(R_SENS_PIN));

    int error = map(RSensVal, black_R, white_R, 0, SENSOR_RANGE) 
              - map(LSensVal, black_L, white_L, 0, SENSOR_RANGE);

    Serial.println(error);

    return error;
}