#include "gyro.h"
#include "motors.h"


void setup()
{
  InitMotors();
  initGyro();
}

void loop()
{
  turnLeft();
}


void turnLeft(){
  fuck_angls();
  long int time_0 = millis();
  float degrees;
  while (degrees > -90){
    degrees, time_0 = get_degrees(degrees, time_0);
    SetTankSpeed(-150, 150);  
  }
  SetTankSpeed(0,0);
}


