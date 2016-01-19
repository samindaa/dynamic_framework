/*
 * Node.h
 *
 *  Created on: Jan 18, 2016
 *      Author: sabeyruw
 */

#ifndef NODE_H_
#define NODE_H_

#if defined(ENERGIA) && !defined(EMBEDDED_MODE)
#define EMBEDDED_MODE
#endif

#include "Vector.h"

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




#endif /* NODE_H_ */
