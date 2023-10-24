#include "turn.h"


void setup()
{
  InitMotors();
  initGyro();
}

void loop()
{
  turnLeft(90);
}