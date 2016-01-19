/*
 * RepresentationPointer.h
 *
 *  Created on: Jan 18, 2016
 *      Author: sabeyruw
 */

#ifndef REPRESENTATIONPOINTER_H_
#define REPRESENTATIONPOINTER_H_

#include "Controller.h"

template<const char* (*getModuleName)(), const char* (*getRepresentationName)(), typename T>
class RepresentationPointer
{
  protected:
    T* theInstance;

  public:
    RepresentationPointer() : theInstance(0) {  }
    virtual ~RepresentationPointer() { }

  protected:
    inline T* getRepresentation()
    {
      if (!theInstance)
        theInstance = (T*) Controller::getInstance().getRepresentation(getModuleName(), getRepresentationName());
      return theInstance;
    }

    inline T* getRepresentation() const
    {
      if (!theInstance)
        return theInstance;
      return (T*) Controller::getInstance().getRepresentation(getModuleName(), getRepresentationName()); //<< SLOW for CONST ACCESS
    }

  public:
    virtual bool isNull()                   { return (this->getRepresentation() == 0); }
    virtual bool operator==(const T* other) { return this->getRepresentation() == other; }
    virtual bool operator!=(const T* other) { return !(this->getRepresentation() == other); }

};



#endif /* REPRESENTATIONPOINTER_H_ */
