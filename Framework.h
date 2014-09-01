/*
 * Graph.h
 *
 *  Created on: Jul 12, 2013
 *      Author: sam
 */

#ifndef FRAMEWORK_H_
#define FRAMEWORK_H_

#if defined(ENERGIA)
#define EMBEDDED_MODE
#endif
//
#if defined(EMBEDDED_MODE)
#include "Energia.h"
#include "pins_energia.h"
#include "Wire.h"
#endif
//
#include <string.h>
//
#if !defined(EMBEDDED_MODE)
#include <mutex>
#include "Config.h"
#endif

/**
 * This framework creates a topologically sorted graph from computational and representational units
 * created by the user. Since, the framework needs to be run on a MCU, the size (in bytes) of the
 * framework is minimized. At most INT_MAX nodes can be allocated within the framework.
 */

/**
 * This is a very simple vector class
 */
template<typename T>
class Vector
{
  public:
    typedef T* iterator;
    typedef const T* const_iterator;

  private:
    int theSize;
    int theCapacity;
    T* objects;

  public:
    explicit Vector() :
        theSize(0), theCapacity(0), objects(0)
    {
    }

    Vector(const Vector& that) :
        objects(0)
    {
      operator=(that);
    }

    virtual ~Vector()
    {
      clear();
    }

    const Vector & operator=(const Vector& that)
    {
      if (this != &that)
      {
        if (objects)
          free(objects);
        objects = 0;
        theSize = that.size();
        theCapacity = that.theCapacity;

        objects = (T*)malloc(capacity() * sizeof(T));
        for (int k = 0; k < size(); k++)
          objects[k] = that.objects[k];

      }

      return *this;
    }

    void resize(int newSize)
    {
      if (newSize > theCapacity)
        reserve(newSize * 2 + 1);
      theSize = newSize;
    }

    void reserve(int newCapacity)
    {
      if (newCapacity < theSize)
        return;

      T *oldArray = objects;

      objects =  (T*)malloc(newCapacity * sizeof(T));
      for (int k = 0; k < theSize; k++)
        objects[k] = oldArray[k];

      theCapacity = newCapacity;

      if (oldArray)
       free(oldArray);
    }

    T& operator[](int index) { return objects[index]; }
    const T& operator[](int index) const { return objects[index]; }
    bool empty() const { return size() == 0;  }
    int size() const { return theSize; }
    int capacity() const { return theCapacity; }

    void push_back(T x)
    {
      if (theSize == theCapacity)
        reserve(2 * theCapacity + 1);
      objects[theSize++] = x;
    }

    const T& front() const { return objects[0]; }
    T& front() { return objects[0]; }

    void erase(int index)
    {
      for (int i = index; i < size() - 1; ++i)    // for each item that follows 'index'
        objects[i] = objects[i + 1];            // shift the item down one slot in memory
      --theSize;
    }

    void clear()
    {
      theSize = 0;
      theCapacity = 0;
      if (objects)
        delete[] objects;
      objects = 0;
    }

    iterator begin()             { return objects; }
    iterator end()               { return objects + theSize; }
    const_iterator begin() const { return objects; }
    const_iterator end()   const { return objects + theSize; }

};

// Streams
class ObjectInputStream
{
  public:
    virtual ~ObjectInputStream() {}
    virtual void read(unsigned char* destination, const int& size) =0;
};

class ObjectOutputStream
{
  public:
    virtual ~ObjectOutputStream() {}
    virtual void write(const unsigned char* source, const int& size) =0;
};

class BufferInputOutputStream : public ObjectInputStream, public ObjectOutputStream
{
  public:
    unsigned char* buffer;
    int bufferLocation;

    BufferInputOutputStream(unsigned char* buffer) : buffer(buffer), bufferLocation(0) {}
    virtual ~BufferInputOutputStream() {}
    void read(unsigned char* destination, const int& size) { memcpy(destination, buffer + bufferLocation, size); bufferLocation += size; }
    void write(const unsigned char* source, const int& size) { memcpy(buffer + bufferLocation, source, size); bufferLocation += size; }
    void reset() { bufferLocation = 0; }
};

class ObjectInput
{
  private:
    ObjectInputStream* in;

  public:
    ObjectInput(ObjectInputStream* in) : in(in) {}
    virtual ~ObjectInput() {}

                      void read(unsigned char* destination, const int& size) { in->read(destination, size); }
    template<class T> void read(T& d) { read((unsigned char*) &d, sizeof(T)); }
};

class ObjectOutput
{
  private:
    ObjectOutputStream* out;

  public:
    ObjectOutput(ObjectOutputStream* out) : out(out) {}
    virtual ~ObjectOutput() {}

                      void write(const unsigned char* source, const int& size) { out->write(source, size); }
    template<class T> void write(const T& d) { write((const unsigned char*) &d, sizeof(T)); }
};

class Serializable
{
  public:
    virtual ~Serializable() {}
    virtual void serialize(ObjectInput* in, ObjectOutput* out) =0;

    virtual size_t readFromBuffer(unsigned char *buffer)
    {
      BufferInputOutputStream b(buffer);
      ObjectInput in(&b);
      serialize(&in, 0);
      return b.bufferLocation;
    }

    virtual size_t writeToBuffer(unsigned char *buffer)
    {
      BufferInputOutputStream b(buffer);
      ObjectOutput out(&b);
      serialize(0, &out);
      return b.bufferLocation;
    }

  protected:
    void serializeBuffer(ObjectInput* in, ObjectOutput* out, unsigned char* p, const int& size)
    {
      if (in)
        in->read(p, size);
      if (out)
        out->write(p, size);
    }

    template <class T> void serializeObject(ObjectInput* in, ObjectOutput* out, const char*, T& p)
    {
        if (in)
          in->read(p);
        if (out)
          out->write(p);
    }
};

#define SERIALIZE_BUFFER(NAME, IN_OUT, SIZE)  serializeBuffer(in, out, IN_OUT, SIZE);

// ADT's for building the graph.
/**
 * Every object in the graph is an instance of a Node class. But this node
 * behaves differently according to its responsibilities.
 */
class Node
{
  public:
    typedef Vector<Node*> NodeVector;

  private:
    NodeVector nextNodes;
    NodeVector auxiliaryNodes;
    NodeVector previousNodes;
    int inDegrees;
    bool initialized;
    bool computationNode;
    int threadIndex;
    Node* transferredNode;

  public:
    explicit Node() : inDegrees(0), initialized(false), computationNode(false), threadIndex(0), transferredNode(0) {}
    virtual ~Node() { nextNodes.clear(); auxiliaryNodes.clear(); previousNodes.clear(); }
    bool isInitialized()                                const { return initialized; }
    void setInitialized(const bool initialized)               { this->initialized = initialized; }
    bool isComputationNode()                            const { return computationNode; }
    void setComputationNode(const bool computationNode)       { this->computationNode = computationNode; }
    void addAuxiliaryNode(Node* that)                         { this->auxiliaryNodes.push_back(that);  }
    void addPreviousNode(Node* that)                          { this->previousNodes.push_back(that); }
    void addNextNode(Node* that)                              { this->nextNodes.push_back(that);  }

    bool isPreviousNodesEmpty()                         const { return this->previousNodes.empty(); }
    bool auxiliaryNodesEmpty()                          const { return this->auxiliaryNodes.empty(); }
    NodeVector& getNextNodes()                                { return this->nextNodes; }
    NodeVector& getPreviousNodes()                            { return this->previousNodes; }
    NodeVector& getAuxiliaryNodes()                           { return this->auxiliaryNodes; }
    void operator++()                                         { this->inDegrees++;  }
    void operator--()                                         { this->inDegrees--; }
    int getInDegrees()                                  const { return inDegrees; }
    void setInDegrees(const int& inDegrees)                   { this->inDegrees = inDegrees; }
    int getThreadIndex()                                const { return threadIndex; }
    void setThreadIndex(const int& threadIndex)               { this->threadIndex = threadIndex; }
    void setTransferredNode(Node* transferredNode)              { this->transferredNode = transferredNode; }
    Node* getTransferredNode()                          const { return this->transferredNode; }
    virtual const char* getName()                       const =0;
    virtual unsigned int getSize()                      const =0;

  protected: // Copy constructor and operator is not allowed inside the framework
    Node(Node const&);
    Node& operator=(Node const&);
};

// -- Modules
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

template<class T>
class ModuleTemplate : public Module
{
  public: ModuleTemplate()          {}
  public: virtual ~ModuleTemplate() {}
  public: virtual unsigned int getSize() const { return sizeof(T); }
  public: void stream(Node*)      {}
};

// -- Representations
class Representation: public Node, public Serializable
{
  public: void (*updateThis)(Node* , Node* );
  public: Representation() : Node(), updateThis(0)  {}
  public: virtual ~Representation()                 {}
  public: virtual void draw() const                 {}
  public:
    virtual void serialize(ObjectInput* in, ObjectOutput* out)
    {
      const size_t baseSize = sizeof(Representation);
      unsigned char* p = (unsigned char*) this;
      SERIALIZE_BUFFER(getName(), p + baseSize, getSize() - baseSize);
    }
#if !defined(EMBEDDED_MODE)
  public:
    std::mutex sync;
#endif
};

template<class T>
class RepresentationTemplate : public Representation
{
  public: RepresentationTemplate()          {}
  public: virtual ~RepresentationTemplate() {}
  public: virtual unsigned int getSize() const { return sizeof(T); }
};

class RepresentationCloneable
{
  public: RepresentationCloneable() {}
  public: virtual ~RepresentationCloneable() {}
  public: virtual Node* clone(Node* that)  =0;
};

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

    class Thread
    {
      public:
        const char* threadName;
        int threadIndex;
        int threadPriority;
        bool isActive;
        typedef Vector<Node*> NodeVector;
        //-- Intermediate vectors
        NodeVector graphStructureVector;
        NodeVector transferredVector;
        NodeVector topoQueue;
        //-- Operation vector
        NodeVector operationVector;
        //-- Transfer objects between threads
        unsigned char* buffer;
#if !defined(EMBEDDED_MODE)
        enum { BUFFER_SIZE = 512 }; // some storage capacity (bytes)
#endif
        Thread(const char* threadName, const int& threadPriority, const int& threadIndex) :
            threadName(threadName), threadIndex(threadIndex), threadPriority(threadPriority), isActive(
                false), buffer(0)
        {
#if !defined(EMBEDDED_MODE)
          buffer = new unsigned char[BUFFER_SIZE];
#endif
        }

        virtual ~Thread()
        {
          for (NodeVector::iterator iter = transferredVector.begin();
              iter != transferredVector.end(); ++iter)
            delete *iter;
          graphStructureVector.clear();
          transferredVector.clear();
          topoQueue.clear();
          operationVector.clear();

#if !defined(EMBEDDED_MODE)
          delete[] buffer;
#endif
        }
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

    /** Computational resources */
    void activateThreads(const bool& threadsActivated);
    void computeGraph();
    void sort();

    static void threadAllocate(Thread* thread);
#if !defined(EMBEDDED_MODE)
    static void threadTransfer(Thread* thread);
#endif
    static void threadUpdate(Thread* thread);
    static void threadLoop(Thread* thread);

#if !defined(EMBEDDED_MODE)
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
    void main(const bool& threadsActivated);
#endif
#if defined(EMBEDDED_MODE)
    void setup(unsigned long baudRate);
    void loop();
#endif


    /** verbose */
    void stream();
};

// All the computational units are loaded into an instance of this class.
template <class T>
class ModuleLoader
{
  private:
    T theInstance;

  public:
    ModuleLoader(const char* threadName, const int& threadPriority) { Controller::getInstance().addModule(threadName, threadPriority, &theInstance); }
    virtual ~ModuleLoader() { }
};

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

template<const char* (*getModuleName)(), const char* (*getRepresentationName)(), class T>
class RepresentationPointer
{
  protected:
    T* theInstance;

  public:
    RepresentationPointer() : theInstance(0) {  }
    virtual ~RepresentationPointer() { }

  protected:
    inline T* getRepresentation()
    {
      if (!theInstance)
        theInstance = (T*) Controller::getInstance().getRepresentation(getModuleName(), getRepresentationName());
      return theInstance;
    }

    inline T* getRepresentation() const
    {
      if (!theInstance)
        return theInstance;
      return (T*) Controller::getInstance().getRepresentation(getModuleName(), getRepresentationName()); //<< SLOW for CONST ACCESS
    }

  public:
    virtual bool isNull()                   { return (this->getRepresentation() == 0); }
    virtual bool operator==(const T* other) { return this->getRepresentation() == other; }
    virtual bool operator!=(const T* other) { return !(this->getRepresentation() == other); }

};

template<const char* (*getModuleName)(), const char* (*getRepresentationName)(), class T>
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

template<const char* (*getModuleName)(), const char* (*getRepresentationName)(), class T>
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


#endif /* FRAMEWORK_H_ */
