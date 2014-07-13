/*
 * Test2Rep.h
 *
 *  Created on: Jun 30, 2014
 *      Author: sam
 */

#ifndef TEST2REP_H_
#define TEST2REP_H_

#include "Template.h"

REPRESENTATION(Test2Rep)
class Test2Rep: public Test2RepBase
{
  public:
    int test2;
    Test2Rep() :
        test2(0)
    {
    }
};

#endif /* TEST2REP_H_ */
