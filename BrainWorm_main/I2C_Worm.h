#include "Arduino.h"
#pragma once
#define I2C_Worm_H
#define str(x) String(x)
#include <Wire.h>   
#include "Debuger.h"
#include "I2C_Config.h"
#include "ArduinoDriver.h"

class BrainWorm{
  private:
    unsigned long timeLastRec = 0;  // Врямя последнего I2C запроса
    bool I2cRecFalg = true;         // True, чтобы при запуске ардуинка не ушла в бесконечный перезапуск
    uint8_t ExeCommands[10] = {
        SuddenStop_com, // 2 Остановка реверсом
        RaiseGrip_com,  // 3 Поднять банку
        LowerGrip_com,  // 4 Опустить бунку
        CamDir_com,     // 6 Команда для управления Сервой камеры
        UpCam_com,      // 7 Поднять камеру в нормальное положене
        LowCam_com     // 8 Опустить камеру
    };

  public: 
    volatile uint8_t stack[BufferSize];
    volatile uint8_t command = 0;
    volatile uint8_t ExeCommand = 0; // Текущая выполняемая команда
    volatile uint8_t DoneCommand = 0; // Последняя выполненная команда
    ArduinoDriver Driver;

    BrainWorm();
    ~BrainWorm();
    void init(){Driver.init();}

    void InternalRequestCycle();
    void InternalReciveCycle(); // функция, автоматически вызываемая при получении команды
    void ExternalCycle();       // функция, работающая вне прерывания

    uint8_t checkComand();
    uint8_t recData();
    
    void CheckQueue();
    void CheckButton();
    void TankSpeed();
    void(* resetFunc) (void) = 0; // Перезагрузка ардуино (На самом деле мы просто переводим стек в начало кода)

};