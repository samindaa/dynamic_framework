/*
 * Representation.h
 *
 *  Created on: Jan 18, 2016
 *      Author: sabeyruw
 */

#ifndef REPRESENTATION_H_
#define REPRESENTATION_H_

#include "Node.h"
#include "Serializable.h"
#if !defined(EMBEDDED_MODE)
#include <mutex>
#endif

#define SERIALIZE_BUFFER(NAME, IN_OUT, SIZE)  serializeBuffer(in, out, IN_OUT, SIZE);

// -- Representations
class Representation: public Node, public Serializable
{
  public: void (*updateThis)(Node* , Node* );
  public: Representation() : Node(), updateThis(0)  {}
  public: virtual ~Representation()                 {}
  public: virtual void draw() const                 {}
  public:
    virtual void serialize(ObjectInput* in, ObjectOutput* out)
    {
      const size_t baseSize = sizeof(Representation);
      unsigned char* p = (unsigned char*) this;
      SERIALIZE_BUFFER(getName(), p + baseSize, getSize() - baseSize);
    }
#if !defined(EMBEDDED_MODE)
  public:
    std::mutex sync;
#endif
};




#endif /* REPRESENTATION_H_ */
