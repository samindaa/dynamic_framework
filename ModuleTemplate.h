/*
 * ModuleTemplate.h
 *
 *  Created on: Jan 18, 2016
 *      Author: sabeyruw
 */

#ifndef MODULETEMPLATE_H_
#define MODULETEMPLATE_H_

#include "Module.h"

template<typename T>
class ModuleTemplate : public Module
{
  public: ModuleTemplate()          {}
  public: virtual ~ModuleTemplate() {}
  public: virtual unsigned int getSize() const { return sizeof(T); }
};



#endif /* MODULETEMPLATE_H_ */
