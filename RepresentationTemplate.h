/*
 * RepresentationTemplate.h
 *
 *  Created on: Jan 18, 2016
 *      Author: sabeyruw
 */

#ifndef REPRESENTATIONTEMPLATE_H_
#define REPRESENTATIONTEMPLATE_H_

#include "Representation.h"

template<typename T>
class RepresentationTemplate : public Representation
{
  public: RepresentationTemplate()          {}
  public: virtual ~RepresentationTemplate() {}
  public: virtual unsigned int getSize() const { return sizeof(T); }
};



#endif /* REPRESENTATIONTEMPLATE_H_ */
