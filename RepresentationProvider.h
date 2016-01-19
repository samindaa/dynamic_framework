/*
 * RepresentationProvider.h
 *
 *  Created on: Jan 18, 2016
 *      Author: sabeyruw
 */

#ifndef REPRESENTATIONPROVIDER_H_
#define REPRESENTATIONPROVIDER_H_

#include "Controller.h"

template<const char* (*getModuleName)(), void (*updateRepresentation)(Node*, Node*), class T>
class RepresentationProvider : public RepresentationCloneable
{
  private :
    T theInstance;

  public:
    RepresentationProvider() { Controller::getInstance().providedRepresentation(getModuleName(), &theInstance, updateRepresentation, this); }
    virtual ~RepresentationProvider() { }
    Node* clone(Node* that)
    {
      Node* newNode = new T();
      newNode->setTransferredNode(that);
      return newNode;
    }
};




#endif /* REPRESENTATIONPROVIDER_H_ */
