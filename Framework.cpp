/*
 * Graph.cpp
 *
 *  Created on: Jul 12, 2013
 *      Author: sam
 */

#include "Framework.h"
//
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
//
#if !defined(EMBEDDED_MODE)
#include <iostream>
#include <vector> // with threads
#include <cstring>
#include <chrono>
#include <thread>
#include <pthread.h>
#include <sched.h>
#endif
/**
 *
 */
Controller* Controller::theInstance = 0;
/**
 *
 */
Controller::Controller() :
    threadsActivated(false), errorState(false)
{
}

Controller::~Controller()
{
  for (ThreadVector::iterator iter = threadVector.begin(); iter != threadVector.end(); iter++)
  {
    Thread* thread = *iter;
    // Release()
#if !defined(EMBEDDED_MODE)
    for (Thread::NodeVector::iterator iter2 = thread->operationVector.begin();
        iter2 != thread->operationVector.end(); iter2++)
    {
      Node* node = *iter2;
      if (node->isComputationNode())
        ((Module*) node)->config.persist();
    }
#endif
    delete thread;
  }
  threadVector.clear();
}

void Controller::addModule(const char* threadName, const int& threadPriority, Node* theInstance)
{
  // Check if a module type exits
  for (ModuleVector::iterator iter = moduleVector.begin(); iter != moduleVector.end(); iter++)
  {
    if (strcmp((*iter)->moduleNode->getName(), theInstance->getName()) == 0)
    {
      errorMsg += (" moduleByName=");
      errorMsg += (theInstance->getName());
      errorMsg += (" in threadName=");
      errorMsg += ((*iter)->threadName);
      errorMsg += ("exists!");
      forcedExit(errorMsg);
    }
  }

  theInstance->setComputationNode(true);
  ModuleEntry* newModuleEntry = new ModuleEntry(threadName, threadPriority, theInstance);
  moduleVector.push_back(newModuleEntry);
}

void Controller::providedRepresentation(const char* moduleName, Node* theInstance,
    void (*updateRepresentation)(Node*, Node*), RepresentationCloneable* representationCloneable)
{
  // Check if a representation type exits
  for (RepresentationVector::iterator iter = representationVector.begin();
      iter != representationVector.end(); iter++)
  {
    if (strcmp((*iter)->representationNode->getName(), theInstance->getName()) == 0)
    {
      errorMsg += (" representationByName=");
      errorMsg += (theInstance->getName());
      errorMsg += (" exists, and  providedModuleName=");
      errorMsg += ((*iter)->providedModuleName);
      forcedExit(errorMsg);
    }
  }
  RepresentationEntry* newRepresentationEntry = new RepresentationEntry(moduleName, theInstance,
      updateRepresentation, representationCloneable);
  representationVector.push_back(newRepresentationEntry);
}

void Controller::requiredRepresentation(const char* moduleName, const char* representationName)
{
  ModuleRepresentationEntry* newModuleRepresentationEntry = new ModuleRepresentationEntry(
      moduleName, representationName);
  moduleRepresentationRequiredVector.push_back(newModuleRepresentationEntry);
}

void Controller::usedRepresentation(const char* moduleName, const char* representationName)
{
  ModuleRepresentationEntry* newModuleRepresentationEntry = new ModuleRepresentationEntry(
      moduleName, representationName);
  moduleRepresentationUsedVector.push_back(newModuleRepresentationEntry);
}

/**
 * This is called at most once per required representation per module.
 */
Node* Controller::getRepresentation(const char* moduleName, const char* representationName)
{
  Node* moduleNode = 0;
  for (ThreadVector::iterator iter = threadVector.begin(); iter != threadVector.end(); iter++)
  {
    Thread* thread = *iter;
    for (Thread::NodeVector::iterator iter2 = thread->operationVector.begin();
        iter2 != thread->operationVector.end(); iter2++)
    {
      Node* node = *iter2;
      if (node->isComputationNode() && strcmp(node->getName(), moduleName) == 0)
      {
        moduleNode = node;
        break;
      }
    }
  }

  if (!moduleNode)
  {
    errorMsg += (" missing module. moduleName=");
    errorMsg += (moduleName);
    forcedExit(errorMsg);
    return 0;
  }

  Thread* thread = threadVector[moduleNode->getThreadIndex()];
  Node* representationNode = 0;
  for (Thread::NodeVector::iterator iter = thread->operationVector.begin();
      iter != thread->operationVector.end(); iter++)
  {
    Node* node = *iter;
    if (!node->isComputationNode() && strcmp(node->getName(), representationName) == 0)
    {
      if (!node->isInitialized())
      {
        errorMsg += (" missing representation. representationName=");
        errorMsg += (representationName);
        forcedExit(errorMsg);
      }
      representationNode = node;
      break;
    }
  }

  if (!representationNode)
  {
    for (Thread::NodeVector::iterator iter = thread->transferredVector.begin();
        iter != thread->transferredVector.end(); iter++)
    {
      Node* node = *iter;
      if (strcmp(node->getName(), representationName) == 0)
      {
        if (!node->isInitialized())
        {
          errorMsg += (" missing representation. representationName=");
          errorMsg += (representationName);
          forcedExit(errorMsg);
        }
        representationNode = node;
        break;
      }
    }
  }

  if (representationNode)
    return representationNode;

  errorMsg += (" this should not happen. representationName=");
  errorMsg += (representationName);
  forcedExit(errorMsg);
  return 0;
}

void Controller::computeGraph()
{
  // 0) Loading errors
  errorHandler();
  // 1) Add modules
  for (ModuleVector::iterator iter = moduleVector.begin(); iter != moduleVector.end(); iter++)
  {
    ModuleEntry* moduleEntry = *iter;
    Thread* theThread = 0;
    for (ThreadVector::iterator iter2 = threadVector.begin(); iter2 != threadVector.end(); iter2++)
    {
      if (strcmp((*iter2)->threadName, moduleEntry->threadName) == 0)
      {
        theThread = *iter2;
        break;
      }
    }

    if (threadsActivated)
    {
      if (!theThread)
      {
        theThread = new Thread(moduleEntry->threadName, moduleEntry->threadPriority,
            threadVector.size());
        threadVector.push_back(theThread);
      }
    }
    else
    {
      if (threadVector.empty())
      {
        theThread = new Thread("def", 20, threadVector.size());
        threadVector.push_back(theThread);
      }
      else
        theThread = threadVector[0];
    }

#if !defined(EMBEDDED_MODE)
    theThread->threadPriority = std::max(theThread->threadPriority, moduleEntry->threadPriority);
#else
    theThread->threadPriority = max(theThread->threadPriority, moduleEntry->threadPriority);
#endif
    theThread->graphStructureVector.push_back(moduleEntry->moduleNode);
    moduleEntry->moduleNode->setThreadIndex(theThread->threadIndex);
  }

  // 2) Provides representations
  for (RepresentationVector::iterator iter = representationVector.begin();
      iter != representationVector.end(); iter++)
  {
    RepresentationEntry* representationEntry = *iter;
    for (ModuleVector::iterator iter2 = moduleVector.begin(); iter2 != moduleVector.end(); iter2++)
    {
      ModuleEntry* moduleEntry = *iter2;
      if (strcmp(moduleEntry->moduleNode->getName(), representationEntry->providedModuleName) == 0)
      {
        representationEntry->representationNode->addPreviousNode(moduleEntry->moduleNode);
        ((Representation*) representationEntry->representationNode)->updateThis =
            representationEntry->update;
        moduleEntry->moduleNode->addNextNode(representationEntry->representationNode);
        threadVector[moduleEntry->moduleNode->getThreadIndex()]->graphStructureVector.push_back(
            representationEntry->representationNode);
        representationEntry->representationNode->setThreadIndex(
            moduleEntry->moduleNode->getThreadIndex());
      }
    }
  }

  // 3) Requires representations
  for (ModuleRepresentationVector::iterator iter = moduleRepresentationRequiredVector.begin();
      iter != moduleRepresentationRequiredVector.end(); iter++)
  {
    ModuleRepresentationEntry* moduleRepresentationEntry = *iter;
    Node *moduleNode = 0, *representationNode = 0;
    for (ModuleVector::iterator iter2 = moduleVector.begin(); iter2 != moduleVector.end(); iter2++)
    {
      ModuleEntry* moduleEntry = *iter2;
      if (strcmp(moduleEntry->moduleNode->getName(), moduleRepresentationEntry->requiredModuleName)
          == 0)
        moduleNode = moduleEntry->moduleNode;
    }

    for (RepresentationVector::iterator iter2 = representationVector.begin();
        iter2 != representationVector.end(); iter2++)
    {
      RepresentationEntry* representationEntry = *iter2;
      if (strcmp(representationEntry->representationNode->getName(),
          moduleRepresentationEntry->requiredRepresentationName) == 0)
        representationNode = representationEntry->representationNode;
    }

    if (moduleNode == 0)
    {
      errorMsg += (" requiredModuleName=");
      errorMsg += (moduleRepresentationEntry->requiredModuleName);
      errorMsg += (" is missing!");
#if !defined(EMBEDDED_MODE)
      forcedExit(errorMsg);
#else
      errorState = true;
#endif
    }
    if (representationNode == 0)
    {
      errorMsg += (" requiredRepresentationName=");
      errorMsg += (moduleRepresentationEntry->requiredRepresentationName);
      errorMsg += (" is missing! for requiredModuleName=");
      errorMsg += (moduleRepresentationEntry->requiredModuleName);
#if !defined(EMBEDDED_MODE)
      forcedExit(errorMsg);
#endif
    }
    if (!(moduleNode && representationNode))
    {
      errorMsg += (" moduleNode and/or representationNode are NULL!");
      forcedExit(errorMsg);
    }
    errorHandler();
    representationNode->addNextNode(moduleNode);
    moduleNode->addPreviousNode(representationNode);
  }

  // 4) Uses representation
  for (ModuleRepresentationVector::iterator iter = moduleRepresentationUsedVector.begin();
      iter != moduleRepresentationUsedVector.end(); iter++)
  {
    ModuleRepresentationEntry* moduleRepresentationEntry = *iter;
    Node *moduleNode = 0, *representationNode = 0;
    for (ModuleVector::iterator iter2 = moduleVector.begin(); iter2 != moduleVector.end(); iter2++)
    {
      ModuleEntry* moduleEntry = *iter2;
      if (strcmp(moduleEntry->moduleNode->getName(), moduleRepresentationEntry->requiredModuleName)
          == 0)
        moduleNode = moduleEntry->moduleNode;
    }

    for (RepresentationVector::iterator iter2 = representationVector.begin();
        iter2 != representationVector.end(); iter2++)
    {
      RepresentationEntry* representationEntry = *iter2;
      if (strcmp(representationEntry->representationNode->getName(),
          moduleRepresentationEntry->requiredRepresentationName) == 0)
        representationNode = representationEntry->representationNode;
    }

    if (!(moduleNode && representationNode))
    {
      errorMsg += (" moduleNode and representationNode are NULL!");
      forcedExit(errorMsg);
    }
    errorHandler();
    representationNode->addAuxiliaryNode(moduleNode);
  }
}

void Controller::sort()
{
  // Calculate in-degrees
  for (ThreadVector::iterator iter = threadVector.begin(); iter != threadVector.end(); iter++)
  {
    Thread* thread = *iter;
    Thread::NodeVector& graphStructureVector = thread->graphStructureVector;

    for (Thread::NodeVector::iterator i = graphStructureVector.begin();
        i != graphStructureVector.end(); i++)
    {
      Node* x = *i;
      x->setInitialized(false);
      for (Node::NodeVector::iterator j = x->getNextNodes().begin(); j != x->getNextNodes().end();
          j++)
      {
        Node* y = *j;
        if (thread->threadIndex == y->getThreadIndex())
        {
          ++(*y);
          y->setInitialized(false);
        }
      }
    }

    for (Thread::NodeVector::iterator i = graphStructureVector.begin();
        i != graphStructureVector.end(); i++)
    {
      Node* x = *i;
      if (x->isComputationNode())
      {
        for (Node::NodeVector::iterator j = x->getPreviousNodes().begin();
            j != x->getPreviousNodes().end(); j++)
        {
          Node* y = *j;
          if (thread->threadIndex != y->getThreadIndex())
          {
            // Find the cloneable entry
            for (RepresentationVector::iterator iter = representationVector.begin();
                iter != representationVector.end(); iter++)
            {
              RepresentationEntry* representationEntry = *iter;
              if (strcmp(representationEntry->representationNode->getName(), y->getName()) == 0)
              {
                Node* newNode = representationEntry->representationCloneable->clone(y);
                newNode->setThreadIndex(thread->threadIndex);
                newNode->setInitialized(true);
                newNode->addNextNode(x);
                thread->transferredVector.push_back(newNode);
                break;
              }
            }
          }
        }
      }
      if (x->getInDegrees() == 0)
        thread->topoQueue.push_back(x);
    }

    // Main loop
    while (!thread->topoQueue.empty())
    {
      Node* x = thread->topoQueue.front();
      thread->topoQueue.erase(0);
      if (x->isInitialized())
      {
        thread->operationVector.push_back(x);
        errorMsg += (" cycle detected in main loop at=");
        errorMsg += (x->getName());
        errorMsg += ("\n");
        errorMsg += (" for thread=");
        errorMsg += (thread->threadName);
        errorMsg += ("\n");
        int tabCounter = 0;
        for (Thread::NodeVector::const_iterator j = thread->operationVector.begin();
            j != thread->operationVector.end(); j++)
        {
          for (int k = 0; k < tabCounter; k++)
            errorMsg += ("\t");
          const Node* y = *j;
          errorMsg += (y->getName());
          errorMsg += ("\n");
          ++tabCounter;
        }
        forcedExit(errorMsg);
#if defined(EMBEDDED_MODE)
        errorHandler();
#endif
      }
      x->setInitialized(true);
      thread->operationVector.push_back(x);
      for (Node::NodeVector::iterator j = x->getNextNodes().begin(); j != x->getNextNodes().end();
          j++)
      {
        Node* y = *j;
        if (thread->threadIndex == y->getThreadIndex())
        {
          --(*y);
          if (y->getInDegrees() == 0)
            thread->topoQueue.push_back(y);
        }
      }
    }

    if (thread->operationVector.size() != thread->graphStructureVector.size())
    {
      errorMsg += (" cycle detected!");
      errorMsg += ("\n");
      errorMsg += (" for thread=");
      errorMsg += (thread->threadName);
      errorMsg += ("\n");
      forcedExit(errorMsg);
#if defined(EMBEDDED_MODE)
      errorHandler();
#endif
    }

    if (thread->operationVector.size() != thread->graphStructureVector.size())
    {
      errorMsg += ("graphOutput.size() != graphStructureVector.size()");
      forcedExit(errorMsg);
#if defined(EMBEDDED_MODE)
      errorHandler();
#endif
    }
  }

  // Purge entries
  purgeEntries();
}

void Controller::initialize(const bool& threadsActivated)
{
  /*Graph manipulations*/
  this->threadsActivated = threadsActivated;
  computeGraph();
  sort();
  stream();
}

void Controller::setup(unsigned long baudRate, const bool& threadsActivated)
{
#if defined(EMBEDDED_MODE)
  /* Some boards names are not defined.
   * If the code doesn't compile remove those
   * lines below.
   */
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BLUE_LED, LOW);

  pinMode(PUSH1, INPUT_PULLUP); // left - note _PULLUP
  pinMode(PUSH2, INPUT_PULLUP);// right - note _PULLUP

  /**Serial*/
  Serial.begin(baudRate);
  // I2C
  Wire.begin();
#endif

  initialize(threadsActivated);

  /*Initialize modules in threads*/
  for (ThreadVector::iterator thread = threadVector.begin(); thread != threadVector.end(); thread++)
    threadAllocate(*thread);
}

void Controller::loop()
{
  /*Update modules and representations in threads*/
  for (ThreadVector::iterator thread = threadVector.begin(); thread != threadVector.end(); thread++)
  {
#if !defined(EMBEDDED_MODE)
    threadTransfer(*thread);
#endif
    threadUpdate(*thread);
  }
}

#if !defined(EMBEDDED_MODE)
void Controller::mainLoop()
{
  if (!threadsActivated || (threadVector.size() <= 1))
  {
    for (ThreadVector::iterator thread = threadVector.begin(); thread != threadVector.end();
        thread++)
      threadAllocate(*thread);

    for (;;)
    {
      for (ThreadVector::iterator thread = threadVector.begin(); thread != threadVector.end();
          thread++)
      {
        threadTransfer(*thread);
        threadUpdate(*thread);
      }
    }
  }
  else
    mainThreadLoop();
}

void Controller::mainThreadLoop()
{
  std::vector<std::thread> threads;
  for (ThreadVector::iterator thread = threadVector.begin(); thread != threadVector.end(); thread++)
  {
    threads.push_back(std::thread(threadLoop, *thread));
    pthread_t threadHandle = threads[threads.size() - 1].native_handle();
    sched_param sch;
    int policy;
    if (pthread_getschedparam(threadHandle, &policy, &sch) != 0)
      std::cout << "Unsuccessful in pthread_getschedparam" << std::endl;
    else
      std::cout << "Successful in pthread_getschedparam" << std::endl;

    sch.sched_priority = (*thread)->threadPriority; //<< configurable
    if (pthread_setschedparam(threadHandle, SCHED_FIFO, &sch))
      std::cout << "Failed to pthread_setschedparam: " << std::strerror(errno)
          << " threadPriority: " << (*thread)->threadPriority << std::endl;
    else
      std::cout << "Set pthread_setschedparam to threadPriority: " << (*thread)->threadPriority
          << std::endl;
  }

  for (std::vector<std::thread>::iterator iter = threads.begin(); iter != threads.end(); iter++)
    (*iter).join();
}

void Controller::main(const bool& threadsActivated)
{
  initialize(threadsActivated);
  mainLoop();
}

void Controller::threadLoop(Thread* thread)
{
  thread->isActive = true;
  threadAllocate(thread);
  while (thread->isActive)
  {
    threadTransfer(thread);
    threadUpdate(thread);
    //std::this_thread::sleep_for(std::chrono::milliseconds(5)); //<< debug and timing
  }
}

void Controller::threadTransfer(Thread* thread)
{
  for (Thread::NodeVector::iterator iter2 = thread->transferredVector.begin();
      iter2 != thread->transferredVector.end(); ++iter2)
  {
    Representation* thisNode = (Representation*) *iter2;
    Representation* thatNode = (Representation*) thisNode->getTransferredNode();
    if (thread->isActive)
      thatNode->sync.lock();
    ((Serializable*) thatNode)->writeToBuffer(thread->buffer);
    ((Serializable*) thisNode)->readFromBuffer(thread->buffer);
    if (thread->isActive)
      thatNode->sync.unlock();
  }
}

#endif

void Controller::threadAllocate(Thread* thread)
{
  for (Thread::NodeVector::iterator iter2 = thread->operationVector.begin();
      iter2 != thread->operationVector.end(); iter2++)
  {
    // 1) Init()
    Node* node = *iter2;
    if (node->isComputationNode())
    {
      Module* module = (Module*) node;
#if !defined(EMBEDDED_MODE)
      module->config.setName(module->getName());
      module->config.setPath("config");
      module->config.resurrect();
#endif
      module->init();
#if defined(EMBEDDED_MODE)
      Serial.println(node->getName());
#endif
    }
  }
}

void Controller::threadUpdate(Thread* thread)
{
  // 2) Execute / Update
  for (Thread::NodeVector::iterator iter2 = thread->operationVector.begin();
      iter2 != thread->operationVector.end(); iter2++)
  {
    // 2.1) Execute() / 2.2) Update()
    Node* node = *iter2;
    if (node->isComputationNode())
      ((Module*) node)->execute();
    else
    {
#if !defined(EMBEDDED_MODE)
      if (thread->isActive)
        ((Representation*) node)->sync.lock();
#endif
      ((Representation*) node)->updateThis(*node->getPreviousNodes().begin(), node);
#if !defined(EMBEDDED_MODE)
      if (thread->isActive)
        ((Representation*) node)->sync.unlock();
#endif
      ((Representation*) node)->draw();
    }
  }
}

void Controller::purgeEntries()
{
  for (ModuleVector::iterator iter = moduleVector.begin(); iter != moduleVector.end(); iter++)
    delete *iter;
  for (RepresentationVector::iterator iter = representationVector.begin();
      iter != representationVector.end(); iter++)
    delete *iter;
  for (ModuleRepresentationVector::iterator iter = moduleRepresentationRequiredVector.begin();
      iter != moduleRepresentationRequiredVector.end(); iter++)
    delete *iter;
  for (ModuleRepresentationVector::iterator iter = moduleRepresentationUsedVector.begin();
      iter != moduleRepresentationUsedVector.end(); iter++)
    delete *iter;
  moduleVector.clear();
  representationVector.clear();
  moduleRepresentationRequiredVector.clear();
  moduleRepresentationUsedVector.clear();

  for (ThreadVector::iterator iter = threadVector.begin(); iter != threadVector.end(); iter++)
  {
    Thread* thread = *iter;
    thread->graphStructureVector.clear();
    thread->topoQueue.clear();
  }
}

void Controller::closeThreads()
{
  for (ThreadVector::iterator iter = threadVector.begin(); iter != threadVector.end(); iter++)
    (*iter)->isActive = false;
}

Controller & Controller::getInstance()
{
  if (!theInstance)
    theInstance = new Controller;
  return *theInstance;
}

void Controller::deleteInstance()
{
  if (theInstance)
  {
    theInstance->closeThreads();
    delete theInstance;
  }
  theInstance = 0;
}
