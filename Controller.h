/*
 * Controller.h
 *
 *  Created on: Jan 18, 2016
 *      Author: sabeyruw
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#if defined(ENERGIA) && !defined(EMBEDDED_MODE)
#define EMBEDDED_MODE
#endif
//
#if defined(EMBEDDED_MODE)
#include "Energia.h"
#include "pins_energia.h"
#include "Wire.h"
#endif
//
#if !defined(EMBEDDED_MODE)
#include <iostream>
#endif
//
#include "RepresentationCloneable.h"
#include "Thread.h"
/**
 * This framework creates a topologically sorted graph from computational and representational units
 * created by the user. Since, the framework needs to be run on a MCU, the size (in bytes) of the
 * framework is minimized. At most INT_MAX nodes can be allocated within the framework.
 */
class Controller
{
  private:
    class ModuleEntry
    {
      public:
        const char* threadName;
        const int threadPriority;
        Node* moduleNode;
        ModuleEntry(const char* threadName, const int& threadPriority, Node* moduleNode) : threadName(threadName), threadPriority(threadPriority), moduleNode(moduleNode) {}
    };

    class RepresentationEntry
    {
      public:
        const char* providedModuleName;
        Node* representationNode;
        void (*update)(Node*, Node*);
        RepresentationCloneable* representationCloneable;

        RepresentationEntry(const char* providedModuleName, Node* representationNode, void (*update)(Node*, Node*), RepresentationCloneable* representationCloneable) :
            providedModuleName(providedModuleName), representationNode(representationNode), update(update), representationCloneable(representationCloneable) {}
    };

    class ModuleRepresentationEntry
    {
      public:
        const char* requiredModuleName;
        const char* requiredRepresentationName;

        ModuleRepresentationEntry(const char*  requiredModuleName, const char* requiredRepresentationName):
          requiredModuleName(requiredModuleName), requiredRepresentationName(requiredRepresentationName) {}
    };

    // For topological sort
    bool threadsActivated;
    bool errorState;
#if defined(EMBEDDED_MODE)
    typedef String MyString;
#else
    typedef std::string MyString;
#endif
    MyString errorMsg;

    typedef Vector<ModuleEntry*> ModuleVector;
    typedef Vector<RepresentationEntry*> RepresentationVector;
    typedef Vector<ModuleRepresentationEntry*> ModuleRepresentationVector;
    typedef Vector<Thread*> ThreadVector;

    ModuleVector moduleVector;
    RepresentationVector representationVector;
    ModuleRepresentationVector moduleRepresentationRequiredVector;
    ModuleRepresentationVector moduleRepresentationUsedVector;
    ThreadVector threadVector;

    void initialize(const bool& threadsActivated);
    /** Computational resources */
    void computeGraph();
    void sort();

    static void threadAllocate(Thread* thread);
    static void threadUpdate(Thread* thread);

#if !defined(EMBEDDED_MODE)
    static void threadTransfer(Thread* thread);
    static void threadLoop(Thread* thread);

    void mainLoop();
    void mainThreadLoop();
#endif

    void closeThreads();
    void errorHandler();
    void purgeEntries();
    void forcedExit(const MyString& errorMsg);

  protected:
    Controller();
    ~Controller();
    Controller(Controller const&);
    Controller& operator=(Controller const&);
    static Controller* theInstance;

  public:
    static Controller& getInstance();
    static void deleteInstance();
    void addModule(const char* threadName, const int& threadPriority, Node* theInstance);
    void providedRepresentation(const char* moduleName, Node* theInstance, void (*updateRepresentation)(Node* , Node* ), RepresentationCloneable* representationCloneable);
    void requiredRepresentation(const char* moduleName, const char* representationName);
    void usedRepresentation(const char* moduleName, const char* representationName);
    Node* getRepresentation(const char* moduleName, const char* representationName);

#if !defined(EMBEDDED_MODE)
    /* Entry points for the user */
    void main(const bool& threadsActivated);
#endif
    /*Give control to other threads*/
    void setup(unsigned long baudRate, const bool& threadsActivated = false);
    void loop();
    /** verbose */
    void verbose();
};



#endif /* CONTROLLER_H_ */
