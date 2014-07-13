/*
 * Test1Rep.h
 *
 *  Created on: Jun 30, 2014
 *      Author: sam
 */

#ifndef TEST1REP_H_
#define TEST1REP_H_

#include "Template.h"

REPRESENTATION(Test1Rep)
class Test1Rep: public Test1RepBase
{
  public:
    int test1;
    Test1Rep() :
        test1(0)
    {
    }
};

#endif /* TEST1REP_H_ */
