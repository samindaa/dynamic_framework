/*
 * TivaCTestModule.h
 *
 *  Created on: Jan 18, 2016
 *      Author: sabeyruw
 */

#ifndef TIVACTESTMODULE_H_
#define TIVACTESTMODULE_H_

#include "Template.h"

MODULE(TivaCTestModule)
END_MODULE

class TivaCTestModule : public TivaCTestModuleBase
{
  public:
    uint8_t state;

    TivaCTestModule();
    ~TivaCTestModule();
    void execute();
};

#endif /* TIVACTESTMODULE_H_ */
