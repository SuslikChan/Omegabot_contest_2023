from dataclasses import dataclass


@dataclass
class I2C_Config():

    ArduinoAdress = 18  # Адрес Arduino на который Raspberry отправляет команды

    Reset_com = 130  # Ребут ардуино
    TankSpeed_com = 131  # Задать направление моторам по танковой схеме
    SuddenStop_com = 132  # Остановка реверсом
    RaiseGrip_com = 133  # Поднять банку
    LowerGrip_com = 134  # Опустить бунку
    IdleGrip_com = 135  # Холостое положение захвата
    SetGrip_com = 139
    CamDir_com = 136  # Команда для управления Сервой камеры
    UpCam_com = 137  # Поднять камеру в нормальное положене
    LowCam_com = 138  # Опустить камеру
    TurnCross_com = 139  # Поворот на перекрёстке
    # По датчикам доехать до места захвата  CheckQueue_com  230 // Проверка работы червя (Проверка выполнил ли червь последнюю команду)
    Drive2Bank_com = 140

    # Проверка работы червя (Проверка выполнил ли червь последнюю команду)
    CheckQueue_com = 230
    # Проверка состояния кнопки (Для старта прог  KillSwitchTime  10000// Время молчяния i2c, после которого Ардуинка остановится
    CheckButton_com = 231
