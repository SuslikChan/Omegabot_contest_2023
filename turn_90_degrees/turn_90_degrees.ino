#include "turn.h"


void setup()
{
  InitMotors();
  initGyro();
}

void loop()
{
  turn(90);
}