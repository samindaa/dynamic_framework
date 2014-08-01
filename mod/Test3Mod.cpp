/*
 * Test3Mod.cpp
 *
 *  Created on: Jun 30, 2014
 *      Author: sam
 */

/*
 * Test2Mod.cpp
 *
 *  Created on: Jun 30, 2014
 *      Author: sam
 */

#include "Test3Mod.h"

MAKE_MODULE(Test3Mod, Thread Two, 25)

void Test3Mod::update(Test3Rep& theTest3Rep)
{
  std::cout << "Test3Mod->provides:" << std::endl;
  theTest3Rep.test3 = rand();
  std::cout << "\t theTest3Rep.test3=" << theTest3Rep.test3 << std::endl;
}

