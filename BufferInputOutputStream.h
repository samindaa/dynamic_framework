/*
 * BufferInputOutputStream.h
 *
 *  Created on: Jan 18, 2016
 *      Author: sabeyruw
 */

#ifndef BUFFERINPUTOUTPUTSTREAM_H_
#define BUFFERINPUTOUTPUTSTREAM_H_

#include "ObjectInputStream.h"
#include "ObjectOutputStream.h"
//
#include <string.h>

class BufferInputOutputStream : public ObjectInputStream, public ObjectOutputStream
{
  public:
    unsigned char* buffer;
    int bufferLocation;

    BufferInputOutputStream(unsigned char* buffer) : buffer(buffer), bufferLocation(0) {}
    virtual ~BufferInputOutputStream() {}
    void read(unsigned char* destination, const int& size) { memcpy(destination, buffer + bufferLocation, size); bufferLocation += size; }
    void write(const unsigned char* source, const int& size) { memcpy(buffer + bufferLocation, source, size); bufferLocation += size; }
    void reset() { bufferLocation = 0; }
};



#endif /* BUFFERINPUTOUTPUTSTREAM_H_ */
