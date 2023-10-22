#include "motors.h"

void InitMotors(){
  pinMode(LM_PWM, OUTPUT);
  pinMode(LM_DIR, OUTPUT);
  pinMode(RM_PWM, OUTPUT);
  pinMode(RM_DIR, OUTPUT);
}

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
}
