/*
 * ObjectOutputStream.h
 *
 *  Created on: Jan 18, 2016
 *      Author: sabeyruw
 */

#ifndef OBJECTOUTPUTSTREAM_H_
#define OBJECTOUTPUTSTREAM_H_

class ObjectOutputStream
{
  public:
    virtual ~ObjectOutputStream() {}
    virtual void write(const unsigned char* source, const int& size) =0;
};

#endif /* OBJECTOUTPUTSTREAM_H_ */
