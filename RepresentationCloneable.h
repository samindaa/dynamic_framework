/*
 * RepresentationCloneable.h
 *
 *  Created on: Jan 18, 2016
 *      Author: sabeyruw
 */

#ifndef REPRESENTATIONCLONEABLE_H_
#define REPRESENTATIONCLONEABLE_H_

#include "Node.h"

class RepresentationCloneable
{
  public: RepresentationCloneable() {}
  public: virtual ~RepresentationCloneable() {}
  public: virtual Node* clone(Node* that)  =0;
};



#endif /* REPRESENTATIONCLONEABLE_H_ */
