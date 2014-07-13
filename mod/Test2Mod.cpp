/*
 * Test2Mod.cpp
 *
 *  Created on: Jun 30, 2014
 *      Author: sam
 */

#include "Test2Mod.h"
#include <cstdlib>

MAKE_MODULE(Test2Mod, Thread Two)

void Test2Mod::update(Test2Rep& theTest2Rep)
{
  std::cout << "requires->Test2Mod" << std::endl;
  std::cout << "theTest1Rep->test1=" << theTest1Rep->test1 << std::endl;
  std::cout << "provides->Test2Mod=" << std::endl;
  theTest2Rep.test2 = rand();
  std::cout << "theTest2Rep.test2=" << theTest2Rep.test2 << std::endl;
}
