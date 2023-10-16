#ifndef I2C_CONF
  #define I2C_CONF

  #define ArduinoAdress   18  // Адрес Arduino на который Raspberry отправляет команды
  #define BufferSize      8   // Кол-во байт в буфере I2C

  #define TankSpeed_com   131 // Задать направление моторам по танковой схеме
  #define SuddenStop_com  132 // Остановка реверсом

  #define RaiseGrip_com   133 // Поднять банку
  #define LowerGrip_com   134 // Опустить бунку

  #define CamDir_com      136 // Команда для управления Сервой камеры
  #define UpCam_com       137 // Поднять камеру в нормальное положене
  #define LowCam_com      138 // Опустить камеру

  #define CheckQueue_com  230 // Проверка работы червя (Проверка выполнил ли червь последнюю команду)
  #define CheckButton_com 231 // Проверка состояния кнопки (Для старта программы)

  #define KillSwitchTime  20000// Время молчяния i2c, после которого Ардуинка остановится

#endif