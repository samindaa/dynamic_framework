/*
 * Serializable.h
 *
 *  Created on: Jan 18, 2016
 *      Author: sabeyruw
 */

#ifndef SERIALIZABLE_H_
#define SERIALIZABLE_H_

#include "ObjectInput.h"
#include "ObjectOutput.h"
#include "BufferInputOutputStream.h"

class Serializable
{
  public:
    virtual ~Serializable() {}
    virtual void serialize(ObjectInput* in, ObjectOutput* out) =0;

    virtual size_t readFromBuffer(unsigned char *buffer)
    {
      BufferInputOutputStream b(buffer);
      ObjectInput in(&b);
      serialize(&in, 0);
      return b.bufferLocation;
    }

    virtual size_t writeToBuffer(unsigned char *buffer)
    {
      BufferInputOutputStream b(buffer);
      ObjectOutput out(&b);
      serialize(0, &out);
      return b.bufferLocation;
    }

  protected:
    void serializeBuffer(ObjectInput* in, ObjectOutput* out, unsigned char* p, const int& size)
    {
      if (in)
        in->read(p, size);
      if (out)
        out->write(p, size);
    }

    template <typename T> void serializeObject(ObjectInput* in, ObjectOutput* out, const char*, T& p)
    {
        if (in)
          in->read(p);
        if (out)
          out->write(p);
    }
};


#endif /* SERIALIZABLE_H_ */
