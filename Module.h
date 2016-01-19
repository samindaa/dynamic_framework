/*
 * Module.h
 *
 *  Created on: Jan 18, 2016
 *      Author: sabeyruw
 */

#ifndef MODULE_H_
#define MODULE_H_

#include "Node.h"
#if !defined(EMBEDDED_MODE)
#include "Config.h"
#endif

class Module : public Node
{
#if !defined(EMBEDDED_MODE)
  public: Config config;
#endif
  public: Module() : Node()       {}
  public: virtual ~Module()       {}
  public: virtual void init()     {}
  public: virtual void execute()  {}
};



#endif /* MODULE_H_ */
