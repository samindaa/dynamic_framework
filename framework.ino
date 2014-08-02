#include <Wire.h>
#include "Framework.h"

void setup()
{
}

void loop()
{
  Controller::getInstance().main(false);
}
