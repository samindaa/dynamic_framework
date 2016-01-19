/*
 * ModuleLoader.h
 *
 *  Created on: Jan 18, 2016
 *      Author: sabeyruw
 */

#ifndef MODULELOADER_H_
#define MODULELOADER_H_

template <typename T>
class ModuleLoader
{
  private:
    T theInstance;

  public:
    ModuleLoader(const char* threadName, const int& threadPriority) { Controller::getInstance().addModule(threadName, threadPriority, &theInstance); }
    virtual ~ModuleLoader() { }
};



#endif /* MODULELOADER_H_ */
