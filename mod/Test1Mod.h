/*
 * Test1Mod.h
 *
 *  Created on: Jun 30, 2014
 *      Author: sam
 */

#ifndef TEST1MOD_H_
#define TEST1MOD_H_

#include "Template.h"
#include "rep/Test2Rep.h"
#include "rep/Test3Rep.h"
#include "rep/Test1Rep.h"

MODULE(Test1Mod)
  REQUIRES(Test2Rep) //
  REQUIRES(Test3Rep) //
  PROVIDES(Test1Rep)
END_MODULE
class Test1Mod: public Test1ModBase
{
  public:
    void update(Test1Rep&);
};

#endif /* TEST1MOD_H_ */
