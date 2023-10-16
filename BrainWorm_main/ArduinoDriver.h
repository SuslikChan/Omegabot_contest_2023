#pragma once

#define BLACK_MAX  800
#define WHITE_MAX  300

#define ArduinoDriver_H
#include <Arduino.h>
#include <Servo.h> 
#include "Debuger.h"
#include "PinOut_Config.h"
#define str(x) String(x)

class ArduinoDriver {

  public: 
    ArduinoDriver();                                          // Конструктор
    ~ArduinoDriver();                                         // Дестуктор 

    Servo Lifter;           // Серва в основании захвата
    //Servo Griper;           // Серва в захвате
    Servo HorizontalServ;   // Серва в основании камеры
    Servo VerticalServ;     // Серва на камере

    int rSpeed = 0;         // Текущая скорость правого мотора
    int lSpeed = 0;         // Текущая скорость левого мотора

    enum _turn{Left, Right, NON};                             // Направления поворота
    enum side {Left_side, Right_side};

    void init();                                              // Инициализация всех компонентов
    void init_motors();                                       // Установка PinMOde на моторах  
    void init_servo();                                        // Инициализация сервоприводов
    void lock_servo();                                        // Отключить сервы
    void init_button(uint8_t pin = Button_pin);               // Умная настройка пинов кнопок   

    void SetSpeed(int pwm, uint8_t dir_pin, uint8_t pwm_pin); // Установка скорости "Некого" мотора
    void SetLSpeed(int speed = 0);                            // Установка скорости левого мотора
    void SetRSpeed(int speed = 0);                            // Установка скорости правого мотора
    void SetTankSpeed(int L_speed = 0, int R_speed = 0);      // Установка скорости для двух моторов
    void SuddenStop();                                        // Резкая остановка машины

    void RaiseGrip();                                         // Поднять захват
    void LowerGrip();                                         // Опустить захват
    void SetCamDir(uint8_t horizontal=0, uint8_t vertical=0); // Задать ориентацию камеры (0,0) - идеальо прямо   

    int CheckLen();                                           // Проверка показаний с дальномера
    bool CheckButton(uint8_t pin = Button_pin);               // Проверка состояния конопики    

};