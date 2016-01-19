/*
 * ObjectOutput.h
 *
 *  Created on: Jan 18, 2016
 *      Author: sabeyruw
 */

#ifndef OBJECTOUTPUT_H_
#define OBJECTOUTPUT_H_

#include "ObjectOutputStream.h"

class ObjectOutput
{
  private:
    ObjectOutputStream* out;

  public:
    ObjectOutput(ObjectOutputStream* out) : out(out) {}
    virtual ~ObjectOutput() {}

                      void write(const unsigned char* source, const int& size) { out->write(source, size); }
    template<typename T> void write(const T& d) { write((const unsigned char*) &d, sizeof(T)); }
};



#endif /* OBJECTOUTPUT_H_ */
