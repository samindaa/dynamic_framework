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

MAKE_MODULE(Test3Mod, Thread Two, 3)

void Test3Mod::update(Test3Rep& theTest3Rep)
{
  std::cout << "Test3Mod->provides:" << std::endl;
  theTest3Rep.iTest3 = rand();
  theTest3Rep.fTest3 = ((float) rand()) / 12345.0f;
  theTest3Rep.dTest3 = ((double) rand()) / 12345.0f;
  std::cout << "\t theTest3Rep.iTest3=" << theTest3Rep.iTest3 << std::endl;
  std::cout << "\t theTest3Rep.fTest3=" << theTest3Rep.fTest3 << std::endl;
  std::cout << "\t theTest3Rep.dTest3=" << theTest3Rep.dTest3 << std::endl;
}

