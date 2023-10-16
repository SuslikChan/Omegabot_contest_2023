#include "PinOut_Config.h"
#include "Debuger.h"
#include "Arduino.h"
#pragma once
#include "ArduinoDriver.h"

int sign(double x){return (x > 0) - (x < 0);};
void(* resetFunc) (void) = 0; // Перезагрузка ардуино (На самом деле мы просто переводим стек в начало кода)

ArduinoDriver::ArduinoDriver(){}                                           // Конструктор
ArduinoDriver::~ArduinoDriver(){}                                          // Дестуктор 

void ArduinoDriver::init(){                                               // Настройка дебагеа, пинов, начального положения серв

  DEBUG_INIT();                   // Открытие последовательного порта для дебага

  init_motors();
  init_servo();
  SetCamDir();

  init_button();               // Кнопка для запуска

  pinMode(LED_PIN, OUTPUT);    // Встроенный светодиод
  pinMode(GripPin, OUTPUT);    // Пин Электромагнита

  DEBUG_PRINT("Arduino configurate done!\nPress button to start)");
  //while(!CheckButton()){delay(10);}
  digitalWrite(LED_PIN, 1);
  delay(500);
  digitalWrite(LED_PIN, 0);

}     
void ArduinoDriver::init_motors(){                                 // Установка PinMOde на моторах 
  pinMode(LM_DIR, OUTPUT);        // Направление левого мотора
  pinMode(RM_DIR, OUTPUT);        // Направление правого мотора
  pinMode(LM_PWM, OUTPUT);        // ШИМ левого мотора
  pinMode(RM_PWM, OUTPUT);        // ШИМ правого мотора
}
void ArduinoDriver::init_servo(){                                  // Инициализация сервоприводов
  Lifter.attach(LiftPin, 0, 180);                   // Инициализация подъёмника
  HorizontalServ.attach(HorizontalServPin, 0, 180); // Инициализация Сервы камеры по горизонтали
  VerticalServ.attach(VerticalServPin, 0, 180);     // Инициализация Сервы камеры по вертикали
  Lifter.write(LiftHigh);
  // LowerGrip();   // Опустить захват перед стартом
  // SetCamDir();   // Привести Камеру в начальное положене 
}
void ArduinoDriver::lock_servo(){                                  // Отлключение сервоприводов, используется для защиты серв от тряски во время работы I2C
  Lifter.detach();         // Инициализация подъёмника
  HorizontalServ.detach(); // Инициализация Сервы камеры по горизонтали
  VerticalServ.detach();   // Инициализация Сервы камеры по вертикали
}
void ArduinoDriver::init_button(uint8_t pin = Button_pin){         // Умная настройка пинов кнопок
  if(pin == 12) pinMode(pin, INPUT_PULLUP);     // к 12 пину на шилде робота распаяна кнопка
  else pinMode(pin, INPUT);
}                               
void ArduinoDriver::SetSpeed(int pwm, uint8_t dir_pin, uint8_t pwm_pin){  // Установка скорости мотора
  bool dir = 0;                        // Состояние пина направления
  if (pwm > 0) dir = 1;                // Движение прямо                          
  if (abs(pwm) > 255) pwm = 255;       // Ограничение скорости 

  digitalWrite(dir_pin, dir);     // Направление
  analogWrite(pwm_pin, abs(pwm)); // Скорость
} 
void ArduinoDriver::SetLSpeed(int speed){                         // Установка скорости левого мотора
  SetSpeed(speed, LM_DIR, LM_PWM);
 // DEBUG_PRINT("Set L speed " + str(speed));
}
void ArduinoDriver::SetRSpeed(int speed){                         // Установка скорости правого мотора
  SetSpeed(speed, RM_DIR, RM_PWM);
  //DEBUG_PRINT("Set R speed " + str(speed));
}
void ArduinoDriver::SetTankSpeed(int L_speed, int R_speed){       // Установка скорости для двух моторов
  lSpeed = L_speed;
  rSpeed = R_speed;
  SetLSpeed(L_speed);
  SetRSpeed(R_speed);
}
void ArduinoDriver::SuddenStop(){                                 // Резкая остановка машины
  if(abs(lSpeed) > 200) lSpeed = sign(lSpeed) * 200;
  if(abs(rSpeed) > 200) rSpeed = sign(rSpeed) * 200;

  SetTankSpeed(-lSpeed, -rSpeed);
  delay(30);
  SetTankSpeed(0, 0);
}
bool ArduinoDriver::CheckButton(uint8_t pin = Button_pin){        // Проверка состояния кнопки
  bool Data = digitalRead(pin);
  if(pin == 12) Data = !Data;
  DEBUG_PRINT(Data);
  return Data;
}
void ArduinoDriver::RaiseGrip(){                     // Поднять захват
    Lifter.attach(LiftPin, 0, 180);
    DEBUG_PRINT("Grip compression - Start" );
    digitalWrite(GripPin, 1);

    ArduinoDriver::Lifter.write(LiftLow);
    delay(750);
    ArduinoDriver::Lifter.write(LiftLow);
    delay(500);

    DEBUG_PRINT("Grip compression - Done" );
    Lifter.detach();
}
void ArduinoDriver::LowerGrip(){                     // Опустить захват
    Lifter.attach(LiftPin, 0, 180);
    DEBUG_PRINT("Grip unclenching - Start" );
    digitalWrite(GripPin, 0);
    ArduinoDriver::Lifter.write(LiftLow + 20);
    delay(2000);
    ArduinoDriver::Lifter.write(LiftHigh);
    delay(500);
    DEBUG_PRINT("Grip unclenching - Done" );
    Lifter.detach();
}
void ArduinoDriver::SetCamDir(uint8_t h=0, uint8_t v=0){          // Задать ориентацию камеры (0,0) - идеальо прямо
  ArduinoDriver::HorizontalServ.write(HorizontalZero + h);
  ArduinoDriver::VerticalServ.write(VerticalZero + v);
  DEBUG_PRINT("Set camera direction h = " + str(h) + "  v = " + str(v) );
}


