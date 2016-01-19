#include <Wire.h>
#include "Controller.h"

void setup()
{
  Controller::getInstance().setup(115200);  
}

void loop()
{
  Controller::getInstance().loop();
}
