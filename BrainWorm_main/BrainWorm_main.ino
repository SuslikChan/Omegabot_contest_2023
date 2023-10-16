#include "I2C_Worm.h"
#include "ArduinoDriver.h"

BrainWorm Worm;

void setup() {
  DEBUG_INIT();
  Worm.init();
  Wire.begin(ArduinoAdress);  // Инициализация адреса слэйва (Arduino)
  Wire.onReceive([](){Worm.InternalReciveCycle();  });  // Инициализация функции прерывания для получения команд
  Wire.onRequest([](){Worm.InternalRequestCycle(); });  // Инициализация функции прерывания для запросов информаци
}

void loop() {
  Worm.ExternalCycle();  // Обработка событий вне прерываний
}