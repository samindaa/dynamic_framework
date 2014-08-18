#include <Wire.h>
#include "Framework.h"

void setup()
{
  Controller::getInstance().setup();
}

void loop()
{
  Controller::getInstance().loop();
}
