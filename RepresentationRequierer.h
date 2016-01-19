/*
 * RepresentationRequierer.h
 *
 *  Created on: Jan 18, 2016
 *      Author: sabeyruw
 */

#ifndef REPRESENTATIONREQUIERER_H_
#define REPRESENTATIONREQUIERER_H_

#include "RepresentationPointer.h"

template<const char* (*getModuleName)(), const char* (*getRepresentationName)(), typename T>
class RepresentationRequierer : public RepresentationPointer<getModuleName, getRepresentationName, T>
{
  public:
    RepresentationRequierer() { Controller::getInstance().requiredRepresentation(getModuleName(), getRepresentationName()); }
    virtual ~RepresentationRequierer() { }

  public:
    const T* operator->()        { return this->getRepresentation(); }
    const T* operator->() const  { return this->getRepresentation(); }
    const T& operator *()        { return *(this->getRepresentation()); }
    const T& operator *() const  { return *(this->getRepresentation()); }
    operator const   T*()        { return this->getRepresentation(); }
    operator const   T*() const  { return this->getRepresentation(); }

};




#endif /* REPRESENTATIONREQUIERER_H_ */
