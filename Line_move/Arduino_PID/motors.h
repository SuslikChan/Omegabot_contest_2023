#pragma once

#define motors_H
#include <Arduino.h>

#define RM_DIR  4   // Направление левого мотора
#define RM_PWM  5   // ШИМ левого мотора
#define LM_DIR  7   // Направление правого мотора
#define LM_PWM  6   // ШИМ правого мотора


void InitMotors();
void SetSpeed(int pwm, uint8_t dir_pin, uint8_t pwm_pin);
void SetLSpeed(int speed);
void SetRSpeed(int speed);
void SetTankSpeed(int L_speed, int R_speed);