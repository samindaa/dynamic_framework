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
    int iTest3;
    float fTest3;
    double dTest3;

    Test3Rep() :
        iTest3(0), fTest3(0.0f), dTest3(0.0f)
    {
    }

    void serialize(ObjectInput* in, ObjectOutput* out)
    {
      SERIALIZE(iTest3)
      SERIALIZE(fTest3)
      SERIALIZE(dTest3)
    }
};

#endif /* TEST3REP_H_ */
