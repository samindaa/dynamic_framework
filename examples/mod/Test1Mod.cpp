/*
 * Test1Mod.cpp
 *
 *  Created on: Jun 30, 2014
 *      Author: sam
 */

#include "Test1Mod.h"
#include <cstdlib>

MAKE_MODULE(Test1Mod, Thread One)

void Test1Mod::update(Test1Rep& theTest1Rep)
{
  //std::cout << "Test1Mod->requires:" << std::endl;
  //std::cout << "\t theTest2Rep->test2=" << theTest2Rep->test2 << std::endl;
  //std::cout << "\t theTest3Rep.iTest3=" << theTest3Rep->iTest3 << std::endl;
  //std::cout << "\t theTest3Rep.fTest3=" << theTest3Rep->fTest3 << std::endl;
  //std::cout << "\t theTest3Rep.dTest3=" << theTest3Rep->dTest3 << std::endl;
  //std::cout << "Test1Mod->provides:" << std::endl;
  theTest1Rep.test1 = rand();
  //std::cout << "\t theTest1Rep.test1=" << theTest1Rep.test1 << std::endl;
}
