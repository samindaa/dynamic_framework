/*
 * ObjectInput.h
 *
 *  Created on: Jan 18, 2016
 *      Author: sabeyruw
 */

#ifndef OBJECTINPUT_H_
#define OBJECTINPUT_H_

#include "ObjectInputStream.h"

class ObjectInput
{
  private:
    ObjectInputStream* in;

  public:
    ObjectInput(ObjectInputStream* in) : in(in) {}
    virtual ~ObjectInput() {}

                      void read(unsigned char* destination, const int& size) { in->read(destination, size); }
    template<typename T> void read(T& d) { read((unsigned char*) &d, sizeof(T)); }
};


#endif /* OBJECTINPUT_H_ */
