/*
 * Test3Mod.h
 *
 *  Created on: Jun 30, 2014
 *      Author: sam
 */

#ifndef TEST3MOD_H_
#define TEST3MOD_H_

#include "Template.h"
#include "rep/Test3Rep.h"

MODULE(Test3Mod)
  PROVIDES(Test3Rep)
END_MODULE
class Test3Mod: public Test3ModBase
{
  public:
    void update(Test3Rep&);
};

#endif /* TEST3MOD_H_ */
