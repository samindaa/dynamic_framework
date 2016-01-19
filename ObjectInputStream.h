/*
 * ObjectInputStream.h
 *
 *  Created on: Jan 18, 2016
 *      Author: sabeyruw
 */

#ifndef OBJECTINPUTSTREAM_H_
#define OBJECTINPUTSTREAM_H_

// Streams
class ObjectInputStream
{
  public:
    virtual ~ObjectInputStream() {}
    virtual void read(unsigned char* destination, const int& size) =0;
};


#endif /* OBJECTINPUTSTREAM_H_ */
