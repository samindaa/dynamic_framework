/*
 * Test3Rep.h
 *
 *  Created on: Jun 30, 2014
 *      Author: sam
 */

#ifndef TEST3REP_H_
#define TEST3REP_H_

#include "Template.h"

REPRESENTATION(Test3Rep)
class Test3Rep: public Test3RepBase
{
  public:
    int test3;
    Test3Rep() :
        test3(0)
    {
    }
};

#endif /* TEST3REP_H_ */
