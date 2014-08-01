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
  std::cout << "Test2Mod->requires:" << std::endl;
  std::cout << "\t theTest1Rep->test1=" << theTest1Rep->test1 << std::endl;
  std::cout << "Test2Mod->provides:" << std::endl;
  theTest2Rep.test2 = rand();
  std::cout << "\t theTest2Rep.test2=" << theTest2Rep.test2 << std::endl;
}
