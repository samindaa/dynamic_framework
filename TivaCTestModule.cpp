/*
 * TivaCTestModule.cpp
 *
 *  Created on: Jan 18, 2016
 *      Author: sabeyruw
 */

#include "TivaCTestModule.h"

TivaCTestModule::TivaCTestModule() : state(0x0)
{
}

TivaCTestModule::~TivaCTestModule()
{
}

void TivaCTestModule::execute()
{
#if defined(EMBEDDED_MODE)
  state ^= 0x1;
  digitalWrite(RED_LED, state);
  delay(3000);
  Serial.println(state);
#endif
}

MAKE_MODULE(TivaCTestModule)

