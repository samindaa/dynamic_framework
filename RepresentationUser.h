/*
 * RepresentationUser.h
 *
 *  Created on: Jan 18, 2016
 *      Author: sabeyruw
 */

#ifndef REPRESENTATIONUSER_H_
#define REPRESENTATIONUSER_H_

#include "RepresentationPointer.h"

template<const char* (*getModuleName)(), const char* (*getRepresentationName)(), typename T>
class RepresentationUser : public RepresentationPointer<getModuleName, getRepresentationName, T>
{
  public:
    RepresentationUser() { Controller::getInstance().usedRepresentation(getModuleName(), getRepresentationName()); }
    virtual ~RepresentationUser() { }

  public:
    const T* operator->()        { return this->getRepresentation(); }
    const T* operator->() const  { return this->getRepresentation(); }
    const T& operator *()        { return *(this->getRepresentation()); }
    const T& operator *() const  { return *(this->getRepresentation()); }
    operator const   T*()        { return this->getRepresentation(); }
    operator const   T*() const  { return this->getRepresentation(); }

};



#endif /* REPRESENTATIONUSER_H_ */
