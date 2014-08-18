/*
 * Test2Mod.h
 *
 *  Created on: Jun 30, 2014
 *      Author: sam
 */

#ifndef TEST2MOD_H_
#define TEST2MOD_H_

#include "Template.h"
#include "rep/Test1Rep.h"
#include "rep/Test2Rep.h"

MODULE(Test2Mod)
  REQUIRES(Test1Rep) //
  PROVIDES(Test2Rep)
END_MODULE
class Test2Mod: public Test2ModBase
{
  public:
    void update(Test2Rep&);
};

#endif /* TEST2MOD_H_ */
