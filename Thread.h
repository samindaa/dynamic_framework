/*
 * Thread.h
 *
 *  Created on: Jan 18, 2016
 *      Author: sabeyruw
 */

#ifndef THREAD_H_
#define THREAD_H_

#include "Vector.h"

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
            threadName(threadName), threadIndex(threadIndex), threadPriority(threadPriority), isActive(false), buffer(0)
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



#endif /* THREAD_H_ */
