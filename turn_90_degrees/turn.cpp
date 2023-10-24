#include "turn.h"

void turn(int angle){
  long int time_0 = millis();
  float degrees = 0.0;
  if (angle > 0){
  while (degrees < angle){
    degrees, time_0 = get_degrees(degrees, time_0);
    SetTankSpeed(150, -150);  
  }
  }
  else{
    while (degrees > angle){
    degrees, time_0 = get_degrees(degrees, time_0);
    SetTankSpeed(-150, 150);  
  }
  }
  SetTankSpeed(0,0);
}

