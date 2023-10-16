try:
    from smbus2 import SMBus
except Exception as e:
    print(e)

import time
import threading
from raspberry_i2c.i2c_config import I2C_Config


def sign(a): return 1 - 2*(a < 0)


_busLock = threading.Lock()


def BusLockWrapper(func):
    def _wrapper(*args, **kwargs):
        _busLock.acquire()
        try:
            return func(*args, **kwargs)
        except IOError as err:
            print(err)
            return None
        finally:
            _busLock.release()
            # time.sleep(0.001)
    return _wrapper


class Omegabot(I2C_Config):

    def __init__(self):
        self.log("I2C Bus Initialization - Start")
        self.bus = SMBus(1)
        time.sleep(0.1)
        self.log("I2C Bus Initialization - Done")

    @staticmethod
    def log(msg): print(msg)

    @BusLockWrapper
    def SendCommand(self, command_byte: int) -> None:
        """Отправить на Arduino 1 байт с командой"""
        self.log(f'SendCommand {command_byte}')
        self.bus.write_byte(self.ArduinoAdress, command_byte)

    @BusLockWrapper
    def SendData(self, command_byte: int, data: list = []) -> None:
        """Отправить на Arduino 1 байт с командой и массив байтов данных"""
        self.log(f'SendCommand {command_byte}, SendData {data}')
        self.bus.write_block_data(self.ArduinoAdress, command_byte, data)

    @BusLockWrapper
    def ReadData(self, command_byte: int, length: int = 0) -> list:
        """Отправить байт и получить массив с ответом"""
        self.log(f'ReadCommand {command_byte}')
        data = self.bus.read_i2c_block_data(
            self.ArduinoAdress, command_byte, length)
        return data

    def ByteLimiter(self, b):
        """Тупое ограничение скорости до -255 +255"""
        if abs(b) > 255:
            b = sign(b) * 255
            # self.log("!Warning The maximum permissible byte value has been exceeded. byte reduced to 255!")
        return b

    def SignedData(self, data: int = 0):
        """Упаковка данных типа int c сохранением знака"""
        bp = self.ByteLimiter(
            data) if data > 0 else 0  # Байт если значение положительное
        # Байт если значение отрицательное
        bm = self.ByteLimiter(-data) if data < 0 else 0
        return [bp, bm]

    def TankSpeed(self, LSpeed: int = 0, RSpeed: int = 0):
        """Задать направление моторам по танковой схеме

        Args:
            LSpeed (int, optional): Скорость левого мотора от -255 до 255. Defaults to 0.
            RSpeed (int, optional): Скорость левого мотора от -255 до 255. Defaults to 0.
        """
        L = self.SignedData(LSpeed)
        R = self.SignedData(RSpeed)
        data = L + R
        self.log(f"Send TankSpeed {LSpeed}, {RSpeed}. Data = {data}")
        self.SendData(self.TankSpeed_com, data)

    def SetGrip(self, lift: int = 0, grip: int = 0):

        data = [lift, grip]
        self.log(f"Send TankSpeed {lift}, {grip}. Data = {data}")
        self.SendData(self.SetGrip_com, data)

    def RaiseGrip(self):  # Поднять банку
        self.log("RaiseGrip")
        self.SendCommand(self.RaiseGrip_com)

    def LowerGrip(self):  # Опустить бунку
        self.SendCommand(self.LowerGrip_com)
        self.log("LowerGrip")

    def CheckQueue(self, length=2):
        """Проверить какую задачу сейчас выполняет конечный автомат

        Args:
            length (int, optional): _description_. Defaults to 2.

        Returns:
            _type_: _description_
        """
        return self.ReadData(self.CheckQueue_com, length)

    def CheckButton(self, length=1):
        """Проверить состояние нажатия кнопки (0/1)

        Args:
            length (int, optional): _description_. Defaults to 1.

        Returns:
            _type_: _description_
        """
        data = self.ReadData(self.CheckButton_com, length)
        print(data)
        if data != None:
            return data[0]
        else:
            return 0

    def UpCap(self):
        self.SendCommand(self.UpCam_com)

    def LowCap(self):
        self.SendCommand(self.LowCam_com)


if __name__ == "__main__":
    test = Omegabot()
    test.RaiseGrip()
    time.sleep(5)
    test.LowerGrip()
