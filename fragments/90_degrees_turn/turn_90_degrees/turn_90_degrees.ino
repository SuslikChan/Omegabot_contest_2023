
#include "ICM_20948.h" 

#define RM_DIR  4   // Направление левого мотора
#define RM_PWM  5   // ШИМ левого мотора
#define LM_DIR  7   // Направление правого мотора
#define LM_PWM  6 

//#define USE_SPI       
#define SERIAL_PORT Serial
#define SPI_PORT SPI 
#define CS_PIN 2    
#define WIRE_PORT Wire
#define AD0_VAL 0
#ifdef USE_SPI
ICM_20948_SPI myICM; 
#else
ICM_20948_I2C myICM; 
#endif


float degrees = 0.0;
float speed_1;
float speed_0 = 0.0;
const float time = 0.002;
const float k = 15.2;

void setup()
{

  SERIAL_PORT.begin(115200);
  while (!SERIAL_PORT)
  {
  };

#ifdef USE_SPI
  SPI_PORT.begin();
#else
  WIRE_PORT.begin();
  WIRE_PORT.setClock(400000);
#endif

  bool initialized = false;
  while (!initialized)
  {

#ifdef USE_SPI
    myICM.begin(CS_PIN, SPI_PORT);
#else
    myICM.begin(WIRE_PORT, AD0_VAL);
#endif

    SERIAL_PORT.print(F("Initialization of the sensor returned: "));
    SERIAL_PORT.println(myICM.statusString());
    if (myICM.status != ICM_20948_Stat_Ok)
    {
      SERIAL_PORT.println("Trying again...");
      delay(500);
    }
    else
    {
      initialized = true;
    }
  }

  ICM_20948_fss_t myFSS;
  myFSS.g = dps2000; 
  myICM.setFullScale((ICM_20948_Internal_Acc | ICM_20948_Internal_Gyr), myFSS);
}

void loop()
{
  turnLeft();
  delay(10000);
}

float get_degrees(float degrees_0){
  if (myICM.dataReady())
  {
  myICM.getAGMT();         
  speed_1 =  myICM.agmt.gyr.axes.z;
  degrees = degrees + (speed_0+speed_1)/2*time/k;
  speed_0 = speed_1;
  return degrees;
  }
  else
  {
    SERIAL_PORT.println("Sorry, no data. Let's try again");
    return 0;
  }
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

void turnLeft(){
  degrees = 0.0;
  while (degrees > -95){
    degrees = get_degrees(degrees);
    SetTankSpeed(-150, 150);
    //SERIAL_PORT.println(degrees);
    //delay(30);
  }
  SetTankSpeed(0,0);
}