#include <stdint.h>
#pragma once
#include "I2C_Worm.h"

BrainWorm::BrainWorm(){}
BrainWorm::~BrainWorm(){}

bool reverse(uint8_t side, uint8_t revers){return revers == false ? (bool)side : !(bool)side;}

void BrainWorm::CheckQueue(){            // Оnправка текущего статуса выполнения команд во внешнем цикле
  DEBUG_PRINT("CheckQueue_com");
  Wire.write(ExeCommand);
  Wire.write(DoneCommand);
}
void BrainWorm::CheckButton(){           // Отпрака состояния кнопки
  DEBUG_PRINT("CheckButton_com");
  Wire.write(Driver.CheckButton());
}
void BrainWorm::TankSpeed(){             // Парсер данных для TankSpeed
  DEBUG_PRINT("TankSpeed_com");
  int LS = stack[0] - stack[1];
  int RS = stack[2] - stack[3];
  Driver.SetTankSpeed(LS, RS);
}
void BrainWorm::InternalRequestCycle(){  // Команды, вызываемые при запросе данных
  switch (command) {
    case CheckQueue_com: {
      CheckQueue();
      command = 0;
      break;
    }
    case CheckButton_com:{   
      CheckButton();
      command = 0;
      break;
    }   
  }
}
void BrainWorm::InternalReciveCycle(){   // Команды, вызываемые внутри прерываний
  Driver.lock_servo();           // Деактивировоть сервоприводы для защиты от помех
  checkComand();                 // Получить команду
  recData();                     // Получить дополнительные байты данных, если они есть
  I2cRecFalg = true;             // Поднять флаг, показывающий, что связь живая
  switch (command) {
    case TankSpeed_com:           // Установка скорости для двух моторов
      DEBUG_PRINT("TankSpeed_com");
      TankSpeed(); 
    break;
    }
  for(uint8_t i = 0; i < sizeof(ExeCommands); i++) if(ExeCommands[i] == command) ExeCommand  = command;
}
void BrainWorm::ExternalCycle(){         // Команды, выполняемые вне прерываний

  unsigned long TT = millis();  // Время входа во внешний цикл
  Driver.init_servo();          // Включение сервоприводов для исполнения задачи
  if(I2cRecFalg){               // флаг подниается каждый раз когда приходит сообщение по I2c
    timeLastRec = TT;
    I2cRecFalg = false;
    DEBUG_PRINT("udate timeLastRec: "+str(timeLastRec));
  }
  if(TT - timeLastRec > KillSwitchTime){  // Перезагрузка Arduino при потере I2C соединения
    Driver.SetTankSpeed(0, 0);
    DEBUG_PRINT("I2C timeout: TL - M ="+str(TT-timeLastRec) + "ms.\n------------------------------------------------------");
    delay(100);
    resetFunc();
  }
  if (ExeCommand != 0){
    DEBUG_PRINT("Running the command: " + str(ExeCommand));

    if(ExeCommand == SuddenStop_com){ Driver.SuddenStop();                                    }  // 2 Остановка реверсом
    if(ExeCommand == RaiseGrip_com) { Driver.RaiseGrip();                                     }  // 3 Поднять банку
    if(ExeCommand == LowerGrip_com) { Driver.LowerGrip();                                     }  // 4 Опустить бунку
    if(ExeCommand == CamDir_com)    { Driver.SetCamDir(stack[0],stack[1]);                    }  // 6 Команда для управления Сервой камеры
    if(ExeCommand == UpCam_com)     { Driver.VerticalServ.write(VerticalZero);                }  // 7 Поднять камеру в нормальное положене
    if(ExeCommand == LowCam_com)    { Driver.VerticalServ.write(VerticalLow);                 }  // 8 Опустить камеру
    
    DoneCommand = ExeCommand;
    ExeCommand = 0;
  }
}
uint8_t BrainWorm::checkComand(){         // Проверка поступления новой команды
  command = 0;
  if (Wire.available()){
    BrainWorm::command = Wire.read();
    DEBUG_PRINT("Recive command " + str(command));
    }
  return command;
}
uint8_t BrainWorm::recData(){             // Проверка поступления пакета байт 
  uint8_t data_size = 0;
  if(Wire.available()){            // Если что-то есть, получаем размер пакета
    data_size = Wire.read();
    for ( uint8_t counter = 0; counter < data_size; counter++){
      if(Wire.available()) stack[counter] = Wire.read();         // Читаем нужное кол-во байт
      else DEBUG_PRINT("!Warning - data loss!");                 // Если что-то не так, кидаем лог, но не останавливамся
    }
    while(Wire.available()){DEBUG_PRINT("!Warning - excess data: "+str(Wire.read())+"!");} // Если что-то не так, кидаем лог, но не останавливамся
  }
  return data_size;
}